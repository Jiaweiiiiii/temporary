/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : perspective_whole_int.cpp
 * Authors    : jmqi@taurus
 * Create Time: 2020-05-11:17:07:31
 * Description:
 *
 */

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "mdl_common.h"
#include "mdl_debug.h"
#include "perspective_whole_int.h"
#include "matrix.h"

void perspective_whole_int(const data_info_s *src,
                           const int box_num, const data_info_s *dst,
                           const box_affine_info_s *boxes,//fixme
                           const uint32_t *coef, const uint32_t *offset)
{
    const uint8_t bpp = 4;
    const uint32_t zero_point = boxes->zero_point;
    uint8_t *src_base = (uint8_t *)src->base;
    // get inverse matrix
    float inverse[9];
    get_inverse_matrix(boxes->matrix, inverse);

    //float to s32
    int32_t matrix[9] = {0};
    matrix_float_to_s32(inverse, matrix);

#ifdef MDL_DEBUG
//    printf("[GLD]Matrix:\n");
//    printf("[%d, %d, %d]\n", matrix[0], matrix[1], matrix[2]);
//    printf("[%d, %d, %d]\n", matrix[3], matrix[4], matrix[5]);
//    printf("[%d, %d, %d]\n", matrix[6], matrix[7], matrix[8]);
#endif

    int16_t sx_p, sy_p;
    uint16_t sx_w, sy_w;
    bool out_bound[4];
    uint16_t dx, dy, k;
    for (dx = 0; dx < dst->width; dx++) {//fixme
        for (dy = 0; dy < dst->height; dy++) {//fixme
            //caculate src
            perspective_dst2src(matrix, dx, dy, &sx_p, &sy_p, &sx_w, &sy_w);
            //judge over bound or not
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
                if ((dx == debug_dx) && (dy == debug_dy)) {
                    printf("dst(%d,%d),src(%d,%d) weight:%d, %d, %d,%d,%d,%d, -- %d,%d,%d,%d -- %d\n",
                           dx, dy, sx_p, sy_p, sx_w, sy_w, out_bound[0], out_bound[1], out_bound[2], out_bound[3], p00, p01, p10, p11, val);
                }
            }
        }
    }
}
