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

//#define CHIP_SIM_ENV
#include <stdio.h>
#include <stdlib.h>
#include "t_vector_t.h"
#ifdef CHIP_SIM_ENV
#include "bscaler_hal.c"
#include "platform.c"
#endif

#define L2_SIZE_OFST 0x00080000 //default 512KB
#define ORAM_VBASE  0xB2600000 + L2_SIZE_OFST
#define ORAM_PBASE  0x12600000 + L2_SIZE_OFST

#define ORAM_VDST   0xB26B0000
#define ORAM_PDST   0x126B0000

#define ORAM_VBOX   0xB26E0000
#define ORAM_PBOX   0x126E0000

#define OPEN_INT    1

int int_flag = 0;
int int_timeout_flag = 0;
int int_task_flag = 0;

int bst_wait_finish()
{
#if OPEN_INT
    //interrupt
    int cnt = 0;
    while (int_flag != 1) {
        pf_printf("FRMT CTRL READ REG:0x%x\n",pf_read_reg(BSCALER_BASE + BSCALER_FRMT_CTRL));
    }
#else
    //polling
    int fail_flag=0;
    do {
        fail_flag++;
        if((pf_read_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_CTRL) & 0x40) >> 6){
            pf_printf("TIMEOUT HIT\n");
            //clear timeout flag
            pf_write_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_CTRL,
                         pf_read_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_CTRL) & 0xffffffbf);
        }

        if (fail_flag >= 0x24000000) {
            break;
        }

    } while((pf_read_reg(BSCALER_BASE + BSCALER_FRMT_CTRL) & 0x1) != 0);
#endif
}

int result_check(bst_hw_once_cfg_s *cfg, uint8_t *gld)
{
    int errnum = 0;
    //checksum
    uint32_t dut_isum = pf_read_reg(BSCALER_BASE + BSCALER_FRMT_ISUM);
    uint32_t dut_osum = pf_read_reg(BSCALER_BASE + BSCALER_FRMT_OSUM);
    uint32_t gld_isum = cfg->isum;
    uint32_t gld_osum = cfg->osum;
    if (gld_isum != dut_isum) {
        errnum++;
        pf_printf("error: frmc ISUM failed: %0x -> %0x \n", gld_isum, dut_isum);
    } else {
        pf_printf("frmc ISUM sucess: %0x -> %0x \n", gld_isum ,dut_isum);
    }

    if (gld_osum != dut_osum) {
        errnum++;
        pf_printf("error: frmc OSUM failed: %0x -> %0x \n", gld_osum ,dut_osum);
    } else {
        pf_printf("frmc OSUM sucess: %0x -> %0x \n", gld_osum ,dut_osum);
    }

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
#ifdef SRC_CHIP
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

void update_cfg(bst_hw_once_cfg_s *cfg)
{
#if OPEN_INT
    cfg->irq_mask = 0;
#else
    cfg->irq_mask = 1;
#endif
    if (cfg->ibuft_bus == 0) {
        cfg->src_base0 = src00;
        cfg->src_base1 = src01;
#if (defined CHIP_SIM_ENV) && (!defined SRC_CPU)
        cfg->src_base0 = (uint8_t *)cMalloc(src00, sizeof(src00), 1);
        cfg->src_base1 = (uint8_t *)cMalloc(src01, sizeof(src01), 1);
#endif
    } else {
        cfg->src_base0 = (uint8_t *)ORAM_VBASE;
        int size = cfg->src_line_stride * cfg->src_h;
        cfg->src_base1 = (uint8_t *)(ORAM_VBASE + size + 64);
    }

    if (cfg->dst_format == 3) {
        uint8_t frame_num = ((cfg->kernel_size == 3) ? 5 :
                             (cfg->kernel_size == 2) ? 3 : 1);
        if (cfg->obuft_bus & 0x1) {
            cfg->dst_base0 = (uint8_t *)ORAM_VDST;//oram
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
}

#if 1
void dump_cfg_info(bst_hw_once_cfg_s *cfg)
{
    pf_printf("src_format = %d\n", cfg->src_format);
    pf_printf("dst_format = %d\n", cfg->dst_format);
    pf_printf("kernel_size = %d\n", cfg->kernel_size);
    pf_printf("kernel_xstride = %d\n", cfg->kernel_xstride);
    pf_printf("kernel_ystride = %d\n", cfg->kernel_ystride);
    pf_printf("zero_point = %d\n", cfg->zero_point);
    pf_printf("pad_left = %d\n", cfg->pad_left);
    pf_printf("pad_right = %d\n", cfg->pad_right);
    pf_printf("pad_top = %d\n", cfg->pad_top);
    pf_printf("pad_bottom = %d\n", cfg->pad_bottom);
    pf_printf("task_len = %d\n", cfg->task_len);
    pf_printf("chain_bus = %d\n", cfg->chain_bus);
    pf_printf("ibuft_bus = %d\n", cfg->ibuft_bus);
    pf_printf("obuft_bus = %d\n", cfg->obuft_bus);
    pf_printf("src_base0 = %p\n", cfg->src_base0);
    pf_printf("src_base1 = %p\n", cfg->src_base1);
    pf_printf("src_w = %d\n", cfg->src_w);
    pf_printf("src_h = %d\n", cfg->src_h);
    pf_printf("src_line_stride = %d\n", cfg->src_line_stride);
    pf_printf("dst_base0 = %p\n", cfg->dst_base0);
    pf_printf("dst_base1 = %p\n", cfg->dst_base1);
    pf_printf("dst_line_stride = %d\n", cfg->dst_line_stride);
    pf_printf("dst_plane_stride = %d\n", cfg->dst_plane_stride);
    pf_printf("nv2bgr_coef = {");
    int i;
    for (i = 0; i < 9; i++) {
        pf_printf("%d, ", cfg->nv2bgr_coef[i]);
    }
    pf_printf("}\n");

    pf_printf("nv2bgr_ofst = {");
    for (i = 0; i < 2; i++) {
        pf_printf("%d, ", cfg->nv2bgr_ofst[i]);
    }
    pf_printf("}\n");

    pf_printf("isum = %d\n", cfg->isum);
    pf_printf("osum = %d\n", cfg->osum);
    pf_printf("irq_mask = %d\n", cfg->irq_mask);
}
#endif

void bscaler_int_handler()
{
    pf_printf("BST INTC HERE!\n");
    //pf_write_reg(BSCALER_BASE + BSCALER_FRMT_CTRL, 0);
    //int_flag = 1;

    if ((pf_read_reg(BSCALER_BASE + BSCALER_FRMT_CTRL) & 0x00000100) >> 8){
        pf_printf("BST  T INT here\n");
        pf_write_reg(BSCALER_BASE + BSCALER_FRMT_CTRL,
                     pf_read_reg(BSCALER_BASE + BSCALER_FRMT_CTRL) & 0xfffffeff);
        int_flag = 1;
        pf_printf("int_t_flag:%d\n", int_flag);
    }

    if ((pf_read_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_CTRL) & 0x40) >> 6){
        pf_printf("BST TIMEOUT INT here\n");
        pf_write_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_CTRL,
                     pf_read_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_CTRL) & 0xffffffbf);
        int_timeout_flag = 1;
        pf_printf("int_timeout_flag:%d\n", int_timeout_flag);
    }

    if ((pf_read_reg(BSCALER_BASE + BSCALER_FRMT_TASK) & 0x04) >> 2){
        pf_printf("BST TASK INT here\n");
        pf_write_reg(BSCALER_BASE + BSCALER_FRMT_TASK,
                     pf_read_reg(BSCALER_BASE + BSCALER_FRMT_TASK) & 0xfffffffb);
        int_task_flag = 1;
        pf_printf("int_task_flag:%d\n", int_task_flag);
    }

}

#define INTC_BSCALER  40

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
    int int_flag = 0;

    pf_printf("============ 1 ================\n");
    // 2. IP bscaler_init();
    // 2. bscaler software reset
    // bscaler_softreset_set();
    bscaler_frmt_soft_reset();

#if ((defined OPEN_INT) && (defined CHIP_SIM_ENV))
    pf_write_reg(IMR,0x0);
    pf_write_reg(IMR1,0x00100000);//NOT
    pf_write_reg(IMCR,0xffffffff);
    pf_write_reg(IMCR1,0xffffffff);
    setvbr(TP_INT | INTC_BSCALER, bscaler_int_handler);
#endif

    uint32_t  frmt_ckg_set = 0;
    uint32_t  frmc_ckg_set = 0;
    //clock_gate
    pf_printf("============ 2 ================\n");
    //bscaler_clkgate_mask_set(frmt_ckg_set, frmc_ckg_set);
    bscaler_frmt_clkgate_mask_set(frmt_ckg_set);

    // oram src init
    uint8_t *oram_src0 =(uint8_t *)ORAM_VBASE;
    int src0_size = sizeof(src00);
    uint8_t *oram_src1 =(uint8_t *)(ORAM_VBASE + src0_size + 64);
    int src1_size = sizeof(src01);
    if (cfg0->ibuft_bus == 1) {
        int n;
        for (n = 0; n < src0_size; n ++) {
            oram_src0[n] = src00[n];
        }
        for (n = 0; n < src1_size; n ++) {
            oram_src1[n] = src01[n];
        }
    }
    // 3. set timeout
    uint32_t t_timeout_val = 0x26;
    uint32_t frmt_task_mask ;
#if OPEN_INT
    pf_write_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_CTRL,
                 pf_read_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_CTRL) & 0xffffffdf);
    frmt_task_mask = 0;
#else
    pf_write_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_CTRL,
                 pf_read_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_CTRL) | (1 << 5));
    frmt_task_mask = 1;
#endif
    pf_write_reg(BSCALER_BASE + BSCALER_FRMT_TIMEOUT, t_timeout_val);

    // 4. update frmt_cfg
    pf_printf("============ 3 ================\n");
    update_cfg(cfg0);

    pf_printf("============ 4 ================\n");
    //dump_cfg_info(&cfg);
    pf_printf("============ 5 ================\n");
    // 5. configure register
    bscaler_frmt_cfg(cfg0);

    pf_printf("============ 6 ================\n");

    int32_t pixel_kernel_h = ((cfg0->kernel_size == 3) ? 7 :
                              (cfg0->kernel_size == 2) ? 5 :
                              (cfg0->kernel_size == 1) ? 3 : 1);
    uint32_t kernel_frame_h = (cfg0->src_h + cfg0->pad_top + cfg0->pad_bottom - pixel_kernel_h) / cfg0->kernel_ystride + 1;

    uint32_t imp_task_ynum = (cfg0->dst_format == 3) ? kernel_frame_h : cfg0->src_h;

    pf_printf("============ 7 ================\n");
    pf_printf("%08x\n", BSCALER_BASE + BSCALER_FRMT_TASK);
    pf_printf("val : %08x\n", (cfg0->task_len << 16) | 1 | 1 << 1);
    pf_write_reg(BSCALER_BASE + BSCALER_FRMT_TASK, (cfg0->task_len << 16) | 1 | frmt_task_mask << 1);
    pf_printf("============ 8 ================\n");
    int imp_task_y = 0;
    while (1) {
        pf_printf("============ 9 ================\n");
#if OPEN_INT
        pf_printf("TASK INTC READ REG:0x%x\n", pf_read_reg(BSCALER_BASE + BSCALER_FRMT_TASK));
        if (int_task_flag == 1) {//task finish
            int_task_flag = 0;
#else
        if ((pf_read_reg(BSCALER_BASE + BSCALER_FRMT_TASK) & 0x1) == 0) {//task finish
#endif
            pf_printf("============ 10 ================\n");
            uint32_t task_len_real = (((imp_task_y + cfg0->task_len) > imp_task_ynum) ?
                                      (imp_task_ynum - imp_task_y) : cfg0->task_len);
            imp_task_y += cfg0->task_len;
            if (imp_task_y >= imp_task_ynum) {
                pf_printf("============ 12 ================\n");
                break;
            } else {
                pf_write_reg(BSCALER_BASE + BSCALER_FRMT_TASK, (cfg0->task_len << 16) | 1 | frmt_task_mask << 1);
                pf_printf("============ 11 ================\n");
            }
        }
    }
    pf_printf("============ 13 ================\n");
    // 6. wait finish
    bst_wait_finish();
    pf_printf("============ 14 ================\n");
    // 7. check result

    int errnum = result_check(cfg0, gld_dst0);

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
