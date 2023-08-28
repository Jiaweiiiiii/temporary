/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : rand_converter.cpp
 * Authors    : jmqi@moses.ic.jz.com
 * Create Time: 2020-08-10:08:56:50
 * Description:
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Matrix.h"//not necessary
#include "image_process.h"//not necessary
#include "bscaler_api.h"
#include "bscaler_mdl_api.h"
#include "bscaler_hal.h"
#include "bscaler_wrap.h"
#include "random_api.h"
#ifdef CSE_SIM_ENV
#include "aie_mmap.h"
#else
#include "platform.h"
#endif

int check_task_data(uint8_t *gld_y, uint8_t *gld_c,
                    uint8_t *dst_y, uint8_t *dst_c,
                    bs_data_format_e dst_format, int line_stride,
                    int gld_plane_stride, int dut_plane_stride, uint32_t real_task_len,
                    uint32_t width, int cg_num
    )
{
    int errnum = 0;
    int x, y;
    if (dst_format == BS_DATA_NV12) {
        // Y
        for (y = 0; y < real_task_len; y++) {
            for (x = 0; x < width; x++) {
                uint8_t g_val = gld_y[y * line_stride + x];
                uint8_t d_val = dst_y[y * line_stride + x];
                if (g_val != d_val) {
                    if (errnum < 5) {
                        printf("[Error] Y: (%d, %d) : (G)0x%x -- (E)0x%x\n",
                               x, y, g_val, d_val);
                    }
                    errnum++;
                }
            }
        }
        // UV
        for (y = 0; y < real_task_len/2; y++) {
            for (x = 0; x < width; x++) {
                uint8_t g_val = gld_c[y * line_stride + x];
                uint8_t d_val = dst_c[y * line_stride + x];
                if (g_val != d_val) {
                    if (errnum < 5) {
                        printf("[Error] C: (%d, %d) : (G)0x%x -- (E)0x%x\n",
                               x, y, g_val, d_val);
                    }
                    errnum++;
                }
            }
        }
    } else if ((dst_format == BS_DATA_BGRA) || (dst_format == BS_DATA_GBRA) ||
               (dst_format == BS_DATA_RBGA) || (dst_format == BS_DATA_BRGA) ||
               (dst_format == BS_DATA_GRBA) || (dst_format == BS_DATA_RGBA) ||
               (dst_format == BS_DATA_ABGR) || (dst_format == BS_DATA_AGBR) ||
               (dst_format == BS_DATA_ARBG) || (dst_format == BS_DATA_ABRG) ||
               (dst_format == BS_DATA_AGRB) || (dst_format == BS_DATA_ARGB)) {
        for (y = 0; y < real_task_len; y++) {
            for (x = 0; x < width; x++) {
                for (int k = 0; k < 4; k++) {
                    uint8_t g_val = gld_y[y * line_stride + x * 4 + k];
                    uint8_t d_val = dst_y[y * line_stride + x * 4 + k];
                    if (g_val != d_val) {
                        if (errnum < 5) {
                            printf("[Error] RGB: (%d, %d, %d) : (G)0x%x -- (E)0x%x\n",
                                   x, y, k, g_val, d_val);
                        }
                        errnum++;
                    }
                }
            }
        }
    } else if ((dst_format == BS_DATA_VBGR) || (dst_format == BS_DATA_VRGB)) {
        for (int cg = 0; cg < cg_num; cg++) {
            for (y = 0; y < real_task_len; y++) {
                for (x = 0; x < line_stride; x++) {
                    uint8_t g_val = gld_y[cg * gld_plane_stride + y * line_stride + x];
                    uint8_t d_val = dst_y[cg * dut_plane_stride + y * line_stride + x];
                    if (g_val != d_val) {
                        if (errnum < 5) {

                            printf("%d : gld=%p, dut=%p\n", cg * gld_plane_stride + y * line_stride + x,
                                   &gld_y[cg * gld_plane_stride + y * line_stride + x],
                                   &dst_y[cg * dut_plane_stride + y * line_stride + x]);

                            printf("[Error] VCHN: (%d, %d, %d) : (G)0x%x -- (E)0x%x\n",
                                   x, y, cg, g_val, d_val);
                        }
                        errnum++;
                    }
                }
            }
        }

    } else {
        assert(0);
    }
    return errnum;
}

int main(int argc, char** argv)
{

    int seed = (int)time(NULL);
    seed = 0x6004f3d8;
    printf("seed = 0x%08x\n", seed);
    srand(seed);

    // 2. bscaler init
    bscaler_init();

    //optional
    const uint32_t coef[9] = {1220542, 0, 1673527,
                              1220542, 409993, 852492,
                              1220542, 2116026, 0};
    //optional
    const uint32_t offset[2] = {16, 128};
    //optional
    task_info_s task_info, mdl_task_info;
    data_info_s src, dst, gld;
    bst_random_api(&src, &dst, &task_info);

    memcpy(&gld, &dst, sizeof(data_info_s));
    memcpy(&mdl_task_info, &task_info, sizeof(task_info_s));

    // gld
    int plane_num = 1;//((task_info.kw * task_info.kh * 3 + 31) >> 5);//fixme
    uint32_t gld_plane_stride = gld.height * gld.line_stride;
    //printf("gld.height=%d, gld.line_stride=%d\n", gld.height, gld.line_stride);
    mdl_task_info.task_len = gld.height;//not same
    mdl_task_info.plane_stride = gld_plane_stride;//not same

    //alloc space
    int src_buf_size = (src.format == BS_DATA_NV12) ?
        src.height * src.line_stride * 3 / 2 : src.height * src.line_stride;
    //printf("src_buf_size=%d\n", src_buf_size);
    src.base = bscaler_malloc(64, src_buf_size);
    src.base1 = NULL;
    uint8_t *src_ptr = (uint8_t *)src.base;
    for (int i = 0; i < src_buf_size; i++) {
        src_ptr[i] = rand() % 256;
        //src_ptr[i] = 1;
    }

    int dst_buf_size = 0;
    if ((dst.format == BS_DATA_VBGR) || (dst.format == BS_DATA_VRGB)) {
        dst_buf_size = task_info.task_len * dst.line_stride * plane_num;
    } else {
        dst_buf_size = task_info.task_len * dst.line_stride;
    }
    //printf("dstsize=%d, task_len=%d, dst.line_stride=%d\n", dst_buf_size, task_info.task_len, dst.line_stride);
    dst.base = bscaler_malloc(64, dst_buf_size);
    dst.base1 = NULL;
    int gld_buf_size = 0;
    if ((dst.format == BS_DATA_VBGR) || (dst.format == BS_DATA_VRGB)) {
        //gld_buf_size = gld.height * dst.line_stride * plane_num;
    } else {
        gld_buf_size = gld.height * gld.line_stride;
    }
    //printf("gldsize=%d, task_len=%d, gld.line_stride=%d\n", gld_buf_size, task_info.task_len, gld.line_stride);
    gld.base = bscaler_malloc(64, gld_buf_size);
    gld.base1 = NULL;
    printf("======= model start ==========\n");
    bs_covert_mdl(&src, &gld, coef, offset, &mdl_task_info);
    printf("======= model finish ==========\n");
    printf("======= hardware cfg ==========\n");
    bs_covert_cfg(&src, &dst, coef, offset, &task_info);

    __aie_flushcache((void *)src.base, src_buf_size);

    int times = (dst.height + task_info.task_len - 1) / task_info.task_len;
    int errnum = 0;
    int line_cnt = 0;
    for (int i = 0; i < times; i++) {
        uint8_t *task_dut_base = (uint8_t *)dst.base;
        printf("======= hardware start ==========\n");
        bs_covert_step_start(&task_info, task_dut_base, BS_DATA_NMEM);
        printf("======= hardware wait ==========\n");
        bs_covert_step_wait();

        //check task data
        uint32_t real_task_len = (line_cnt + task_info.task_len) > dst.height ?
            dst.height - line_cnt : task_info.task_len;
        uint8_t *task_gld_base = (uint8_t *)gld.base +
            i * task_info.task_len * gld.line_stride;

        uint8_t *gld_y = NULL;
        uint8_t *gld_c = NULL;
        uint8_t *dst_c = NULL;
        errnum += check_task_data(task_gld_base, gld_c, task_dut_base, dst_c,
                                  dst.format, dst.line_stride,
                                  mdl_task_info.plane_stride,
                                  task_info.plane_stride, real_task_len,
                                  dst.width, plane_num);
        line_cnt += task_info.task_len;
        if (errnum) {
            printf("======= FAILED %d: %d==========\n", times, i);
            break;
        }
    }

    if (errnum) {
        printf("======= FAILED ==========\n");
    } else {
        printf("======= PASS ==========\n");
    }

    bscaler_free(src.base);
    bscaler_free(dst.base);
    bscaler_free(gld.base);
#ifdef EYER_SIM_ENV
    eyer_stop();
#endif
}
