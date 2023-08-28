/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : random_api.h
 * Authors    : jmqi@ingenic.st.jz.com
 * Create Time: 2020-08-01:17:55:21
 * Description:
 *
 */

#ifndef __RANDOM_API_H__
#define __RANDOM_API_H__
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "bscaler_api.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RANDOM                      = 0,
    RSZ                         = 1,
    AFFINE                      = 2,
    PERSP                       = 3
} bs_run_mode_e;

typedef struct {
    bs_run_mode_e               mode;
    bs_data_format_e            src_format;
    int                         src_w;
    int                         src_h;
    int                         src_line_stride;
    bool                        src_locate;
    uint8_t                     *src_base;

    bs_data_format_e            dst_format;
    int                         dst_w;
    int                         dst_h;
    int                         dst_line_stride;
    bool                        dst_locate;
    uint8_t                     *dst_base;
    float                       matrix[9];
} bs_api_s;

void set_run_mode(bs_run_mode_e mode, bs_api_s *cfg);

void matrix_random(bs_api_s *cfg);

int get_src_buffer_size(bs_api_s *cfg);

int get_dst_buffer_size(bs_api_s *cfg);

void random_init_space(void *ptr, int size);

int data_check(data_info_s *gld, data_info_s *dut);

void show_bs_api(bs_api_s *cfg);

void bs_api_recoder(bs_api_s *cfg);

void bst_random_api(data_info_s *src, data_info_s *dst, task_info_s *info);

#ifdef __cplusplus
}
#endif
#endif /* __RANDOM_API_H__ */

