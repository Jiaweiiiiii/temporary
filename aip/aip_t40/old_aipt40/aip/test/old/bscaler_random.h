/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : bscaler_case_func.h
 * Authors    : lzhu@abel.ic.jz.com
 * Create Time: 2020-06-04:12:18:31
 * Description:
 *
 */

#ifndef __BSCALER_CASE_FUNC_H__
#define __BSCALER_CASE_FUNC_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <string.h>
//#include "t_bscaler.h"
#include "bscaler_hw_api.h"

typedef enum {
    bresize_chn2chn,//0
    bresize_nv2chn,//1
    lresize_chn2chn,//2
    lresize_nv2chn,//3
    perspective_bgr2bgr,//4
    perspective_nv2bgr,//5
    perspective_nv2nv,//6
}frmc_mode_sel_e;

typedef struct {
    uint8_t *ybase_dst;
    uint8_t *cbase_dst;
    uint32_t frmc_bs_xnum;
    uint32_t frmc_bs_ynum;
    uint32_t frmc_dst_store_mode;
    uint32_t frmc_cfg_fail;
    uint32_t frmc_line_sbx;
    uint32_t frmc_line_sby;
    uint32_t frmc_line_sw;
    uint32_t frmc_line_sh;
    uint32_t dst_line_stride1;
    uint8_t  is_perspective;
    uint8_t *md_base_dst[64];
    uint8_t *md_ybase_dst;
    uint8_t *md_cbase_dst;
    uint8_t  is_first_frame;//only for chain

    FILE *flog;

}frmc_random_s;

void set_run_mode(frmc_mode_sel_e mode_sel_e, bs_hw_once_cfg_s *cfg, uint8_t is_first_frame);
void set_split_ofst(bs_hw_once_cfg_s *cfg);
void init_base(bs_hw_once_cfg_s *cfg, frmc_random_s *radom_cfg);
void set_frmc_src_info(bs_hw_once_cfg_s *cfg);
void set_frmc_dst_info(bs_hw_once_cfg_s *cfg, frmc_random_s *radom_cfg);
void set_bresize_matrix(bs_hw_once_cfg_s *cfg, frmc_random_s *radom_cfg);
void set_lresize_box_info(bs_hw_once_cfg_s *cfg, frmc_random_s *radom_cfg);
void src_image_init(bs_hw_once_cfg_s *cfg);
void printf_info(bs_hw_once_cfg_s *cfg);
uint32_t error_seed_collect(bs_hw_once_cfg_s *cfg, frmc_random_s *radom_cfg, uint32_t seed);

//void bscaler_frmc_init(bs_hw_once_cfg_s *cfg, frmc_random_s *radom_cfg);
void nv2bgr_random(bs_hw_once_cfg_s *cfg);
void perspective_mono_extreme(bs_hw_once_cfg_s *cfg);
void bscaler_mdl_dst_malloc(frmc_random_s *radom_cfg, bs_hw_once_cfg_s *frmc_cfg);
bs_chain_ret_s  bscaler_chain_init(uint32_t frame, bs_hw_once_cfg_s *frmc_cfg, frmc_random_s *radom_cfg);
void bscaler_chain_flog_open(bs_hw_once_cfg_s *cfg, frmc_random_s *radom_cfg);

#endif /* __BSCALER_CASE_FUNC_H__ */

