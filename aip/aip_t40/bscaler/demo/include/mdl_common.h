/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : mdl_common.h
 * Authors    : jmqi@taurus
 * Create Time: 2020-05-28:18:06:28
 * Description:
 *
 */

#ifndef __MDL_COMMON_H__
#define __MDL_COMMON_H__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>

#include "mdl_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBOX_SIZE       (64)
#define BS_CSCA         (20) // Color Space Conversion Accuracy
#define MAT_ACC         (16)
#define MAT_ONE         ((1 << MAT_ACC) - 1)
#define MAT_HALF_ONE    (1 << (MAT_ACC - 1))

#define MSCALEX         (0) // horizontal scale factor
#define MSKEWX          (1) // horizontal skew factor
#define MTRANSX         (2) // horizontal translation
#define MSKEWY          (3) // vertical skew factor
#define MSCALEY         (4) // vertical scale factor
#define MTRANSY         (5) // vertical translation
#define MPERSP0         (6) // input x perspective factor
#define MPERSP1         (7) // input y perspective factor
#define MPERSP2         (8) // perspective bias

#define S32_MAX         (INT_MAX)
#define S32_MIN         (INT_MIN)

#define S16_MIN         (SHRT_MIN)
#define S16_MAX         (SHRT_MAX)
#define F32_MIN         (-0x8000)//fixme
#define F32_MAX         (0x7FFF)

#define MAX(a, b)       (a > b ? a : b)
#define MIN(a, b)       (a < b ? a : b)
#define CLIP(val, min, max)     ((val) < (min) ? (min) : (val) > (max) ? (max) : (val))

static inline uint8_t bilinear_u8(uint8_t p0, uint8_t p1, uint8_t p2, uint8_t p3,
                                  uint16_t xw, uint16_t yw)
{
    uint32_t v0 = (p0 * (MAT_ONE - xw) + p1 * xw + MAT_HALF_ONE) >> MAT_ACC;
    uint32_t v1 = (p2 * (MAT_ONE - xw) + p3 * xw + MAT_HALF_ONE) >> MAT_ACC;
    uint32_t v = (v0 * (MAT_ONE - yw) + v1 * yw + MAT_HALF_ONE) >> MAT_ACC;
    return (uint8_t)(v > 255 ? 255 : v);
}

static inline uint8_t bilinear_u4(uint8_t p0, uint8_t p1, uint8_t p2, uint8_t p3,
                                  uint16_t xw, uint16_t yw)
{
    uint8_t val = 0;
    uint8_t bn_sel = 0xF;
    int shift;
    for (shift = 0; shift < 8; shift +=4) {
        uint8_t p0_4b = p0 >> shift;
        uint8_t p1_4b = p1 >> shift;
        uint8_t p2_4b = p2 >> shift;
        uint8_t p3_4b = p3 >> shift;
        uint8_t val0_4b = ((p0_4b & bn_sel) * (MAT_ONE - xw) + (p1_4b & bn_sel) * xw + MAT_HALF_ONE) >> MAT_ACC;
        uint8_t val1_4b = ((p2_4b & bn_sel) * (MAT_ONE - xw) + (p3_4b & bn_sel) * xw + MAT_HALF_ONE) >> MAT_ACC;
        uint8_t val_4b = (val0_4b * (MAT_ONE - yw) + val1_4b * yw + MAT_HALF_ONE) >> MAT_ACC;
        //clip 0 ~ 15 //attention !!! fixme
        val |= val_4b << shift;
    }
    return val;
}

static inline uint8_t bilinear_u2(uint8_t p0, uint8_t p1, uint8_t p2, uint8_t p3,
                                  uint16_t xw, uint16_t yw)
{
    uint8_t val = 0;
    uint8_t bn_sel = 3;
    int n;
    for (n = 0; n < 4; n++) {
        int shift = n * 2;
        uint8_t p0_2b = p0 >> shift;
        uint8_t p1_2b = p1 >> shift;
        uint8_t p2_2b = p2 >> shift;
        uint8_t p3_2b = p3 >> shift;
        uint8_t val0_2b = ((p0_2b & bn_sel) * (MAT_ONE - xw) + (p1_2b & bn_sel) * xw + MAT_HALF_ONE) >> MAT_ACC;
        uint8_t val1_2b = ((p2_2b & bn_sel) * (MAT_ONE - xw) + (p3_2b & bn_sel) * xw + MAT_HALF_ONE) >> MAT_ACC;
        uint8_t val_2b = (val0_2b * (MAT_ONE - yw) + val1_2b * yw + MAT_HALF_ONE) >> MAT_ACC;
        //clip 0 ~ 3 //attention !!! fixme
        val |= val_2b << shift;
    }
    return val;
}

static inline void bilinear_bound_check(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                                        int16_t cur_x, int16_t cur_y, bool *out_bound)
{
    int16_t x0 = cur_x;
    int16_t x1 = cur_x + 1;
    int16_t y0 = cur_y;
    int16_t y1 = cur_y + 1;
    uint16_t last_x = w + x - 1;
    uint16_t last_y = h + y - 1;
    out_bound[0] = ((x0 < x) || (x0 > last_x) ||
                    (y0 < y) || (y0 > last_y)) ? true : false;
    out_bound[1] = ((x1 < x) || (x1 > last_x) ||
                    (y0 < y) || (y0 > last_y)) ? true : false;
    out_bound[2] = ((x0 < x) || (x0 > last_x) ||
                    (y1 < y) || (y1 > last_y)) ? true : false;
    out_bound[3] = ((x1 < x) || (x1 > last_x) ||
                    (y1 < y) || (y1 > last_y)) ? true : false;
}

inline static void perspective_dst2src(const int32_t *matrix,
                                       const int32_t dx, const int32_t dy,
                                       int16_t *x_p, int16_t *y_p,
                                       uint16_t *x_w, uint16_t *y_w)
{
    assert(dx < 65536);
    assert(dy < 65536);
    int64_t sx_s64 = (dx * (int64_t)matrix[MSCALEX] +
                      dy * (int64_t)matrix[MSKEWX] + matrix[MTRANSX]);
    int64_t sy_s64 = (dx * (int64_t)matrix[MSKEWY] +
                      dy * (int64_t)matrix[MSCALEY] + matrix[MTRANSY]);
    int64_t sz_s64 = (dx * (int64_t)matrix[MPERSP0] +
                      dy * (int64_t)matrix[MPERSP1] + matrix[MPERSP2]);

    //must initialized, if not do this, when sz is zero, will be error
    int32_t sx_p = -2, sy_p = -2;
    uint32_t sx_w = 0, sy_w = 0;
    if (sz_s64) {
        int64_t sx_s64_final = sx_s64 * (1 << MAT_ACC) / sz_s64;
        int64_t sy_s64_final = sy_s64 * (1 << MAT_ACC) / sz_s64;
        int32_t sx_s32_clip = CLIP(sx_s64_final, S32_MIN, S32_MAX);
        int32_t sy_s32_clip = CLIP(sy_s64_final, S32_MIN, S32_MAX);
        sx_p = sx_s32_clip >> MAT_ACC;
        sy_p = sy_s32_clip >> MAT_ACC;
        sx_w = sx_s32_clip & ((1 << MAT_ACC) - 1);
        sy_w = sy_s32_clip & ((1 << MAT_ACC) - 1);
#ifdef MDL_DEBUG
        if ((dx == debug_dx) && (dy == debug_dy)) {
            printf("S32_MIN=%d, S32_MAX=%d\n", S32_MIN, S32_MAX);
            printf("Matrix:\n");
            printf("[%d, %d, %d]\n", matrix[0], matrix[1], matrix[2]);
            printf("[%d, %d, %d]\n", matrix[3], matrix[4], matrix[5]);
            printf("[%d, %d, %d]\n", matrix[6], matrix[7], matrix[8]);
            printf("dst(%d,%d),src(%d,%d) weight:%d, %d -- %lld, %lld, %d, %d, %d, %d\n",
                   dx, dy, sx_p, sy_p, sx_w, sy_w, sx_s64_final, sy_s64_final, sx_s32_clip, sy_s32_clip, S32_MIN, S32_MAX);
        }
#endif
    }
    *x_p = sx_p;
    *y_p = sy_p;
    *x_w = sx_w;
    *y_w = sy_w;
}

#ifdef __cplusplus
}
#endif
#endif /* __MDL_COMMON_H__ */

