/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : converter_demo.cpp
 * Authors    : jmqi@moses.ic.jz.com
 * Create Time: 2020-08-10:08:56:50
 * Description:
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "Matrix.h"//not necessary
#include "image_process.h"//not necessary
#include "bscaler_api.h"
#include "bscaler_hal.h"
#include "aie_mmap.h"
//#include "aie_nndma.h"
#define USE_DMA 0

inline void __aie_run_nndma_WCH0(uint32_t ptr)
{
    fast_iob();
    volatile uint32_t data = 0;
    uint32_t nndma_io_vbase = __aie_get_nndma_io_vbase();
    *(volatile uint32_t *)(nndma_io_vbase + 0x10) = (0x1 << 31 | ptr);
    data = *(volatile uint32_t *)(nndma_io_vbase + 0x10);
}

int main(int argc, char** argv)
{
    float time_use = 0;
    struct timeval start;
    struct timeval end;
    gettimeofday(&start, NULL);

    int ret = 0;
    nna_cache_attr_t desram_cache_attr = NNA_UNCACHED_ACCELERATED;
    nna_cache_attr_t oram_cache_attr = NNA_UNCACHED_ACCELERATED;
    nna_cache_attr_t ddr_cache_attr = NNA_CACHED;
    if ((ret = __aie_mmap(0x4000000, 1, desram_cache_attr, oram_cache_attr, ddr_cache_attr)) == 0) { //64MB
        printf ("nna box_base virtual generate failed !!\n");
        return 0;
    }

    const uint32_t coef[9] = {1220542, 0, 1673527,
                              1220542, 409993, 852492,
                              1220542, 2116026, 0};
    const uint32_t offset[2] = {16, 128};

    uint32_t zero_point = 0x00807060;
    int task_len = 3;
    int plane_stride = 0;
    int img_w = 1024;
    int img_h = 1024;
    bs_data_format_e src_format = BS_DATA_NV12;
    bs_data_format_e dst_format = BS_DATA_BGRA;
    data_info_s src, dst_task;
    task_info_s task_info;
    src.base   = NULL;
    src.base1  = NULL;
    src.format = src_format;
    src.chn    = 1;
    src.width  = img_w;
    src.height = img_h;
    src.line_stride = img_w;
    src.locate = BS_DATA_NMEM;

    dst_task.base   = NULL;
    dst_task.base1   = NULL;
    dst_task.format = dst_format;
    dst_task.chn    = 4;
    dst_task.width  = img_w;
    dst_task.height = task_len;
    dst_task.line_stride = img_w * 4;
    //dst_task.locate = BS_DATA_ORAM;
    dst_task.locate = BS_DATA_NMEM;

    task_info.zero_point = zero_point;
    task_info.task_len = task_len;
    task_info.plane_stride = task_info.task_len * dst_task.line_stride;

    //alloc space
    int src_size = img_w * img_h * 3 / 2;
    src.base = ddr_memalign(64, src_size);
    if (src.base == NULL) {
        printf("malloc src_base failed!\n");
    }
    FILE *fpi;
    fpi = fopen(argv[1], "rb+");
    if (fpi == NULL) {
        fprintf(stderr, "Open %s failed!\n", argv[1]);

    }
    fread(src.base, 1, src_size, fpi);

    gettimeofday(&end, NULL);
    time_use = (float)((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec)) / 1000.0f;//ms
    printf("1 -- %.3fms\n", time_use);

    __aie_flushcache(src.base, src_size);

    gettimeofday(&end, NULL);
    time_use = (float)((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec)) / 1000.0f;//ms
    printf("2 -- %.3fms\n", time_use);
    int dst_task_size = task_info.task_len * dst_task.line_stride;
    void *dst_task_ping = NULL;
    void *dst_task_pong = NULL;
    if (dst_task.locate == BS_DATA_NMEM) {
        dst_task_ping = ddr_memalign(64, dst_task_size);
        dst_task_pong = ddr_memalign(64, dst_task_size);
    } else {
        dst_task_ping = oram_memalign(64, dst_task_size);
        dst_task_pong = oram_memalign(64, dst_task_size);
    }

    if ((dst_task_ping == NULL) || (dst_task_pong == NULL)) {
        printf("[Error]: alloc task ping-pong buffer failed!\n");
    }

    int dst_all_size = img_w * img_h * 4;
    void *dst_all = (uint32_t *)malloc(dst_all_size);
    memset(dst_all, 0, dst_all_size);
    void *dst_ptr = dst_all;

    gettimeofday(&end, NULL);
    time_use = (float)((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec)) / 1000.0f;//ms
    printf("3 -- %.3fms\n", time_use);

    //while (1) {
    
    bs_covert_cfg(&src, &dst_task, coef, offset, &task_info);
    int times = (img_h + task_info.task_len - 1) / task_info.task_len;
    static int cnt = 0;
    uint32_t phy_dst_base;

    for (int i = 0; i < times; i++) {
        //gettimeofday(&end, NULL);
        //time_use = (float)((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec)) / 1000.0f;//ms
        //printf("times:%d -- %.3fms\n", i, time_use);

        int cur_task_len = (i == times - 1) ? ((img_h - 1) % task_len) + 1 : task_len;
        task_info.task_len = cur_task_len;
        int cur_task_size = cur_task_len * dst_task.line_stride;
        if (i%2) {
            if (dst_task.locate == BS_DATA_NMEM) {
                bs_covert_step_start(&task_info, dst_task_pong, BS_DATA_NMEM);
            } else {
                bs_covert_step_start(&task_info, dst_task_pong, BS_DATA_ORAM);
            }
        } else {
            if (dst_task.locate == BS_DATA_NMEM) {
                bs_covert_step_start(&task_info, dst_task_ping, BS_DATA_NMEM);
            } else {
                bs_covert_step_start(&task_info, dst_task_ping, BS_DATA_ORAM);
            }
        }

        bs_covert_step_wait();
        
        uint32_t des_addr = __aie_get_nndma_desram_vbase();
        uint32_t des_entry = __aie_get_desram_ptr(des_addr);
        uint32_t ddr_paddr = __aie_get_ddr_paddr((uint32_t)dst_ptr);
        
        if (i%2) {
#if USE_DMA
            uint32_t oram_paddr = __aie_get_oram_offset((uint32_t)dst_task_pong);
            __aie_push_nndma_bignode_maylast(1, &des_addr, oram_paddr, ddr_paddr, cur_task_size);
            __aie_run_nndma_WCH0(des_entry);
            i_rdhwr(11);
#else 
            if (ddr_cache_attr & NNA_CACHED) {
                __aie_flushcache(dst_task_pong, dst_task_size);
            }
            memcpy(dst_ptr, dst_task_pong, cur_task_size);
#endif
        } else {
#if USE_DMA
            uint32_t oram_paddr = __aie_get_oram_offset((uint32_t)dst_task_ping);
            __aie_push_nndma_bignode_maylast(1, &des_addr, oram_paddr, ddr_paddr, cur_task_size);
            __aie_run_nndma_WCH0(des_entry);
            i_rdhwr(11);
#else
            if (ddr_cache_attr & NNA_CACHED) {
                __aie_flushcache(dst_task_ping, dst_task_size);
            }
            memcpy(dst_ptr, dst_task_ping, cur_task_size);
#endif
        }
        dst_ptr += cur_task_size;
    }
    //}
    gettimeofday(&end, NULL);
    time_use = (float)((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec)) / 1000.0f;//ms
    printf("4 -- %.3fms\n", time_use);

    stbi_write_jpg("convert_out.jpg", img_w, img_h, 4, dst_all, 90);

    if (dst_task.locate == BS_DATA_NMEM) {
        ddr_free(dst_task_ping);
        ddr_free(dst_task_pong);
    } else {
        oram_free(dst_task_ping);
        oram_free(dst_task_pong);
    }
    ddr_free(dst_task.base);
    ddr_free(src.base);
    free(dst_all);
    __aie_munmap();
    return 0;
}
