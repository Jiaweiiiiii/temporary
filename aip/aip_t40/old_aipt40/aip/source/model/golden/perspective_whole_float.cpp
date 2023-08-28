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
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "mdl_common.h"
#include "mdl_debug.h"
#include "perspective_whole_float.h"
#include "matrix.h"

static inline uint8_t bilinear_f32(uint8_t p0, uint8_t p1, uint8_t p2, uint8_t p3,
                                   float xw, float yw)
{
    double v = ((p0 * (1 - xw) + p1 * xw) * (1 - yw) +
                (p2 * (1 - xw) + p3 * xw) * yw);
    return (uint8_t)(v > 255 ? 255 : v);
}

void perspective_whole_float(const data_info_s *src,
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
            float sx = matrix[0] * dx + matrix[1] * dy + matrix[2];
            float sy = matrix[3] * dx + matrix[4] * dy + matrix[5];
            float sz = matrix[6] * dx + matrix[7] * dy + matrix[8];
            float inverse_sz = 0;
            if (sz) {
                inverse_sz = 1 / sz;
            }
            float final_sx = sx * inverse_sz;
            float final_sy = sy * inverse_sz;
            int sx_p, sy_p;
            if (final_sx > 0) {
                sx_p = (int)final_sx;
            } else {
                sx_p = (int)round(final_sx - 0.5);
            }

            if (final_sy > 0) {
                sy_p = (int)final_sy;
            } else {
                sy_p = (int)round(final_sy - 0.5);
            }
            sx_p = CLIP(sx_p, -32768, 32767);
            sy_p = CLIP(sy_p, -32768, 32767);

            int sx_w = (int)(fabs(final_sx - sx_p) * (1 << MAT_ACC));
            int sy_w = (int)(fabs(final_sy - sy_p) * (1 << MAT_ACC));

            float sx_w_f = final_sx - sx_p;
            float sy_w_f = final_sy - sy_p;

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
                uint8_t val_f = bilinear_f32(p00, p01, p10, p11, sx_w_f, sy_w_f);

                int diff = abs(val - val_f);
                if (diff > 1) {
                    printf("(%d, %d) -- %f, %f, %f, %f\n", val, val_f, (float)sx_w/65536, (float)sy_w/65536, sx_w_f, sy_w_f);

                }

                int dst_idx = dy * dst->line_stride + dx * bpp + k;
                ((uint8_t *)dst->base)[dst_idx] = val;
                if ((dx == debug_dx) && (dy == debug_dy)) {
                    printf("float(%d,%d),src(%d,%d) weight:%d, %d, %d,%d,%d,%d, -- %d,%d,%d,%d -- %d\n",
                           dx, dy, sx_p, sy_p, sx_w, sy_w, out_bound[0], out_bound[1], out_bound[2], out_bound[3], p00, p01, p10, p11, val);
                }

            }
        }
    }
}
