/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : bscaler_hal.c
 * Authors    : jmqi@taurus
 * Create Time: 2020-04-20:15:02:19
 * Description:
 *
 */

#include <assert.h>
#include "bscaler_hal.h"
#include "platform.h"
//#define OPEN_BSC_CSUM
#define SYMBOL_EXPORT __attribute__ ((visibility("default")))

uint32_t bsc_csum = 0;

void bscaler_clock_set()
{
#if 0
    /* Set BSCALER Clock Gate */
#define CPM_CLKGR0                      (cpm_base + 0x20)
#define CPM_CLKGR0_BSCALER              (~(0x1 << 2))
#define SET_CPM_BSCALER_CLKGR(a)        pf_write_reg(CPM_CLKGR0, a)
#define GET_CPM_BSCALER_CLKGR()         pf_read_reg(CPM_CLKGR0)
    pf_printf("Open BSCALER clock!\n");
    SET_CPM_BSCALER_CLKGR(GET_CPM_BSCALER_CLKGR() & CPM_CLKGR0_BSCALER);
    pf_printf("Open BSCALER clock finish!\n");

    /* VPLL Configure */
#define CGU_BASE              cpm_base
#define CPVPCR                (CGU_BASE + 0xE0)
#define CPVPCR_VPLLM(a)       (a)<<20
#define CPVPCR_VPLLEN         1 << 0
    pf_write_reg(CPVPCR, (pf_read_reg(CPVPCR) & 0xFFFFFFFE));
    pf_write_reg(CPVPCR, ((pf_read_reg(CPVPCR) & 0x000FFFFF) | CPVPCR_VPLLM(21)));//24*21=504M
    pf_write_reg(CPVPCR, (pf_read_reg(CPVPCR) | CPVPCR_VPLLEN));

    /* BSCALER PLL select */
#define CPM_BSCALERCDR            (CGU_BASE + 0xA0)
    pf_write_reg(CPM_BSCALERCDR, (pf_read_reg(CPM_BSCALERCDR) | (1 << 29)));
    pf_write_reg(CPM_BSCALERCDR, (pf_read_reg(CPM_BSCALERCDR) | (0x2 << 30) | (1 << 0)));//VPLL
#endif
}

//inline, error for static library, fixme
int write_chain(uint32_t reg, uint32_t val, uint32_t **addr, uint32_t tm)
{
    *(*addr)++ = val;
    *(*addr)++ = (tm << 31) | reg;
//    printf("write_chain:reg[0x%x], val[0x%x], *addr[0x%x], tm[%d]\n",
//    		reg, val, *addr, tm);
#ifdef OPEN_BSC_CSUM
    uint32_t a = val;
    uint32_t b = ((tm << 31) | reg);
    bsc_csum += ((a & 0xFF) + ((a >> 8) & 0xFF) + ((a >> 16) & 0xFF) + ((a >> 24) & 0xFF) +
                 (b & 0xFF) + ((b >> 8) & 0xFF) + ((b >> 16) & 0xFF) + ((b >> 24) & 0xFF));
#endif
    return 8;//2 words
}

SYMBOL_EXPORT
void bscaler_frmt_soft_reset()
{
    bscaler_write_reg(BSCALER_FRMT_CHAIN_CTRL, 1<<16);
    while (bscaler_read_reg(BSCALER_FRMT_CHAIN_CTRL, 0) & 0x10000);
}

SYMBOL_EXPORT
void bscaler_frmc_soft_reset()
{
    bscaler_write_reg(BSCALER_FRMC_CHAIN_CTRL, 1<<16);
    while (bscaler_read_reg(BSCALER_FRMC_CHAIN_CTRL, 0) & 0x10000);
}

void bscaler_frmt_clkgate_mask_set(uint8_t t)
{
    uint32_t frmt_ctrl = bscaler_read_reg(BSCALER_FRMT_CHAIN_CTRL, 0);
    if (t)
        bscaler_write_reg(BSCALER_FRMT_CHAIN_CTRL, frmt_ctrl | 0x40000);
    else
        bscaler_write_reg(BSCALER_FRMT_CHAIN_CTRL, frmt_ctrl & 0xfffbffff);
}

void bscaler_frmc_clkgate_mask_set(uint8_t c)
{
    uint32_t frmc_ctrl = bscaler_read_reg(BSCALER_FRMC_CHAIN_CTRL, 0);
    if (c)
        bscaler_write_reg(BSCALER_FRMC_CHAIN_CTRL, frmc_ctrl | 0x40000);
    else
        bscaler_write_reg(BSCALER_FRMC_CHAIN_CTRL, frmc_ctrl & 0xfffbffff);
}

SYMBOL_EXPORT
void bscaler_common_param_cfg(uint32_t *coef, uint8_t *offset, uint8_t alpha)
{
    bscaler_write_reg(BSCALER_PARAM0, coef[0]);
    bscaler_write_reg(BSCALER_PARAM1, coef[1]);
    bscaler_write_reg(BSCALER_PARAM2, coef[2]);
    bscaler_write_reg(BSCALER_PARAM3, coef[3]);
    bscaler_write_reg(BSCALER_PARAM4, coef[4]);
    bscaler_write_reg(BSCALER_PARAM5, coef[5]);
    bscaler_write_reg(BSCALER_PARAM6, coef[6]);
    bscaler_write_reg(BSCALER_PARAM7, coef[7]);
    bscaler_write_reg(BSCALER_PARAM8, coef[8]);
    bscaler_write_reg(BSCALER_PARAM9, ((offset[0] << 16) |
                                       (offset[1] << 24) |
                                       (alpha << 8)));
}

void bst_order_cfg(uint8_t order_t)
{
    bscaler_write_reg(BSCALER_PARAM9,
                      bscaler_read_reg(BSCALER_PARAM9, 0) |
                      ((order_t & 0xF)));
}

void bsc_order_cfg(uint8_t order_c)
{
    bscaler_write_reg(BSCALER_PARAM9,
                      bscaler_read_reg(BSCALER_PARAM9, 0) |
                      ((order_c & 0xF) << 4));
}

void bscaler_frmt_cfg(bst_hw_once_cfg_s *cfg)
{
    uint8_t order = (cfg->dst_format >> 4) & 0xF;
    bst_order_cfg(order);

    uint32_t phy_src_base0, phy_src_base1, phy_dst_base0, phy_dst_base1;
#if (defined EYER_SIM_ENV)
    phy_src_base0 = (uint32_t)cfg->src_base0;
    phy_src_base1 = (uint32_t)cfg->src_base1;
    phy_dst_base0 = (uint32_t)cfg->dst_base0;
    phy_dst_base1 = (uint32_t)cfg->dst_base1;
#elif (defined CSE_SIM_ENV)
    if (cfg->ibuft_bus == 1) {
        phy_src_base0 = __aie_get_oram_paddr((uint32_t)cfg->src_base0);
        phy_src_base1 = __aie_get_oram_paddr((uint32_t)cfg->src_base1);
    } else if (cfg->ibuft_bus == 0) {
        phy_src_base0 = __aie_get_ddr_paddr((uint32_t)cfg->src_base0);
        phy_src_base1 = __aie_get_ddr_paddr((uint32_t)cfg->src_base1);
    } else if (cfg->ibuft_bus == 2) {//rmem physical address
        phy_src_base0 = (uint32_t)cfg->src_base0;
        phy_src_base1 = (uint32_t)cfg->src_base1;
    }

    if (cfg->obuft_bus) {
        phy_dst_base0 = __aie_get_oram_paddr((uint32_t)cfg->dst_base0);
        phy_dst_base1 = __aie_get_oram_paddr((uint32_t)cfg->dst_base1);
    } else {
        phy_dst_base0 = __aie_get_ddr_paddr((uint32_t)cfg->dst_base0);
        phy_dst_base1 = __aie_get_ddr_paddr((uint32_t)cfg->dst_base1);
    }
#elif (defined CHIP_SIM_ENV)
    phy_src_base0 = pf_va_2_pa(cfg->src_base0);
    phy_src_base1 = pf_va_2_pa(cfg->src_base1);
    phy_dst_base0 = pf_va_2_pa(cfg->dst_base0);
    phy_dst_base1 = pf_va_2_pa(cfg->dst_base1);
#endif

    bscaler_write_reg(BSCALER_FRMT_YBASE_SRC, phy_src_base0);
    bscaler_write_reg(BSCALER_FRMT_CBASE_SRC, phy_src_base1);
    bscaler_write_reg(BSCALER_FRMT_YBASE_DST, phy_dst_base0);
    bscaler_write_reg(BSCALER_FRMT_CBASE_DST, phy_dst_base1);

    bscaler_write_reg(BSCALER_FRMT_WH_SRC, (cfg->src_h << 16 |
                                            cfg->src_w));
    bscaler_write_reg(BSCALER_FRMT_PS_SRC, (cfg->src_line_stride & 0xFFFF));
    bscaler_write_reg(BSCALER_FRMT_PS_DST, cfg->dst_line_stride);
    bscaler_write_reg(BSCALER_FRMT_FS_DST, cfg->dst_plane_stride);
    bscaler_write_reg(BSCALER_FRMT_DUMMY, cfg->zero_point);//fixme
    bscaler_write_reg(BSCALER_FRMT_FORMAT, ((cfg->src_format & 0x1) << 0 | //[1:0]
                                            (cfg->dst_format & 0x3) << 2 | //[3:2]
                                            (cfg->kernel_xstride & 0x3) << 4 | //[5:4]
                                            (cfg->kernel_ystride & 0x3) << 6 | //[7:6]
                                            (cfg->kernel_size & 0x3) << 8 | //[9:8]
                                            (cfg->pad_left & 0x7) << 16 | //[18:16]
                                            (cfg->pad_right & 0x7) << 20 | //[22:20]
                                            (cfg->pad_top & 0x7) << 24 | //[26:24]
                                            (cfg->pad_bottom & 0x7) << 28)); //[30:28]
    bscaler_write_reg(BSCALER_FRMT_ISUM, 0); //clear summary, fixme
    bscaler_write_reg(BSCALER_FRMT_OSUM, 0); //clear summary, fixme
    bscaler_write_reg(BSCALER_FRMT_TIMEOUT, 0xFFFFFFFF);//must reset, fixed timeout bug.
    bscaler_write_reg(BSCALER_FRMT_CTRL, ((cfg->obuft_bus & 0x1) << 11 |
                                          (cfg->ibuft_bus & 0x1) << 10 |
                                          (cfg->irq_mask & 0x1) << 9 | 1));
}

void bscaler_frmc_cfg(bsc_hw_once_cfg_s *cfg)
{
    uint8_t nv2bgr_order = (cfg->dst_format >> 1) & 0xF;
    bsc_order_cfg(nv2bgr_order);

    uint32_t src_format = cfg->src_format & 0x1;
    uint32_t dst_format = cfg->dst_format & 0x1;
    uint32_t bpp_mode = (cfg->src_format >> 5) & 0x3;
    int i, box_num;
    if (!dst_format)
        box_num = 2;
    else
        box_num = cfg->box_num;

    uint32_t phy_src_base0, phy_src_base1, phy_boxes_info;
    uint32_t phy_dst_base[64] = {0};
#if (defined EYER_SIM_ENV)
    phy_src_base0 = (uint32_t)cfg->src_base0;
    phy_src_base1 = (uint32_t)cfg->src_base1;
    phy_boxes_info = (uint32_t)cfg->boxes_info;
    for (i = 0; i < box_num; i++) {
        phy_dst_base[i] = (uint32_t)cfg->dst_base[i];
    }
#elif (defined CSE_SIM_ENV)
    if (cfg->ibufc_bus == 1) {
        phy_src_base0 = __aie_get_oram_paddr((uint32_t)cfg->src_base0);
        phy_src_base1 = __aie_get_oram_paddr((uint32_t)cfg->src_base1);
    } else if (cfg->ibufc_bus == 0) {
        phy_src_base0 = __aie_get_ddr_paddr((uint32_t)cfg->src_base0);
        phy_src_base1 = __aie_get_ddr_paddr((uint32_t)cfg->src_base1);
    } else if (cfg->ibufc_bus == 2) {
        phy_src_base0 = (uint32_t)cfg->src_base0;
        phy_src_base1 = (uint32_t)cfg->src_base1;
    }

    if (cfg->box_bus) {
        phy_boxes_info = __aie_get_oram_paddr((uint32_t)cfg->boxes_info);
    } else {
        phy_boxes_info = __aie_get_ddr_paddr((uint32_t)cfg->boxes_info);
    }

    for (i = 0; i < box_num; i++) {
        if (cfg->obufc_bus) {
            phy_dst_base[i] = __aie_get_oram_paddr((uint32_t)cfg->dst_base[i]);
        } else {
            phy_dst_base[i] = __aie_get_ddr_paddr((uint32_t)cfg->dst_base[i]);
        }
    }
#elif (defined CHIP_SIM_ENV)
    phy_src_base0 = pf_va_2_pa(cfg->src_base0);
    phy_src_base1 = pf_va_2_pa(cfg->src_base1);
    phy_boxes_info = pf_va_2_pa(cfg->boxes_info);

    for (i = 0; i < box_num; i++) {
        phy_dst_base[i] = pf_va_2_pa(cfg->dst_base[i]);
    }
#endif

    bscaler_write_reg(BSCALER_FRMC_YBASE_SRC, phy_src_base0);
    bscaler_write_reg(BSCALER_FRMC_CBASE_SRC, phy_src_base1);
    for (i = 0; i < box_num; i++) {
        bscaler_write_reg(BSCALER_FRMC_BASE_DST, phy_dst_base[i]);
    }
    bscaler_write_reg(BSCALER_FRMC_BOX_BASE, phy_boxes_info);

    bscaler_write_reg(BSCALER_FRMC_MODE, ((src_format & 0x1) << 0 |
                                          (cfg->affine & 0x1) << 1 |
                                          (cfg->box_mode & 0x1) << 2 |
                                          (dst_format & 0x1) << 3 |
                                          (bpp_mode & 0x3) << 4 |
                                          (cfg->zero_point & 0xff) << 8 |
                                          (cfg->y_gain_exp & 0x7) << 16));
    bscaler_write_reg(BSCALER_FRMC_WH_SRC, (cfg->src_box_h << 16 |
                                            cfg->src_box_w));
    bscaler_write_reg(BSCALER_FRMC_PS_SRC, cfg->src_line_stride);
    bscaler_write_reg(BSCALER_FRMC_WH_DST, (cfg->dst_box_h << 16 |
                                            cfg->dst_box_w));
    bscaler_write_reg(BSCALER_FRMC_PS_DST, ((cfg->dst_line_stride & 0xffff) << 0));

    //fill box info
    bscaler_write_reg(BSCALER_FRMC_BOX0, (cfg->src_box_y << 16 |
                                          cfg->src_box_x) );
    bscaler_write_reg(BSCALER_FRMC_BOX1, (cfg->dst_box_y << 16 |
                                          cfg->dst_box_x) );
    bscaler_write_reg(BSCALER_FRMC_BOX2, cfg->matrix[0]);
    bscaler_write_reg(BSCALER_FRMC_BOX3, cfg->matrix[1]);
    bscaler_write_reg(BSCALER_FRMC_BOX4, cfg->matrix[2]);
    bscaler_write_reg(BSCALER_FRMC_BOX5, cfg->matrix[3]);
    bscaler_write_reg(BSCALER_FRMC_BOX6, cfg->matrix[4]);
    bscaler_write_reg(BSCALER_FRMC_BOX7, cfg->matrix[5]);
    bscaler_write_reg(BSCALER_FRMC_BOX8, cfg->matrix[6]);
    bscaler_write_reg(BSCALER_FRMC_BOX9, cfg->matrix[7]);
    bscaler_write_reg(BSCALER_FRMC_BOX10, cfg->matrix[8]);
    //fill perspective info
    bscaler_write_reg(BSCALER_FRMC_MONO, (cfg->mono_x << 0 | cfg->mono_y << 16));
    for (i = 0; i < cfg->dst_box_w; i += 4) {
        bscaler_write_reg(BSCALER_FRMC_EXTREME_Y, (cfg->extreme_point[i] << 0 |
                                                   cfg->extreme_point[i+1] << 8 |
                                                   cfg->extreme_point[i+2] << 16 |
                                                   cfg->extreme_point[i+3] << 24));
    }

    bscaler_write_reg(BSCALER_FRMC_ISUM, 0); //clear summary, fixme
    bscaler_write_reg(BSCALER_FRMC_OSUM, 0); //clear summary, fixme
    bscaler_write_reg(BSCALER_FRMC_CTRL, (cfg->obufc_bus << 12 |
                                          (cfg->ibufc_bus & 0x1) << 11 |
                                          cfg->box_bus << 10 |
                                          cfg->irq_mask << 9 | 1 << 0));
}

uint32_t bscaler_frmt_cfg_chain(bst_hw_once_cfg_s *cfg, uint32_t *addr)
{
#if 0
    uint32_t len = 0;
    uint32_t phy_src_base0, phy_src_base1, phy_dst_base0, phy_dst_base1;
#if (defined EYER_SIM_ENV)
    phy_src_base0 = (uint32_t)cfg->src_base0;
    phy_src_base1 = (uint32_t)cfg->src_base1;
    phy_dst_base0 = (uint32_t)cfg->dst_base0;
    phy_dst_base1 = (uint32_t)cfg->dst_base1;
#elif (defined CSE_SIM_ENV)
    if (cfg->ibuft_bus == 1) {
        phy_src_base0 = __aie_get_oram_paddr((uint32_t)cfg->src_base0);
        phy_src_base1 = __aie_get_oram_paddr((uint32_t)cfg->src_base1);
    } else if (cfg->ibuft_bus == 0) {
        phy_src_base0 = __aie_get_ddr_paddr((uint32_t)cfg->src_base0);
        phy_src_base1 = __aie_get_ddr_paddr((uint32_t)cfg->src_base1);
    } else if (cfg->ibuft_bus == 2) {
        phy_src_base0 = (uint32_t)cfg->src_base0;
        phy_src_base1 = (uint32_t)cfg->src_base1;
    }
    if (cfg->obuft_bus) {
        phy_dst_base0 = __aie_get_oram_paddr((uint32_t)cfg->dst_base0);
        phy_dst_base1 = __aie_get_oram_paddr((uint32_t)cfg->dst_base1);
    } else {
        phy_dst_base0 = __aie_get_ddr_paddr((uint32_t)cfg->dst_base0);
        phy_dst_base1 = __aie_get_ddr_paddr((uint32_t)cfg->dst_base1);
    }
#elif (defined CHIP_SIM_ENV)
    phy_src_base0 = pf_va_2_pa(cfg->src_base0);
    phy_src_base1 = pf_va_2_pa(cfg->src_base1);
    phy_dst_base0 = pf_va_2_pa(cfg->dst_base0);
    phy_dst_base1 = pf_va_2_pa(cfg->dst_base1);
#endif

    len += write_chain(BSCALER_SRC_BASE0, phy_src_base0, &addr, 0);
    len += write_chain(BSCALER_SRC_BASE1, phy_src_base1, &addr, 0);
    len += write_chain(BSCALER_SRC_BASE0, phy_dst_base0, &addr, 0);
    len += write_chain(BSCALER_SRC_BASE1, phy_dst_base1, &addr, 0);

    len += write_chain(BSCALER_FRMT_WH_SRC, (cur->height << 16 |
                                             cur->width), &addr, 0);
    len += write_chain(BSCALER_FRMT_PS_SRC,
                       (cfg->src_line_stride & 0xFFFF), &addr, 0);
    len += write_chain(BSCALER_FRMT_PS_DST, cfg->dst_line_stride, &addr, 0);
    len += write_chain(BSCALER_FRMT_FS_DST, cfg->dst_plane_stride, &addr, 0);
    len += write_chain(BSCALER_FRMT_DUMMY, cfg->zero_point, &addr, 0);
    len += write_chain(BSCALER_FRMT_FORMAT,
                       ((cfg->src_format & 0x1) << 0 | //[1:0]
                        (cfg->dst_format & 0x3) << 2 | //[3:2]
                        (cfg->kernel_xstride & 0x3) << 4 | //[5:4]
                        (cfg->kernel_ystride & 0x3) << 6 | //[7:6]
                        (cfg->kernel_size & 0x3) << 8 | //[9:8]
                        (cfg->pad_left & 0x7) << 16 | //[18:16]
                        (cfg->pad_right & 0x7) << 20 | //[22:20]
                        (cfg->pad_top & 0x7) << 24 | //[26:24]
                        (cfg->pad_bottom & 0x7) << 28), &addr, 0); //[30:28]
    len += write_chain(BSCALER_FRMT_ISUM, 0, &addr, 0); //clear summary, fixme
    len += write_chain(BSCALER_FRMT_OSUM, 0, &addr, 0); //clear summary, fixme
    len += write_chain(BSCALER_FRMT_CTRL, 1, &addr, 1);
    {
        uint32_t back[2];
        back[0] = addr;
        back[1] = chain_len;
        return (back);
    }
#endif
}

bs_chain_ret_s bscaler_frmc_chain_cfg(bsc_hw_once_cfg_s *cfg, uint32_t *addr)
{
    uint32_t len = 0;
    uint32_t i;
    uint32_t box_num;
    uint32_t src_format = cfg->src_format & 0x1;
    uint32_t dst_format = cfg->dst_format & 0x1;
    uint32_t bpp_mode = (cfg->src_format >> 5) & 0x3;
    bs_chain_ret_s ret;
    if (!dst_format)
        box_num = 2;
    else
        box_num = cfg->box_num;

    uint32_t phy_src_base0, phy_src_base1, phy_boxes_info;
    uint32_t phy_dst_base[64] = {0};
#if (defined EYER_SIM_ENV)
    phy_src_base0 = (uint32_t)cfg->src_base0;
    phy_src_base1 = (uint32_t)cfg->src_base1;
    phy_boxes_info = (uint32_t)cfg->boxes_info;
    for (i = 0; i < box_num; i++) {
        phy_dst_base[i] = (uint32_t)cfg->dst_base[i];
    }
#elif (defined CSE_SIM_ENV)
    if (cfg->ibufc_bus == 1) {
        phy_src_base0 = __aie_get_oram_paddr((uint32_t)cfg->src_base0);
        phy_src_base1 = __aie_get_oram_paddr((uint32_t)cfg->src_base1);
    } else if (cfg->ibufc_bus == 0) {
        phy_src_base0 = __aie_get_ddr_paddr((uint32_t)cfg->src_base0);
        phy_src_base1 = __aie_get_ddr_paddr((uint32_t)cfg->src_base1);
    } else if (cfg->ibufc_bus == 2) {
        phy_src_base0 = (uint32_t)cfg->src_base0;
        phy_src_base1 = (uint32_t)cfg->src_base1;
    }

    if (cfg->box_bus) {
        phy_boxes_info = __aie_get_oram_paddr((uint32_t)cfg->boxes_info);
    } else {
        phy_boxes_info = __aie_get_ddr_paddr((uint32_t)cfg->boxes_info);
    }

    for (i = 0; i < box_num; i++) {
        if (cfg->obufc_bus) {
            phy_dst_base[i] = __aie_get_oram_paddr((uint32_t)cfg->dst_base[i]);
        } else {
            phy_dst_base[i] = __aie_get_ddr_paddr((uint32_t)cfg->dst_base[i]);
        }
    }
#elif (defined CHIP_SIM_ENV)
    phy_src_base0 = pf_va_2_pa(cfg->src_base0);
    phy_src_base1 = pf_va_2_pa(cfg->src_base1);
    phy_boxes_info = pf_va_2_pa(cfg->boxes_info);

    for (i = 0; i < box_num; i++) {
        phy_dst_base[i] = pf_va_2_pa(cfg->dst_base[i]);
    }
#endif
    //printf("BSC: %08x(%08x), %08x(%08x), %08x(%08x), %08x(%08x)\n",
    //        cfg->src_base0, phy_src_base0, cfg->src_base1, phy_src_base1,
    //        cfg->dst_base[0], phy_dst_base[0], cfg->dst_base[1], phy_dst_base[1]);

    len += write_chain(BSCALER_FRMC_YBASE_SRC, phy_src_base0, &addr, 0);
    len += write_chain(BSCALER_FRMC_CBASE_SRC, phy_src_base1, &addr, 0);
    for (i = 0; i < box_num; i++) {
        len += write_chain(BSCALER_FRMC_BASE_DST, phy_dst_base[i], &addr, 0);
    }

    len += write_chain(BSCALER_FRMC_BOX_BASE, phy_boxes_info, &addr, 0);

    //printf("cfg->matrix[4]=%d -- ofst: %d, %d\n", cfg->matrix[4], cfg->src_box_x, cfg->src_box_y);
    if ((cfg->matrix[4] >=  2*65536) && (cfg->src_box_y == 0)
        && (cfg->src_box_x == 0) && (bpp_mode != 0)
        && (cfg->dst_box_y == 0) && (cfg->dst_box_x == 0)) {
        len += write_chain(BSCALER_FRMC_MODE,
                           ((src_format & 0x1) << 0 |
                            (cfg->affine & 0x1) << 1 |
                            (cfg->box_mode & 0x1) << 2 |
                            (dst_format & 0x1) << 3 |
                            (bpp_mode & 0x3) << 4 |
                            //1 << 6 | // optimize skip y featch data
                            (cfg->zero_point & 0xff) << 8 //|
                            /*(cfg->y_gain_exp & 0x7) << 16*/), &addr, 0);
    } else {
        len += write_chain(BSCALER_FRMC_MODE,
                           ((src_format & 0x1) << 0 |
                            (cfg->affine & 0x1) << 1 |
                            (cfg->box_mode & 0x1) << 2 |
                            (dst_format & 0x1) << 3 |
                            (bpp_mode & 0x3) << 4 |
                            //1 << 6 | // optimize skip y featch data
                            (cfg->zero_point & 0xff) << 8 |
                            (cfg->y_gain_exp & 0x7) << 16), &addr, 0);

    }
    len += write_chain(BSCALER_FRMC_WH_SRC, (cfg->src_box_h << 16 |
                                             cfg->src_box_w), &addr, 0);
    len += write_chain(BSCALER_FRMC_PS_SRC, cfg->src_line_stride, &addr, 0);
    len += write_chain(BSCALER_FRMC_WH_DST, (cfg->dst_box_h << 16 |
                                             cfg->dst_box_w), &addr, 0);
    len += write_chain(BSCALER_FRMC_PS_DST,
                       ((cfg->dst_line_stride & 0xffff) << 0), &addr, 0);
    //fill box info
    len += write_chain(BSCALER_FRMC_BOX0, (cfg->src_box_y << 16 |
                                           cfg->src_box_x), &addr, 0);
    len += write_chain(BSCALER_FRMC_BOX1, (cfg->dst_box_y << 16 |
                                           cfg->dst_box_x), &addr, 0);
    len += write_chain(BSCALER_FRMC_BOX2, cfg->matrix[0], &addr, 0);
    len += write_chain(BSCALER_FRMC_BOX3, cfg->matrix[1], &addr, 0);
    len += write_chain(BSCALER_FRMC_BOX4, cfg->matrix[2], &addr, 0);
    len += write_chain(BSCALER_FRMC_BOX5, cfg->matrix[3], &addr, 0);
    len += write_chain(BSCALER_FRMC_BOX6, cfg->matrix[4], &addr, 0);
    len += write_chain(BSCALER_FRMC_BOX7, cfg->matrix[5], &addr, 0);
    len += write_chain(BSCALER_FRMC_BOX8, cfg->matrix[6], &addr, 0);
    len += write_chain(BSCALER_FRMC_BOX9, cfg->matrix[7], &addr, 0);
    len += write_chain(BSCALER_FRMC_BOX10, cfg->matrix[8], &addr, 0);
    len += write_chain(BSCALER_FRMC_MONO, (cfg->mono_x << 0 |
                                           cfg->mono_y << 16), &addr, 0);
    for (i = 0; i < cfg->dst_box_w; i += 4) {
        len += write_chain(BSCALER_FRMC_EXTREME_Y,
                           (cfg->extreme_point[i] << 0 |
                            cfg->extreme_point[i+1] << 8 |
                            cfg->extreme_point[i+2] << 16 |
                            cfg->extreme_point[i+3] << 24), &addr, 0);
    }

    len += write_chain(BSCALER_FRMC_CTRL,
                       ((cfg->obufc_bus & 0x1) << 12 |
                        (cfg->ibufc_bus & 0x1) << 11 |
                        cfg->box_bus << 10 |
                        cfg->irq_mask << 9 | 1 << 0), &addr, 1);

    //printf("bsc_csum = 0x%08x\n", bsc_csum);

    {//fixme
        ret.bs_ret_base = addr;
        ret.bs_ret_len = len;
        return (ret);
    }
}

void bsc_chain_hw_cfg(bs_chain_cfg_s *cfg)
{
    uint32_t des_chain_phy_addr = 0;
#if (defined EYER_SIM_ENV)
    des_chain_phy_addr = (uint32_t)cfg->bs_chain_base;
#elif (defined CSE_SIM_ENV || defined FPGA_SIM_ENV)
    if (cfg->chain_bus) {//oram
        des_chain_phy_addr = __aie_get_oram_paddr((uint32_t)cfg->bs_chain_base);
    } else {
        des_chain_phy_addr = __aie_get_ddr_paddr((uint32_t)cfg->bs_chain_base);
        //printf("chain addr : %08x(%08x)\n", cfg->bs_chain_base, des_chain_phy_addr);
    }

#elif (defined CHIP_SIM_ENV)
    des_chain_phy_addr = pf_va_2_pa(cfg->bs_chain_base);
#endif

    bscaler_write_reg(BSCALER_FRMC_TIMEOUT, 0xFFFFFFFF);//must reset, fixed timeout bug.
    bscaler_write_reg(BSCALER_FRMC_CHAIN_BASE, des_chain_phy_addr);
    bscaler_write_reg(BSCALER_FRMC_CHAIN_LEN, cfg->bs_chain_len);
    bscaler_write_reg(BSCALER_FRMC_CHAIN_CTRL, (cfg->bs_chain_irq_mask << 3 |
                                                cfg->c_timeout_irq_mask << 5 |
                                                //1 << 8 | // isum debug
                                                cfg->chain_bus << 1 |
                                                1 << 0));//start hw
#ifdef OPEN_BSC_CSUM
    bsc_csum = 0;
#endif
}
