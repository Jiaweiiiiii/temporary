/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : matrix.c
 * Authors    : jmqi@taurus
 * Create Time: 2020-05-26:12:07:17
 * Description:
 *
 */

#include <assert.h>
#include "matrix.h"
//#define STATIC_INLINE  static inline
#define STATIC_INLINE  static

STATIC_INLINE float scross(float a, float b, float c, float d)
{
#if 1 // devoid x86 optimi
    float t0 = a * b;
    float t1 = c * d;
    return t0 - t1;
#else
    return a * b - c * d;
#endif
}

STATIC_INLINE float scross_dscale(float a, float b, float c, float d, float scale)
{
    return (float)(scross(a, b, c, d) * scale);
}

STATIC_INLINE float dcross(float a, float b, float c, float d)
{
#if 1 // devoid x86 optimi
    float t0 = a * b;
    float t1 = c * d;
    return t0 - t1;
#else
    return a * b - c * d;
#endif
}

STATIC_INLINE float dcross_dscale(float a, float b, float c, float d, float scale)
{
    return (float)(dcross(a, b, c, d) * scale);
}

static void compute_inv(float dst[9], const float src[9], float invDet, bool is_persp)
{
    assert(src != dst);
    assert(src && dst);

    if (is_persp) {
        dst[MSCALEX] = scross_dscale(src[MSCALEY], src[MPERSP2], src[MTRANSY], src[MPERSP1], invDet);
        dst[MSKEWX]  = scross_dscale(src[MTRANSX], src[MPERSP1], src[MSKEWX], src[MPERSP2], invDet);
        dst[MTRANSX] = scross_dscale(src[MSKEWX], src[MTRANSY], src[MTRANSX], src[MSCALEY], invDet);

        dst[MSKEWY]  = scross_dscale(src[MTRANSY], src[MPERSP0], src[MSKEWY], src[MPERSP2], invDet);
        dst[MSCALEY] = scross_dscale(src[MSCALEX], src[MPERSP2], src[MTRANSX], src[MPERSP0], invDet);
        dst[MTRANSY] = scross_dscale(src[MTRANSX], src[MSKEWY], src[MSCALEX], src[MTRANSY], invDet);

        dst[MPERSP0] = scross_dscale(src[MSKEWY], src[MPERSP1], src[MSCALEY], src[MPERSP0], invDet);
        dst[MPERSP1] = scross_dscale(src[MSKEWX], src[MPERSP0], src[MSCALEX], src[MPERSP1], invDet);
        dst[MPERSP2] = scross_dscale(src[MSCALEX], src[MSCALEY], src[MSKEWX], src[MSKEWY], invDet);
    } else { // not perspective
        dst[MSCALEX] = (float)(src[MSCALEY] * invDet);
        dst[MSKEWX]  = (float)(-src[MSKEWX] * invDet);
        dst[MTRANSX] = dcross_dscale(src[MSKEWX], src[MTRANSY], src[MSCALEY], src[MTRANSX], invDet);

        dst[MSKEWY]  = (float)(-src[MSKEWY] * invDet);
        dst[MSCALEY] = (float)(src[MSCALEX] * invDet);
        dst[MTRANSY] = dcross_dscale(src[MSKEWY], src[MTRANSX], src[MSCALEX], src[MTRANSY], invDet);

        dst[MPERSP0] = 0;
        dst[MPERSP1] = 0;
        dst[MPERSP2] = 1;
    }
}

/**
 * get affine Matrix inverse matrix
 */
void get_inverse_matrix(const float *matrix, float *inverse)
{
    bool scale_x_is_one = IS_ONE(matrix[MSCALEX]);
    bool scale_y_is_one = IS_ONE(matrix[MSCALEY]);
    bool persp2_is_one = IS_ONE(matrix[MPERSP2]);
    bool trans_x_is_zero = IS_ZERO(matrix[MTRANSX]);
    bool trans_y_is_zero = IS_ZERO(matrix[MTRANSY]);
    bool skew_x_is_zero = IS_ZERO(matrix[MSKEWX]);
    bool skew_y_is_zero = IS_ZERO(matrix[MSKEWY]);
    bool persp0_is_zero = IS_ZERO(matrix[MPERSP0]);
    bool persp1_is_zero = IS_ZERO(matrix[MPERSP1]);

    bool is_identity = (scale_x_is_one && scale_y_is_one && persp2_is_one &&
                        trans_x_is_zero && trans_y_is_zero && skew_x_is_zero &&
                        skew_y_is_zero && persp0_is_zero && persp1_is_zero);
    bool is_scale_trans = (skew_x_is_zero && skew_y_is_zero &&
                           persp2_is_one &&
                           persp0_is_zero && persp1_is_zero);
    bool is_translate_only = (scale_x_is_one && scale_y_is_one && persp2_is_one &&
                              skew_x_is_zero &&
                              skew_y_is_zero && persp0_is_zero && persp1_is_zero);
    bool is_perspective = !(persp0_is_zero && persp1_is_zero && persp2_is_one);
    if (is_identity) {
        inverse[MSCALEX] = inverse[MSCALEY] = inverse[MPERSP2] = 1.0f;
        inverse[MSKEWX] = inverse[MSKEWY] = inverse[MTRANSX] =
            inverse[MTRANSY] = inverse[MPERSP0] = inverse[MPERSP1] = 0.0f;
    } else {
        if (is_scale_trans) {
            inverse[MSCALEX] = FLOATINVERT(matrix[MSCALEX]);
            inverse[MSCALEY] = FLOATINVERT(matrix[MSCALEY]);
            inverse[MSKEWX] = inverse[MSKEWY] =
                inverse[MPERSP0] = inverse[MPERSP1] = 0;
            inverse[MTRANSX] = -matrix[MTRANSX] * inverse[MSCALEX];
            inverse[MTRANSY] = -matrix[MTRANSY] * inverse[MSCALEY];
            inverse[MPERSP2] = 1.0f;
        } else if (is_translate_only) {
            inverse[MSCALEX] = inverse[MSCALEY] = inverse[MPERSP2] = 1.0f;
            inverse[MSKEWX] = inverse[MSKEWY] =
                inverse[MPERSP0] = inverse[MPERSP1] = 0.0f;
            inverse[MTRANSX] = -matrix[MTRANSX];
            inverse[MTRANSY] = -matrix[MTRANSY];
            inverse[MPERSP2] = 1.0f;
        } else {
            float det;
            if (is_perspective) {
                float t0 = matrix[MSCALEX] *
                    dcross(matrix[MSCALEY], matrix[MPERSP2], matrix[MTRANSY], matrix[MPERSP1]);
                float t1 = matrix[MSKEWX] *
                    dcross(matrix[MTRANSY], matrix[MPERSP0], matrix[MSKEWY], matrix[MPERSP2]);
                float t2 = matrix[MTRANSX] *
                    dcross(matrix[MSKEWY], matrix[MPERSP1], matrix[MSCALEY], matrix[MPERSP0]);
                float t3 = t0 + t1;
                float t4 = t3 + t2;
                //printf("t: %x, %x, %x, %x, %x\n", *((uint32_t *)(&t0)), *((uint32_t *)(&t1)), *((uint32_t *)(&t2)), *((uint32_t *)(&t3)), *((uint32_t *)(&t4)));
                det = 1.0 / t4;
                compute_inv(inverse, matrix, det, true);
            } else {//affine
                float t0 = dcross(matrix[MSCALEX], matrix[MSCALEY], matrix[MSKEWX], matrix[MSKEWY]);
                det = 1.0 / t0;
                compute_inv(inverse, matrix, det, false);
            }
        }
    }
}
