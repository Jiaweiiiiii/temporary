/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : bst_and_bsc.cpp
 * Authors    : jmqi@ingenic.st.jz.com
 * Create Time: 2020-08-03:10:36:31
 * Description:
 *
 */

#include <string.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>

#include "affine_whole_float.h"
#include "affine_whole_int.h"
#include "image_process.h"
#include "bscaler_hal.h"
#include "bscaler_api.h"
#include "bscaler_mdl.h"
#include "bscaler_wrap.h"
#include "random_api.h"
#ifdef CSE_SIM_ENV
#include "aie_mmap.h"
#else
#include "platform.h"
#endif

#define ORAM_TEST       0
uint8_t *bsc_src_base = NULL;
uint8_t *bsc_dst_base = NULL;
uint8_t *bst_src_base = NULL;
uint8_t *bst_dst_base = NULL;

uint32_t bsc_gld_isum = 0;
uint32_t bsc_gld_osum = 0;
uint8_t *bsc_gld_base = NULL;
void parse_random_cfg(bs_api_s *cfg, char *file)
{
    FILE *fpi;
    fpi = fopen(file, "rb+");
    if (fpi == NULL) {
        fprintf(stderr, "Open %s failed!\n", file);
        exit(1);
    }
    fread(&cfg->mode, 4, 1, fpi);
    fread(&cfg->src_format, 4, 1, fpi);
    fread(&cfg->src_w, 4, 1, fpi);
    fread(&cfg->src_h, 4, 1, fpi);
    fread(&cfg->src_line_stride, 4, 1, fpi);
    fread(&cfg->src_locate, 4, 1, fpi);
    int src_size;
    fread(&src_size, 4, 1, fpi);
#if (!ORAM_TEST)
    cfg->src_locate = 0;
#endif
    if (cfg->src_locate) {
        cfg->src_base = (uint8_t*)bscaler_malloc_oram(1, src_size);
        printf("cfg->src_base in oram\n");
    } else {
        //cfg->src_base = (uint8_t*)bscaler_malloc(1, src_size);
        cfg->src_base = bsc_src_base;
    }
    fread(cfg->src_base, 1, src_size, fpi);
    fread(&cfg->dst_format, 4, 1, fpi);
    fread(&cfg->dst_w, 4, 1, fpi);
    fread(&cfg->dst_h, 4, 1, fpi);
    fread(&cfg->dst_line_stride, 4, 1, fpi);
    fread(&cfg->dst_locate, 4, 1, fpi);
    fread(&cfg->matrix, 4, 9, fpi);
    fread(&bsc_gld_isum, 4, 1, fpi);
    fread(&bsc_gld_osum, 4, 1, fpi);
#if 1
    int dst_size;
    fread(&dst_size, 4, 1, fpi);
    bsc_gld_base = (uint8_t*)malloc(dst_size);
    fread(bsc_gld_base, 1, dst_size, fpi);
#endif
    fclose(fpi);
}

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

int main(int argc, char **argv)
{
    // 1. bscaler init
    bscaler_init();
    bscaler_frmc_soft_reset();
    bscaler_frmt_soft_reset();

    const uint32_t coef[9] = {1220542, 1673527, 0,
                              1220542, 409993, 852492,
                              1220542, 2116026, 0};
    const uint32_t offset[2] = {16, 128};
    const uint8_t offset_u8[2] = {16, 128};

    bscaler_common_param_cfg(coef, offset_u8, 0xFF);

    bsc_src_base = bscaler_malloc(1, 1024*1024*4);
    bsc_dst_base = bscaler_malloc(1, 1024*1024*4);
    bst_src_base = bscaler_malloc(1, 4096*1024*4);
    bst_dst_base = bscaler_malloc(1, 4096*1024*4);

    bs_api_s cfg;
    parse_random_cfg(&cfg, "0x2858a6f2.bin");
    int dst_buf_size = get_dst_buffer_size(&cfg);
#if (!ORAM_TEST)
    cfg.dst_locate = 0;
#endif
    if (cfg.dst_locate) {
        cfg.dst_base = (uint8_t*)bscaler_malloc_oram(1, dst_buf_size);
        printf("dst_base in oram\n");
    } else {
        //cfg.dst_base = (uint8_t*)bscaler_malloc(1, dst_buf_size);
        cfg.dst_base = bsc_dst_base;
        memset(cfg.dst_base, 0xAA, 1024*1024*4);
    }
    // 5. Run a specified number of floating-point and integer models
    const uint8_t zero_point = 0;
    const int box_num = 1;
    int src_bpp_mode = (cfg.src_format >> 5) & 0x3;
    int dst_bpp_mode = (cfg.dst_format >> 5) & 0x3;
    int src_bpp = 1 << (2 + src_bpp_mode);
    int dst_bpp = 1 << (2 + dst_bpp_mode);

    data_info_s bsc_src = {cfg.src_base, NULL, cfg.src_format, src_bpp,
                           cfg.src_w, cfg.src_h, cfg.src_line_stride, cfg.src_locate};
    data_info_s bsc_dut = {cfg.dst_base, NULL, cfg.dst_format, dst_bpp,
                           cfg.dst_w, cfg.dst_h, cfg.dst_line_stride, cfg.dst_locate};
    box_resize_info_s *resize_infos = NULL;
    box_affine_info_s *affine_infos = NULL;

    if (cfg.mode == RSZ) {
        resize_infos = (box_resize_info_s *)malloc(sizeof(box_resize_info_s) * box_num);
        for (int i = 0; i < box_num; i++) {
            resize_infos[i].box.x = 0;
            resize_infos[i].box.y = 0;
            resize_infos[i].box.w = cfg.src_w;
            resize_infos[i].box.h = cfg.src_h;
            resize_infos[i].wrap = 0;
            resize_infos[i].zero_point = zero_point;//fix me and fix the alpha
        }
    } else if ((cfg.mode == AFFINE) || (cfg.mode == PERSP)) {
        affine_infos = (box_affine_info_s *)malloc(sizeof(box_affine_info_s) * box_num);
        for (int i = 0; i < box_num; i++) {
            affine_infos[i].box.x = 0;
            affine_infos[i].box.y = 0;
            affine_infos[i].box.w = cfg.src_w;
            affine_infos[i].box.h = cfg.src_h;
            affine_infos[i].wrap = 0;
            affine_infos[i].zero_point = zero_point;//fix me and fix the alpha
            for (int j = 0; j < 9; j++) {
                affine_infos[i].matrix[j] = cfg.matrix[j];
            }
        }
    } else {
        assert(0);
    }

    //bst cfg
    task_info_s task_info, mdl_task_info;
    data_info_s bst_src, bst_dst, bst_gld;
    bst_random_api(&bst_src, &bst_dst, &task_info);
    memcpy(&bst_gld, &bst_dst, sizeof(data_info_s));
    memcpy(&mdl_task_info, &task_info, sizeof(task_info_s));
    // gld
    int plane_num = 1;//((task_info.kw * task_info.kh * 3 + 31) >> 5);//fixme
    uint32_t gld_plane_stride = bst_gld.height * bst_gld.line_stride;
    mdl_task_info.task_len = bst_gld.height;//not same
    mdl_task_info.plane_stride = gld_plane_stride;//not same
    //alloc space
    int bst_src_buf_size = (bst_src.format == BS_DATA_NV12) ?
        bst_src.height * bst_src.line_stride * 3 / 2 : bst_src.height * bst_src.line_stride;
    bst_src.base = bst_src_base;
    uint8_t *src_ptr = (uint8_t *)bst_src.base;
    for (int i = 0; i < bst_src_buf_size; i++) {
        src_ptr[i] = 0x55;
    }

    int bst_dst_buf_size = 0;
    if ((bst_dst.format == BS_DATA_VBGR) || (bst_dst.format == BS_DATA_VRGB)) {
        dst_buf_size = task_info.task_len * bst_dst.line_stride * plane_num;
    } else {
        dst_buf_size = task_info.task_len * bst_dst.line_stride;
    }

    bst_dst.base = bst_dst_base;
    int bst_gld_buf_size = 0;
    if ((bst_dst.format == BS_DATA_VBGR) || (bst_dst.format == BS_DATA_VRGB)) {
        bst_gld_buf_size = bst_gld.height * bst_dst.line_stride * plane_num;
    } else {
        bst_gld_buf_size = bst_gld.height * bst_gld.line_stride;
    }

    bst_gld.base = malloc(bst_gld_buf_size);
    bs_covert_mdl(&bst_src, &bst_gld, coef, offset, &mdl_task_info);
    int times = (bst_dst.height + task_info.task_len - 1) / task_info.task_len;

    //bsc and bst hw

    while (1) {
        bscaler_frmc_soft_reset();
        bscaler_write_reg(BSCALER_FRMC_ISUM, 0);
        bscaler_write_reg(BSCALER_FRMC_OSUM, 0);
        bscaler_write_reg(BSCALER_FRMC_CSUM, 0);
        if (cfg.mode == RSZ) {
            bs_resize_start(&bsc_src, box_num, &bsc_dut, resize_infos, coef, offset);
            //bs_resize_wait();
        } else if (cfg.mode == AFFINE) {
            bs_affine_start(&bsc_src, box_num, &bsc_dut, affine_infos, coef, offset);
            //bs_affine_wait();
        } else if (cfg.mode == PERSP) {
            bs_perspective_start(&bsc_src, box_num, &bsc_dut, affine_infos, coef, offset);
            //bs_perspective_wait();
        }

        bs_covert_cfg(&bst_src, &bst_dst, coef, offset, &task_info);
        int bst_errnum = 0;
        int line_cnt = 0;
        for (int i = 0; i < times; i++) {
            uint8_t *task_dut_base = (uint8_t *)bst_dst.base;
            bs_covert_step_start(&task_info, task_dut_base, 0);
            //printf("======= hardware start ==========\n");
            bs_covert_step_wait();
            //check task data
            uint32_t real_task_len = (line_cnt + task_info.task_len) > bst_dst.height ?
                bst_dst.height - line_cnt : task_info.task_len;
            uint8_t *task_gld_base = (uint8_t *)bst_gld.base +
                i * task_info.task_len * bst_gld.line_stride;

            uint8_t *gld_y = NULL;
            uint8_t *gld_c = NULL;
            uint8_t *dst_c = NULL;
            bst_errnum += check_task_data(task_gld_base, gld_c, task_dut_base, dst_c,
                                          bst_dst.format, bst_dst.line_stride,
                                          mdl_task_info.plane_stride,
                                          task_info.plane_stride, real_task_len,
                                          bst_dst.width, plane_num);
            line_cnt += task_info.task_len;
            if (bst_errnum) {
                printf("======= FAILED %d: %d==========\n", times, i);
                break;
            }
        }

        if (cfg.mode == RSZ) {
            bs_resize_wait();
        } else if (cfg.mode == AFFINE) {
            bs_affine_wait();
        } else if (cfg.mode == PERSP) {
            bs_perspective_wait();
        }

        uint32_t bsc_dut_isum = bscaler_read_reg(BSCALER_FRMC_ISUM, 0);
        uint32_t bsc_dut_osum = bscaler_read_reg(BSCALER_FRMC_OSUM, 0);
        uint32_t bsc_dut_csum = bscaler_read_reg(BSCALER_FRMC_CSUM, 0);
        printf("bsc_dut_csum = 0x%08x\n", bsc_dut_csum);
        int bsc_errnum = 0;
        //fixme, hardware isum have bug, 20200803
        if (bsc_gld_isum != bsc_dut_isum) {
            printf("bsc isum Error: (G)0x%08x -- (E)0x%08x\n", bsc_gld_isum, bsc_dut_isum);
        }
        int vague_check = 0;
        if (bsc_gld_osum != bsc_dut_osum) {
            printf("bsc osum Error: (G)0x%08x -- (E)0x%08x\n", bsc_gld_osum, bsc_dut_osum);
            vague_check++;
        }
        data_info_s bsc_gld = {bsc_gld_base, NULL, cfg.dst_format, dst_bpp,
                               cfg.dst_w, cfg.dst_h, cfg.dst_line_stride, 0};

        if (vague_check) {
            bsc_errnum = data_check(&bsc_gld, &bsc_dut);
        }
        if (bsc_errnum != 0) {
            show_bs_api(&cfg);
        }

        static int cnt = 0;
        if (bsc_errnum != 0) {
            printf("****** BSC FAILED ****** %d\n", cnt++);
        } else {
            printf("****** BSC PASSED ****** %d\n", cnt++);
        }

        while ((bscaler_read_reg(BSCALER_FRMT_CTRL, 0) & 0x1) != 0);
        static int bst_cnt = 0;
        if (bst_errnum) {
            printf("======= BST FAILED ========== %d\n", bst_cnt++);
        } else {
            printf("======= BST PASS ========== %d\n", bst_cnt++);
        }
    }
}
