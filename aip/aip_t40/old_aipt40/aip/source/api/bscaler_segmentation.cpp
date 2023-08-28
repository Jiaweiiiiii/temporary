/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : bscaler_segmentation.cpp
 * Authors    : jmqi@ingenic
 * Create Time: 2021-01-14:15:10:18
 * Description:
 *
 */

#define SYMBOL_EXPORT __attribute__ ((visibility("default")))

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "bscaler_segmentation.h"
#include "matrix.h"

enum MONO {
    INCREASE            = 0,
    DECREASE            = 1,
};

/**
 * get scale up exponent
 */
uint32_t get_exp(float scale_y)
{
    uint32_t n = 1;
    while ((scale_y * (1 << n)) < 1.0f) {
        n++;
    }
    return n;
}

#ifdef RELEASE
void matrix_s32_dump(int32_t *matrix)
{
    printf("Matrix S32:\n");
    printf("%d, %d, %d\n", matrix[0], matrix[1], matrix[2]);
    printf("%d, %d, %d\n", matrix[3], matrix[4], matrix[5]);
    printf("%d, %d, %d\n", matrix[6], matrix[7], matrix[8]);
}

void matrix_f32_dump(float *matrix)
{
    printf("Matrix F32:\n");
    printf("%f, %f, %f\n", matrix[0], matrix[1], matrix[2]);
    printf("%f, %f, %f\n", matrix[3], matrix[4], matrix[5]);
    printf("%f, %f, %f\n", matrix[6], matrix[7], matrix[8]);
}
#endif

/**
 * caculate source sub-box base on destionation box,
 * resize coef, and source whole box
 */
static void resize_cac_sub_box(box_info_s *src, box_info_s *dst,
                               box_info_s *wbox,
                               float scale_x, float scale_y,
                               float trans_x, float trans_y)
{
    float dst_xl = dst->x; // dst_x left
    float dst_xr = dst->x + dst->w - 1; // dst_x right
    float dst_yt = dst->y; // dst_y top
    float dst_yb = dst->y + dst->h - 1; // dst_y bottom
    float t0 = scale_x * dst_xl;
    float t1 = scale_y * dst_yt;
    float t2 = scale_x * dst_xr;
    float t3 = scale_y * dst_yb;

    float src_x0 = t0 + trans_x;
    float src_y0 = t1 + trans_y;
    float src_x1 = t2 + trans_x;
    float src_y1 = t1 + trans_y;
    float src_x2 = t0 + trans_x;
    float src_y2 = t3 + trans_y;
    float src_x3 = t2 + trans_x;
    float src_y3 = t3 + trans_y;

    float max_x = MAX(MAX(src_x0, src_x1), MAX(src_x2, src_x3));
    float min_x = MIN(MIN(src_x0, src_x1), MIN(src_x2, src_x3));
    float max_y = MAX(MAX(src_y0, src_y1), MAX(src_y2, src_y3));
    float min_y = MIN(MIN(src_y0, src_y1), MIN(src_y2, src_y3));

    int src_wbox_w = wbox->w - 1;
    int src_wbox_h = wbox->h - 1;
    min_x = CLIP(min_x, 0, src_wbox_w);
    max_x = CLIP(max_x, 0, src_wbox_w);
    min_y = CLIP(min_y, 0, src_wbox_h);
    max_y = CLIP(max_y, 0, src_wbox_h);

    src->x = ((int)min_x) % 2 ? (int)min_x - 1 : (int)min_x;
    src->y = ((int)min_y) % 2 ? (int)min_y - 1 : (int)min_y;
    src->w = (int)ceilf(max_x) - src->x + 1;
    src->h = (int)ceilf(max_y) - src->y + 1;
    if (src->w % 2) { //maybe overflow, hardware ok, but software maybe error
        src->w++;
    }
    if (src->h % 2) { //maybe overflow, hardware ok, but software maybe error
        src->h++;
    }
}

/**
 * caculate source sub-box base on destionation box,
 * affine matrix, and source whole box
 */
static void affine_cac_sub_box(box_info_s *src, box_info_s *dst,
                               int32_t *matrix, box_info_s *wbox)
{
    int dst_xl = dst->x; // left
    int dst_xr = dst->x + dst->w - 1; // right
    int dst_yt = dst->y; // top
    int dst_yb = dst->y + dst->h - 1; // bottom

    int64_t t0 = (int64_t)matrix[MSCALEX] * dst_xl;
    int64_t t1 = (int64_t)matrix[MSKEWX] * dst_yt;
    int64_t t2 = (int64_t)matrix[MSKEWY] * dst_xl;
    int64_t t3 = (int64_t)matrix[MSCALEY] * dst_yt;
    int64_t t4 = (int64_t)matrix[MSCALEX] * dst_xr;
    int64_t t5 = (int64_t)matrix[MSKEWY] * dst_xr;
    int64_t t6 = (int64_t)matrix[MSKEWX] * dst_yb;
    int64_t t7 = (int64_t)matrix[MSCALEY] * dst_yb;

    int64_t src_s64_x0 = t0 + t1 + matrix[MTRANSX];
    int64_t src_s64_y0 = t2 + t3 + matrix[MTRANSY];
    int64_t src_s64_x1 = t4 + t1 + matrix[MTRANSX];
    int64_t src_s64_y1 = t5 + t3 + matrix[MTRANSY];
    int64_t src_s64_x2 = t0 + t6 + matrix[MTRANSX];
    int64_t src_s64_y2 = t2 + t7 + matrix[MTRANSY];
    int64_t src_s64_x3 = t4 + t6 + matrix[MTRANSX];
    int64_t src_s64_y3 = t5 + t7 + matrix[MTRANSY];

    int32_t src_x0 = (int32_t)CLIP(src_s64_x0 >> MAT_ACC, 0, S32_MAX);
    int32_t src_y0 = (int32_t)CLIP(src_s64_y0 >> MAT_ACC, 0, S32_MAX);
    int32_t src_x1 = (int32_t)CLIP(src_s64_x1 >> MAT_ACC, 0, S32_MAX);
    int32_t src_y1 = (int32_t)CLIP(src_s64_y1 >> MAT_ACC, 0, S32_MAX);
    int32_t src_x2 = (int32_t)CLIP(src_s64_x2 >> MAT_ACC, 0, S32_MAX);
    int32_t src_y2 = (int32_t)CLIP(src_s64_y2 >> MAT_ACC, 0, S32_MAX);
    int32_t src_x3 = (int32_t)CLIP(src_s64_x3 >> MAT_ACC, 0, S32_MAX);
    int32_t src_y3 = (int32_t)CLIP(src_s64_y3 >> MAT_ACC, 0, S32_MAX);

    int32_t max_x = MAX(MAX(src_x0, src_x1), MAX(src_x2, src_x3));
    int32_t min_x = MIN(MIN(src_x0, src_x1), MIN(src_x2, src_x3));
    int32_t max_y = MAX(MAX(src_y0, src_y1), MAX(src_y2, src_y3));
    int32_t min_y = MIN(MIN(src_y0, src_y1), MIN(src_y2, src_y3));

    int src_wbox_w = wbox->w - 1;
    int src_wbox_h = wbox->h - 1;
    min_x = CLIP(min_x, 0, src_wbox_w);
    max_x = CLIP(max_x + 1, 0, src_wbox_w);
    min_y = CLIP(min_y, 0, src_wbox_h);
    max_y = CLIP(max_y + 1, 0, src_wbox_h);

    src->x = min_x % 2 ? min_x - 1 : min_x;
    src->y = min_y % 2 ? min_y - 1 : min_y;

    int32_t src_w = max_x + 1 - src->x;
    int32_t src_h = max_y + 1 - src->y;
    if (src_w % 2) { //maybe overflow, hardware ok, but software maybe error
        src_w++;
    }

    if (src_h % 2) { //maybe overflow, hardware ok, but software maybe error
        src_h++;
    }
    src->w = src_w;
    src->h = src_h;
}

/**
 * caculate source sub-box base on destionation box,
 * perspective matrix, and source whole box
 */
static void perspective_cac_sub_box(box_info_s *src, box_info_s *dst,
                                    int32_t *matrix, box_info_s *wbox)
{
    int dst_xl = dst->x;
    int dst_xr = dst->x + dst->w - 1;
    int dst_yt = dst->y;
    int dst_yb = dst->y + dst->h - 1;
    int64_t t0 = (int64_t)matrix[0] * dst_xl;
    int64_t t1 = (int64_t)matrix[1] * dst_yt;
    int64_t t2 = (int64_t)matrix[3] * dst_xl;
    int64_t t3 = (int64_t)matrix[4] * dst_yt;
    int64_t t4 = (int64_t)matrix[6] * dst_xl;
    int64_t t5 = (int64_t)matrix[7] * dst_yt;
    int64_t t6 = (int64_t)matrix[0] * dst_xr;
    int64_t t7 = (int64_t)matrix[3] * dst_xr;
    int64_t t8 = (int64_t)matrix[6] * dst_xr;
    int64_t t9 = (int64_t)matrix[1] * dst_yb;
    int64_t t10 = (int64_t)matrix[4] * dst_yb;
    int64_t t11 = (int64_t)matrix[7] * dst_yb;
    int64_t sx0_s64 = (t0 + t1 + matrix[2]);
    int64_t sy0_s64 = (t2 + t3 + matrix[5]);
    int64_t sz0_s64 = (t4 + t5 + matrix[8]);
    int64_t sx1_s64 = (t1 + t6 + matrix[2]);
    int64_t sy1_s64 = (t3 + t7 + matrix[5]);
    int64_t sz1_s64 = (t5 + t8 + matrix[8]);
    int64_t sx2_s64 = (t0 + t9 + matrix[2]);
    int64_t sy2_s64 = (t2 + t10 + matrix[5]);
    int64_t sz2_s64 = (t4 + t11 + matrix[8]);
    int64_t sx3_s64 = (t6 + t9 + matrix[2]);
    int64_t sy3_s64 = (t7 + t10 + matrix[5]);
    int64_t sz3_s64 = (t8 + t11 + matrix[8]);

    int32_t src_x0 = -2, src_y0 = -2;
    if (sz0_s64) {
        int64_t sx_s64_final = sx0_s64 * (1 << MAT_ACC) / sz0_s64;
        int64_t sy_s64_final = sy0_s64 * (1 << MAT_ACC) / sz0_s64;
        int32_t sx_s32_clip = (int32_t)CLIP(sx_s64_final, INT_MIN, INT_MAX);
        int32_t sy_s32_clip = (int32_t)CLIP(sy_s64_final, INT_MIN, INT_MAX);
        src_x0 = sx_s32_clip >> MAT_ACC;
        src_y0 = sy_s32_clip >> MAT_ACC;
    }

    int32_t src_x1 = -2, src_y1 = -2;
    if (sz1_s64) {
        int64_t sx_s64_final = sx1_s64 * (1 << MAT_ACC) / sz1_s64;
        int64_t sy_s64_final = sy1_s64 * (1 << MAT_ACC) / sz1_s64;
        int32_t sx_s32_clip = (int32_t)CLIP(sx_s64_final, INT_MIN, INT_MAX);
        int32_t sy_s32_clip = (int32_t)CLIP(sy_s64_final, INT_MIN, INT_MAX);
        src_x1 = sx_s32_clip >> MAT_ACC;
        src_y1 = sy_s32_clip >> MAT_ACC;
    }

    int32_t src_x2 = -2, src_y2 = -2;
    if (sz2_s64) {
        int64_t sx_s64_final = sx2_s64 * (1 << MAT_ACC) / sz2_s64;
        int64_t sy_s64_final = sy2_s64 * (1 << MAT_ACC) / sz2_s64;
        int32_t sx_s32_clip = (int32_t)CLIP(sx_s64_final, INT_MIN, INT_MAX);
        int32_t sy_s32_clip = (int32_t)CLIP(sy_s64_final, INT_MIN, INT_MAX);
        src_x2 = sx_s32_clip >> MAT_ACC;
        src_y2 = sy_s32_clip >> MAT_ACC;
    }

    int32_t src_x3 = -2, src_y3 = -2;
    if (sz3_s64) {
        int64_t sx_s64_final = sx3_s64 * (1 << MAT_ACC) / sz3_s64;
        int64_t sy_s64_final = sy3_s64 * (1 << MAT_ACC) / sz3_s64;
        int32_t sx_s32_clip = (int32_t)CLIP(sx_s64_final, INT_MIN, INT_MAX);
        int32_t sy_s32_clip = (int32_t)CLIP(sy_s64_final, INT_MIN, INT_MAX);
        src_x3 = sx_s32_clip >> MAT_ACC;
        src_y3 = sy_s32_clip >> MAT_ACC;
    }

    int32_t max_x = MAX(MAX(src_x0, src_x1), MAX(src_x2, src_x3));
    int32_t min_x = MIN(MIN(src_x0, src_x1), MIN(src_x2, src_x3));
    int32_t max_y = MAX(MAX(src_y0, src_y1), MAX(src_y2, src_y3));
    int32_t min_y = MIN(MIN(src_y0, src_y1), MIN(src_y2, src_y3));

    int src_wbox_w = wbox->w - 1;
    int src_wbox_h = wbox->h - 1;
    min_x = CLIP(min_x, 0, src_wbox_w);
    max_x = CLIP(max_x + 1, 0, src_wbox_w);
    min_y = CLIP(min_y, 0, src_wbox_h);
    max_y = CLIP(max_y + 1, 0, src_wbox_h);
    src->x = min_x % 2 ? min_x - 1 : min_x;
    src->y = min_y % 2 ? min_y - 1 : min_y;
    src->w = (max_x == min_x) ? 0 : max_x - src->x + 1;
    src->h = (max_y == min_y) ? 0 : max_y - src->y + 1;
    if (src->w % 2) {//maybe overflow, hardware ok, but software maybe error
        src->w++;
    }

    if (src->h % 2) {//maybe overflow, hardware ok, but software maybe error
        src->h++;
    }
}

/**
 * generate hardware once configure information
 */
void bs_affine_cfg_stuff(std::vector<bsc_hw_once_cfg_s> &bs_cfgs,
                         box_affine_info_s *info,
                         const data_info_s *src, data_info_s *dst,
                         const uint32_t *coef, const uint32_t *offset)
{
    assert(src->format != BS_DATA_VBGR);
    assert(src->format != BS_DATA_VRGB);
    assert(src->format != BS_DATA_FMU2);
    assert(src->format != BS_DATA_FMU4);
    assert(src->format != BS_DATA_FMU8);

    bsc_hw_once_cfg_s cfg;
    box_info_s *box = &(info->box);
    uint8_t *src_base1 = NULL;
    if (src->format == BS_DATA_NV12) {
        assert(box->x % 2 == 0);
        assert(box->y % 2 == 0);
        assert(box->w % 2 == 0);
        assert(box->h % 2 == 0);
        cfg.src_base0 = (uint8_t *)src->base +
            box->y * src->line_stride + box->x;
        src_base1 = (uint8_t *)src->base + src->height * src->line_stride +
            box->y / 2 * src->line_stride + box->x;
    } else if (src->format & 0x1) {//RGBA,ARGB,BRGA....
        cfg.src_base0 = (uint8_t *)src->base +
            box->y * src->line_stride + box->x * 4;
    } else {
        assert(0);
    }
    cfg.src_base1 = src_base1;
    cfg.src_format = (bsc_hw_data_format_e)src->format;
    cfg.src_line_stride = src->line_stride;
    cfg.src_box_x = 0;
    cfg.src_box_y = 0;
    cfg.src_box_w = box->w;
    cfg.src_box_h = box->h;

    //single destination box info(size must same)
    cfg.dst_base[0] = (uint8_t *)dst->base;
    if (dst->format == BS_DATA_NV12) {
        cfg.dst_base[1] = (uint8_t *)dst->base + dst->height * dst->line_stride;
    }
    cfg.dst_format = (bsc_hw_data_format_e)dst->format;
    cfg.dst_line_stride = dst->line_stride;
    cfg.dst_box_x = 0;
    cfg.dst_box_y = 0;
    cfg.dst_box_w = dst->width;
    cfg.dst_box_h = dst->height;

    cfg.affine = true;
    cfg.box_mode = true;
    for (int i = 0; i < 9; i++) {
        cfg.coef[i] = coef[i];
    }
    for (int i = 0; i < 2; i++) {
        cfg.offset[i] = offset[i];
    }
    cfg.zero_point = info->zero_point;
    float inverse[9];
    get_inverse_matrix(info->matrix, inverse);
    matrix_float_to_s32(inverse, cfg.matrix);

    cfg.y_gain_exp = 0;
    cfg.box_num = 1; //always 1
    cfg.boxes_info = NULL;//not active
    cfg.box_bus = 0; //not active
    cfg.ibufc_bus = src->locate;
    cfg.obufc_bus = dst->locate;
    cfg.irq_mask = 1; //fixme
    cfg.isum = 0;//delete me??
    cfg.osum = 0;//delete me??
    bs_cfgs.push_back(cfg);
}

static void perspective_mono(int32_t *matrix,
                             int32_t *sx_mono, bool *sx_mono_increase,
                             int32_t *sy_mono, bool *sy_mono_increase)
{
    double m1xm8 = (double)matrix[1] * matrix[8];
    double m2xm7 = (double)matrix[2] * matrix[7];
    double m0xm7 = (double)matrix[0] * matrix[7];
    double m1xm6 = (double)matrix[1] * matrix[6];

    double m4xm8 = (double)matrix[4] * matrix[8];
    double m5xm7 = (double)matrix[5] * matrix[7];
    double m3xm7 = (double)matrix[3] * matrix[7];
    double m4xm6 = (double)matrix[4] * matrix[6];

    //matrix_s32_dump(matrix);

    // sx = f'(dx)
    double sxf_dx;
    if (m0xm7 - m1xm6) {
        sxf_dx = (m1xm8 - m2xm7) / (m0xm7 - m1xm6);
        *sx_mono_increase = (m1xm6 > m0xm7) ? INCREASE : DECREASE;
    } else {
        sxf_dx = -1;
        if ((m1xm8 - m2xm7) < 0) { //always decrease
            *sx_mono_increase = DECREASE;
        } else { //always increase
            *sx_mono_increase = INCREASE;
        }
    }
    int sxf_falg = 0;//add by lzhu
    if ((sxf_dx < 0) & (sxf_dx > -1)) {
        sxf_falg = 1;
    }

    // sy = f'(dx)
    double syf_dx;
    if (m3xm7 - m4xm6) {
        syf_dx = (m4xm8 - m5xm7) / (m3xm7 - m4xm6);
        *sy_mono_increase = (m4xm6 > m3xm7) ? INCREASE : DECREASE;
    } else {
        syf_dx = -1;
        if ((m4xm8 - m5xm7) < 0) {
            *sy_mono_increase = DECREASE;
        } else {
            *sy_mono_increase = INCREASE;
        }
    }

    int syf_falg = 0;//add by lzhu
    if ((syf_dx < 0) & (syf_dx > -1)) {
        syf_falg = 1;
    }
    if (sxf_falg == 1) {
        *sx_mono = 0;
    } else {
        *sx_mono = (int32_t)ceil(sxf_dx);
    }
    if (syf_falg == 1) {
        *sy_mono = 0;
    } else {
        *sy_mono = (int32_t)ceil(syf_dx);
    }
}

/**
 * generate hardware once configure information
 */
void bs_perspective_cfg_stuff(std::vector<bsc_hw_once_cfg_s> &bs_cfgs,
                              box_affine_info_s *info,
                              const data_info_s *src, data_info_s *dst,
                              const uint32_t *coef, const uint32_t *offset)
{
    assert(src->format != BS_DATA_VBGR);
    assert(src->format != BS_DATA_VRGB);
    assert(src->format != BS_DATA_FMU2);
    assert(src->format != BS_DATA_FMU4);
    assert(src->format != BS_DATA_FMU8);

    bsc_hw_once_cfg_s cfg;
    box_info_s *box = &(info->box);
    uint8_t *src_base1 = NULL;
    if (src->format == BS_DATA_NV12) {
        assert(box->x % 2 == 0);
        assert(box->y % 2 == 0);
        assert(box->w % 2 == 0);
        assert(box->h % 2 == 0);
        cfg.src_base0 = (uint8_t *)src->base +
            box->y * src->line_stride + box->x;
        src_base1 = (uint8_t *)src->base + src->height * src->line_stride +
            box->y / 2 * src->line_stride + box->x;
    } else if (src->format & 0x1) {//RGBA,ARGB,BRGA....
        cfg.src_base0 = (uint8_t *)src->base +
            box->y * src->line_stride + box->x * 4;
    } else {
        assert(0);
    }
    cfg.src_base1 = src_base1;
    cfg.src_format = (bsc_hw_data_format_e)src->format;
    cfg.src_line_stride = src->line_stride;
    cfg.src_box_x = 0;
    cfg.src_box_y = 0;
    cfg.src_box_w = box->w;
    cfg.src_box_h = box->h;

    //single destination box info(size must same)
    cfg.dst_base[0] = (uint8_t *)dst->base;
    if (dst->format == BS_DATA_NV12) {
        cfg.dst_base[1] = (uint8_t *)dst->base + dst->height * dst->line_stride;
    }
    cfg.dst_format = (bsc_hw_data_format_e)dst->format;
    cfg.dst_line_stride = dst->line_stride;
    cfg.dst_box_x = 0;
    cfg.dst_box_y = 0;
    cfg.dst_box_w = dst->width;
    cfg.dst_box_h = dst->height;

    cfg.affine = true;
    cfg.box_mode = true;
    for (int i = 0; i < 9; i++) {
        cfg.coef[i] = coef[i];
    }
    for (int i = 0; i < 2; i++) {
        cfg.offset[i] = offset[i];
    }
    cfg.zero_point = info->zero_point;
    float inverse[9];
    get_inverse_matrix(info->matrix, inverse);
    matrix_float_to_s32(inverse, cfg.matrix);

    int32_t mono_x_ep, mono_y_ep;//extreme_point
    bool mono_sx, mono_sy;
    perspective_mono(cfg.matrix, &mono_x_ep, &mono_sx, &mono_y_ep, &mono_sy);
    if (mono_x_ep > 64) {
        cfg.mono_x = (mono_sx << 7) | 64;
    } else if (mono_x_ep < 0) {
        cfg.mono_x = (mono_sx << 7) | 0;
    } else {
        cfg.mono_x = (mono_sx << 7) | mono_x_ep;
    }

    if (mono_y_ep > 64) {
        cfg.mono_y = (mono_sy << 7) | 64;
    } else if (mono_y_ep < 0) {
        cfg.mono_y = (mono_sy << 7) | 0;
    } else {
        cfg.mono_y = (mono_sy << 7) | mono_y_ep;
    }
    double m6_div_m7 = 0;
    double m8_div_m7 = 0;
    if (cfg.matrix[7]) {
        m6_div_m7 = (double)cfg.matrix[6] / (double)cfg.matrix[7];
        m8_div_m7 = (double)cfg.matrix[8] / (double)cfg.matrix[7];
    }

    for (int dx = 0; dx < dst->width; dx++) {
        double act_extreme_point = -dx * m6_div_m7 - m8_div_m7;
        int act_extreme_point_s32 = act_extreme_point;
        int flag = 0;
        if ((act_extreme_point >-1) & (act_extreme_point <0)) {
            flag = 1;
        }
        if (flag) {
            act_extreme_point_s32 = 0;
        }
        else if(act_extreme_point_s32 == act_extreme_point){
            act_extreme_point_s32 = act_extreme_point;
        }else{
            act_extreme_point_s32 += 1;
            /*if (mono_sy == INCREASE) {
              if (dx > mono_y_ep) {
              act_extreme_point_s32 += 1;
              }
              } else {
              if (dx < mono_y_ep) {
              act_extreme_point_s32 += 1;
              }
              }*/
        }

        cfg.extreme_point[dx] = (int8_t)(act_extreme_point_s32 < 0 ? 0://-1 :
                                         act_extreme_point_s32 > (dst->height - 1) ? 64 :
                                         act_extreme_point_s32);
    }

    cfg.y_gain_exp = 0;
    cfg.box_num = 1;//always 1
    cfg.boxes_info = NULL;//not active
    cfg.box_bus = 0; //not active
    cfg.ibufc_bus = src->locate;
    cfg.obufc_bus = dst->locate;
    cfg.irq_mask = 1; //fixme
    cfg.isum = 0;//delete me ???
    cfg.osum = 0;//delete me ???
    bs_cfgs.push_back(cfg);
}

/**
 * user configure affine mode but it's can used resize
 */
void bs_affine_resize_cfg_stuff(std::vector<bsc_hw_once_cfg_s> &bs_cfgs,
                                box_affine_info_s *info,
                                const data_info_s *src, data_info_s *dst,
                                const uint32_t *coef, const uint32_t *offset)
{
    bsc_hw_once_cfg_s cfg;
    box_info_s *box = &(info->box);
    uint8_t *src_base1 = NULL;
    if (src->format == BS_DATA_NV12) {
        assert(box->x % 2 == 0);
        assert(box->y % 2 == 0);
        assert(box->w % 2 == 0);
        assert(box->h % 2 == 0);
        cfg.src_base0 = (uint8_t *)src->base +
            box->y * src->line_stride + box->x;
        src_base1 = (uint8_t *)src->base + src->height * src->line_stride +
            box->y / 2 * src->line_stride + box->x;
    } else if(src->format == BS_DATA_FMU2) {
        cfg.src_base0 = (uint8_t *)src->base +
            box->y * src->line_stride + box->x * 8;
    } else if(src->format == BS_DATA_FMU4) {
        cfg.src_base0 = (uint8_t *)src->base +
            box->y * src->line_stride + box->x * 16;
    } else if(src->format == BS_DATA_FMU8) {
        cfg.src_base0 = (uint8_t *)src->base +
            box->y * src->line_stride + box->x * 32;
    } else if (src->format & 0x1) {//RGBA,ARGB, BRGA....
        cfg.src_base0 = (uint8_t *)src->base +
            box->y * src->line_stride + box->x * 4;
    } else {
        assert(0);
    }

    cfg.src_base1 = src_base1;
    cfg.src_format = (bsc_hw_data_format_e)src->format;
    cfg.src_line_stride = src->line_stride;
    cfg.src_box_x = 0;
    cfg.src_box_y = 0;
    cfg.src_box_w = box->w;
    cfg.src_box_h = box->h;

    //single destination box info(size must same)
    cfg.dst_base[0] = (uint8_t *)dst->base;
    if (dst->format == BS_DATA_NV12) {
        cfg.dst_base[1] = (uint8_t *)dst->base + dst->height * dst->line_stride;
    }
    cfg.dst_format = (bsc_hw_data_format_e)dst->format;
    cfg.dst_line_stride = dst->line_stride;
    cfg.dst_box_x = 0;
    cfg.dst_box_y = 0;
    cfg.dst_box_w = dst->width;
    cfg.dst_box_h = dst->height;

    cfg.affine = false;
    cfg.box_mode = true;
    for (int i = 0; i < 9; i++) {
        cfg.coef[i] = coef[i];
    }
    for (int i = 0; i < 2; i++) {
        cfg.offset[i] = offset[i];
    }
    cfg.zero_point = info->zero_point;//fixme

    float scale_x = info->matrix[MSCALEX];
    float scale_y = info->matrix[MSCALEY];
    float trans_x = info->matrix[MTRANSX];
    float trans_y = info->matrix[MTRANSY];

    float inverse_scale_x = scale_x ? (1 / scale_x) : 0;
    float inverse_trans_x = trans_x ? (1 / trans_x) : 0;
    float inverse_scale_y = scale_y ? (1 / scale_y) : 0;
    float inverse_trans_y = trans_y ? (1 / trans_y) : 0;

    cfg.matrix[MSCALEX] = (int32_t)round(inverse_scale_x * (1 << MAT_ACC));
    cfg.matrix[MSKEWX] = 0;
    cfg.matrix[MTRANSX] = (int32_t)round(inverse_trans_x * (1 << MAT_ACC));
    cfg.matrix[MSKEWY] = 0;
    cfg.matrix[MSCALEY] = (int32_t)round(inverse_scale_y * (1 << MAT_ACC));
    cfg.matrix[MTRANSY] = (int32_t)round(inverse_trans_y * (1 << MAT_ACC));
    cfg.matrix[MPERSP0] = 0;
    cfg.matrix[MPERSP1] = 0;
    cfg.matrix[MPERSP2] = 0;

    cfg.y_gain_exp = 0;
    cfg.box_num = 1;//always 1
    cfg.boxes_info = NULL;//not active
    cfg.box_bus = 0; //not active
    cfg.ibufc_bus = src->locate;
    cfg.obufc_bus = dst->locate;
    cfg.irq_mask = 1; //fixme
    cfg.isum = 0;//delete me ???
    cfg.osum = 0;//delete me ???
    bs_cfgs.push_back(cfg);
}

/**
 * affine mode, but hardware can not do it directly, must do it separately
 */
void affine_box_split(std::vector<bsc_hw_once_cfg_s> &bs_cfgs,
                      box_affine_info_s *info,
                      const data_info_s *src, data_info_s *dst,
                      const uint32_t *coef, const uint32_t *offset)
{
    assert(src->format != BS_DATA_VBGR);
    assert(src->format != BS_DATA_VRGB);
    assert(src->format != BS_DATA_FMU2);
    assert(src->format != BS_DATA_FMU4);
    assert(src->format != BS_DATA_FMU8);

    box_info_s *box = &(info->box);
    /* 1. caculate source whole-box base address */
    uint8_t *src_wbox_base0 = NULL;
    uint8_t *src_wbox_base1 = NULL;
    if (src->format == BS_DATA_NV12) {
        assert(box->x % 2 == 0);
        assert(box->y % 2 == 0);
        assert(box->w % 2 == 0);
        assert(box->h % 2 == 0);
        src_wbox_base0 = (uint8_t *)src->base +
            box->y * src->line_stride + box->x;
        if (src->base1 == NULL) {
            src_wbox_base1 = (uint8_t *)src->base + src->height * src->line_stride +
                box->y / 2 * src->line_stride + box->x;
        } else {
            src_wbox_base1 = (uint8_t *)src->base1;
        }
    } else if (src->format & 0x1) {//RGBA,ARGB, BRGA....
        src_wbox_base0= (uint8_t *)src->base +
            box->y * src->line_stride + box->x * 4;
    } else {
        assert(0);
    }

    /* 2. caculate source whole-box base address */
    uint8_t *dst_wbox_base0 = (uint8_t *)(dst->base);
    uint8_t *dst_wbox_base1 = NULL;
    if (dst->format == BS_DATA_NV12) {
        if (dst->base1 == NULL) {
            dst_wbox_base1 = (uint8_t *)dst->base + dst->height * dst->line_stride;
        } else {
            dst_wbox_base1 = (uint8_t *)dst->base1;
        }
    }

    /* 3. caculate destination/source sub-box information */
    float inverse[9];
    get_inverse_matrix(info->matrix, inverse);
    int32_t matrix_s32[9];
    matrix_float_to_s32(inverse, matrix_s32);

    const int sbox_num_w = (dst->width + BS_AFFINE_SIZE - 1) / BS_AFFINE_SIZE;
    const int sbox_num_h = (dst->height + BS_AFFINE_SIZE - 1) / BS_AFFINE_SIZE;
    int sbox_num = sbox_num_w * sbox_num_h;
    // caculate dst sub-box information
    box_info_s *dst_sbox = (box_info_s *)malloc(sizeof(box_info_s) * sbox_num);
    if (dst_sbox == NULL) {
        fprintf(stderr, "alloc space failed!\n");
    }
    int i, j;
    for (j = 0; j < sbox_num_h; j++) {
        for (i = 0; i < sbox_num_w; i++) {
            dst_sbox[j * sbox_num_w + i].x = i * BS_AFFINE_SIZE;
            dst_sbox[j * sbox_num_w + i].y = j * BS_AFFINE_SIZE;
            int sbox_w = (i == (sbox_num_w - 1)) ?
                (dst->width - 1) % BS_AFFINE_SIZE + 1 : BS_AFFINE_SIZE;
            int sbox_h = (j == (sbox_num_h - 1)) ?
                (dst->height - 1) % BS_AFFINE_SIZE + 1 : BS_AFFINE_SIZE;
            dst_sbox[j * sbox_num_w + i].w = sbox_w;
            dst_sbox[j * sbox_num_w + i].h = sbox_h;
        }
    }
    // caculate src sub-box information
    box_info_s *src_sbox = (box_info_s *)malloc(sizeof(box_info_s) * sbox_num);
    if (src_sbox == NULL) {
        fprintf(stderr, "alloc space failed!\n");
    }

    box_info_s wbox = {0, 0, box->w, box->h};
    for (int i = 0; i < sbox_num; i++) {
        affine_cac_sub_box(src_sbox + i, dst_sbox + i, matrix_s32, &wbox);
    }

    /* 4. most members of each sub-box are the same,
       only sub-box x,y,w,h are different. */
    bsc_hw_once_cfg_s cfg;
    memcpy(cfg.matrix, matrix_s32, sizeof(int32_t) * 9);

    cfg.y_gain_exp = 0;
    cfg.src_format = (bsc_hw_data_format_e)src->format;
    cfg.src_line_stride = src->line_stride;

    cfg.dst_format = (bsc_hw_data_format_e)dst->format;
    cfg.dst_line_stride = dst->line_stride;
    cfg.affine = true;
    cfg.box_mode = true;

    for (int i = 0; i < 9; i++) {
        cfg.coef[i] = coef[i];
    }
    for (int i = 0; i < 2; i++) {
        cfg.offset[i] = offset[i];
    }

    cfg.zero_point = info->zero_point;
    cfg.box_num = 1;//always 1
    cfg.boxes_info = NULL;//not active
    cfg.box_bus = 0; //not active
    cfg.ibufc_bus = src->locate;
    cfg.obufc_bus = dst->locate;
    cfg.irq_mask = 1; //fixme
    cfg.isum = 0;//delete me ??
    cfg.osum = 0;//delete me ??

    for (int i = 0; i < sbox_num; i++) {
        uint8_t *src_sbox_base0 = NULL;
        uint8_t *src_sbox_base1 = NULL;
        if (src->format == BS_DATA_NV12) {
            assert(box->x % 2 == 0);
            assert(box->y % 2 == 0);
            assert(box->w % 2 == 0);
            assert(box->h % 2 == 0);
            src_sbox_base0 = src_wbox_base0 +
                src_sbox[i].y * src->line_stride + src_sbox[i].x;
            src_sbox_base1 = src_wbox_base1 +
                src_sbox[i].y / 2 * src->line_stride + src_sbox[i].x;
        } else if(src->format == BS_DATA_FMU2) {
            src_sbox_base0 = src_wbox_base0 +
                src_sbox[i].y * src->line_stride + src_sbox[i].x * 8;
        } else if(src->format == BS_DATA_FMU4) {
            src_sbox_base0 = src_wbox_base0 +
                src_sbox[i].y * src->line_stride + src_sbox[i].x * 16;
        } else if(src->format == BS_DATA_FMU8) {
            src_sbox_base0 = src_wbox_base0 +
                src_sbox[i].y * src->line_stride + src_sbox[i].x * 32;
        } else if (src->format & 0x1) {//RGBA,ARGB, BRGA....
            src_sbox_base0 = src_wbox_base0 +
                src_sbox[i].y * src->line_stride + src_sbox[i].x * 4;
        }

        cfg.src_base0 = src_sbox_base0;
        cfg.src_base1 = src_sbox_base1;

        cfg.src_box_x = src_sbox[i].x;
        cfg.src_box_y = src_sbox[i].y;
        cfg.src_box_w = src_sbox[i].w;
        cfg.src_box_h = src_sbox[i].h;

        uint8_t *dst_sbox_base0 = NULL;
        uint8_t *dst_sbox_base1 = NULL;
        if (dst->format == BS_DATA_NV12) {
            assert(box->x % 2 == 0);
            assert(box->y % 2 == 0);
            assert(box->w % 2 == 0);
            assert(box->h % 2 == 0);
            dst_sbox_base0 = dst_wbox_base0 +
                dst_sbox[i].y * dst->line_stride + dst_sbox[i].x;
            dst_sbox_base1 = dst_wbox_base0 + dst->height * dst->line_stride +
                dst_sbox[i].y / 2 * dst->line_stride + dst_sbox[i].x;
        } else if (dst->format & 0x1) {//RGBA,ARGB, BRGA....
            dst_sbox_base0 = dst_wbox_base0 +
                dst_sbox[i].y * dst->line_stride + dst_sbox[i].x * 4;
        }

        cfg.dst_base[0] = dst_sbox_base0;
        cfg.dst_base[1] = dst_sbox_base1;
        cfg.dst_box_x = dst_sbox[i].x;
        cfg.dst_box_y = dst_sbox[i].y;
        cfg.dst_box_w = dst_sbox[i].w;
        cfg.dst_box_h = dst_sbox[i].h;
        bs_cfgs.push_back(cfg);
    }
    free(dst_sbox);
    free(src_sbox);
}

/**
 * perspective mode, but hardware can not do it directly, must do it separately
 */
void perspective_box_split(std::vector<bsc_hw_once_cfg_s> &bs_cfgs,
                           box_affine_info_s *info,
                           const data_info_s *src, data_info_s *dst,
                           const uint32_t *coef, const uint32_t *offset)
{
    assert(src->format != BS_DATA_VBGR);
    assert(src->format != BS_DATA_VRGB);
    assert(src->format != BS_DATA_FMU2);
    assert(src->format != BS_DATA_FMU4);
    assert(src->format != BS_DATA_FMU8);

    box_info_s *box = &(info->box);
    /* 1. caculate source whole-box base address */
    uint8_t *src_wbox_base0 = NULL;
    uint8_t *src_wbox_base1 = NULL;
    if (src->format == BS_DATA_NV12) {
        assert(box->x % 2 == 0);
        assert(box->y % 2 == 0);
        assert(box->w % 2 == 0);
        assert(box->h % 2 == 0);
        src_wbox_base0 = (uint8_t *)src->base +
            box->y * src->line_stride + box->x;
        if (src->base1 == NULL) {
            src_wbox_base1 = (uint8_t *)src->base + src->height * src->line_stride +
                box->y / 2 * src->line_stride + box->x;
        } else {
            src_wbox_base1 = (uint8_t *)src->base1;
        }
    } else if (src->format & 0x1) {//RGBA,ARGB,BRGA....
        src_wbox_base0= (uint8_t *)src->base +
            box->y * src->line_stride + box->x * 4;
    } else {
        assert(0);
    }

    /* 2. caculate source whole-box base address */
    uint8_t *dst_wbox_base0 = (uint8_t *)(dst->base);
    uint8_t *dst_wbox_base1 = NULL;
    if (dst->format == BS_DATA_NV12) {
        if (dst->base1 == NULL) {
            dst_wbox_base1 = (uint8_t *)dst->base + dst->height * dst->line_stride;
        } else {
            dst_wbox_base1 = (uint8_t *)dst->base1;
        }
    }

    /* 3. caculate destination/source sub-box information */
    float inverse[9];
    get_inverse_matrix(info->matrix, inverse);
    int32_t matrix_s32[9];
    matrix_float_to_s32(inverse, matrix_s32);

    const int sbox_num_w = (dst->width + BS_AFFINE_SIZE - 1) / BS_AFFINE_SIZE;
    const int sbox_num_h = (dst->height + BS_AFFINE_SIZE - 1) / BS_AFFINE_SIZE;
    int sbox_num = sbox_num_w * sbox_num_h;
    // caculate dst sub-box information
    box_info_s *dst_sbox = (box_info_s *)malloc(sizeof(box_info_s) * sbox_num);
    if (dst_sbox == NULL) {
        fprintf(stderr, "alloc space failed!\n");
    }
    int i, j;
    for (j = 0; j < sbox_num_h; j++) {
        for (i = 0; i < sbox_num_w; i++) {
            dst_sbox[j * sbox_num_w + i].x = i * BS_AFFINE_SIZE;
            dst_sbox[j * sbox_num_w + i].y = j * BS_AFFINE_SIZE;
            int sbox_w = (i == (sbox_num_w - 1)) ?
                (dst->width - 1) % BS_AFFINE_SIZE + 1 : BS_AFFINE_SIZE;
            int sbox_h = (j == (sbox_num_h - 1)) ?
                (dst->height - 1) % BS_AFFINE_SIZE + 1 : BS_AFFINE_SIZE;
            dst_sbox[j * sbox_num_w + i].w = sbox_w;
            dst_sbox[j * sbox_num_w + i].h = sbox_h;
        }
    }
    // caculate src sub-box information
    box_info_s *src_sbox = (box_info_s *)malloc(sizeof(box_info_s) * sbox_num);
    if (src_sbox == NULL) {
        fprintf(stderr, "alloc space failed!\n");
    }

    box_info_s wbox = {0, 0, box->w, box->h};
    for (int i = 0; i < sbox_num; i++) {
        perspective_cac_sub_box(src_sbox + i, dst_sbox + i, matrix_s32, &wbox);
    }

    /* 4. most members of each sub-box are the same,
       only sub-box x,y,w,h are different. */
    bsc_hw_once_cfg_s cfg;
    matrix_float_to_s32(inverse, cfg.matrix);

    int32_t mono_x_ep, mono_y_ep;
    bool mono_sx, mono_sy;
    perspective_mono(cfg.matrix, &mono_x_ep, &mono_sx, &mono_y_ep, &mono_sy);
    cfg.y_gain_exp = 0;
    cfg.src_format = (bsc_hw_data_format_e)src->format;
    cfg.src_line_stride = src->line_stride;

    cfg.dst_format = (bsc_hw_data_format_e)dst->format;
    cfg.dst_line_stride = dst->line_stride;
    cfg.affine = true;
    cfg.box_mode = true;

    for (int i = 0; i < 9; i++) {
        cfg.coef[i] = coef[i];
    }
    for (int i = 0; i < 2; i++) {
        cfg.offset[i] = offset[i];
    }

    cfg.zero_point = info->zero_point;
    double extreme_point[DST_MAX_WIDTH];
    assert(dst->width < DST_MAX_WIDTH);
    double m6_div_m7 = 0;
    double m8_div_m7 = 0;
    if (cfg.matrix[7]) {
        m6_div_m7 = (double)cfg.matrix[6] / (double)cfg.matrix[7];
        m8_div_m7 = (double)cfg.matrix[8] / (double)cfg.matrix[7];
    }

    for (int i = 0; i < dst->width; i++) {
        extreme_point[i] = -i * m6_div_m7 - m8_div_m7;
    }
    cfg.box_num = 1;//always 1
    cfg.boxes_info = NULL;//not active
    cfg.box_bus = 0; //not active
    cfg.ibufc_bus = src->locate;
    cfg.obufc_bus = dst->locate;
    cfg.irq_mask = 1; //fixme
    cfg.isum = 0;//delete me ??
    cfg.osum = 0;//delete me ??

    for (int i = 0; i < sbox_num; i++) {
        if (mono_x_ep > (dst_sbox[i].x + dst_sbox[i].w)) {
            cfg.mono_x = (mono_sx << 7) | 64;
        } else if (mono_x_ep <= dst_sbox[i].x) {
            cfg.mono_x = (mono_sx << 7) | 0;
        } else {
            cfg.mono_x = (mono_sx << 7) | (mono_x_ep - dst_sbox[i].x);
        }

        if (mono_y_ep > (dst_sbox[i].x + dst_sbox[i].w)) {
            cfg.mono_y = (mono_sy << 7) | 64;
        } else if (mono_y_ep <= dst_sbox[i].x) {
            cfg.mono_y = (mono_sy << 7) | 0;
        } else {
            cfg.mono_y = (mono_sy << 7) | (mono_y_ep - dst_sbox[i].x);
        }

        for (int dx = 0; dx < dst_sbox[i].w; dx++) {
            double act_extreme_point = extreme_point[(dx + dst_sbox[i].x)];
            int act_extreme_point_s32 = act_extreme_point;
            int flag =0;
            if((act_extreme_point >-1) & (act_extreme_point <0)){
                flag = 1;
            }
            if(flag){
                act_extreme_point_s32 = 0;
            }
            else if (!IS_ZERO(act_extreme_point - act_extreme_point_s32)) {
                act_extreme_point_s32 += 1;
                /* if (mono_sy == INCREASE) {
                   if ((dx + dst_sbox[i].x) > mono_y_ep) {
                   act_extreme_point_s32 += 1;
                   }
                   } else {
                   if ((dx + dst_sbox[i].x) < mono_y_ep) {
                   act_extreme_point_s32 += 1;
                   }
                   }*/
            }
            else if(act_extreme_point == act_extreme_point_s32){
                act_extreme_point_s32 = act_extreme_point;
            }

            cfg.extreme_point[dx] = (int8_t)(act_extreme_point_s32 < dst_sbox[i].y ? 0://-1 :
                                             act_extreme_point_s32 > (dst_sbox[i].y + dst_sbox[i].h - 1) ? 64 :
                                             act_extreme_point_s32 - dst_sbox[i].y);
        }

        uint8_t *src_sbox_base0 = NULL;
        uint8_t *src_sbox_base1 = NULL;
        if (src->format == BS_DATA_NV12) {
            assert(box->x % 2 == 0);
            assert(box->y % 2 == 0);
            assert(box->w % 2 == 0);
            assert(box->h % 2 == 0);
            src_sbox_base0 = src_wbox_base0 +
                src_sbox[i].y * src->line_stride + src_sbox[i].x;
            src_sbox_base1 = src_wbox_base1 +
                src_sbox[i].y / 2 * src->line_stride + src_sbox[i].x;
        } else if (src->format & 0x1) {//RGBA,ARGB, BRGA....
            src_sbox_base0 = src_wbox_base0 +
                src_sbox[i].y * src->line_stride + src_sbox[i].x * 4;
        }

        cfg.src_base0 = src_sbox_base0;
        cfg.src_base1 = src_sbox_base1;

        cfg.src_box_x = src_sbox[i].x;
        cfg.src_box_y = src_sbox[i].y;
        cfg.src_box_w = src_sbox[i].w;
        cfg.src_box_h = src_sbox[i].h;

        uint8_t *dst_sbox_base0 = NULL;
        uint8_t *dst_sbox_base1 = NULL;
        if (dst->format == BS_DATA_NV12) {
            assert(box->x % 2 == 0);
            assert(box->y % 2 == 0);
            assert(box->w % 2 == 0);
            assert(box->h % 2 == 0);
            dst_sbox_base0 = dst_wbox_base0 +
                dst_sbox[i].y * dst->line_stride + dst_sbox[i].x;
            dst_sbox_base1 = dst_wbox_base0 + dst->height * dst->line_stride +
                dst_sbox[i].y / 2 * dst->line_stride + dst_sbox[i].x;
        } else if (dst->format & 0x1) {//RGBA,ARGB, BRGA....
            dst_sbox_base0 = dst_wbox_base0 +
                dst_sbox[i].y * dst->line_stride + dst_sbox[i].x * 4;
        }

        cfg.dst_base[0] = dst_sbox_base0;
        cfg.dst_base[1] = dst_sbox_base1;
        cfg.dst_box_x = dst_sbox[i].x;
        cfg.dst_box_y = dst_sbox[i].y;
        cfg.dst_box_w = dst_sbox[i].w;
        cfg.dst_box_h = dst_sbox[i].h;
        if ((src_sbox[i].w != 0) && (src_sbox[i].h != 0)) {
            bs_cfgs.push_back(cfg);
        }
    }
    free(dst_sbox);
    free(src_sbox);
}

/**
 * user configure affine mode, but it's can used resize separately
 */
void affine_resize_box_split(std::vector<bsc_hw_once_cfg_s> &bs_cfgs,
                             box_affine_info_s *info,
                             const data_info_s *src, data_info_s *dst,
                             const uint32_t *coef, const uint32_t *offset)
{
    box_info_s *box = &(info->box);
    /* 1. caculate source whole-box base address */
    uint8_t *src_wbox_base0 = NULL;
    uint8_t *src_wbox_base1 = NULL;
    if (src->format == BS_DATA_NV12) {
        assert(box->x % 2 == 0);
        assert(box->y % 2 == 0);
        assert(box->w % 2 == 0);
        assert(box->h % 2 == 0);
        src_wbox_base0 = (uint8_t *)src->base +
            box->y * src->line_stride + box->x;
        if (src->base1 == NULL) {
            src_wbox_base1 = (uint8_t *)src->base + src->height * src->line_stride +
                box->y / 2 * src->line_stride + box->x;
        } else {
            src_wbox_base1 = (uint8_t *)src->base1;
        }
    } else if (src->format & 0x1) {//RGBA,ARGB, BRGA....
        src_wbox_base0= (uint8_t *)src->base +
            box->y * src->line_stride + box->x * 4;
    } else {
        assert(0);
    }

    /* 2. caculate source whole-box base address */
    uint8_t *dst_wbox_base0 = (uint8_t *)(dst->base);
    uint8_t *dst_wbox_base1 = NULL;
    if (dst->format == BS_DATA_NV12) {
        if (dst->base1 == NULL) {
            dst_wbox_base1 = (uint8_t *)dst->base + dst->height * dst->line_stride;
        } else {
            dst_wbox_base1 = (uint8_t *)dst->base1;
        }
    }

    /* 3. caculate destination/source sub-box information */
    float inverse[9];
    get_inverse_matrix(info->matrix, inverse);

    const int sbox_num_w = (dst->width + BS_RESIZE_W - 1) / BS_RESIZE_W;
    const int sbox_num_h = (dst->height + BS_RESIZE_H - 1) / BS_RESIZE_H;
    int sbox_num = sbox_num_w * sbox_num_h;
    // caculate dst sub-box information
    box_info_s *dst_sbox = (box_info_s *)malloc(sizeof(box_info_s) * sbox_num);
    if (dst_sbox == NULL) {
        fprintf(stderr, "alloc space failed!\n");
    }

    int i, j;
    for (j = 0; j < sbox_num_h; j++) {
        for (i = 0; i < sbox_num_w; i++) {
            dst_sbox[j * sbox_num_w + i].x = i * BS_RESIZE_W;
            dst_sbox[j * sbox_num_w + i].y = j * BS_RESIZE_H;
            int sbox_w = (i == (sbox_num_w - 1)) ?
                (dst->width - 1) % BS_RESIZE_W + 1 : BS_RESIZE_W;
            int sbox_h = (j == (sbox_num_h - 1)) ?
                (dst->height - 1) % BS_RESIZE_H + 1 : BS_RESIZE_H;
            dst_sbox[j * sbox_num_w + i].w = sbox_w;
            dst_sbox[j * sbox_num_w + i].h = sbox_h;
        }
    }

    // caculate src sub-box information
    box_info_s *src_sbox = (box_info_s *)malloc(sizeof(box_info_s) * sbox_num);
    if (src_sbox == NULL) {
        fprintf(stderr, "alloc space failed!\n");
    }

    int32_t matrix_s32[9];
    matrix_float_to_s32(inverse, matrix_s32);
    box_info_s wbox = {0, 0, box->w, box->h};
    for (int i = 0; i < sbox_num; i++) {
        affine_cac_sub_box(src_sbox + i, dst_sbox + i, matrix_s32, &wbox);
    }

    /* 4. caculate inverse matrix scale up exponent */
    // must after cac_sub_box !!!
    uint8_t y_gain_exp = 0;
    if (fabs(inverse[MSCALEY]) < 1.0f) {// condition maybe error!!!//fixme
        y_gain_exp = get_exp(fabs(inverse[MSCALEY]));
        assert((y_gain_exp > 0) && (y_gain_exp <= 6));
        inverse[MSKEWY] *= (1 << y_gain_exp);
        inverse[MSCALEY] *= (1 << y_gain_exp);
        inverse[MTRANSY] *= (1 << y_gain_exp);
    }

    /* 5. most members of each sub-box are the same,
       only sub-box x,y,w,h are different. */
    bsc_hw_once_cfg_s cfg;
    for (int i = 0; i < 6; i++) {
        cfg.matrix[i] = (int32_t)round(inverse[i] * (1 << MAT_ACC));
    }

    cfg.y_gain_exp = y_gain_exp;
    cfg.src_format = (bsc_hw_data_format_e)src->format;
    cfg.src_line_stride = src->line_stride;

    cfg.dst_format = (bsc_hw_data_format_e)dst->format;
    cfg.dst_line_stride = dst->line_stride;
    cfg.affine = false;
    cfg.box_mode = true;

    for (int i = 0; i < 9; i++) {
        cfg.coef[i] = coef[i];
    }
    for (int i = 0; i < 2; i++) {
        cfg.offset[i] = offset[i];
    }
    cfg.zero_point = info->zero_point;
    cfg.box_num = 1;//check me
    cfg.boxes_info = NULL;//check me
    cfg.box_bus = 0; //fixme
    cfg.ibufc_bus = src->locate;
    cfg.obufc_bus = dst->locate;
    cfg.irq_mask = 1; //fixme
    cfg.isum = 0;
    cfg.osum = 0;

    for (int i = 0; i < sbox_num; i++) {
        uint8_t *src_sbox_base0 = NULL;
        uint8_t *src_sbox_base1 = NULL;
        if (src->format == BS_DATA_NV12) {
            assert(box->x % 2 == 0);
            assert(box->y % 2 == 0);
            assert(box->w % 2 == 0);
            assert(box->h % 2 == 0);
            src_sbox_base0 = src_wbox_base0 +
                src_sbox[i].y * src->line_stride + src_sbox[i].x;
            src_sbox_base1 = src_wbox_base1 +
                src_sbox[i].y / 2 * src->line_stride + src_sbox[i].x;
        } else if (src->format & 0x1) {//RGBA,ARGB, BRGA....
            src_sbox_base0 = src_wbox_base0 +
                src_sbox[i].y * src->line_stride + src_sbox[i].x * 4;
        }

        cfg.src_base0 = src_sbox_base0;
        cfg.src_base1 = src_sbox_base1;

        cfg.src_box_x = src_sbox[i].x;
        cfg.src_box_y = src_sbox[i].y;
        cfg.src_box_w = src_sbox[i].w;
        cfg.src_box_h = src_sbox[i].h;

        uint8_t *dst_sbox_base0 = NULL;
        uint8_t *dst_sbox_base1 = NULL;
        if (dst->format == BS_DATA_NV12) {
            assert(box->x % 2 == 0);
            assert(box->y % 2 == 0);
            assert(box->w % 2 == 0);
            assert(box->h % 2 == 0);
            dst_sbox_base0 = dst_wbox_base0 +
                dst_sbox[i].y * dst->line_stride + dst_sbox[i].x;
            dst_sbox_base1 = dst_wbox_base0 + dst->height * dst->line_stride +
                dst_sbox[i].y / 2 * dst->line_stride + dst_sbox[i].x;
        } else if (dst->format & 0x1) {//RGBA,ARGB, BRGA....
            dst_sbox_base0 = dst_wbox_base0 +
                dst_sbox[i].y * dst->line_stride + dst_sbox[i].x * 4;
        }

        cfg.dst_base[0] = dst_sbox_base0;
        cfg.dst_base[1] = dst_sbox_base1;
        cfg.dst_box_x = dst_sbox[i].x;
        cfg.dst_box_y = dst_sbox[i].y;
        cfg.dst_box_w = dst_sbox[i].w;
        cfg.dst_box_h = dst_sbox[i].h;
        bs_cfgs.push_back(cfg);
    }
    free(dst_sbox);
    free(src_sbox);
}

static void bs_resize_cfg_stuff(std::vector<bsc_hw_once_cfg_s> &bs_cfgs,
                                const box_resize_info_s *info,
                                const data_info_s *src, const data_info_s *dst,
                                const uint32_t *coef, const uint32_t *offset)
{
    bsc_hw_once_cfg_s cfg;
    const box_info_s *box = &(info->box);
    uint8_t *src_base1 = NULL;
    if (src->format == BS_DATA_NV12) {
        printf("%s: box: %d,%d,%d,%d\n", __func__, box->x, box->y, box->w, box->h);
        assert(box->x % 2 == 0);
        assert(box->y % 2 == 0);
        assert(box->w % 2 == 0);
        assert(box->h % 2 == 0);
        cfg.src_base0 = (uint8_t *)src->base +
            box->y * src->line_stride + box->x;
        if (src->base1 == NULL) {
            src_base1 = (uint8_t *)src->base + src->height * src->line_stride +
                box->y / 2 * src->line_stride + box->x;
        } else {
            src_base1 = (uint8_t *)src->base1;
        }
    } else if(src->format == BS_DATA_FMU2) {
        cfg.src_base0 = (uint8_t *)src->base +
            box->y * src->line_stride + box->x * 8;
    } else if(src->format == BS_DATA_FMU4) {
        cfg.src_base0 = (uint8_t *)src->base +
            box->y * src->line_stride + box->x * 16;
    } else if(src->format == BS_DATA_FMU8) {
        cfg.src_base0 = (uint8_t *)src->base +
            box->y * src->line_stride + box->x * 32;
    } else if (src->format & 0x1) {//RGBA,ARGB, BRGA....
        cfg.src_base0 = (uint8_t *)src->base +
            box->y * src->line_stride + box->x * 4;
    } else {
        assert(0);
    }

    cfg.src_base1 = src_base1;
    cfg.src_format = (bsc_hw_data_format_e)src->format;
    cfg.src_line_stride = src->line_stride;
    cfg.src_box_x = 0;
    cfg.src_box_y = 0;
    cfg.src_box_w = box->w;
    cfg.src_box_h = box->h;

    //single destination box info(size must same)
    cfg.dst_base[0] = (uint8_t *)dst->base;
    if (dst->format == BS_DATA_NV12) {
        if (dst->base1 == NULL) {
            cfg.dst_base[1] = (uint8_t *)dst->base + dst->height * dst->line_stride;
        } else {
            cfg.dst_base[1] = (uint8_t *)dst->base1;
        }
    }
    cfg.dst_format = (bsc_hw_data_format_e)dst->format;
    cfg.dst_line_stride = dst->line_stride;
    cfg.dst_box_x = 0;
    cfg.dst_box_y = 0;
    cfg.dst_box_w = dst->width;
    cfg.dst_box_h = dst->height;

    cfg.box_mode = true;
    for (int i = 0; i < 9; i++) {
        cfg.coef[i] = coef[i];
    }
    for (int i = 0; i < 2; i++) {
        cfg.offset[i] = offset[i];
    }
    cfg.zero_point = info->zero_point;

    float scale_x = (float)box->w / (float)dst->width;
    float scale_y = (float)box->h / (float)dst->height;
    float trans_x = 0.5f * scale_x - 0.5f;
    float trans_y = 0.5f * scale_y - 0.5f;
    uint8_t y_gain_exp = 0;
    bool affine_mode = false;

    //if (((src->format & 0x1) || (src->format == BS_DATA_NV12)) && //image
    //    (fabs(scale_y) < 1.0f)) { //amplify
//    if (((src->format & 0x1) || (src->format == BS_DATA_NV12))) {
    if (((src->format <= BS_DATA_ARGB) && (src->format >= BS_DATA_NV12))) { //amplify                                  //using affine
#if 0
		static int timess = 3;
       //using affine
	   	if(timess){
        	printf("using affine **** xhshen ****\n");
			timess--;
		}
#endif
        affine_mode = true;
        cfg.matrix[MPERSP0] = 0;
        cfg.matrix[MPERSP1] = 0;
        cfg.matrix[MPERSP2] = 1 << MAT_ACC;
    } else if (fabs(scale_y) < 1.0f) {
        y_gain_exp = get_exp(fabs(scale_y));
        assert(y_gain_exp > 0);
        scale_y *= (1 << y_gain_exp);
        trans_y *= (1 << y_gain_exp);
    }
    cfg.affine = affine_mode;

    cfg.matrix[MSCALEX] = (int32_t)round(scale_x * (1 << MAT_ACC));
    cfg.matrix[MSKEWX] = 0;
    cfg.matrix[MTRANSX] = (int32_t)round(trans_x * (1 << MAT_ACC));
    cfg.matrix[MSKEWY] = 0;
    cfg.matrix[MSCALEY] = (int32_t)round(scale_y * (1 << MAT_ACC));
    cfg.matrix[MTRANSY] = (int32_t)round(trans_y * (1 << MAT_ACC));

    cfg.y_gain_exp = y_gain_exp;
    //line mode, multi-box info
    cfg.box_num = 1;
    cfg.boxes_info = NULL;
    cfg.box_bus = 0; //fixme
    cfg.ibufc_bus = src->locate;
    cfg.obufc_bus = dst->locate;
    cfg.irq_mask = 1; //fixme
    cfg.isum = 0;
    cfg.osum = 0;
    bs_cfgs.push_back(cfg);
}

void resize_box_split(std::vector<bsc_hw_once_cfg_s> &bs_cfgs,
                      const box_resize_info_s *info,
                      const data_info_s *src, const data_info_s *dst,
                      const uint32_t *coef, const uint32_t *offset)
{
    const box_info_s *box = &(info->box);
    /* 1. caculate source whole-box base address */
    uint8_t *src_wbox_base0 = NULL;
    uint8_t *src_wbox_base1 = NULL;
    if (src->format == BS_DATA_NV12) {
        assert(box->x % 2 == 0);
        assert(box->y % 2 == 0);
        assert(box->w % 2 == 0);
        assert(box->h % 2 == 0);
        src_wbox_base0 = (uint8_t *)src->base +
            box->y * src->line_stride + box->x;
        if (src->base1 == NULL) {
            src_wbox_base1 = (uint8_t *)src->base + src->height * src->line_stride +
                box->y / 2 * src->line_stride + box->x;
        } else {
            src_wbox_base1 = (uint8_t *)src->base1;
        }
    } else if(src->format == BS_DATA_FMU2) {
        src_wbox_base0= (uint8_t *)src->base +
            box->y * src->line_stride + box->x * 8;
    } else if(src->format == BS_DATA_FMU4) {
        src_wbox_base0 = (uint8_t *)src->base +
            box->y * src->line_stride + box->x * 16;
    } else if(src->format == BS_DATA_FMU8) {
        src_wbox_base0= (uint8_t *)src->base +
            box->y * src->line_stride + box->x * 32;
    } else if (src->format & 0x1) {//RGBA,ARGB, BRGA....
        src_wbox_base0= (uint8_t *)src->base +
            box->y * src->line_stride + box->x * 4;
    } else {
        assert(0);
    }

    /* 2. caculate source whole-box base address */
    uint8_t *dst_wbox_base0 = (uint8_t *)(dst->base);
    uint8_t *dst_wbox_base1 = NULL;
    if (dst->format == BS_DATA_NV12) {
        if (dst->base1 == NULL) {
            dst_wbox_base1 = (uint8_t *)dst->base + dst->height * dst->line_stride;
        } else {
            dst_wbox_base1 = (uint8_t *)dst->base1;
        }
    }

    /* 3. caculate destination/source sub-box information */
    float scale_x = (float)box->w / (float)dst->width;
    float scale_y = (float)box->h / (float)dst->height;
    float trans_x = 0.5f * scale_x - 0.5f;
    float trans_y = 0.5f * scale_y - 0.5f;

    int sbox_h_max = BS_RESIZE_H;
    //if (((src->format & 0x1) || (src->format == BS_DATA_NV12)) && //image
    //    (fabs(scale_y) < 1.0f)) { //amplify
    if (((src->format & 0x1) || (src->format == BS_DATA_NV12))) { //amplify
        //using affine
        sbox_h_max = BS_AFFINE_SIZE;
    }
    const int sbox_num_w = (dst->width + BS_RESIZE_W - 1) / BS_RESIZE_W;
    const int sbox_num_h = (dst->height + sbox_h_max - 1) / sbox_h_max;
    int sbox_num = sbox_num_w * sbox_num_h;
    // caculate dst sub-box information
    box_info_s *dst_sbox = (box_info_s *)malloc(sizeof(box_info_s) * sbox_num);
    if (dst_sbox == NULL) {
        fprintf(stderr, "alloc space failed!\n");
    }

    int i, j;
    for (j = 0; j < sbox_num_h; j++) {
        for (i = 0; i < sbox_num_w; i++) {
            dst_sbox[j * sbox_num_w + i].x = i * BS_RESIZE_W;
            dst_sbox[j * sbox_num_w + i].y = j * sbox_h_max;
            int sbox_w = (i == (sbox_num_w - 1)) ?
                (dst->width - 1) % BS_RESIZE_W + 1 : BS_RESIZE_W;
            int sbox_h = (j == (sbox_num_h - 1)) ?
                (dst->height - 1) % sbox_h_max + 1 : sbox_h_max;
            dst_sbox[j * sbox_num_w + i].w = sbox_w;
            dst_sbox[j * sbox_num_w + i].h = sbox_h;
        }
    }
    // caculate src sub-box information
    box_info_s *src_sbox = (box_info_s *)malloc(sizeof(box_info_s) * sbox_num);
    if (src_sbox == NULL) {
        fprintf(stderr, "alloc space failed!\n");
    }

    box_info_s wbox = {0, 0, box->w, box->h};
    for (int i = 0; i < sbox_num; i++) {
        resize_cac_sub_box(src_sbox + i, dst_sbox + i, &wbox,
                           scale_x, scale_y, trans_x, trans_y);
    }

    /* 4. caculate inverse matrix scale up exponent */
    // must after cac_sub_box !!!
    uint8_t y_gain_exp = 0;
    bool affine_mode = false;
    //if (((src->format & 0x1) || (src->format == BS_DATA_NV12)) && //image
    //    (fabs(scale_y) < 1.0f)) { //amplify
    //if (((src->format & 0x1) || (src->format == BS_DATA_NV12)){ //amplify                                  //using affine
    if (((src->format <= BS_DATA_ARGB) && (src->format >= BS_DATA_NV12))) { //amplify                                  //using affine
        affine_mode = true; /* 为什么开启放射？*/
    } else if (fabs(scale_y) < 1.0f) {
        y_gain_exp = get_exp(fabs(scale_y));
        assert(y_gain_exp > 0);
        scale_y *= (1 << y_gain_exp);
        trans_y *= (1 << y_gain_exp);
    }

    /* 5. most members of each sub-box are the same,
       only sub-box x,y,w,h are different. */
    bsc_hw_once_cfg_s cfg;
    memset(cfg.matrix, 0, sizeof(int32_t) * 9);
    cfg.matrix[MSCALEX] = (int32_t)round(scale_x * (1 << MAT_ACC));
    cfg.matrix[MSKEWX] = 0;
    cfg.matrix[MTRANSX] = (int32_t)round(trans_x * (1 << MAT_ACC));
    cfg.matrix[MSKEWY] = 0;
    cfg.matrix[MSCALEY] = (int32_t)round(scale_y * (1 << MAT_ACC));
    cfg.matrix[MTRANSY] = (int32_t)round(trans_y * (1 << MAT_ACC));
    cfg.matrix[MPERSP0] = 0;
    cfg.matrix[MPERSP1] = 0;
    cfg.matrix[MPERSP2] = 1 << MAT_ACC;

    cfg.y_gain_exp = y_gain_exp;
    cfg.src_format = (bsc_hw_data_format_e)src->format;
    cfg.src_line_stride = src->line_stride;

    cfg.dst_format = (bsc_hw_data_format_e)dst->format;
    cfg.dst_line_stride = dst->line_stride;
    cfg.affine = affine_mode;
    cfg.box_mode = true;

    for (int i = 0; i < 9; i++) {
        cfg.coef[i] = coef[i];
    }
    for (int i = 0; i < 2; i++) {
        cfg.offset[i] = offset[i];
    }
    cfg.zero_point = info->zero_point;
    cfg.box_num = 1;
    cfg.boxes_info = NULL;
    cfg.box_bus = 0; //fixme
    cfg.ibufc_bus = src->locate;
    cfg.obufc_bus = dst->locate;
    cfg.irq_mask = 1; //fixme
    cfg.isum = 0;
    cfg.osum = 0;

    for (int i = 0; i < sbox_num; i++) {
        uint8_t *src_sbox_base0 = NULL;
        uint8_t *src_sbox_base1 = NULL;
        if (src->format == BS_DATA_NV12) {
            assert(box->x % 2 == 0);
            assert(box->y % 2 == 0);
            assert(box->w % 2 == 0);
            assert(box->h % 2 == 0);
            src_sbox_base0 = src_wbox_base0 +
                src_sbox[i].y * src->line_stride + src_sbox[i].x;
            src_sbox_base1 = src_wbox_base1 +
                src_sbox[i].y / 2 * src->line_stride + src_sbox[i].x;
        } else if(src->format == BS_DATA_FMU2) {
            src_sbox_base0 = src_wbox_base0 +
                src_sbox[i].y * src->line_stride + src_sbox[i].x * 8;
        } else if(src->format == BS_DATA_FMU4) {
            src_sbox_base0 = src_wbox_base0 +
                src_sbox[i].y * src->line_stride + src_sbox[i].x * 16;
        } else if(src->format == BS_DATA_FMU8) {
            src_sbox_base0 = src_wbox_base0 +
                src_sbox[i].y * src->line_stride + src_sbox[i].x * 32;
        } else if (src->format & 0x1) {//RGBA,ARGB, BRGA....
            src_sbox_base0 = src_wbox_base0 +
                src_sbox[i].y * src->line_stride + src_sbox[i].x * 4;
        }
        cfg.src_base0 = src_sbox_base0;
        cfg.src_base1 = src_sbox_base1;
        cfg.src_box_x = src_sbox[i].x;
        cfg.src_box_y = src_sbox[i].y;
        cfg.src_box_w = src_sbox[i].w;
        cfg.src_box_h = src_sbox[i].h;
        uint8_t *dst_sbox_base0 = NULL;
        uint8_t *dst_sbox_base1 = NULL;
        if (dst->format == BS_DATA_NV12) {
            assert(box->x % 2 == 0);
            assert(box->y % 2 == 0);
            assert(box->w % 2 == 0);
            assert(box->h % 2 == 0);
            dst_sbox_base0 = dst_wbox_base0 +
                dst_sbox[i].y * dst->line_stride + dst_sbox[i].x;
            dst_sbox_base1 = dst_wbox_base1 +
                dst_sbox[i].y / 2 * dst->line_stride + dst_sbox[i].x;
        } else if(dst->format == BS_DATA_FMU2) {
            dst_sbox_base0 = dst_wbox_base0 +
                dst_sbox[i].y * dst->line_stride + dst_sbox[i].x * 8;
        } else if(dst->format == BS_DATA_FMU4) {
            dst_sbox_base0 = dst_wbox_base0 +
                dst_sbox[i].y * dst->line_stride + dst_sbox[i].x * 16;
        } else if(dst->format == BS_DATA_FMU8) {
            dst_sbox_base0 = dst_wbox_base0 +
                dst_sbox[i].y * dst->line_stride + dst_sbox[i].x * 32;
        } else if (dst->format & 0x1) {//RGBA,ARGB, BRGA....
            dst_sbox_base0 = dst_wbox_base0 +
                dst_sbox[i].y * dst->line_stride + dst_sbox[i].x * 4;
        }
        cfg.dst_base[0] = dst_sbox_base0;
        cfg.dst_base[1] = dst_sbox_base1;
        cfg.dst_box_x = dst_sbox[i].x;
        cfg.dst_box_y = dst_sbox[i].y;
        cfg.dst_box_w = dst_sbox[i].w;
        cfg.dst_box_h = dst_sbox[i].h;
        bs_cfgs.push_back(cfg);
    }
    free(dst_sbox);
    free(src_sbox);
}

/**
 * resize line mode configure
 */
void bs_resize_line_cfg_stuff(std::vector<bsc_hw_once_cfg_s> &bs_cfgs,
                              std::vector<std::pair<int, const box_resize_info_s *>> &res_resize_boxes,
                              const data_info_s *src, const data_info_s *dst,
                              const uint32_t *coef, const uint32_t *offset)
{
#if 1
    std::vector<std::pair<int, const box_resize_info_s *>>::iterator it;
    for (it = res_resize_boxes.begin(); it != res_resize_boxes.end(); it++) {
        int idx = (*it).first;
        const box_resize_info_s *cur_box = (*it).second;
        const data_info_s *cur_dst = &(dst[idx]);
        bs_resize_cfg_stuff(bs_cfgs, cur_box, src, cur_dst, coef, offset);
    }
#else
    int box_num = res_resize_boxes.size();
    int tl_x = 65535, tl_y = 65535;
    int br_x = 0, br_y = 0;
    std::vector<std::pair<int, const box_resize_info_s *>>::iterator it;
    for (it = res_resize_boxes.begin(); it != res_resize_boxes.end(); it++) {
        int idx = (*it).first;
        const box_resize_info_s *cur_box = (*it).second;
        const data_info_s *cur_dst = &(dst[idx]);
        const box_info_s *ibox = &(cur_box->box);
        if (ibox->x < tl_x) {
            tl_x = ibox->x;
        }

        if (ibox->y < tl_y) {
            tl_y = ibox->y;
        }

        if ((ibox->x + ibox->w - 1) > br_x) {
            br_x = ibox->x + ibox->w - 1;
        }

        if ((ibox->y + ibox->h - 1) > br_y) {
            br_y = ibox->y + ibox->h - 1;
        }
    }
    box_info_s iwbox;
    iwbox.x = tl_x;
    iwbox.y = tl_y;
    iwbox.w = br_x - tl_x + 1;
    iwbox.h = br_y - tl_y + 1;

    bsc_hw_once_cfg_s cfg;
    uint8_t *src_base1 = NULL;
    if (src->format == BS_DATA_NV12) {
        cfg.src_base0 = (uint8_t *)src->base +
            iwbox.y * src->line_stride + iwbox.x;
        src_base1 = (uint8_t *)src->base + src->height * src->line_stride +
            iwbox.y / 2 * src->line_stride + iwbox.x;
    } else if(src->format == BS_DATA_FMU2) {
        cfg.src_base0 = (uint8_t *)src->base +
            iwbox.y * src->line_stride + iwbox.x * 8;
    } else if(src->format == BS_DATA_FMU4) {
        cfg.src_base0 = (uint8_t *)src->base +
            iwbox.y * src->line_stride + iwbox.x * 16;
    } else if(src->format == BS_DATA_FMU8) {
        cfg.src_base0 = (uint8_t *)src->base +
            iwbox.y * src->line_stride + iwbox.x * 32;
    } else if (src->format & 0x1) {//RGBA,ARGB, BRGA....
        cfg.src_base0 = (uint8_t *)src->base +
            iwbox.y * src->line_stride + iwbox.x * 4;
    } else {
        assert(0);
    }

    cfg.src_base1 = src_base1;
    cfg.src_format = (bsc_hw_data_format_e)src->format;
    cfg.src_line_stride = src->line_stride;
    cfg.src_box_x = 0;
    cfg.src_box_y = 0;
    cfg.src_box_w = iwbox.w;
    cfg.src_box_h = iwbox.h;

    for (int i = 0; i < box_num; i++) {
        data_info_s cur_dst = dst[i];
        cfg.dst_base[i] = (uint8_t *)(cur_dst.base);
    }
    cfg.dst_format = (bsc_hw_data_format_e)dst->format;
    cfg.dst_line_stride = dst->line_stride;
    cfg.dst_box_x = 0;
    cfg.dst_box_y = 0;
    cfg.dst_box_w = dst->width;
    cfg.dst_box_h = dst->height;
    cfg.affine = false;
    cfg.box_mode = false;

    for (int i = 0; i < 9; i++) {
        cfg.coef[i] = coef[i];
    }
    for (int i = 0; i < 2; i++) {
        cfg.offset[i] = offset[i];
    }
    //max y gain exp
    uint8_t y_gain_exp_max = 0;
    for (it = res_resize_boxes.begin(); it != res_resize_boxes.end(); it++) {
        int idx = (*it).first;
        const box_resize_info_s *cur_info = (*it).second;
        const box_info_s *cur_box = &(cur_info->box);
        const data_info_s *cur_dst = &(dst[idx]);
        float scale_y = (float)cur_box->h / (float)cur_dst->height;
        uint8_t y_gain_exp = 0;
        if (fabs(scale_y) < 1.0f) {// condition maybe error!!!//fixme
            y_gain_exp = get_exp(fabs(scale_y));
            assert(y_gain_exp > 0);
            if (y_gain_exp > y_gain_exp_max) {
                y_gain_exp_max = y_gain_exp;
            }
        }
    }

    memset(cfg.matrix, 0, sizeof(int32_t) * 9);
    int cnt = 0;
    assert(box_num <= 64);
    cfg.box_num = box_num;
    cfg.boxes_info = (uint32_t *)bscaler_malloc(8, box_num * 6 * 4);//fixme, not free

    for (it = res_resize_boxes.begin(); it != res_resize_boxes.end(); it++) {
        int idx = (*it).first;
        const box_resize_info_s *cur_info = (*it).second;
        const box_info_s *cur_box = &(cur_info->box);
        const data_info_s *cur_dst = &(dst[idx]);
        float scale_y = (float)cur_box->h / (float)cur_dst->height;
        float scale_x = (float)cur_box->w / (float)cur_dst->width;
        float trans_x = 0.5f;
        float trans_y = 0.5f;
        if (y_gain_exp_max) {
            scale_y *= (1 << y_gain_exp_max);
            trans_y *= (1 << y_gain_exp_max);
        }
        cfg.zero_point = cur_info->zero_point;//fixme
        cfg.boxes_info[cnt * 6 + 0] = (((cur_box->x - iwbox.x) & 0xFFFF) << 16 |
                                       ((cur_box->y - iwbox.y) & 0xFFFF));
        cfg.boxes_info[cnt * 6 + 1] = ((cur_box->h & 0xFFFF) << 16 |
                                       (cur_box->w & 0xFFFF));
        cfg.boxes_info[cnt * 6 + 2] = (int32_t)(scale_x * (1 << MAT_ACC));
        cfg.boxes_info[cnt * 6 + 3] = (int32_t)(scale_y * (1 << MAT_ACC));
        cfg.boxes_info[cnt * 6 + 4] = (int32_t)(trans_x * (1 << MAT_ACC));
        cfg.boxes_info[cnt * 6 + 5] = (int32_t)(trans_y * (1 << MAT_ACC));
        cnt++;
    }

    cfg.y_gain_exp = y_gain_exp_max;
    cfg.box_bus = 0; //fixme
    cfg.ibufc_bus = src->locate;
    cfg.obufc_bus = dst->locate;
    cfg.irq_mask = 1;//fixme
    cfg.osum = 0;//fixme
    cfg.isum = 0;//fixme
    if (res_resize_boxes.size()) {
        bs_cfgs.push_back(cfg);
    }
#endif
}

bst_hw_data_format_e bs_format_to_bst(bs_data_format_e src)
{
    bst_hw_data_format_e dst;
    switch (src) {
    case BS_DATA_NV12:
        dst = BST_HW_DATA_FM_NV12;
        break;
    case BS_DATA_BGRA:
        dst = BST_HW_DATA_FM_BGRA;
        break;
    case BS_DATA_GBRA:
        dst = BST_HW_DATA_FM_GBRA;
        break;
    case BS_DATA_RBGA:
        dst = BST_HW_DATA_FM_RBGA;
        break;
    case BS_DATA_BRGA:
        dst = BST_HW_DATA_FM_BRGA;
        break;
    case BS_DATA_GRBA:
        dst = BST_HW_DATA_FM_GRBA;
        break;
    case BS_DATA_RGBA:
        dst = BST_HW_DATA_FM_RGBA;
        break;
    case BS_DATA_ABGR:
        dst = BST_HW_DATA_FM_ABGR;
        break;
    case BS_DATA_AGBR:
        dst = BST_HW_DATA_FM_AGBR;
        break;
    case BS_DATA_ARBG:
        dst = BST_HW_DATA_FM_ARBG;
        break;
    case BS_DATA_ABRG:
        dst = BST_HW_DATA_FM_ABRG;
        break;
    case BS_DATA_AGRB:
        dst = BST_HW_DATA_FM_AGRB;
        break;
    case BS_DATA_ARGB:
        dst = BST_HW_DATA_FM_ARGB;
        break;
    case BS_DATA_VBGR:
        dst = BST_HW_DATA_FM_VBGR;
        break;
    case BS_DATA_VRGB:
        dst = BST_HW_DATA_FM_VRGB;
        break;
    case BS_DATA_FMU2:
        assert(0);
        break;
    case BS_DATA_FMU4:
        assert(0);
        break;
    case BS_DATA_FMU8:
        assert(0);
        break;
    default:
        assert(0);
    }
    return dst;
}
