/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : rand_resize_api.cpp
 * Authors    : jmqi@taurus
 * Create Time: 2020-05-20:20:21:23
 * Description:
 *
 */
#include <iostream>

#include "bscaler_hal.h"
#include "bscaler_api.h"
#include "bscaler_mdl.h"
#include "bscaler_mdl_api.h"
#include "bscaler_wrap.h"
#include "random_api.h"
#ifdef CSE_SIM_ENV
#include "aie_mmap.h"
#else
#include "platform.h"
#endif

#define ORAM_TEST       0

int main(int argc, char** argv)
{
    // 1. set random seed
    int seed = (int)time(NULL);
    printf("seed = 0x%08x\n", seed);
    srand(seed);

    // 2. bscaler init
    bscaler_init();

    const uint32_t coef[9] = {1220542, 1673527, 0,
                              1220542, 409993, 852492,
                              1220542, 2116026, 0};
    const uint32_t offset[2] = {16, 128};

    // 3. random parameter
    bs_api_s cfg_s, *cfg = &cfg_s;
    bs_run_mode_e mode = PERSP;
    set_run_mode(mode, cfg);
    matrix_random(cfg);

    // 4. alloc space and random init
    int src_buf_size = get_src_buffer_size(cfg);
    int dst_buf_size = get_dst_buffer_size(cfg);
#if (!ORAM_TEST)
    cfg->src_locate = 0;
    cfg->dst_locate = 0;
#endif
    if (cfg->src_locate) {
        cfg->src_base = (uint8_t*)bscaler_malloc_oram(1, src_buf_size);
    } else {
        cfg->src_base = (uint8_t*)bscaler_malloc(1, src_buf_size);
    }

    if (cfg->dst_locate) {
        cfg->dst_base = (uint8_t*)bscaler_malloc_oram(1, dst_buf_size);
    } else {
        cfg->dst_base = (uint8_t*)bscaler_malloc(1, dst_buf_size);
    }

    random_init_space(cfg->src_base, src_buf_size);

    uint8_t *gld_base = (uint8_t*)malloc(dst_buf_size);
    if (gld_base == NULL) {
        fprintf(stderr, "malloc space for gld error!\n");
        exit(1);
    }

    // 5. Run a specified number of floating-point and integer models
    const uint8_t zero_point = 0;
    const int box_num = 1;
    int src_bpp_mode = (cfg->src_format >> 5) & 0x3;
    int dst_bpp_mode = (cfg->dst_format >> 5) & 0x3;
    int src_bpp = 1 << (2 + src_bpp_mode);
    int dst_bpp = 1 << (2 + dst_bpp_mode);

    data_info_s src = {cfg->src_base, NULL, cfg->src_format, src_bpp,
                       cfg->src_w, cfg->src_h, cfg->src_line_stride};
    data_info_s gld = {gld_base, NULL, cfg->dst_format, dst_bpp,
                       cfg->dst_w, cfg->dst_h, cfg->dst_line_stride};
    data_info_s dut = {cfg->dst_base, NULL, cfg->dst_format, dst_bpp,
                       cfg->dst_w, cfg->dst_h, cfg->dst_line_stride};
    box_affine_info_s *infos = (box_affine_info_s *)malloc(sizeof(box_affine_info_s) * box_num);
    for (int i = 0; i < box_num; i++) {
        int resize_src_x = 0;
        int resize_src_y = 0;
        int resize_src_w = cfg->src_w;
        int resize_src_h = cfg->src_h;

        infos[i].box.x = resize_src_x;
        infos[i].box.y = resize_src_y;
        infos[i].box.w = resize_src_w;
        infos[i].box.h = resize_src_h;
        infos[i].wrap = 0;
        infos[i].zero_point = zero_point;//fix me and fix the alpha
    }

    // dut integer affine
    bscaler_write_reg(BSCALER_FRMC_ISUM, 0);
    bscaler_write_reg(BSCALER_FRMC_OSUM, 0);
    bs_perspective_start(&src, box_num, &dut, infos, coef, offset);
    bs_perspective_wait();
    bs_perspective_mdl(&src, box_num, &gld, infos, coef, offset);

    uint32_t gld_isum = get_bsc_isum();
    uint32_t gld_osum = get_bsc_osum();
    uint32_t dut_isum = bscaler_read_reg(BSCALER_FRMC_ISUM, 0);
    uint32_t dut_osum = bscaler_read_reg(BSCALER_FRMC_OSUM, 0);
    if (gld_isum != dut_isum) {
        printf("bsc isum Error: (G)0x%08x -- (E)0x%08x\n", gld_isum, dut_isum);
    }
    if (gld_osum != dut_osum) {
        printf("bsc osum Error: (G)0x%08x -- (E)0x%08x\n", gld_osum, dut_osum);
    }

    int errnum = data_check(&gld, &dut);
    if (errnum != 0) {
        printf("****** FAILED ******\n");
    } else {
        printf("****** PASSED ******\n");
    }
    bscaler_free(cfg->dst_base);
    bscaler_free(cfg->src_base);
    free(gld_base);
    free(infos);

#ifdef EYER_SIM_ENV
    eyer_stop();
#endif
    return 0;
}
