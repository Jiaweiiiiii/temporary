/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : t_bscaler_t.c
 * Authors    : jmqi@joshua
 * Create Time: 2020-06-26:22:43:49
 * Description:
 *
 */

#define CHIP_SIM_ENV
#include <stdio.h>
#include <stdlib.h>
#include "t_vector_t.h"
#ifdef CHIP_SIM_ENV
#include "bscaler_hw_api.c"
#endif
#include "platform.c"

#define OPEN_INT    1
int int_flag = 0;

int bst_wait_finish()
{
#if OPEN_INT
    //polling
    int fail_flag=0;
    while ((plat_read_reg(BSCALER_BASE + BSCALER_FRMT_CTRL) & 0x1) != 0) {
        fail_flag++;
        if (fail_flag >= 0x24000000) {
            break;
        }
    }
#else
    //interrupt
    int cnt = 0;
    while (int_flag != 1) {
        cnt++;
        if (cnt >= 0x24000000) {
            break;
        }
    }
#endif
}

int result_check(bst_hw_once_cfg_s *cfg, uint8_t *gld)
{
    int errnum = 0;
    //checksum
    uint32_t dut_isum = plat_read_reg(BSCALER_BASE + BSCALER_FRMT_ISUM);
    uint32_t dut_osum = plat_read_reg(BSCALER_BASE + BSCALER_FRMT_OSUM);
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

    uint32_t bpp_mode = (cfg->dst_format >> 5) & 0x3;
    uint32_t bpp = 1 << (bpp_mode + 2);
    uint8_t *dut0 = cfg->dst_base0;
    uint8_t *dut1 = cfg->dst_base1;

    if (cfg->dst_format == 0) {//nv12
        uint8_t *gld0 = gld;
        for (int i = 0; i < cfg->frmt_task_len; i++) {
            for (int j = 0; j < cfg->src_w; j++) {
                int dut_idx = i * cfg->dst_line_stride + j;
                int gld_idx = i * cfg->src_w + j;
                if (gld0[gld_idx] != dut0[dut_idx]) {
                    errnum++;
                    plat_printf("(Y)(%d, %d)(G)%02x -- (E)%02x\n", i, j, gld0[gld_idx], dut0[dut_idx]);
                }
            }
        }
        uint8_t *gld1 = gld + cfg->src_h * cfg->src_w;
        for (int i = 0; i < cfg->frmt_task_len/2; i++) {
            for (int j = 0; j < cfg->src_w; j++) {
                int dut_idx = i * cfg->dst_line_stride + j;
                int gld_idx = i * cfg->src_w + j;
                if (dut1[dut_idx] != gld1[gld_idx]) {
                    errnum++;
                    plat_printf("(C)(%d, %d)(G)%02x -- (E)%02x\n", i, j, gld1[gld_idx], dut1[dut_idx]);
                }
            }
        }
    } else if (cfg->dst_format == 1) {//bgr
        uint8_t *gld0 = gld;
        uint8_t *dut0 = cfg->dst_base0;
        for (int j = 0; j < cfg->frmt_task_len; j++) {
            for (int i = 0; i < cfg->src_w; i++) {
                for (int k = 0; k < bpp; k++) {
                    int dut_idx = j * cfg->dst_line_stride +
                        i * bpp + k;
                    int gld_idx = j * cfg->src_w * bpp + i * bpp + k;
                    if (gld0[gld_idx] != dut0[dut_idx]) {
                        errnum++;
                        plat_printf("(%d, %d)(G)%02x -- (E)%02x\n", i, j, gld0[gld_idx], dut0[dut_idx]);
                    }
                }
            }
        }
    } else if (cfg->dst_format == 3) {//virchn
        //TODO
        //fixme
    }
    return errnum;
}

void update_cfg(bst_hw_once_cfg_s *cfg)
{
    cfg->src_base0 = src0;
    cfg->src_base1 = src1;
#if OPEN_INT
    cfg->irq_mask = 0;
#else
    cfg->irq_mask = 1;
#endif

    if (cfg->dst_format == 3) {
        uint8_t frame_num = ((cfg->kernel_size == 3) ? 5 :
                             (cfg->kernel_size == 2) ? 3 : 1);
        if (cfg->frmt_obuft_bus & 0x1) {
            cfg->dst_base0 = (uint8_t *)(0xB2680000);//oram
        } else {
            cfg->dst_base0 = (uint32_t)plat_malloc(1, frame_num*cfg->frmt_fs_dst);
        }
    } else {
        if (cfg->frmt_task_len%2 != 0) {
            plat_printf("error: malloc dst failed! task_len = %0d\n", cfg->frmt_task_len);
        } else {
            if (cfg->frmt_obuft_bus & 0x1) {
                if (cfg->dst_format == 0) //nv12
                    cfg->dst_base1 = (uint32_t)plat_malloc(1, cfg->dst_line_stride*cfg->frmt_task_len/2);
            } else {
                cfg->dst_base0 = (uint32_t)plat_malloc(1, cfg->dst_line_stride*cfg->frmt_task_len);
                if (cfg->dst_format == 0) //nv12
                    cfg->dst_base1 = (uint32_t)plat_malloc(1, cfg->dst_line_stride*cfg->frmt_task_len/2);
            }
        }
    }
}

#if 1
void dump_cfg_info(bst_hw_once_cfg_s *cfg)
{
    plat_printf("src_format = %d\n", cfg->src_format);
    plat_printf("dst_format = %d\n", cfg->dst_format);
    plat_printf("kernel_size = %d\n", cfg->kernel_size);
    plat_printf("kernel_xstride = %d\n", cfg->kernel_xstride);
    plat_printf("kernel_ystride = %d\n", cfg->kernel_ystride);
    plat_printf("kernel_dummy_val = %d\n", cfg->kernel_dummy_val);
    plat_printf("panding_lf = %d\n", cfg->panding_lf);
    plat_printf("panding_rt = %d\n", cfg->panding_rt);
    plat_printf("panding_tp = %d\n", cfg->panding_tp);
    plat_printf("panding_bt = %d\n", cfg->panding_bt);
    plat_printf("frmt_task_len = %d\n", cfg->frmt_task_len);
    plat_printf("frmt_chain_bus = %d\n", cfg->frmt_chain_bus);
    plat_printf("frmt_ibuft_bus = %d\n", cfg->frmt_ibuft_bus);
    plat_printf("frmt_obuft_bus = %d\n", cfg->frmt_obuft_bus);
    plat_printf("src_base0 = %p\n", cfg->src_base0);
    plat_printf("src_base1 = %p\n", cfg->src_base1);
    plat_printf("src_w = %d\n", cfg->src_w);
    plat_printf("src_h = %d\n", cfg->src_h);
    plat_printf("src_line_stride = %d\n", cfg->src_line_stride);
    plat_printf("dst_base0 = %p\n", cfg->dst_base0);
    plat_printf("dst_base1 = %p\n", cfg->dst_base1);
    plat_printf("dst_line_stride = %d\n", cfg->dst_line_stride);
    plat_printf("frmt_fs_dst = %d\n", cfg->frmt_fs_dst);
    plat_printf("nv2bgr_order = %d\n", cfg->nv2bgr_order);
    plat_printf("nv2bgr_coef = {");
    for (int i = 0; i < 9; i++) {
        plat_printf("%d, ", cfg->nv2bgr_coef[i]);
    }
    plat_printf("}\n");

    plat_printf("nv2bgr_ofst = {");
    for (int i = 0; i < 2; i++) {
        plat_printf("%d, ", cfg->nv2bgr_ofst[i]);
    }
    plat_printf("}\n");

    plat_printf("isum = %d\n", cfg->isum);
    plat_printf("osum = %d\n", cfg->osum);
    plat_printf("irq_mask = %d\n", cfg->irq_mask);
}
#endif

void bscaler_int_handler()
{
    plat_write_reg(BSCALER_BASE + BSCALER_FRMT_CTRL, 0);
    int_flag = 1;
    //TODO, check bscaler stat
}

#define INTC_BSCALER  19

#ifdef CHIP_SIM_ENV
#define TEST_NAME  t_bscaler
#else
#define TEST_NAME  main
#endif
int TEST_NAME()
{
    plat_printf("============ 0 ================\n");
    // 1. platform initialized
    plat_init();
    int int_flag = 0;

    plat_printf("============ 1 ================\n");
    // 2. IP bscaler_init();
    // 2. bscaler software reset
    bscaler_softreset_set();

    setvbr(TP_INT | INTC_BSCALER, bscaler_int_handler);

    uint32_t  frmt_ckg_set = 0;
    uint32_t  frmc_ckg_set = 0;
    //clock_gate
    plat_printf("============ 2 ================\n");
    bscaler_clkgate_mask_set(frmt_ckg_set, frmc_ckg_set);

    // 3. update frmt_cfg
    plat_printf("============ 3 ================\n");
    update_cfg(&cfg);

    plat_printf("============ 4 ================\n");
    dump_cfg_info(&cfg);
    plat_printf("============ 5 ================\n");
    // 4. configure register
    bscaler_frmt_cfg(&cfg);

    plat_printf("============ 6 ================\n");

    int32_t pixel_kernel_h = ((cfg.kernel_size == 3) ? 7 :
                              (cfg.kernel_size == 2) ? 5 :
                              (cfg.kernel_size == 1) ? 3 : 1);
    uint32_t kernel_frame_h = (cfg.src_h + cfg.panding_tp + cfg.panding_bt - pixel_kernel_h) / cfg.kernel_ystride + 1;

    uint32_t imp_task_ynum = (cfg.dst_format == 3) ? kernel_frame_h : cfg.src_h;

    plat_printf("============ 7 ================\n");
    plat_printf("%08x\n", BSCALER_BASE + BSCALER_FRMT_TASK);
    plat_printf("val : %08x\n", (cfg.frmt_task_len << 16) | 1 | 1 << 1);
    plat_write_reg(BSCALER_BASE + BSCALER_FRMT_TASK, (cfg.frmt_task_len << 16) | 1 | 1 << 1);
    plat_printf("============ 8 ================\n");
    int imp_task_y = 0;
    while (1) {
        plat_printf("============ 9 ================\n");
        if ((plat_read_reg(BSCALER_BASE + BSCALER_FRMT_TASK) & 0x1) == 0) {//task finish
            plat_printf("============ 10 ================\n");
            uint32_t frmt_task_len_real = (((imp_task_y + cfg.frmt_task_len) > imp_task_ynum) ?
                                           (imp_task_ynum - imp_task_y) : cfg.frmt_task_len);
            imp_task_y += cfg.frmt_task_len;
            if (imp_task_y >= imp_task_ynum) {
                plat_printf("============ 12 ================\n");
                break;
            } else {
                plat_write_reg(BSCALER_BASE + BSCALER_FRMT_TASK, (cfg.frmt_task_len << 16) | 1 | 1 << 1);
                plat_printf("============ 11 ================\n");
            }
        }
    }
    plat_printf("============ 13 ================\n");
    // 5. wait finish
    bst_wait_finish();
    plat_printf("============ 14 ================\n");
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
