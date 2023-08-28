/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : affine_whole_int.cpp
 * Authors    : jmqi@taurus
 * Create Time: 2020-05-11:17:07:31
 * Description:
 *
 */

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "affine_whole_int.h"
#include "mdl_common.h"
#include "mdl_debug.h"
#include "matrix.h"

void affine_whole_int(const data_info_s *src,
                      const int box_num, const data_info_s *dst,
                      const box_affine_info_s *boxes,//fixme
                      const uint32_t *coef, const uint32_t *offset)
{
    const uint8_t bpp = 4;
    const uint32_t zero_point = boxes->zero_point;
    uint8_t *src_base = (uint8_t *)src->base;
    // get inverse matrix
    float inverse[9];
#if 1
    get_inverse_matrix(boxes->matrix, inverse);
#else
    printf("Golden inverse Matrix:\n");
    for (int i = 0; i < 9; i++) {
        inverse[i] = boxes->matrix[i];
        printf("%f, ", inverse[i]);
    }
    printf("\n");
#endif

    int32_t matrix[9] = {0};
    for (int i = 0; i < 9; i++) {
        matrix[i] = (int32_t)round(inverse[i] * (1 << MAT_ACC));
    }

    uint16_t dx, dy, k;
    for (dx = 0; dx < dst->width; dx++) {//fixme
        for (dy = 0; dy < dst->height; dy++) {//fixme
            int64_t sx_s64 = (int64_t)matrix[0] * dx + (int64_t)matrix[1] * dy + matrix[2];
            int64_t sy_s64 = (int64_t)matrix[3] * dx + (int64_t)matrix[4] * dy + matrix[5];
            int32_t sx_s32 = CLIP(sx_s64, S32_MIN, S32_MAX);
            int32_t sy_s32 = CLIP(sy_s64, S32_MIN, S32_MAX);

            int32_t sx_p, sy_p, sx_w, sy_w;
            sx_p = sx_s32 >> MAT_ACC;
            sy_p = sy_s32 >> MAT_ACC;
            sx_w = sx_s32 & ((1 << MAT_ACC) - 1);
            sy_w = sy_s32 & ((1 << MAT_ACC) - 1);

            bool out_bound[4];
            bilinear_bound_check(0, 0, boxes->box.w, boxes->box.h,
                                 sx_p, sy_p, out_bound);
            uint32_t p00_idx = ((sy_p - 0) * src->line_stride +
                                (sx_p - 0) * bpp);
            uint32_t p01_idx = p00_idx + bpp;
            uint32_t p10_idx = p00_idx + src->line_stride;
            uint32_t p11_idx = p10_idx + bpp;

            for ( k = 0; k < bpp; k++) {
                uint8_t p00 = out_bound[0] ? zero_point : src_base[p00_idx + k];
                uint8_t p01 = out_bound[1] ? zero_point : src_base[p01_idx + k];
                uint8_t p10 = out_bound[2] ? zero_point : src_base[p10_idx + k];
                uint8_t p11 = out_bound[3] ? zero_point : src_base[p11_idx + k];
                uint8_t val = bilinear_u8(p00, p01, p10, p11, sx_w, sy_w);
                int dst_idx = dy * dst->line_stride + dx * bpp + k;
                ((uint8_t *)dst->base)[dst_idx] = val;
                //if ((0 == dx) && (1091 == dy)) {
                //    printf("gld(%d,%d)--src:p(%d,%d),w(%d,%d),src_val:(%d,%d,%d,%d)(%d,%d,%d,%d),val:%d\n",
                //           dx, dy, sx_p, sy_p, sx_w, sy_w, p00, p01, p10, p11, out_bound[0], out_bound[1], out_bound[2], out_bound[3], val);
                //}
            }
        }
    }
}
