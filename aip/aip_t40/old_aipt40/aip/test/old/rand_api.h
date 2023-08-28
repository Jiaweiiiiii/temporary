/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : rand_api.h
 * Authors    : lzhu@joshua
 * Create Time: 2020-07-14:16:01:03
 * Description:
 *
 */

#ifndef __RAND_API_H__
#define __RAND_API_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "bscaler_api.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    bresize_chn2chn,//0
    bresize_nv2chn,//1
    affine_bgr2bgr,//2
    affine_nv2bgr,//3
    affine_nv2nv,//4
    perspective_bgr2bgr,//5
    perspective_nv2bgr,//6
    perspective_nv2nv,//7
}api_mode_sel_e;

typedef struct {
    bs_data_format_e    format;
    int                 mode;
} api_info_s;

void set_run_mode(api_mode_sel_e mode_sel_e, api_info_s *src_cfg, api_info_s *dst_cfg);
void src_image_init(uint32_t src_format, uint32_t bpp_mode,
                    int src_w,int src_ps,int src_h,
                    uint8_t *src_base0);
int matrix_random(uint32_t src_bpp_mode, int mode, float *matrix, int *src_w, int *src_h,
                  int *dst_w, int *dst_h);

#ifdef __cplusplus
}
#endif
#endif /* __RAND_API_H__ */

