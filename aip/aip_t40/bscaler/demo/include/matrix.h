/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : matrix.h
 * Authors    : jmqi@taurus
 * Create Time: 2020-05-26:12:10:04
 * Description:
 *
 */

#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>

#define MSCALEX         (0) // horizontal scale factor
#define MSKEWX          (1) // horizontal skew factor
#define MTRANSX         (2) // horizontal translation
#define MSKEWY          (3) // vertical skew factor
#define MSCALEY         (4) // vertical scale factor
#define MTRANSY         (5) // vertical translation
#define MPERSP0         (6) // input x perspective factor
#define MPERSP1         (7) // input y perspective factor
#define MPERSP2         (8) // perspective bias

#define IS_ZERO(a)      (fabs(a) < 1e-6 ? true : false)
#define IS_ONE(a)       ((fabs(a - 1.0f)) < 1e-6 ? true : false)
#define FLOATINVERT(x)  (1.0f / (x))
#define MAX(a, b)       (a > b ? a : b)
#define MIN(a, b)       (a < b ? a : b)
#define CLIP(val, min, max) ((val) < (min) ? (min) : (val) > (max) ? (max) : (val))

#define BS_AFFINE_SIZE  (64)
#define BS_RESIZE_W     (64)
#define BS_RESIZE_H     (256)
#define OBOX_SIZE       (64)
#define MAT_ACC         (16)
#define MAT_ONE         ((1 << MAT_ACC) - 1)
#define MAT_HALF_ONE    (1 << (MAT_ACC - 1))

#define S32_MIN         (INT_MIN)
#define S32_MAX         (INT_MAX)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * return true, if current Matrix is only scale and translate
 */
static inline bool affine_is_scale_translate(const float *matrix)
{

    bool skew_x_is_zero = IS_ZERO(matrix[MSKEWX]);
    bool skew_y_is_zero = IS_ZERO(matrix[MSKEWY]);
    bool persp0_is_zero = IS_ZERO(matrix[MPERSP0]);
    bool persp1_is_zero = IS_ZERO(matrix[MPERSP1]);
    bool persp2_is_one = IS_ONE(matrix[MPERSP2]);

    bool is_scale_trans = (skew_x_is_zero && skew_y_is_zero &&
                           persp2_is_one &&
                           persp0_is_zero && persp1_is_zero);
    return is_scale_trans;
}

/**
 * convert float matrix to int32_t
 */
static inline void matrix_float_to_s32(float *src, int32_t *dst)
{
    for (int i = 0; i < 9; i++) {
        float s = CLIP(src[i], -32768.0f, 32767.0f);
        dst[i] = (int32_t)round(s * (1 << MAT_ACC));
    }
}

/**
 * matrix inversion
 */
void get_inverse_matrix(const float *matrix, float *inverse);

#ifdef __cplusplus
}
#endif
#endif /* __MATRIX_H__ */

