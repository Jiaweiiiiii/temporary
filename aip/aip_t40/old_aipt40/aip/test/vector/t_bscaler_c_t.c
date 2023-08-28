/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : t_bscaler_c_t.c
 * Authors    : lzhu@ehud.ic.jz.com
 * Create Time: 2020-08-13:14:51:48
 * Description:
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "t_vector_c_t_c.h"
#include "t_vector_c_t_t.h"
#ifdef CHIP_SIM_ENV
#include "bscaler_hw_api.c"
#include "platform.c"
#endif

#define L2_SIZE_OFST 0x00080000 //default 512KB
#define ORAM_VBASE  0xB2600000 + L2_SIZE_OFST
#define ORAM_PBASE  0x12600000 + L2_SIZE_OFST

#define ORAM_VDST   0xB26B0000
#define ORAM_PDST   0x126B0000

#define ORAM_VBOX   0xB26E0000
#define ORAM_PBOX   0x126E0000

void update_c_cfg(bsc_hw_once_cfg_s *cfg)
{
#if OPEN_INT
    cfg->irq_mask  = 0;
#else
    cfg->irq_mask  = 1;
#endif
    if (cfg->ibufc_bus == 0) {
        cfg->src_base0 = c_src00;
        cfg->src_base1 = c_src01;
#if (defined CHIP_SIM_ENV) && (!defined SRC_CPU)
        cfg->src_base0 = (uint8_t *)cMalloc(c_src00, sizeof(c_src00), 1);
        cfg->src_base1 = (uint8_t *)cMalloc(c_src01, sizeof(c_src01), 1);
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
        } else {
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
            if (cfg->box_bus == 0) {
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

void update_t_cfg(bst_hw_once_cfg_s *cfg)
{
#if OPEN_INT
    cfg->irq_mask = 0;
#else
    cfg->irq_mask = 1;
#endif
    if (cfg->ibuft_bus == 0) {
        cfg->src_base0 = t_src00;
        cfg->src_base1 = t_src01;
#if (defined CHIP_SIM_ENV) && (!defined SRC_CPU)
        cfg->src_base0 = (uint8_t *)cMalloc(t_src00, sizeof(t_src00), 1);
        cfg->src_base1 = (uint8_t *)cMalloc(t_src01, sizeof(t_src01), 1);
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

    // 2. init frmc and frmt
    //frmc init
    bscaler_frmt_soft_reset();
    bscaler_frmc_soft_reset();

    uint32_t  frmc_ckg_set =0;
    bscaler_frmc_clkgate_mask_set(frmc_ckg_set);
    uint32_t  frmt_ckg_set =0;
    bscaler_frmt_clkgate_mask_set(frmt_ckg_set);

    //open isum_debug /timeout irq mask
    bscaler_write_reg(BSCALER_FRMC_CHAIN_CTRL,
                      bscaler_read_reg(BSCALER_BASE + BSCALER_FRMC_CHAIN_CTRL, 0) /*| 1<<8*/ | 1 << 5);

    bscaler_write_reg(BSCALER_FRMT_CHAIN_CTRL,
                      bscaler_read_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_CTRL, 0) | 1 << 5);

    while (1) {
        if((pf_read_reg(BSCALER_BASE + BSCALER_FRMC_CTRL)&0x3F) == 0){
            pf_printf("test0\n");
            //bscaler_frmc_free(frmc_cfg, radom_cfg);
            pf_printf("test1\n");
            pf_write_reg (BSCALER_BASE + BSCALER_FRMC_TIMEOUT,0x8240000);
            update_c_cfg(c_cfg0);
            bscaler_frmc_cfg(c_cfg0);
        }
        if ((pf_read_reg(BSCALER_BASE + BSCALER_FRMT_CTRL) & 0x1) == 0){
            pf_printf("test2\n");
            //bscaler_frmt_free(frmt_cfg);
            pf_printf("test3\n");
            pf_write_reg (BSCALER_BASE + BSCALER_FRMT_TIMEOUT,0x8840000);
            update_t_cfg(t_cfg0);
            bscaler_frmt_cfg(t_cfg0);
            pf_printf("task_len = 0x%0x, src_h = 0x%0x\n", t_cfg0->task_len, t_cfg0->src_h);
            pf_write_reg(BSCALER_BASE + BSCALER_FRMT_TASK, (t_cfg0->task_len << 16) | 1 | 1 << 1);
        }
        if (((pf_read_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_CTRL) & 0x40) >> 6) == 1) {
            pf_write_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_CTRL,
                         bscaler_read_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_CTRL,0) & 0xffffffbf);
            pf_printf("FRMT FAILED\n");
        }
        if (((pf_read_reg(BSCALER_BASE + BSCALER_FRMT_CHAIN_CTRL) & 0x40) >> 6) == 1) {
            pf_write_reg(BSCALER_BASE + BSCALER_FRMC_CHAIN_CTRL,
                         bscaler_read_reg(BSCALER_BASE + BSCALER_FRMC_CHAIN_CTRL,0) & 0xffffffbf);
            pf_printf("FRMC FAILED\n");
        }

    }
    pf_printf("================ CASE FINISH =================\n");
    pf_deinit();
    return 0;

}
#ifdef CHIP_SIM_ENV
TEST_MAIN(TEST_NAME);
#endif
