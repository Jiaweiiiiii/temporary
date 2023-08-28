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

//#define CHIP_SIM_ENV

#include <stdio.h>
#include <stdlib.h>
#include "t_vector_c.h"
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

#define OPEN_INT 0 //int for chip
int int_flag =0;
int int_timeout_flag =0;

int bscaler_wait_finish()
{
#if OPEN_INT
    int cnt = 0;
    while (int_flag != 1) {
        pf_printf("CTRL REG:0x%x\n",pf_read_reg(BSCALER_BASE + BSCALER_FRMC_CTRL));
    }
#else
    //polling
    int fail_flag=0;
    while ((pf_read_reg(BSCALER_BASE + BSCALER_FRMC_CTRL) & 0x3F) != 0) {
        fail_flag++;
        if((pf_read_reg(BSCALER_BASE + BSCALER_FRMC_CHAIN_CTRL) & 0x40) >> 6){
            pf_printf("TIMEOUT HIT\n");
            //clear timeout flag
            pf_write_reg(BSCALER_BASE + BSCALER_FRMC_CHAIN_CTRL,
                         pf_read_reg(BSCALER_BASE + BSCALER_FRMC_CHAIN_CTRL) & 0xffffffbf);
        }
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
    uint32_t dut_isum = pf_read_reg(BSCALER_BASE + BSCALER_FRMC_ISUM);
    uint32_t dut_osum = pf_read_reg(BSCALER_BASE + BSCALER_FRMC_OSUM);
    uint32_t gld_isum = cfg->isum;
    uint32_t gld_osum = cfg->osum;
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
    uint32_t bpp_mode = (cfg->dst_format >> 5) & 0x3;
    uint32_t bpp = 1 << (bpp_mode + 2);
    uint8_t *dut0;
    uint8_t *dut1;
    if (cfg->obufc_bus == 0) {
        dut0 = cfg->dst_base[0];
        dut1 = cfg->dst_base[1];
    } else {
        dut0 =(uint8_t *)ORAM_VDST;
        dut1 =(uint8_t *)(ORAM_VDST + 0x10000); //fix me
    }

    if (cfg->dst_format == BSC_HW_DATA_FM_NV12) {
        uint8_t *gld0 = gld;
        int i, j;
        for (i = 0; i < cfg->dst_box_h; i++) {
            for (j = 0; j < cfg->dst_box_w; j++) {
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
        for (i = 0; i < cfg->dst_box_h/2; i++) {
            for (j = 0; j < cfg->dst_box_w; j++) {
                int dut_idx = (cfg->dst_box_y/2  + i) * cfg->dst_line_stride +
                    cfg->dst_box_x + j;
                int gld_idx = i * cfg->dst_box_w + j;
#ifdef SRC_CHIP
                uint8_t dut_val = CpuRead8(pf_va_2_pa(&dut1[dut_idx]));
#else
                uint8_t dut_val = dut1[dut_idx];
#endif
                if (gld0[gld_idx] != dut_val) {
                    errnum++;
                    pf_printf("(C)(%d, %d)(G)%02x -- (E)%02x\n", i, j, gld1[gld_idx], dut1[dut_idx]);
                }
            }
        }
    } else {
        if (cfg->box_mode) {
            uint8_t *gld0 = gld;
            uint8_t *dut0;
            if (cfg->obufc_bus == 0) {
                dut0 = cfg->dst_base[0];
            } else {
                dut0 = (uint8_t *)ORAM_VDST;
            }
            int j , i, k;
            for (j = 0; j < cfg->dst_box_h; j++) {
                for (i = 0; i < cfg->dst_box_w; i++) {
                    for (k = 0; k < bpp; k++) {
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
            int bid;
            for (bid = 0; bid < cfg->box_num; bid++) {
                uint8_t *gld0 = gld + bid * cfg->dst_box_h * cfg->dst_box_w * bpp;
                uint8_t *dut0 = cfg->dst_base[bid];
                int j , i, k;
                for (j = 0; j < cfg->dst_box_h; j++) {
                    for (i = 0; i < cfg->dst_box_w; i++) {
                        for (k = 0; k < bpp; k++) {
                            int dut_idx = j * cfg->dst_line_stride + i * bpp + k;
                            int gld_idx = j * cfg->dst_box_w * bpp + i * bpp + k;
#ifdef SRC_CHIP
                            uint8_t dut_val = CpuRead8(pf_va_2_pa(&dut0[dut_idx]));
#else
                            uint8_t dut_val = dut0[dut_idx];
#endif
                            if (dut_val != gld0[gld_idx]) {                                    errnum++;
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

void update_cfg(bsc_hw_once_cfg_s *cfg)
{
#if OPEN_INT
    cfg->irq_mask  = 0;
#else
    cfg->irq_mask  = 1;
#endif
    if (cfg->ibufc_bus == 0) {
        cfg->src_base0 = src00;
        cfg->src_base1 = src01;
#if (defined CHIP_SIM_ENV) && (!defined SRC_CPU)
        cfg->src_base0 = (uint8_t *)cMalloc(src00, sizeof(src00), 1);
        cfg->src_base1 = (uint8_t *)cMalloc(src01, sizeof(src01), 1);
#endif
    } else {
        cfg->src_base0 = (uint8_t *)ORAM_VBASE;
        int size = cfg->src_line_stride * cfg->src_box_h;
        cfg->src_base1 = (uint8_t *)(ORAM_VBASE + size + 64);
    }

    if (cfg->dst_format == BSC_HW_DATA_FM_NV12) {
        int size = cfg->dst_line_stride * cfg->dst_box_h;
        if (cfg->obufc_bus == 0){
            cfg->dst_base[0] = (uint8_t *)pf_malloc(1, size);
            cfg->dst_base[1] = (uint8_t *)pf_malloc(1, size / 2);
        } else {
            cfg->dst_base[0] = (uint8_t *)ORAM_VDST;
            cfg->dst_base[1] = (uint8_t *)(ORAM_VDST + size + 64);
        }
    } else {
        uint32_t bpp_mode = (cfg->dst_format >> 5) & 0x3;
        int size = cfg->dst_line_stride * cfg->dst_box_h;
        /*if (bpp_mode == 0) {
          size *= 4;
          } else if (bpp_mode == 1) {
          size *= 8;
          } else if (bpp_mode == 2) {
          size *= 16;
          } else if (bpp_mode == 3) {
          size *= 32;
          }*/
        if (cfg->box_mode) {
            if(cfg->obufc_bus == 0){
                cfg->dst_base[0] = (uint8_t *)pf_malloc(1,size);
            } else {
                cfg->dst_base[0] = (uint8_t *)ORAM_VDST;
            }
        } else {
            if(cfg->box_bus == 0){
                cfg->boxes_info = boxes_info0;
#if (defined CHIP_SIM_ENV) && (!defined SRC_CPU)
                cfg->boxes_info = (uint32_t *)cMalloc(boxes_info0, sizeof(boxes_info0), 1);
#endif
            } else {
                cfg->boxes_info = (uint32_t *)ORAM_VBOX;
            }
            void *dst_all;
            if (cfg->obufc_bus == 0) {
                dst_all = pf_malloc(1,size * cfg->box_num);
            } else {
                dst_all = (void *)ORAM_VDST;
            }

            int i;
            for (i = 0; i < cfg->box_num; i++) {
                cfg->dst_base[i] = (uint8_t *)dst_all + size * i;
            }
        }
    }
}

void print_data_format(const char *fmt, bsc_hw_data_format_e format)
{
    pf_printf("%s = %s\n", fmt,
              format == BSC_HW_DATA_FM_NV12 ? "NV12" :
              format == BSC_HW_DATA_FM_BGRA ? "BGRA" :
              format == BSC_HW_DATA_FM_GBRA ? "GBRA" :
              format == BSC_HW_DATA_FM_RBGA ? "RBGA" :
              format == BSC_HW_DATA_FM_BRGA ? "BRGA" :
              format == BSC_HW_DATA_FM_GRBA ? "GRBA" :
              format == BSC_HW_DATA_FM_RGBA ? "RGBA" :
              format == BSC_HW_DATA_FM_ABGR ? "ABGR" :
              format == BSC_HW_DATA_FM_AGBR ? "AGBR" :
              format == BSC_HW_DATA_FM_ARBG ? "ARBG" :
              format == BSC_HW_DATA_FM_ABRG ? "ABRG" :
              format == BSC_HW_DATA_FM_AGRB ? "AGRB" :
              format == BSC_HW_DATA_FM_ARGB  ? "ARGB" :
              format == BSC_HW_DATA_FM_F32_2B ? "F32_2B" :
              format == BSC_HW_DATA_FM_F32_4B ? "F32_4B" :
              format == BSC_HW_DATA_FM_F32_8B ? "F32_8B" : "unknown");
}

void dump_cfg_info(bsc_hw_once_cfg_s *cfg)
{
    pf_printf("src_base0 = %p\n", cfg->src_base0);
    pf_printf("src_base1 = %p\n", cfg->src_base1);
    print_data_format("src_format", cfg->src_format);
    pf_printf("src_line_stride = %d\n", cfg->src_line_stride);
    pf_printf("src_box_x = %d\n", cfg->src_box_x);
    pf_printf("src_box_y = %d\n", cfg->src_box_y);
    pf_printf("src_box_w = %d\n", cfg->src_box_w);
    pf_printf("src_box_h = %d\n", cfg->src_box_h);

    print_data_format("dst_format", cfg->dst_format);
    pf_printf("dst_line_stride = %d\n", cfg->dst_line_stride);
    pf_printf("dst_box_x = %d\n", cfg->dst_box_x);
    pf_printf("dst_box_y = %d\n", cfg->dst_box_y);
    pf_printf("dst_box_w = %d\n", cfg->dst_box_w);
    pf_printf("dst_box_h = %d\n", cfg->dst_box_h);

    pf_printf("affine = %d\n", cfg->affine);
    pf_printf("box_mode = %d\n", cfg->box_mode);
    pf_printf("y_gain_exp = %d\n", cfg->y_gain_exp);
    pf_printf("zero_point = %d\n", cfg->zero_point);
    pf_printf("coef = {");
    int i;
    for (i = 0; i < 9; i++) {
        pf_printf("%d, ", cfg->coef[i]);
    }
    pf_printf("}\n");

    pf_printf("offset = {");
    for (i = 0; i < 2; i++) {
        pf_printf("%d, ", cfg->offset[i]);
    }
    pf_printf("}\n");

    pf_printf("matrix = {");
    for (i = 0; i < 9; i++) {
        pf_printf("%d, ", cfg->matrix[i]);
    }
    pf_printf("}\n");

    pf_printf("box_num = %d\n", cfg->box_num);

    pf_printf("dst_base :\n");
    for (i = 0; i < 64; i++) {
        pf_printf("%p, ", cfg->dst_base[i]);
    }
    pf_printf("\n");

    if (cfg->box_mode == false) {
        for (i = 0; i < cfg->box_num; i++) {
            pf_printf("%08x\n", cfg->boxes_info[i*6 + 0]);
            pf_printf("%08x\n", cfg->boxes_info[i*6 + 1]);
            pf_printf("%08x\n", cfg->boxes_info[i*6 + 2]);
            pf_printf("%08x\n", cfg->boxes_info[i*6 + 3]);
            pf_printf("%08x\n", cfg->boxes_info[i*6 + 4]);
            pf_printf("%08x\n", cfg->boxes_info[i*6 + 5]);
        }
    }
    pf_printf("mono_x = %08x\n", cfg->mono_x);
    pf_printf("mono_y = %08x\n", cfg->mono_y);
    pf_printf("extreme_point = {");
    for (i = 0; i < 64; i++) {
        pf_printf("%d,", cfg->extreme_point[i]);
    }
    pf_printf("}\n");
    pf_printf("isum = %08x\n", cfg->isum);
    pf_printf("osum = %08x\n", cfg->osum);
    pf_printf("box_bus = %08x\n", cfg->box_bus);//[0]-chain,[1]-box,[2]-ibuf,[3]-obuf, 0-ddr, 1-oram
    pf_printf("ibufc_bus = %08x\n", cfg->ibufc_bus);//[0]-chain,[1]-box,[2]-ibuf,[3]-obuf, 0-ddr, 1-oram
    pf_printf("obufc_bus = %08x\n", cfg->obufc_bus);//[0]-chain,[1]-box,[2]-ibuf,[3]-obuf, 0-ddr, 1-oram
    pf_printf("irq_mask = %08x\n", cfg->irq_mask);//1 - mask
}

void bsc_int_handler()
{
    pf_printf("------BSC INT here------\n");
    if ((pf_read_reg(BSCALER_BASE + BSCALER_FRMC_CTRL) & 0x00000100) >> 8){
        pf_printf("BSC  C INT here\n");
        pf_write_reg(BSCALER_BASE + BSCALER_FRMC_CTRL,
                     pf_read_reg(BSCALER_BASE + BSCALER_FRMC_CTRL) & 0xfffffeff);
        int_flag = 1;
        pf_printf("int_c_flag:%d\n", int_flag);
    } else if ((pf_read_reg(BSCALER_BASE + BSCALER_FRMC_CHAIN_CTRL) & 0x40) >> 6){
        pf_printf("BSC  TIMEOUT INT here\n");
        pf_write_reg(BSCALER_BASE + BSCALER_FRMC_CHAIN_CTRL,
                     pf_read_reg(BSCALER_BASE + BSCALER_FRMC_CHAIN_CTRL) & 0xffffffbf);
        int_timeout_flag = 1;
        pf_printf("int_timeout_flag:%d\n", int_timeout_flag);
    }

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
    //bscaler_softreset_set();
    bscaler_frmc_soft_reset();

#if OPEN_INT
#ifdef CHIP_SIM_ENV
    pf_write_reg(IMR,0x0);
    pf_write_reg(IMR1,0x00100000);//NOT
    pf_write_reg(IMCR,0xffffffff);
    pf_write_reg(IMCR1,0xffffffff);
#endif
    setvbr(TP_INT | INTC_BSC, bsc_int_handler);
#endif

    uint32_t  frmt_ckg_set = 0;
    uint32_t  frmc_ckg_set = 0;
    //clock_gate
    //bscaler_clkgate_mask_set(frmt_ckg_set, frmc_ckg_set);
    bscaler_frmc_clkgate_mask_set(frmc_ckg_set);
    // 2. oram src init
    uint8_t *oram_src =(uint8_t *)ORAM_VBASE;
    int size = sizeof(src00);
    //pf_printf("ORAM DEBUG0:0x%x\n", size);
    //size = cfg0->src_line_stride * cfg0->src_box_h;
    if (cfg0->ibufc_bus == 1) {
        int n;
        for (n = 0; n < size; n ++) {
            oram_src[n] = src00[n];
        }
    }

    // 3. update frmt_cfg
    update_cfg(cfg0);

    //dump_cfg_info(&cfg);

    // 4. configure register
    //timeout
    uint32_t c_timeout_val = 0x10;
#if OPEN_INT
    pf_write_reg(BSCALER_BASE + BSCALER_FRMC_CHAIN_CTRL,
                 pf_read_reg(BSCALER_BASE + BSCALER_FRMC_CHAIN_CTRL) & 0xffffffdf);
#else
    pf_write_reg(BSCALER_BASE + BSCALER_FRMC_CHAIN_CTRL,
                 pf_read_reg(BSCALER_BASE + BSCALER_FRMC_CHAIN_CTRL) | (1 << 5));
#endif
    pf_write_reg(BSCALER_BASE + BSCALER_FRMC_TIMEOUT, c_timeout_val);
    bscaler_frmc_cfg(cfg0);

    // cpm reset
#if 0
    int n;
    for (n = 0; n < 10; n++) {
        // cpm stop
        pf_write_reg(SRBC0, pf_read_reg(SRBC0) | 1 <<27);//axi stop enter
        pf_printf("RESET DEBUG0:0x%x\n", pf_read_reg(SRBC0));
        while (!((pf_read_reg(SRBC0) & 0x4000000) >> 26)){
            pf_printf("WAIT AXI EMPTY: %d, 0x%x\n", n, pf_read_reg(SRBC0));
        }
        pf_write_reg(SRBC0, pf_read_reg(SRBC0) | 1 <<28);//soft reset bscaler enter
        pf_write_reg(SRBC0, pf_read_reg(SRBC0) & 0xefffffff);//soft reset bscaler exit
        //error frmc start
        pf_write_reg(SRBC0, pf_read_reg(SRBC0) & 0xf7ffffff);//axi stop exit

        pf_printf("RESET DEBUG1:0x%x\n", pf_read_reg(SRBC0));
        while (((pf_read_reg(SRBC0) & 0x8000000) >> 27)){
            pf_printf("WAIT AXI STOP CLR:0x%x\n", pf_read_reg(SRBC0));
        }
#if OPEN_INT
        pf_write_reg(BSCALER_BASE + BSCALER_FRMC_CHAIN_CTRL,
                     pf_read_reg(BSCALER_BASE + BSCALER_FRMC_CHAIN_CTRL) & 0xffffffdf);
#else
        pf_write_reg(BSCALER_BASE + BSCALER_FRMC_CHAIN_CTRL,
                     pf_read_reg(BSCALER_BASE + BSCALER_FRMC_CHAIN_CTRL) | (1 << 5));
#endif
        pf_write_reg(BSCALER_BASE + BSCALER_FRMC_TIMEOUT, c_timeout_val);
        bscaler_frmc_cfg(cfg0);

    }
#endif

    // 5. wait finish
    bscaler_wait_finish();

    // 6. check result
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
