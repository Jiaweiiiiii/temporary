/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : t_bscaler_chain_c_t.c
 * Authors    : lzhu@abel.ic.jz.com
 * Create Time: 2020-08-27:15:44:54
 * Description:
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "t_vector_t_c_chain_c.h"
#include "t_vector_t_c_chain_t.h"
#ifdef CHIP_SIM_ENV
#include "bscaler_hal.c"
#endif
#include "platform.c"

#define L2_SIZE_OFST 0x00080000 //default 512KB
#define ORAM_VBASE  0xB2600000 + L2_SIZE_OFST
#define ORAM_PBASE  0x12600000 + L2_SIZE_OFST

#define ORAM_VDST   0xB26B0000
#define ORAM_PDST   0x126B0000

#define ORAM_VBOX   0xB26E0000
#define ORAM_PBOX   0x126E0000

uint32_t update_once_c_cfg(bs_chain_cfg_s *chain_cfg, bsc_hw_once_cfg_s *cfg,
                           uint32_t chain_ps, int n)
{
    uint32_t box_num;

    cfg->irq_mask  = 1;
    if (cfg->ibufc_bus == 0) {
        cfg->src_base0 = c_src[2 * n];
        cfg->src_base1 = c_src[2 * n + 1];
#ifndef SRC_CPU
        int size = cfg->src_line_stride * cfg->src_box_h;
        cfg->src_base0 = (uint8_t *)cMalloc(c_src[2 * n], size, 1);
        cfg->src_base1 = (uint8_t *)cMalloc(c_src[2 * n + 1], size, 1);
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
                cfg->boxes_info = c_boxes_info[n];
#ifndef SRC_CPU
                int info_size = cfg->box_num * 24;
                cfg->boxes_info = (uint32_t *)cMalloc(c_boxes_info[n], info_size, 1);
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

void update_once_t_cfg(bst_hw_once_cfg_s *cfg, uint32_t *chain_base, uint32_t n)
{
    uint32_t chain_ps = 22;
    cfg->irq_mask = 1;

    if (cfg->ibuft_bus == 0) {
        cfg->src_base0 = t_src[2 * n];
        cfg->src_base1 = t_src[2 * n + 1];
#ifndef SRC_CPU
        int src0_size = cfg->src_line_stride * cfg->src_h;
        int src1_size = cfg->src_line_stride * cfg->src_h/2;
        cfg->src_base0 = (uint8_t *)cMalloc(t_src[2 * n], src0_size, 1);
        cfg->src_base1 = (uint8_t *)cMalloc(t_src[2 * n + 1], src1_size, 1);
#endif
    } else {
        cfg->src_base0 = (uint8_t *)ORAM_VBASE;//fix me
        int size = cfg->src_line_stride * cfg->src_h;
        cfg->src_base1 = (uint8_t *)(ORAM_VBASE + size + 64);
    }

    if (cfg->dst_format == 3) {
        uint8_t frame_num = ((cfg->kernel_size == 3) ? 5 :
                             (cfg->kernel_size == 2) ? 3 : 1);
        if (cfg->obuft_bus & 0x1) {
            cfg->dst_base0 = (uint8_t *)ORAM_VDST;//oram fix me
        } else {
            cfg->dst_base0 = (uint8_t *)pf_malloc(1, frame_num*cfg->dst_plane_stride);
        }
    } else {
        if (cfg->task_len%2 != 0) {
            pf_printf("error: malloc dst failed! task_len = %0d\n", cfg->task_len);
        } else {
            if (cfg->obuft_bus & 0x1) {
                cfg->dst_base0 = (uint8_t *)ORAM_VDST;
                int base0_size = cfg->dst_line_stride*cfg->task_len;
                if (cfg->dst_format == 0) //nv12
                    //cfg->dst_base1 = (uint32_t)pf_malloc(1, cfg->dst_line_stride*cfg->task_len/2);
                    cfg->dst_base1 = (uint8_t *)ORAM_VDST + base0_size + 64;
            } else {
                cfg->dst_base0 = (uint8_t *)pf_malloc(1, cfg->dst_line_stride*cfg->task_len);
                if (cfg->dst_format == 0) //nv12
                    cfg->dst_base1 = (uint8_t *)pf_malloc(1, cfg->dst_line_stride*cfg->task_len/2);
            }
        }
    }

    //update chain
    chain_base[n * chain_ps + 0] = (cfg->src_base0 != 0) ? pf_va_2_pa(cfg->src_base0) : 0;
    chain_base[n * chain_ps + 2] = (cfg->src_base1 != 0) ? pf_va_2_pa(cfg->src_base1) : 0;
    chain_base[n * chain_ps + 4] = (cfg->dst_base0 != 0) ? pf_va_2_pa(cfg->dst_base0) : 0;
    chain_base[n * chain_ps + 6] = (cfg->dst_base1 != 0) ? pf_va_2_pa(cfg->dst_base1) : 0;
}

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
    bscaler_frmt_soft_reset();

    // 3. CHAIN ISUM CLEAR
    pf_write_reg(BSCALER_BASE + BSCALER_FRMC_ISUM, 0); //clear summary
    pf_write_reg(BSCALER_BASE + BSCALER_FRMC_OSUM, 0); //clear summary

    pf_write_reg(BSCALER_BASE + BSCALER_FRMT_ISUM, 0); //clear summary
    pf_write_reg(BSCALER_BASE + BSCALER_FRMT_OSUM, 0); //clear summary

    uint32_t  frmt_ckg_set = 0;
    uint32_t  frmc_ckg_set = 0;
    // 4. clock_gate
    bscaler_frmc_clkgate_mask_set(frmc_ckg_set);
    bscaler_frmt_clkgate_mask_set(frmt_ckg_set);

    // 5. update chain_base
    // for c
    uint32_t *bsc_chain_base;
    uint32_t bsc_chain_len = sizeof(c_chain_base);
    bsc_chain_base = (uint32_t *)cMalloc(c_chain_base, sizeof(c_chain_base), 1);
    pf_printf("============ 1 ================\n");

    bs_chain_cfg_s chain_cfg;
    chain_cfg.bs_chain_irq_mask  = 1;
    chain_cfg.c_timeout_irq_mask = 1;
    chain_cfg.bs_chain_base = bsc_chain_base;
    chain_cfg.bs_chain_len = sizeof(c_chain_base);

    uint32_t bst_chain_irq_mask = 1;
    uint32_t t_timeout_irq_mask = 1;

    //for t
    uint32_t *bst_chain_base;
    uint32_t bst_chain_len = sizeof(t_chain_base);
    bst_chain_base = (uint32_t *)cMalloc(t_chain_base, sizeof(t_chain_base), 1);
    pf_printf("============ 2 ================\n");

    // 6. update once_cfg
    // for c
    int c_frame = 2;//fix me //fix is 4
    uint32_t c_chain_ps = 0;
    uint32_t c_chain_ps_once = 0;
    for (int n = 0; n < c_frame; n ++) {
        c_chain_ps += c_chain_ps_once;
        c_chain_ps_once = update_once_c_cfg(&chain_cfg, c_cfg[n], c_chain_ps, n);
    }
    pf_printf("============ 3 ================\n");

    // for t
    int t_frame = 1;
    for (int n = 0; n < t_frame; n ++) {
        update_once_t_cfg(t_cfg[n], bst_chain_base, n);
    }
    pf_printf("============ 4 ================\n");

    // 7. cfg coef param
    uint32_t c_nv2bgr_order = (c_cfg[0]->dst_format >> 1) & 0xF;
    //uint32_t t_nv2bgr_order = t_cfg[0]->nv2bgr_order;//fixme
    uint32_t t_nv2bgr_order = (t_cfg[0]->->dst_format >> 4) & 0xF;
    bscaler_param_cfg(c_cfg[0]->coef, c_cfg[0]->offset,
                      c_cfg[0]->nv2bgr_alpha, c_nv2bgr_order, t_nv2bgr_order);
    pf_printf("============ 5 ================\n");

    // 8. hw_start
    uint32_t first_t_cfg = 0;
    uint32_t first_c_cfg = 0;
    uint32_t frame = 2;
    uint32_t c_errnum = 0;
    uint32_t t_errnum = 0;

    while (1) {
        if((pf_read_reg(BSCALER_BASE + BSCALER_FRMC_CHAIN_CTRL) & 0x1) == 0){
            pf_printf("============ 6 ================\n");

            if(first_c_cfg){
                //result check
                uint32_t dut_osum = pf_read_reg(BSCALER_BASE + BSCALER_FRMC_OSUM);
                uint32_t gld_osum = 0;
                for(int n = 0; n < frame; n++){
                    gld_osum += c_cfg[n]->osum;
                }
                if (gld_osum != dut_osum) {
                    c_errnum++;
                    pf_printf("FAILED: frmc OSUM failed: %0x -> %0x \n", gld_osum ,dut_osum);
                } else {
                    pf_printf("frmc OSUM sucess: %0x -> %0x \n", gld_osum ,dut_osum);
                }
            }
            pf_write_reg(BSCALER_BASE + BSCALER_FRMC_ISUM, 0); //clear summary
            pf_write_reg(BSCALER_BASE + BSCALER_FRMC_OSUM, 0); //clear summary
            bsc_chain_hw_cfg(&chain_cfg);
            first_c_cfg = 1;
        }
        if((pf_read_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_CTRL) & 0x1) == 0){
            pf_printf("============ 7 ================\n");
            if(first_t_cfg) {
                //result check
                uint32_t dut_osum = pf_read_reg(BSCALER_BASE + BSCALER_FRMT_OSUM);
                uint32_t gld_osum = 0; //= cfg->osum;
                for(int n = 0; n < t_frame; n++){
                    gld_osum += t_cfg[n]->osum;
                }
                if (gld_osum != dut_osum) {
                    t_errnum++;
                    pf_printf("error: frmt OSUM failed: %0x -> %0x \n", gld_osum ,dut_osum);
                } else {
                    pf_printf("frmt OSUM sucess: %0x -> %0x \n", gld_osum ,dut_osum);
                }
            }

            pf_write_reg(BSCALER_BASE + BSCALER_FRMT_ISUM, 0); //clear summary
            pf_write_reg(BSCALER_BASE + BSCALER_FRMT_OSUM, 0); //clear summary

            pf_write_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_BASE, pf_va_2_pa(bst_chain_base));
            pf_write_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_LEN, bst_chain_len);
            pf_write_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_CTRL, (bst_chain_irq_mask << 3 | //irq_mask
                                                                    t_timeout_irq_mask << 5 |
                                                                    0<<1 | //chain_bus
                                                                    1<<0));//start hw
            //delay for he start
            //svWaitTme(100);
            while ((pf_read_reg(BSCALER_BASE + BSCALER_FRMT_CTRL) & 0x1) == 0);
            pf_printf("FRMT HW START\n");
            pf_write_reg(BSCALER_BASE + BSCALER_FRMT_TASK, (t_cfg0->task_len << 16) | 1 | 1 << 1);

            first_t_cfg = 1;
        }
    }

    return 0;
}
#ifdef CHIP_SIM_ENV

TEST_MAIN(TEST_NAME);
#endif
