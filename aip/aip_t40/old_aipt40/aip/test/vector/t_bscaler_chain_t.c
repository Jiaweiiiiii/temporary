/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : t_bscaler_t_chain.c
 * Authors    : lzhu@abel.ic.jz.com
 * Create Time: 2020-08-20:11:39:39
 * Description:
 *
 */

//#define CHIP_SIM_ENV
#include <stdio.h>
#include <stdlib.h>
#include "t_vector_t_chain.h"
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

#define OPEN_INT 1 //int only for chip
int int_flag =0;

void update_once_cfg(bst_hw_once_cfg_s *cfg, uint32_t *chain_base, uint32_t n)
{
    uint32_t chain_ps = 22;
    cfg->irq_mask = 1;

    if (cfg->ibuft_bus == 0) {
        cfg->src_base0 = src[2 * n];
        cfg->src_base1 = src[2 * n + 1];
#ifndef SRC_CPU
        int src0_size = cfg->src_line_stride * cfg->src_h;
        int src1_size = cfg->src_line_stride * cfg->src_h/2;
        cfg->src_base0 = (uint8_t *)cMalloc(src[2 * n], src0_size, 1);
        cfg->src_base1 = (uint8_t *)cMalloc(src[2 * n + 1], src1_size, 1);
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

int bscaler_wait_finish()
{
#if OPEN_INT
    int cnt = 0;
    while (int_flag != 1) {
        pf_printf("CHAIN CTRL REG:0x%x\n",pf_read_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_CTRL));
    }
#else
    //polling
    int fail_flag=0;
    while ((pf_read_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_CTRL) & 0x1) != 0){
        if (fail_flag >= 0x24000000) {
            break;
        }
    }
#endif
}

int result_check(bst_hw_once_cfg_s *cfg, uint8_t *gld)
{
    int errnum = 0;

    uint32_t bpp_mode = (cfg->dst_format >> 5) & 0x3;
    uint32_t bpp = 1 << (bpp_mode + 2);
    uint8_t *dut0;
    uint8_t *dut1;
    if (cfg->obuft_bus == 0) {
        dut0 = cfg->dst_base0;
        dut1 = cfg->dst_base1;
    } else {
        dut0 =(uint8_t *)ORAM_VDST;
        dut1 =(uint8_t *)(ORAM_VDST + 0x10000); //fix me
    }

    if (cfg->dst_format == 0) {//nv12
        uint8_t *gld0 = gld;
        int i,j;
        for (i = 0; i < cfg->task_len; i++) {
            for (j = 0; j < cfg->src_w; j++) {
                int dut_idx = i * cfg->dst_line_stride + j;
                int gld_idx = i * cfg->src_w + j;
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
        uint8_t *gld1 = gld + cfg->src_h * cfg->src_w;
        for (i = 0; i < cfg->task_len/2; i++) {
            for (j = 0; j < cfg->src_w; j++) {
                int dut_idx = i * cfg->dst_line_stride + j;
                int gld_idx = i * cfg->src_w + j;
#ifdef  SRC_CHIP
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
    } else if (cfg->dst_format == 1) {//bgr
        uint8_t *gld0 = gld;
        uint8_t *dut0 = cfg->dst_base0;
        int j,i,k;
        for (j = 0; j < cfg->task_len; j++) {
            for (i = 0; i < cfg->src_w; i++) {
                for (k = 0; k < bpp; k++) {
                    int dut_idx = j * cfg->dst_line_stride +
                        i * bpp + k;
                    int gld_idx = j * cfg->src_w * bpp + i * bpp + k;
#ifdef  SRC_CHIP
                    uint8_t dut_val = CpuRead8(pf_va_2_pa(&dut0[dut_idx]));
#else
                    uint8_t dut_val = dut0[dut_idx];
#endif
                    if (gld0[gld_idx] != dut_val) {
                        errnum++;
                        pf_printf("(%d, %d)(G)%02x -- (E)%02x\n", i, j, gld0[gld_idx], dut0[dut_idx]);
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

void bsc_chain_int_handler()
{
    pf_printf("------BSC CHAIN INT here------\n");
    if ((pf_read_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_CTRL) & 0x00000010) >> 4){
        pf_write_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_CTRL,
                     pf_read_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_CTRL) & 0xffffffef);
    }
    int_flag = 1;
    pf_printf("int_t_chain_flag:%d\n", int_flag);
}

#define INTC_BSC  40
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
    //bscaler_softreset_set();
    bscaler_frmt_soft_reset();

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
    pf_write_reg(BSCALER_BASE + BSCALER_FRMT_ISUM, 0); //clear summary
    pf_write_reg(BSCALER_BASE + BSCALER_FRMT_OSUM, 0); //clear summary

    uint32_t  frmt_ckg_set = 0;
    uint32_t  frmc_ckg_set = 0;
    //clock_gate
    //bscaler_clkgate_mask_set(frmt_ckg_set, frmc_ckg_set);
    bscaler_frmt_clkgate_mask_set(frmt_ckg_set);

    uint32_t bs_chain_irq_mask = 0;
    uint32_t t_timeout_irq_mask = 0;

#if OPEN_INT
    bs_chain_irq_mask  = 0;
    t_timeout_irq_mask = 1;
#else
    bs_chain_irq_mask  = 1;
    t_timeout_irq_mask = 1;
#endif

    // 3. update chain_base
    uint32_t *bs_chain_base;
    uint32_t bs_chain_len = sizeof(chain_base);
#ifdef SRC_CPU
    bs_chain_base = chain_base;
#else
    bs_chain_base = (uint32_t *)cMalloc(chain_base, sizeof(chain_base), 1);
#endif
    // 4. update once_cfg
    int frame = 2;
    int n;
    for (n = 0; n < frame; n ++) {
        update_once_cfg(cfg[n], bs_chain_base, n);
    }
    // 5. coef param
    bscaler_param_cfg(cfg[0]->nv2bgr_coef, cfg[0]->nv2bgr_ofst,
                      cfg[0]->nv2bgr_alpha, cfg[0]->nv2bgr_order);

    // 6. cfg and start the hw
    pf_write_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_BASE, pf_va_2_pa(bs_chain_base));
    pf_write_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_LEN, bs_chain_len);
    pf_write_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_CTRL, (bs_chain_irq_mask << 3 | //irq_mask
                                                          t_timeout_irq_mask << 5 |
                                                          0<<1 | //chain_bus
                                                          1<<0));//start hw
    // 7. start chain_task
    for (n = 0; n < frame; n ++) {
        // wait frmt_busy and start task
        while ((pf_read_reg(BSCALER_BASE + BSCALER_FRMT_CTRL) & 0x1) == 0);

        int32_t pixel_kernel_h = ((cfg[n]->kernel_size == 3) ? 7 :
                                  (cfg[n]->kernel_size == 2) ? 5 :
                                  (cfg[n]->kernel_size == 1) ? 3 : 1);
        uint32_t kernel_frame_h = (cfg[n]->src_h + cfg[n]->pad_top + cfg[n]->pad_bottom - pixel_kernel_h) / cfg[n]->kernel_ystride + 1;

        uint32_t imp_task_ynum = (cfg[n]->dst_format == 3) ? kernel_frame_h : cfg[n]->src_h;

        pf_printf("============ 7 ================\n");
        pf_printf("%08x\n", BSCALER_BASE + BSCALER_FRMT_TASK);
        pf_printf("val : %08x\n", (cfg[n]->task_len << 16) | 1 | 1 << 1);
        pf_write_reg(BSCALER_BASE + BSCALER_FRMT_TASK, (cfg[n]->task_len << 16) | 1 | 1 << 1);
        pf_printf("============ 8 ================\n");
        int imp_task_y = 0;
        while (1) {
            pf_printf("============ 9 ================\n");
            if ((pf_read_reg(BSCALER_BASE + BSCALER_FRMT_TASK) & 0x1) == 0) {//task finish
                pf_printf("============ 10 ================\n");
                uint32_t task_len_real = (((imp_task_y + cfg[n]->task_len) > imp_task_ynum) ?
                                          (imp_task_ynum - imp_task_y) : cfg[n]->task_len);
                imp_task_y += cfg[n]->task_len;
                if (imp_task_y >= imp_task_ynum) {
                    pf_printf("============ 12 ================\n");
                    break;
                } else {
                    pf_write_reg(BSCALER_BASE + BSCALER_FRMT_TASK, (cfg[n]->task_len << 16) | 1 | 1 << 1);
                    pf_printf("============ 11 ================\n");
                }
            }
        }
    }

    // 8. wait finish
    bscaler_wait_finish();

    // 9. check result
    int errnum = 0;
    for (int n = 0; n < frame; n++) {
        errnum += result_check(cfg[n], gld_dst[n]);
        pf_printf("frame %d check finish: 0x%d\n", n, errnum);
    }

    uint32_t dut_isum = pf_read_reg(BSCALER_BASE + BSCALER_FRMT_ISUM);
    uint32_t dut_osum = pf_read_reg(BSCALER_BASE + BSCALER_FRMT_OSUM);
    uint32_t gld_isum = 0; //cfg->isum;
    uint32_t gld_osum = 0; //= cfg->osum;
    for(int n = 0; n < frame; n++){
        gld_isum += cfg[n]->isum;
    }

    for(int n = 0; n < frame; n++){
        gld_osum += cfg[n]->osum;
    }
    if (gld_isum != dut_isum) {
        errnum++;
        pf_printf("error: frmt ISUM failed: %0x -> %0x \n", gld_isum, dut_isum);
    } else {
        pf_printf("frmt ISUM sucess: %0x -> %0x \n", gld_isum ,dut_isum);
    }

    if (gld_osum != dut_osum) {
        errnum++;
        pf_printf("error: frmt OSUM failed: %0x -> %0x \n", gld_osum ,dut_osum);
    } else {
        pf_printf("frmt OSUM sucess: %0x -> %0x \n", gld_osum ,dut_osum);
    }

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
