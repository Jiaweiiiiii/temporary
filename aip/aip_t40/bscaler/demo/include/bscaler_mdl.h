/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : bscaler_mdl.h
 * Authors    : jmqi@taurus
 * Create Time: 2020-04-10:09:29:03
 * Description:
 *
 */

#ifndef __BSCALER_MDL_H__
#define __BSCALER_MDL_H__
#include <stdint.h>

#include "mdl_common.h"
#include "mdl_debug.h"
#include "../../include/api/hal/bscaler_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t             *base0;
    uint8_t             *base1;
    int                 stride0;
    int                 stride1;
    int                 sbox_x;
    int                 sbox_y;
    int                 sbox_w;
    int                 sbox_h;
    int                 format;
    int                 chn;
} bs_box_s;

typedef struct {
    bs_box_s            *src;
    bs_box_s            *dst;
    int32_t             *matrix;
    uint32_t            *coef;
    uint32_t            *offset;
    uint32_t            nv2bgr_order;
    uint32_t            frmc_bw;
    uint32_t            frmc_affine_dir;
    uint32_t            y_gain_exp;
    uint32_t            frmc_bs_dst;
    uint32_t            frmc_fs_dst;
    uint32_t            *frmc_box_base;
    uint32_t            frmc_box_num;
    uint32_t            frmc_padding_en;
    uint32_t            frmc_zero_point;
} bs_info_s;

void bsc_mdl(bsc_hw_once_cfg_s *cfg);
void bst_mdl(bst_hw_once_cfg_s *cfg);
uint32_t get_bst_isum();
uint32_t get_bst_osum();
uint32_t get_bsc_isum();
uint32_t get_bsc_osum();

#ifdef __cplusplus
}
#endif
#endif /* __BSCALER_MDL_H__ */

