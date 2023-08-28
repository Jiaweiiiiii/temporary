/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : t_bscaler_c.c
 * Authors    : jmqi@joshua
 * Create Time: 2020-06-26:22:43:49
 * Description:
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "bscaler_hw_api.h"
#include "t_vector_c.h"
#include "platform.c"

int bscaler_wait_finish()
{
    //polling
    int fail_flag=0;
    while ((plat_read_reg(BSCALER_BASE + BSCALER_FRMC_CTRL) & 0x3F) != 0) {
        fail_flag++;
        if (fail_flag >= 0x24000000) {
            break;
        }
    }
}

int result_check(bs_hw_once_cfg_s *cfg, uint8_t *gld)
{
    int errnum = 0;
    //checksum
#if 0
    uint32_t dut_isum = plat_read_reg(BSCALER_BASE + BSCALER_FRMC_ISUM);
    uint32_t dut_osum = plat_read_reg(BSCALER_BASE + BSCALER_FRMC_OSUM);
    uint32_t gld_isum = cfg->isum;
    uint32_t gld_osum = cfg->osum;
    if (gld_isum != dut_isum) {
        errnum++;
        plat_printf("error: frmc ISUM failed: %0x -> %0x \n", gld_isum, dut_isum);
    } else {
        plat_printf("frmc ISUM sucess: %0x -> %0x \n", gld_isum ,dut_isum);
    }

    if (gld_osum != dut_osum) {
        errnum++;
        plat_printf("error: frmc OSUM failed: %0x -> %0x \n", gld_osum ,dut_osum);
    } else {
        plat_printf("frmc OSUM sucess: %0x -> %0x \n", gld_osum ,dut_osum);
    }
#endif
    uint32_t bpp_mode = (cfg->dst_format >> 5) & 0x3;
    uint32_t bpp = 1 << (bpp_mode + 2);
    uint8_t *dut0 = cfg->dst_base[0];
    uint8_t *dut1 = cfg->dst_base[1];

    if (cfg->dst_format == BS_HW_DATA_FM_NV12) {
        uint8_t *gld0 = gld;
        for (int i = 0; i < cfg->dst_box_h; i++) {
            for (int j = 0; j < cfg->dst_box_w; j++) {
                int dut_idx = (cfg->dst_box_y + i) * cfg->dst_line_stride +
                    cfg->dst_box_x + j;
                int gld_idx = i * cfg->dst_box_w + j;
                if (gld0[gld_idx] != dut0[dut_idx]) {
                    errnum++;
                    plat_printf("(Y)(%d, %d)(G)%02x -- (E)%02x\n", i, j, gld0[gld_idx], dut0[dut_idx]);
                }
            }
        }
        uint8_t *gld1 = gld + cfg->dst_box_h * cfg->dst_box_w;
        for (int i = 0; i < cfg->dst_box_h/2; i++) {
            for (int j = 0; j < cfg->dst_box_w; j++) {
                int dut_idx = (cfg->dst_box_y/2  + i) * cfg->dst_line_stride +
                    cfg->dst_box_x + j;

                int gld_idx = i * cfg->dst_box_w + j;
                if (dut1[dut_idx] != gld1[gld_idx]) {
                    errnum++;
                    plat_printf("(C)(%d, %d)(G)%02x -- (E)%02x\n", i, j, gld1[gld_idx], dut1[dut_idx]);
                }
            }
        }
    } else {
        if (cfg->box_mode) {
            uint8_t *gld0 = gld;
            uint8_t *dut0 = cfg->dst_base[0];
            for (int j = 0; j < cfg->dst_box_h; j++) {
                for (int i = 0; i < cfg->dst_box_w; i++) {
                    for (int k = 0; k < bpp; k++) {
                        int dut_idx = (cfg->dst_box_y + j) * cfg->dst_line_stride +
                            (cfg->dst_box_x + i) * bpp + k;
                        int gld_idx = j * cfg->dst_box_w * bpp + i * bpp + k;
                        if (gld0[gld_idx] != dut0[dut_idx]) {
                            errnum++;
                            plat_printf("d(%d, %d, %d)(G)%02x -- (E)%02x\n", i, j, k, gld0[gld_idx], dut0[dut_idx]);
                        }
                    }
                }
            }
        } else {
            for (int bid = 0; bid < cfg->box_num; bid++) {
                uint8_t *gld0 = gld + bid * cfg->dst_box_h * cfg->dst_box_w * bpp;
                uint8_t *dut0 = cfg->dst_base[bid];
                for (int j = 0; j < cfg->dst_box_h; j++) {
                    for (int i = 0; i < cfg->dst_box_w; i++) {
                        for (int k = 0; k < bpp; k++) {
                            int dut_idx = j * cfg->dst_line_stride + i * bpp + k;
                            int gld_idx = j * cfg->dst_box_w * bpp + i * bpp + k;
                            if (dut0[dut_idx] != gld0[gld_idx]) {
                                errnum++;
                                plat_printf("bid:%d,(%d, %d)(G)%02x -- (E)%02x\n", bid, i, j, gld0[gld_idx], dut0[dut_idx]);
                            }
                        }
                    }
                }
            }
        }
    }
    return errnum;
}

void update_cfg(bs_hw_once_cfg_s *cfg)
{
    cfg->src_base0 = src0;
    cfg->src_base1 = src1;

    if (cfg->dst_format == BS_HW_DATA_FM_NV12) {
        int size = cfg->dst_line_stride * cfg->dst_box_h;
        cfg->dst_base[0] = plat_malloc(8, size);
        cfg->dst_base[1] = plat_malloc(8, size / 2);
    } else {
        uint32_t bpp_mode = (cfg->dst_format >> 5) & 0x3;
        int size = cfg->dst_line_stride * cfg->dst_box_h;
        if (bpp_mode == 0) {
            size *= 4;
        } else if (bpp_mode == 1) {
            size *= 8;
        } else if (bpp_mode == 2) {
            size *= 16;
        } else if (bpp_mode == 3) {
            size *= 32;
        }

        if (cfg->box_mode) {
            cfg->dst_base[0] = malloc(size);
        } else {
            cfg->boxes_info = boxes_info;
            void *dst_all = malloc(size * cfg->box_num);
            for (int i = 0; i < cfg->box_num; i++) {
                cfg->dst_base[i] = (uint8_t *)dst_all + size * i;
            }
        }
    }
}

inline void print_data_format(const char *fmt, bs_hw_data_format_e format)
{
    plat_printf("%s = %s\n", fmt,
                format == BS_HW_DATA_FM_NV12 ? "NV12" :
                format == BS_HW_DATA_FM_BGRA ? "BGRA" :
                format == BS_HW_DATA_FM_GBRA ? "GBRA" :
                format == BS_HW_DATA_FM_RBGA ? "RBGA" :
                format == BS_HW_DATA_FM_BRGA ? "BRGA" :
                format == BS_HW_DATA_FM_GRBA ? "GRBA" :
                format == BS_HW_DATA_FM_RGBA ? "RGBA" :
                format == BS_HW_DATA_FM_ABGR ? "ABGR" :
                format == BS_HW_DATA_FM_AGBR ? "AGBR" :
                format == BS_HW_DATA_FM_ARBG ? "ARBG" :
                format == BS_HW_DATA_FM_ABRG ? "ABRG" :
                format == BS_HW_DATA_FM_AGRB ? "AGRB" :
                format == BS_HW_DATA_FM_ARGB  ? "ARGB" :
                format == BS_HW_DATA_FM_F32_2B ? "F32_2B" :
                format == BS_HW_DATA_FM_F32_4B ? "F32_4B" :
                format == BS_HW_DATA_FM_F32_8B ? "F32_8B" : "unknown");
}

void dump_cfg_info(bs_hw_once_cfg_s *cfg)
{
    plat_printf("src_base0 = %p\n", cfg->src_base0);
    plat_printf("src_base1 = %p\n", cfg->src_base1);
    print_data_format("src_format", cfg->src_format);
    plat_printf("src_line_stride = %d\n", cfg->src_line_stride);
    plat_printf("src_box_x = %d\n", cfg->src_box_x);
    plat_printf("src_box_y = %d\n", cfg->src_box_y);
    plat_printf("src_box_w = %d\n", cfg->src_box_w);
    plat_printf("src_box_h = %d\n", cfg->src_box_h);

    print_data_format("dst_format", cfg->dst_format);
    plat_printf("dst_line_stride = %d\n", cfg->dst_line_stride);
    plat_printf("dst_box_x = %d\n", cfg->dst_box_x);
    plat_printf("dst_box_y = %d\n", cfg->dst_box_y);
    plat_printf("dst_box_w = %d\n", cfg->dst_box_w);
    plat_printf("dst_box_h = %d\n", cfg->dst_box_h);

    plat_printf("affine = %d\n", cfg->affine);
    plat_printf("box_mode = %d\n", cfg->box_mode);
    plat_printf("y_gain_exp = %d\n", cfg->y_gain_exp);
    plat_printf("zero_point = %d\n", cfg->zero_point);
    plat_printf("coef = {");
    for (int i = 0; i < 9; i++) {
        plat_printf("%d, ", cfg->coef[i]);
    }
    plat_printf("}\n");

    plat_printf("offset = {");
    for (int i = 0; i < 2; i++) {
        plat_printf("%d, ", cfg->offset[i]);
    }
    plat_printf("}\n");

    plat_printf("matrix = {");
    for (int i = 0; i < 9; i++) {
        plat_printf("%d, ", cfg->matrix[i]);
    }
    plat_printf("}\n");

    plat_printf("box_num = %d\n", cfg->box_num);

    plat_printf("dst_base :\n");
    for (int i = 0; i < 64; i++) {
        plat_printf("%p, ", cfg->dst_base[i]);
    }
    plat_printf("\n");

    if (cfg->box_mode == false) {
        for (int i = 0; i < cfg->box_num; i++) {
            plat_printf("%08x\n", cfg->boxes_info[i*6 + 0]);
            plat_printf("%08x\n", cfg->boxes_info[i*6 + 1]);
            plat_printf("%08x\n", cfg->boxes_info[i*6 + 2]);
            plat_printf("%08x\n", cfg->boxes_info[i*6 + 3]);
            plat_printf("%08x\n", cfg->boxes_info[i*6 + 4]);
            plat_printf("%08x\n", cfg->boxes_info[i*6 + 5]);
        }
    }
    plat_printf("mono_x = %08x\n", cfg->mono_x);
    plat_printf("mono_y = %08x\n", cfg->mono_y);
    plat_printf("extreme_point = {");
    for (int i = 0; i < 64; i++) {
        plat_printf("%d,", cfg->extreme_point[i]);
    }
    plat_printf("}\n");
    plat_printf("isum = %08x\n", cfg->isum);
    plat_printf("osum = %08x\n", cfg->osum);
    plat_printf("bus = %08x\n", cfg->bus);//[0]-chain,[1]-box,[2]-ibuf,[3]-obuf, 0-ddr, 1-oram
    plat_printf("irq_mask = %08x\n", cfg->irq_mask);//1 - mask
}

#ifdef CHIP_SIM_ENV
#define TEST_NAME  t_bscaler
#else
#define TEST_NAME  main
#endif
int TEST_NAME()
{
    uint32_t seed = (uint32_t)time(NULL);
    plat_printf("seed = %08x\n", seed);
    srand(seed);

    // 1. platform initialized
    plat_init();

    // 2. IP bscaler_init();
    // 2. bscaler software reset
    bscaler_softreset_set();

    uint32_t  frmt_ckg_set = 0;
    uint32_t  frmc_ckg_set = 0;
    //clock_gate
    bscaler_clkgate_mask_set(frmt_ckg_set, frmc_ckg_set);

    // 3. update frmt_cfg
    update_cfg(&cfg);

    //dump_cfg_info(&cfg);

    // 4. configure register
    bscaler_frmc_cfg(&cfg);

    // 5. wait finish
    bscaler_wait_finish();

    // 6. check result
    int errnum = result_check(&cfg, gld_dst);

    if (errnum) {
        plat_printf("================ Failed =================\n");
    } else {
        plat_printf("================ Passed =================\n");
    }

    plat_deinit();

    return 0;
}
#ifdef CHIP_SIM_ENV
TEST_MAIN(TEST_NAME);
#endif
