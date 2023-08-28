/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : t_bscaler_c_chain.c
 * Authors    : lzhu@abel.ic.jz.com
 * Create Time: 2020-08-10:17:06:06
 * Description:
 *
 */

//#define CHIP_SIM_ENV
#include <stdio.h>
#include <stdlib.h>
#include "t_vector_c_chain.h"
#ifdef CHIP_SIM_ENV
#include "bscaler_hal.c"
#include "platform.c"
#endif

#define L2_SIZE_OFST 0x00080000 //default 512KB(ORAM:1MBYTE-->100000)
#define ORAM_VBASE  0xB2600000 + L2_SIZE_OFST
#define ORAM_PBASE  0x12600000 + L2_SIZE_OFST

#define ORAM_VDST   0xB26B0000
#define ORAM_PDST   0x126B0000

#define ORAM_VBOX   0xB26E0000
#define ORAM_PBOX   0x126E0000

#define ORAM_VCHAIN 0xB26F0000
#define ORAM_PCHAIN 0x126F0000

#define OPEN_INT 0 //int only for chip
int int_flag =0;

uint32_t update_once_cfg (bs_chain_cfg_s *chain_cfg, bsc_hw_once_cfg_s *cfg,
                          uint32_t chain_ps, int n) {
    uint32_t box_num;

    cfg->irq_mask  = 1;
    if (cfg->ibufc_bus == 0) {
        cfg->src_base0 = src[2 * n];
        cfg->src_base1 = src[2 * n + 1];
#ifndef SRC_CPU
        int size = cfg->src_line_stride * cfg->src_box_h;
        cfg->src_base0 = (uint8_t *)cMalloc(src[2 * n], size, 1);
        cfg->src_base1 = (uint8_t *)cMalloc(src[2 * n + 1], size, 1);
#endif
    } else {
        cfg->src_base0 = (uint8_t *)ORAM_VBASE;
        int size = cfg->src_line_stride * cfg->src_box_h;
        cfg->src_base1 = (uint8_t *)(ORAM_VBASE + size + 64);
    }
    if (cfg->dst_format == BSC_HW_DATA_FM_NV12) {
        int size = cfg->dst_line_stride * cfg->dst_box_h;
        if (cfg->obufc_bus == 0) {
            cfg->dst_base[0] = (uint8_t *)pf_malloc(1, size);
            cfg->dst_base[1] = (uint8_t *)pf_malloc(1, size / 2);
        }
        else {
            cfg->dst_base[0] = (uint8_t *)ORAM_VDST;
            cfg->dst_base[1] = (uint8_t *)(ORAM_VDST + size + 64);
        }
    } else {
        uint32_t bpp_mode = (cfg->dst_format >> 5) & 0x3;
        int size = cfg->dst_line_stride * cfg->dst_box_h;
        if (cfg->box_mode) {
            if (cfg->obufc_bus == 0) {
                cfg->dst_base[0] = (uint8_t *)pf_malloc(1,size);
            } else {
                cfg->dst_base[0] = (uint8_t *)ORAM_VDST;
            }
        } else {
            if(cfg->box_bus == 0){
                cfg->boxes_info = boxes_info[n];
#ifndef SRC_CPU
                int info_size = cfg->box_num * 24;
                cfg->boxes_info = (uint32_t *)cMalloc(boxes_info[n], info_size, 1);
#endif
            }else {
                cfg->boxes_info = (uint32_t *)ORAM_VBOX;
            }
            void *dst_all;
            if(cfg->obufc_bus == 0){
                dst_all = pf_malloc(1,size * cfg->box_num);
            } else {
                dst_all = (void *)ORAM_VDST;
            }
            for (int i = 0; i < cfg->box_num; i++) {
                cfg->dst_base[i] = (uint8_t *)dst_all + size * i;
            }
        }
    }
    //update the cfg
    uint32_t dst_format = cfg->dst_format & 0x1;
    if (!dst_format) {
        box_num = 2;
    } else {
        box_num = cfg->box_num;
    }

    chain_cfg->bs_chain_base[chain_ps + 0] = (cfg->src_base0 != 0) ? pf_va_2_pa(cfg->src_base0) : 0;

    chain_cfg->bs_chain_base[chain_ps + 2] = (cfg->src_base1 != 0) ? pf_va_2_pa(cfg->src_base1) : 0;
    chain_cfg->bs_chain_base[chain_ps + 4] = (cfg->boxes_info != 0) ? pf_va_2_pa(cfg->boxes_info): 0;
    for(int i = 0; i < box_num; i ++){
        chain_cfg->bs_chain_base[chain_ps + 6 + 2*i] = (cfg->dst_base[i] != 0) ? pf_va_2_pa(cfg->dst_base[i]) : 0;

    }
    uint32_t  cfg_ext_times = (cfg->dst_box_w / 4) + (cfg->dst_box_w % 4) ? 1 : 0;
    chain_ps = (21 + box_num + cfg_ext_times) * 2;//unit 4byte

    return chain_ps;
}

int bscaler_wait_finish()
{
#if OPEN_INT
    int cnt = 0;
    while (int_flag != 1) {
        pf_printf("CHAIN CTRL REG:0x%x\n",pf_read_reg(BSCALER_BASE + BSCALER_FRMC_CHAIN_CTRL));
    }
#else
    //polling
    int fail_flag=0;
    while ((pf_read_reg(BSCALER_BASE + BSCALER_FRMC_CHAIN_CTRL) & 0x1) != 0){
        if (fail_flag >= 0x24000000) {
            break;
        }
    }
#endif
}

int result_check(bsc_hw_once_cfg_s *cfg, uint8_t *gld)
{
    int errnum = 0;
    //checksum
    uint32_t bpp_mode = (cfg->dst_format >> 5) & 0x3;
    uint32_t bpp = 1 << (bpp_mode + 2);
    uint8_t *dut0 = cfg->dst_base[0];
    uint8_t *dut1 = cfg->dst_base[1];

    if (cfg->dst_format == BSC_HW_DATA_FM_NV12) {
        uint8_t *gld0 = gld;
        for (int i = 0; i < cfg->dst_box_h; i++) {
            for (int j = 0; j < cfg->dst_box_w; j++) {
                int dut_idx = (cfg->dst_box_y + i) * cfg->dst_line_stride +
                    cfg->dst_box_x + j;
                int gld_idx = i * cfg->dst_box_w + j;
#ifdef SRC_CHIP
                uint8_t dut_val = CpuRead8(pf_va_2_pa(&dut0[dut_idx]));
#else
                uint8_t dut_val = dut0[dut_idx];
#endif
                if (gld0[gld_idx] != dut_val) {
                    errnum++;
                    pf_printf("(Y)(%d, %d)(G)%02x -- (E)%02x\n", i, j, gld0[gld_idx], dut0[dut_idx]);
                }
            }
        }
        uint8_t *gld1 = gld + cfg->dst_box_h * cfg->dst_box_w;
        for (int i = 0; i < cfg->dst_box_h/2; i++) {
            for (int j = 0; j < cfg->dst_box_w; j++) {
                int dut_idx = (cfg->dst_box_y/2  + i) * cfg->dst_line_stride +
                    cfg->dst_box_x + j;
                int gld_idx = i * cfg->dst_box_w + j;
#ifdef SRC_CHIP
                uint8_t dut_val = CpuRead8(pf_va_2_pa(&dut1[dut_idx]));
#else
                uint8_t dut_val = dut1[dut_idx];
#endif
                if (dut_val != gld1[gld_idx]) {
                    errnum++;
                    pf_printf("(C)(%d, %d)(G)%02x -- (E)%02x\n", i, j, gld1[gld_idx], dut1[dut_idx]);
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
#ifdef SRC_CHIP
                        uint8_t dut_val = CpuRead8(pf_va_2_pa(&dut0[dut_idx]));
#else
                        uint8_t dut_val = dut0[dut_idx];
#endif
                        if (gld0[gld_idx] != dut_val) {
                            errnum++;
                            pf_printf("d(dx:0x%x, dy:0x%x, cnum:0x%x)(G)%02x -- (E)%02x\n", i, j, k, gld0[gld_idx], dut0[dut_idx]);
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
#ifdef SRC_CHIP
                            uint8_t dut_val = CpuRead8(pf_va_2_pa(&dut0[dut_idx]));
#else
                            uint8_t dut_val = dut0[dut_idx];
#endif
                            if (dut_val != gld0[gld_idx]) {
                                errnum++;
                                pf_printf("bid:%d,(%d, %d)(G)%02x -- (E)%02x\n", bid, i, j, gld0[gld_idx], dut0[dut_idx]);
                            }
                        }
                    }
                }
            }
        }
    }
    return errnum;
}

void bsc_chain_int_handler()
{
    pf_printf("------BSC CHAIN INT here------\n");
    if ((pf_read_reg(BSCALER_BASE + BSCALER_FRMC_CHAIN_CTRL) & 0x00000010) >> 4){
        pf_write_reg(BSCALER_BASE + BSCALER_FRMC_CHAIN_CTRL,
                     pf_read_reg(BSCALER_BASE + BSCALER_FRMC_CHAIN_CTRL) & 0xffffffef);
    }
    int_flag = 1;
    pf_printf("int_c_chain_flag:%d\n", int_flag);

}

#define INTC_BSC  39
#ifdef CHIP_SIM_ENV
#define TEST_NAME  t_bscaler
#else
#define TEST_NAME  main
#endif

int TEST_NAME()
{
    pf_printf("============ 0 ================\n");
    // 1. platform initialized
    pf_init();

    // 2. IP bscaler_init();
    // 2. bscaler software reset
    // bscaler_softreset_set();
    bscaler_frmc_soft_reset();

#if OPEN_INT
#ifdef CHIP_SIM_ENV
    pf_write_reg(IMR,0x0);
    pf_write_reg(IMR1,0x00100000);//NOT
    pf_write_reg(IMCR,0xffffffff);
    pf_write_reg(IMCR1,0xffffffff);
#endif
    setvbr(TP_INT | INTC_BSC, bsc_chain_int_handler);
#endif
    //summary clear
    pf_write_reg(BSCALER_BASE + BSCALER_FRMC_ISUM, 0); //clear summary
    pf_write_reg(BSCALER_BASE + BSCALER_FRMC_OSUM, 0); //clear summary

    uint32_t  frmt_ckg_set = 0;
    uint32_t  frmc_ckg_set = 0;
    //clock_gate
    //bscaler_clkgate_mask_set(frmt_ckg_set, frmc_ckg_set);
    bscaler_frmc_clkgate_mask_set(frmc_ckg_set);

    // 3. update chain_base
    bs_chain_cfg_s chain_cfg;
#if OPEN_INT
    chain_cfg.bs_chain_irq_mask  = 0;
    chain_cfg.c_timeout_irq_mask = 1;
#else
    chain_cfg.bs_chain_irq_mask  = 1;
    chain_cfg.c_timeout_irq_mask = 1;
#endif

    chain_cfg.bs_chain_base = (uint32_t *)cMalloc(chain_base, sizeof(chain_base), 1);
    chain_cfg.bs_chain_len = sizeof(chain_base);

    // 4. update update once_cfg
    int frame = 2;//fix me //fix is 4
    uint32_t chain_ps = 0;
    uint32_t chain_ps_once = 0;
    for (int n = 0; n < frame; n ++) {
        chain_ps += chain_ps_once;
        chain_ps_once = update_once_cfg(&chain_cfg, cfg[n], chain_ps, n);
    }

    //5. cfg coef param
    uint32_t nv2bgr_order = (cfg[0]->dst_format >> 1) & 0xF;
    bscaler_param_cfg(cfg[0]->coef, cfg[0]->offset,
                      cfg[0]->nv2bgr_alpha, nv2bgr_order);

    // 6. cfg and start the hw
    bsc_chain_hw_cfg(&chain_cfg);

    // 6. wait finish
    bscaler_wait_finish();

    // 7. check result
    int errnum = 0;
    for (int n = 0; n < frame; n++) {
        errnum += result_check(cfg[n], gld_dst[n]);
        pf_printf("frame %d check finish: 0x%d\n", n, errnum);
    }
#if 1
    uint32_t dut_isum = pf_read_reg(BSCALER_BASE + BSCALER_FRMC_ISUM);
    uint32_t dut_osum = pf_read_reg(BSCALER_BASE + BSCALER_FRMC_OSUM);
    //uint32_t gld_isum = cfg->isum;
    uint32_t gld_osum = 0; //= cfg->osum;
    for(int n = 0; n < frame; n++){
        gld_osum += cfg[n]->osum;
    }
    /*if (gld_isum != dut_isum) {
      errnum++;
      pf_printf("error: frmc ISUM failed: %0x -> %0x \n", gld_isum, dut_isum);
      } else {
      pf_printf("frmc ISUM sucess: %0x -> %0x \n", gld_isum ,dut_isum);
      }*/

    if (gld_osum != dut_osum) {
        errnum++;
        pf_printf("error: frmc OSUM failed: %0x -> %0x \n", gld_osum ,dut_osum);
    } else {
        pf_printf("frmc OSUM sucess: %0x -> %0x \n", gld_osum ,dut_osum);
    }
#endif

    if (errnum) {
        pf_printf("================ Failed =================\n");
        pf_deinit();
        return 1;
    } else {
        pf_printf("================ Passed =================\n");
        pf_deinit();
        return 0;
    }
}
#ifdef CHIP_SIM_ENV
TEST_MAIN(TEST_NAME);
#endif
