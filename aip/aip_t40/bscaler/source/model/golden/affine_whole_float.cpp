/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : perspective_whole_float.cpp
 * Authors    : jmqi@taurus
 * Create Time: 2020-05-11:16:54:16
 * Description:
 *
 */
#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "affine_whole_float.h"
#include "mdl_common.h"
#include "mdl_debug.h"
#include "matrix.h"

void affine_whole_float(const data_info_s *src,
                        const int box_num, const data_info_s *dst,
                        const box_affine_info_s *boxes,//fixme
                        const uint32_t *coef, const uint32_t *offset)
{
    const uint32_t zero_point = boxes->zero_point;
    const uint8_t bpp = 4;
    uint8_t *src_base = (uint8_t *)src->base;
    // get inverse matrix
    float matrix[9] = {0};//inverse matrix
    get_inverse_matrix(boxes->matrix, matrix);

    uint16_t dx, dy, k;
    for (dx = 0; dx < dst->width; dx++) {//fixme
        for (dy = 0; dy < dst->height; dy++) {//fixme
            //use floating point type to calculate source coordinates.
            double sx = matrix[0] * dx + matrix[1] * dy + matrix[2];
            double sy = matrix[3] * dx + matrix[4] * dy + matrix[5];

            int32_t sx_p, sy_p;
            if (sx > 0) {
                sx_p = (int)sx;
            } else {
                sx_p = (int)round(sx - 0.5);
            }

            if (sy > 0) {
                sy_p = (int)sy;
            } else {
                sy_p = (int)round(sy - 0.5);
            }
            sx_p = CLIP(sx_p, -32768, 32767);
            sy_p = CLIP(sy_p, -32768, 32767);

            int sx_w = (int)(fabs(sx - sx_p) * 65536);
            int sy_w = (int)(fabs(sy - sy_p) * 65536);
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
            }
        }
    }
}
