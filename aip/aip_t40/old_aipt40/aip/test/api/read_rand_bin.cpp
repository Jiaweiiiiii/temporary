/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : read_rand_bin.cpp
 * Authors    : jmqi@ingenic.st.jz.com
 * Create Time: 2020-08-03:10:36:31
 * Description:
 *
 */

#include <iostream>
#include <string.h>

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

#define RANDOM_RST      1
#define ORAM_TEST       0

uint32_t gld_isum = 0;
uint32_t gld_osum = 0;
uint8_t *gld_base = NULL;
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
        cfg->src_base = (uint8_t*)bscaler_malloc(1, src_size);
    }
    fread(cfg->src_base, 1, src_size, fpi);
    fread(&cfg->dst_format, 4, 1, fpi);
    fread(&cfg->dst_w, 4, 1, fpi);
    fread(&cfg->dst_h, 4, 1, fpi);
    fread(&cfg->dst_line_stride, 4, 1, fpi);
    fread(&cfg->dst_locate, 4, 1, fpi);
    fread(&cfg->matrix, 4, 9, fpi);
    fread(&gld_isum, 4, 1, fpi);
    fread(&gld_osum, 4, 1, fpi);
#if 1
    int dst_size;
    fread(&dst_size, 4, 1, fpi);
    gld_base = (uint8_t*)malloc(dst_size);
    fread(gld_base, 1, dst_size, fpi);
#endif
    fclose(fpi);
}

int main(int argc, char** argv)
{
    // 1. bscaler init
    bscaler_init();
    bscaler_frmc_soft_reset();

    //delete me
    uint32_t coef[9] = {1220542, 1673527, 0,
                              1220542, 409993, 852492,
                              1220542, 2116026, 0};
    const uint32_t offset[2] = {16, 128};
    uint8_t offset_u8[2] = {16, 128};

    if (argc < 2) {
        fprintf(stderr, "%s [*.bin]\n", argv[0]);
    }
    //
    bs_api_s cfg;
    parse_random_cfg(&cfg, argv[1]);
    uint8_t t_order = 0;
    uint8_t c_order = (cfg.dst_format >> 1) & 0xF;
    bscaler_common_param_cfg(coef, offset_u8, 0xFF);
    // 4. alloc space and random init
    int dst_buf_size = get_dst_buffer_size(&cfg);
#if (!ORAM_TEST)
    cfg.dst_locate = 0;
#endif
    if (cfg.dst_locate) {
        cfg.dst_base = (uint8_t*)bscaler_malloc_oram(1, dst_buf_size);
        printf("dst_base in oram\n");
    } else {
        cfg.dst_base = (uint8_t*)bscaler_malloc(1, dst_buf_size);
    }
    memset(cfg.dst_base, 0, dst_buf_size);
    printf("%s\n", argv[1]);
    //show_bs_api(&cfg);

    // 5. Run a specified number of floating-point and integer models
    const uint8_t zero_point = 0;
    const int box_num = 1;
    int src_bpp_mode = (cfg.src_format >> 5) & 0x3;
    int dst_bpp_mode = (cfg.dst_format >> 5) & 0x3;
    int src_bpp = 1 << (2 + src_bpp_mode);
    int dst_bpp = 1 << (2 + dst_bpp_mode);

    data_info_s src = {cfg.src_base, NULL, cfg.src_format, src_bpp,
                       cfg.src_w, cfg.src_h, cfg.src_line_stride, (bs_data_locate_e)cfg.src_locate};
    data_info_s dut = {cfg.dst_base, NULL, cfg.dst_format, dst_bpp,
                       cfg.dst_w, cfg.dst_h, cfg.dst_line_stride, (bs_data_locate_e)cfg.dst_locate};
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

    //debug
    //int debug_x = 121;
    //int debug_y = 1;
    //bscaler_write_reg(64*4, (debug_y & 0xFFFF) << 16 | (debug_x & 0xFFFF));
    //clear ISUM and OSUM
    bscaler_write_reg(BSCALER_FRMC_ISUM, 0);
    bscaler_write_reg(BSCALER_FRMC_OSUM, 0);
    if (cfg.mode == RSZ) {
        bs_resize_start(&src, box_num, &dut, resize_infos, coef, offset);
#if RANDOM_RST
        printf("random reset ...\n");
        bscaler_frmc_soft_reset();
        bscaler_write_reg(BSCALER_FRMC_ISUM, 0);
        bscaler_write_reg(BSCALER_FRMC_OSUM, 0);
        bs_resize_start(&src, box_num, &dut, resize_infos, coef, offset);
#endif
        bs_resize_wait();
    } else if (cfg.mode == AFFINE) {
        bs_affine_start(&src, box_num, &dut, affine_infos, coef, offset);
#if RANDOM_RST
        printf("random reset ...\n");
        bscaler_frmc_soft_reset();
        bscaler_write_reg(BSCALER_FRMC_ISUM, 0);
        bscaler_write_reg(BSCALER_FRMC_OSUM, 0);
        bs_affine_start(&src, box_num, &dut, affine_infos, coef, offset);
#endif
        bs_affine_wait();
    } else if (cfg.mode == PERSP) {
        bs_perspective_start(&src, box_num, &dut, affine_infos, coef, offset);
#if RANDOM_RST
        printf("random reset ...\n");
        bscaler_frmc_soft_reset();
        bscaler_write_reg(BSCALER_FRMC_ISUM, 0);
        bscaler_write_reg(BSCALER_FRMC_OSUM, 0);
        bs_perspective_start(&src, box_num, &dut, affine_infos, coef, offset);
#endif
        bs_perspective_wait();
    }
    uint32_t dut_isum = bscaler_read_reg(BSCALER_FRMC_ISUM, 0);
    uint32_t dut_osum = bscaler_read_reg(BSCALER_FRMC_OSUM, 0);
    uint32_t time = bscaler_read_reg(BSCALER_FRMC_TIMEOUT, 0);
    printf("total cycle: %d -- %d/c\n", (0xFFFFFFFF - time), (0xFFFFFFFF - time) / (cfg.dst_w * cfg.dst_h));

    int errnum = 0;
    //fixme, hardware isum have bug, 20200803
    if (gld_isum != dut_isum) {
        //printf("bsc isum Error: (G)0x%08x -- (E)0x%08x\n", gld_isum, dut_isum);
    }
    int vague_check = 0;
    if (gld_osum != dut_osum) {
        printf("bsc osum Error: (G)0x%08x -- (E)0x%08x\n", gld_osum, dut_osum);
        vague_check++;
    }
    data_info_s gld = {gld_base, NULL, cfg.dst_format, dst_bpp,
                       cfg.dst_w, cfg.dst_h, cfg.dst_line_stride, BS_DATA_NMEM};

    if (vague_check) {
        errnum = data_check(&gld, &dut);
    }
    if (errnum != 0) {
        show_bs_api(&cfg);
    }

    if (errnum != 0) {
        printf("****** FAILED ******\n");
        //uint32_t overflow = bscaler_read_reg(65*4, 0);
        //uint32_t spos = bscaler_read_reg(66*4, 0);
        //uint32_t sw = bscaler_read_reg(67*4, 0);
        //uint32_t d0 = bscaler_read_reg(68*4, 0);
        //uint32_t d1 = bscaler_read_reg(69*4, 0);
        //uint32_t d2 = bscaler_read_reg(70*4, 0);
        //uint32_t d3 = bscaler_read_reg(71*4, 0);
        //printf("src(%d, %d) -- %d, %d, -- %08x,%08x,%08x,%08x\n", spos & 0xFFFF, spos >> 16,
        //       sw & 0xFFFF, sw >> 16, d0, d1, d2, d3);
    } else {
        printf("****** PASSED ******\n");
    }
    bscaler_free(cfg.dst_base);
    bscaler_free(cfg.src_base);
    free(resize_infos);
    free(affine_infos);

#ifdef EYER_SIM_ENV
    eyer_stop();
#endif
    return 0;
}
