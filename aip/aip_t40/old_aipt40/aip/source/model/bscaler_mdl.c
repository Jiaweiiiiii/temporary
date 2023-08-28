/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : bscaler_mdl.c
 * Authors    : jmqi@taurus
 * Create Time: 2020-03-20:15:50:09
 * Description:
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>

#include "bscaler_mdl.h"

uint32_t bst_isum = 0;
uint32_t bst_osum = 0;
uint32_t bsc_isum = 0;
uint32_t bsc_osum = 0;

#if MDL_DEBUG
#define bs_debug   printf
#else
#define bs_debug
#endif

enum MONO {
    INCREASE            = 0,
    DECREASE            = 1,
};

typedef struct {
    int32_t             x;
    int32_t             y;
} s32_point_s;

static s32_point_s *point_min(s32_point_s *p0, s32_point_s *p1, bool x_mono, bool y_mono)
{
    int y0 = p0->y >> MAT_ACC;
    int y1 = p1->y >> MAT_ACC;
    int x0 = p0->x >> MAT_ACC;
    int x1 = p1->x >> MAT_ACC;
    int inverse_x0 = x0;
    int inverse_x1 = x1;
    if (x_mono != y_mono) {
        inverse_x0 = -x0;
        inverse_x1 = -x1;
    }

    if (y0 > y1) {
        return p1;
    } if (y0 < y1) {
        return p0;
    } else {
        if (inverse_x0 < inverse_x1)
            return p0;
        else
            return p1;
    }
}

static int get_min_point(s32_point_s *points, uint8_t *mask, bool x_mono, bool y_mono)
{
    int idx = 0;
    s32_point_s *min = NULL;//&points[0];
    int cnt = 0;
    for (int i = 0; i < OBOX_SIZE; i++) {
        if (mask[i] == 0) {
            if (cnt == 0) {
                min = &points[i];
                cnt++;
            } else {
                min = point_min(&points[i], min, x_mono, y_mono);
            }
        }
        if (&points[i] == min) {
            idx = i;
        }
    }
    assert(idx >= 0);
    return idx;
}

static int get_min_point_unified(s32_point_s *points, uint8_t *mask, uint8_t *mono)
{
    int idx = -1;
    s32_point_s *min = NULL;//&points[0];
    int cnt = 0;
    for (int i = 0; i < OBOX_SIZE; i++) {
        //bool x_increase = (mono[i] >> 1) & 0x1 ? true : false;
        //bool y_increase = (mono[i] & 0x1) ? true : false;
        bool x_mono = (((mono[i] >> 1) & 0x1) == INCREASE) ? true : false;
        bool y_mono = ((mono[i] & 0x1) == INCREASE) ? true : false;
        if (x_mono == y_mono) {
            if (mask[i] == 0) {
                if (cnt == 0) {
                    min = &points[i];
                    cnt++;
                } else {
                    min = point_min(&points[i], min, x_mono, y_mono);
                }
            }
            if (&points[i] == min) {
                idx = i;
            }
        }
    }
    assert(idx >= 0);

    return idx;
}

static int get_min_point_ununified(s32_point_s *points, uint8_t *mask, uint8_t *mono)
{
    int idx = -1;
    s32_point_s *min = NULL;//&points[0];
    int cnt = 0;
    for (int i = 0; i < OBOX_SIZE; i++) {
        //bool x_increase = (mono[i] >> 1) & 0x1 ? true : false;
        //bool y_increase = (mono[i] & 0x1) ? true : false;
        bool x_mono = (((mono[i] >> 1) & 0x1) == INCREASE) ? true : false;
        bool y_mono = ((mono[i] & 0x1) == INCREASE) ? true : false;

        if (x_mono != y_mono) {
            if (mask[i] == 0) {
                if (cnt == 0) {
                    min = &points[i];
                    cnt++;
                } else {
                    min = point_min(&points[i], min, x_mono, y_mono);
                }
            }
            if (&points[i] == min) {
                idx = i;
            }
        }
    }
    //assert(idx >= 0);
    return idx;
}

static void dst_col_point_init_affine(s32_point_s *dst_col_point, uint8_t *mask,
                                      int32_t *matrix,
                                      int dst_sbox_x, int dst_sbox_y,
                                      int dst_sbox_w, int dst_sbox_h)
{
    int32_t scale_y = matrix[MSCALEY];
    int32_t skew_x = matrix[MSKEWX];

    bool x_increase = (skew_x > 0) ? 1 : 0;
    bool y_increase = (scale_y > 0) ? 1 : 0;
    assert(dst_sbox_w <= OBOX_SIZE);
    assert(dst_sbox_h <= OBOX_SIZE);

    //1. init mask
    memset(mask, 0, OBOX_SIZE);
    for (int i = dst_sbox_w; i < OBOX_SIZE; i++) {
        mask[i] = 1;
    }

    for (int i = 0; i < OBOX_SIZE; i++) {
        if (i < dst_sbox_w) {
            if (x_increase & y_increase) {
                dst_col_point[i].x = i + dst_sbox_x;
                dst_col_point[i].y = 0 + dst_sbox_y;
            } else if (x_increase & !y_increase) {
                dst_col_point[i].x = i + dst_sbox_x;
                dst_col_point[i].y = dst_sbox_h - 1 + dst_sbox_y;
            } else if (!x_increase & y_increase) {
                dst_col_point[i].x = dst_sbox_w - 1 - i + dst_sbox_x;
                dst_col_point[i].y = 0 + dst_sbox_y;
            } else if (!x_increase & !y_increase) {
                dst_col_point[i].x = dst_sbox_w - 1 - i + dst_sbox_x;
                dst_col_point[i].y = dst_sbox_h - 1 + dst_sbox_y;
            } else {
                assert(0);
            }
        } else {
            if (x_increase & y_increase) {
                //dst_col_point[i].x = OBOX_SIZE - 1 + dst_sbox_x;
                dst_col_point[i].x = dst_sbox_x;
                dst_col_point[i].y = OBOX_SIZE - 1 + dst_sbox_y;
            } else if (x_increase & !y_increase) {
                //dst_col_point[i].x = OBOX_SIZE - 1 + dst_sbox_x;
                dst_col_point[i].x = dst_sbox_x;
                dst_col_point[i].y = dst_sbox_y;
            } else if (!x_increase & y_increase) {
                dst_col_point[i].x = dst_sbox_x;
                dst_col_point[i].y = OBOX_SIZE - 1 + dst_sbox_y;
            } else if (!x_increase & !y_increase) {
                dst_col_point[i].x = dst_sbox_x;
                dst_col_point[i].y = dst_sbox_y;
            } else {
                assert(0);
            }
        }
    }
}

static void col_point_update_affine(s32_point_s *src, s32_point_s *dst,
                                    int dst_x, int dst_y,
                                    int dst_w, int dst_h, int32_t *matrix,
                                    const int idx, uint8_t *mask)
{
    int64_t src_y, src_x;
    int32_t scale_y = matrix[MSCALEY];
    bool y_increase = (scale_y > 0) ? 1 : 0;

    //1. update dst->y
    if (mask[idx] == 0) {
        if (y_increase) {//increase
            dst->y++;
        } else {//decrease
            dst->y--;
        }
    }

    //2. update mask
    if (y_increase) {//increase
        if (dst->y == dst_y + dst_h) {
            mask[idx] = 1;
        }
    } else {//decrease
        if (dst->y == dst_y - 1) {
            mask[idx] = 1;
        }
    }

    //3. update src_col_point
    if (mask[idx] == 0) {
        src->y = dst->x * matrix[MSKEWY] + dst->y * matrix[MSCALEY] + matrix[MTRANSY];
        src_y = (int64_t)matrix[MSKEWY] * dst->x +
            (int64_t)matrix[MSCALEY] * dst->y + matrix[MTRANSY];
        src->x = dst->x * matrix[MSCALEX] + dst->y * matrix[MSKEWX] + matrix[MTRANSX];
        src_x = (int64_t)matrix[MSCALEX] * dst->x +
            (int64_t)matrix[MSKEWX] * dst->y + matrix[MTRANSX];

        int32_t s32_sx = CLIP(src_x, S32_MIN, S32_MAX);
        int32_t s32_sy = CLIP(src_y, S32_MIN, S32_MAX);
        src->x = s32_sx;
        src->y = s32_sy;
    }
}

static inline void affine_dst2src_64(s32_point_s *dst, s32_point_s *src, int32_t *matrix)
{
    int64_t src_x = (int64_t)matrix[MSCALEX] * dst->x +
        (int64_t)matrix[MSKEWX] * dst->y + matrix[MTRANSX];

    int64_t src_y = (int64_t)matrix[MSKEWY] * dst->x +
        (int64_t)matrix[MSCALEY] * dst->y + matrix[MTRANSY];

    int32_t s32_max = 2147483647;
    int32_t s32_min = -2147483648;
    int32_t s32_sx = CLIP(src_x, s32_min, s32_max);
    int32_t s32_sy = CLIP(src_y, s32_min, s32_max);

    src->x = s32_sx;
    src->y = s32_sy;
}

void bs_affine_box_rgb_complex(bs_box_s *src, bs_box_s *dst,
                               int32_t *matrix, uint8_t zero_point,
                               uint32_t is_abgr_order, const uint8_t nv2bgr_alpha)
{
    printf("%s\n", __func__);
    assert(dst->sbox_w <= 64);
    assert(dst->sbox_h <= 64);
    int bpp = 4;
    uint8_t zero[4] = {zero_point,zero_point,zero_point,zero_point}; //fixme
    uint16_t src_sbox_x_last = src->sbox_w + src->sbox_x - 1;
    uint16_t src_sbox_y_last = src->sbox_h + src->sbox_y - 1;

    s32_point_s dst_col_point[OBOX_SIZE] = {0}; //0~15 - x, 16 ~ 31 - y
    s32_point_s src_col_point[OBOX_SIZE] = {0}; //0~15 - x, 16 ~ 31 - y
    uint8_t mask[OBOX_SIZE] = {0};
    dst_col_point_init_affine(dst_col_point, mask, matrix, dst->sbox_x, dst->sbox_y,
                              dst->sbox_w, dst->sbox_h);

    for (int i = 0; i < OBOX_SIZE; i++) {
        affine_dst2src_64(dst_col_point + i, src_col_point + i, matrix);
    }

    bool y_increase = matrix[MSCALEY] > 0 ? true : false;
    bool x_increase = matrix[MSKEWX] > 0 ? true : false;
    int last_x, last_y;
    int hit[64][64] = {0};//for check
    int size = dst->sbox_w * dst->sbox_h;
    for (int i = 0; i < size; i++) {
        int idx = get_min_point(src_col_point, mask, x_increase, y_increase);
        int dx = dst_col_point[idx].x;
        int dy = dst_col_point[idx].y;
        int hit_x = dst_col_point[idx].x - dst->sbox_x;
        int hit_y = dst_col_point[idx].y - dst->sbox_y;
        hit[hit_x][hit_y]++;
        if ((hit_x > dst->sbox_w) || (hit_y > dst->sbox_h)) {
            printf("[Error] : overflow!\n");
        }

        int32_t sx_s32 = src_col_point[idx].x;
        int32_t sy_s32 = src_col_point[idx].y;
        uint16_t sx_w = sx_s32 & ((1 << MAT_ACC) - 1);
        uint16_t sy_w = sy_s32 & ((1 << MAT_ACC) - 1);
        int16_t sx_p0 = sx_s32 >> MAT_ACC;
        int16_t sy_p0 = sy_s32 >> MAT_ACC;
        int16_t sx_p1 = sx_p0 + 1;
        int16_t sy_p1 = sy_p0 + 1;

        if (i > 0) {
            int cur_x = sx_p0;
            int cur_y = sy_p0;
            if ((cur_y == last_y) && (cur_x > last_x) && (x_increase != y_increase)) {
                printf("[Error] :(%d,%d) x not decrease, cur_x =%d, last_x=%d\n", dx, dy, cur_x, last_x);
            } else if ((cur_y == last_y) && (cur_x < last_x) && (x_increase == y_increase)) {
                printf("[Error] :(%d,%d) x not increase, cur_x =%d, last_x=%d\n", dx, dy, cur_x, last_x);
            }

            if (cur_y < last_y) {
                printf("[Error] :(%d,%d) y not increase, cur_y =%d, last_y=%d\n", dx, dy, cur_y, last_y);
            }
        }

        last_x = sx_p0;
        last_y = sy_p0;

        bool p00_over = false;
        bool p01_over = false;
        bool p10_over = false;
        bool p11_over = false;

        if ((sx_p0 < src->sbox_x) || (sx_p0 > src_sbox_x_last) ||
            (sy_p0 < src->sbox_y) || (sy_p0 > src_sbox_y_last)) {
            p00_over = true;
        }

        if ((sx_p1 < src->sbox_x) || (sx_p1 > src_sbox_x_last) ||
            (sy_p0 < src->sbox_y) || (sy_p0 > src_sbox_y_last)) {
            p01_over = true;
        }

        if ((sx_p0 < src->sbox_x) || (sx_p0 > src_sbox_x_last) ||
            (sy_p1 < src->sbox_y) || (sy_p1 > src_sbox_y_last)) {
            p10_over = true;
        }

        if ((sx_p1 < src->sbox_x) || (sx_p1 > src_sbox_x_last) ||
            (sy_p1 < src->sbox_y) || (sy_p1 > src_sbox_y_last)) {
            p11_over = true;
        }

        uint8_t *p00_ptr = (src->base0 + (sy_p0 - src->sbox_y) * src->stride0 +
                            (sx_p0 - src->sbox_x) * bpp);
        uint8_t *p01_ptr = p00_ptr + 1 * bpp;
        uint8_t *p10_ptr = p00_ptr + src->stride0;
        uint8_t *p11_ptr = p10_ptr + 1 * bpp;

        uint8_t *p00 = p00_over ? zero : p00_ptr;
        uint8_t *p01 = p01_over ? zero : p01_ptr;
        uint8_t *p10 = p10_over ? zero : p10_ptr;
        uint8_t *p11 = p11_over ? zero : p11_ptr;

        int dst_idx = (dy - dst->sbox_y) * dst->stride0 +
            (dx - dst->sbox_x) * bpp;
        for (int k = 0; k < bpp; k++) {
            uint8_t val = bilinear_u8(p00[k], p01[k],
                                      p10[k], p11[k], sx_w, sy_w);
            if ((is_abgr_order==0) & (k==3)) {
                dst->base0[dst_idx + k] = nv2bgr_alpha;
            } else if ((is_abgr_order==1) & (k==0)) {
                dst->base0[dst_idx + k] = nv2bgr_alpha;
            } else {
                dst->base0[dst_idx + k] = val;
            }
#ifdef MDL_DEBUG
            if ((dx == debug_dx) && (dy == debug_dy)) {
                printf("dut1(%d,%d),src(0x%x,0x%x) weight:0x%x, 0x%x, 0x%x,0x%x,0x%x,0x%x, -- 0x%x,0x%x,0x%x,0x%x -- %d, %d\n",
                       dx, dy, sx_p0, sy_p0, sx_w, sy_w, p00_over, p01_over, p10_over, p11_over, p00[k], p01[k], p10[k], p11[k], val, dst->base0[dst_idx + k]);
            }
#endif
        }

        col_point_update_affine(src_col_point + idx, dst_col_point + idx,
                                dst->sbox_x, dst->sbox_y, dst->sbox_w, dst->sbox_h, matrix,
                                idx, mask);
    }

    int error = 0;
    for (int y = 0; y < dst->sbox_h; y++) {
        for (int x = 0; x < dst->sbox_w; x++) {
            if (hit[x][y] > 1) {
                printf("[Error]: hit %d times (%d, %d)\n", hit[x][y], dst->sbox_x + x, dst->sbox_y + y);
            } else if (hit[x][y] != 1) {
                printf("[Error]: not hit! (%d, %d)\n", dst->sbox_x + x, dst->sbox_y + y);
                error++;
            }
        }
    }
    if (error) {
        printf("Error: not hit all 64x64!\n");
    }
}

static void dst_col_point_init(const int32_t *matrix,
                               const int dst_sbox_x, const int dst_sbox_y,
                               const int dst_sbox_w, const int dst_sbox_h,
                               uint8_t *mono, int32_t *limit, uint8_t *mask,
                               s32_point_s *dst_col_point,
                               uint8_t mono_x, uint8_t mono_y,
                               int8_t *extreme_point)
{
    assert(dst_sbox_w <= OBOX_SIZE);
    assert(dst_sbox_h <= OBOX_SIZE);

    //1. init mask
    memset(mask, 0, OBOX_SIZE);
    if (dst_sbox_w < OBOX_SIZE) {
        memset(&mask[dst_sbox_w], 1, OBOX_SIZE - dst_sbox_w);
    }
    uint8_t mono_x_pos = (mono_x & 0x7F);
    uint8_t mono_y_pos = (mono_y & 0x7F);
    bool mono_x_init = (mono_x >> 7);
    bool mono_y_init = (mono_y >> 7);

    //2. monotonicity and limit
    for (int i = 0; i < dst_sbox_w; i++) {
        if (i >= mono_x_pos) {
            mono[i] = mono_x_init << 1;
        } else {
            mono[i] = !mono_x_init << 1;
        }

        if (i >= mono_y_pos) {
            mono[i] |= mono_y_init; //increase
        } else {
            mono[i] |= !mono_y_init; //decrease
        }
        limit[i] = extreme_point[i] + dst_sbox_y;
    }
    //3. initial dst_col_point
    for (int i = 0; i < dst_sbox_w; i++) {
        int dx = i + dst_sbox_x;
        if ((mono[i] & 0x1) == INCREASE) {//y increase
            dst_col_point[i].x = dx;
            if ((limit[i] >= dst_sbox_y) && (limit[i] < dst_sbox_y + dst_sbox_h)) {
                dst_col_point[i].y = limit[i];
            } else {
                dst_col_point[i].y = dst_sbox_y;
            }
        } else {//y decrease
            dst_col_point[i].x = dx;
            if ((limit[i] >= dst_sbox_y) && (limit[i] < dst_sbox_y + dst_sbox_h)) {
                dst_col_point[i].y = limit[i];
            } else {
                dst_col_point[i].y = dst_sbox_h - 1 + dst_sbox_y;
            }
        }
    }
}

static void col_point_update(const int dst_x, const int dst_y,
                             const int dst_w, const int dst_h,
                             const int32_t *matrix, const int idx,
                             const uint8_t mono,
                             s32_point_s *src, s32_point_s *dst,
                             uint8_t *mask, int *limit)
{
    bool x_mono = (mono >> 1) & 0x1;
    bool y_mono = (mono & 0x1);
    bool limit_overflow = (limit[idx] > (dst_y + dst_h - 1)) || (limit[idx] < dst_y);

    //1. update dst->y
    if (mask[idx] == 0) {
        if (y_mono == INCREASE) {//increase
            dst->y++;
            if ((!limit_overflow) && (dst->y == dst_y + dst_h)) {
                dst->y = dst_y;
            }
        } else {//decrease
            dst->y--;
            if ((!limit_overflow) && (dst->y == dst_y - 1)) {
                dst->y = dst_y + dst_h - 1;
            }
        }
    }

    //2. update mask
    if (y_mono == INCREASE) {//increase
        if (!limit_overflow) {
            if (dst->y == limit[idx]) {
                mask[idx] = 1;
            }
        } else {
            if (dst->y == dst_y + dst_h) {
                mask[idx] = 1;
            }
        }
    } else {//decrease
        if (!limit_overflow) {
            if (dst->y == limit[idx]) {
                mask[idx] = 1;
            }
        } else {
            if (dst->y == dst_y - 1) {
                mask[idx] = 1;
            }
        }
    }

    //3. update src_col_point
    if (mask[idx] == 0) {
        int16_t sx_p, sy_p;
        uint16_t sx_w, sy_w;
        perspective_dst2src(matrix, dst->x, dst->y, &sx_p, &sy_p, &sx_w, &sy_w);
        src->x = (sx_p << MAT_ACC) | (sx_w & ((1 << MAT_ACC) - 1));
        src->y = (sy_p << MAT_ACC) | (sy_w & ((1 << MAT_ACC) - 1));
    }
}

void bs_perspective_box_rgb_complex(bs_box_s *src, bs_box_s *dst,
                                    int32_t *matrix, uint8_t zero_point,
                                    uint32_t is_abgr_order, const uint8_t nv2bgr_alpha,
                                    uint8_t mono_x, uint8_t mono_y,
                                    int8_t* extreme_point)
{
    assert(dst->sbox_w <= 64);
    assert(dst->sbox_h <= 64);
    assert(dst->base0 != NULL);
    assert(src->base0 != NULL);

    int bpp = 4;
    s32_point_s dst_col_point[OBOX_SIZE] = {0}; //0~15 - x, 16 ~ 31 - y
    s32_point_s src_col_point[OBOX_SIZE] = {0}; //0~15 - x, 16 ~ 31 - y
    uint8_t mask[OBOX_SIZE] = {0};
    uint8_t mono[OBOX_SIZE] = {0};
    int limit[OBOX_SIZE] = {0};
    dst_col_point_init(matrix, dst->sbox_x, dst->sbox_y, dst->sbox_w, dst->sbox_h,
                       mono, limit, mask, dst_col_point, mono_x, mono_y,
                       extreme_point);

    for (int i = 0; i < OBOX_SIZE; i++) {
        int16_t sx_p, sy_p;
        uint16_t sx_w, sy_w;
        if (mask[i] == 0) {
            perspective_dst2src(matrix, dst_col_point[i].x, dst_col_point[i].y,
                                &sx_p, &sy_p, &sx_w, &sy_w);
            src_col_point[i].x = (sx_p << MAT_ACC) | (sx_w & ((1 << MAT_ACC) - 1));
            src_col_point[i].y = (sy_p << MAT_ACC) | (sy_w & ((1 << MAT_ACC) - 1));
        }
    }

    int last_x, last_y;
    int hit[64][64] = {0};//for check

    int size = dst->sbox_w * dst->sbox_h;
    int unified_size = 0;
    int ununified_size = 0;

    for (int i = 0; i < dst->sbox_w; i++) {
        if ((mono[i] == 0) || (mono[i] == 3)) {
            unified_size += dst->sbox_h;
        } else {
            ununified_size += dst->sbox_h;
        }
    }
    //////////////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < unified_size; i++) {
        int idx = get_min_point_unified(src_col_point, mask, mono);
        assert((mono[idx] == 0) || (mono[idx] == 3));
        int dx = dst_col_point[idx].x;
        int dy = dst_col_point[idx].y;
        int32_t sx_s32 = src_col_point[idx].x;
        int32_t sy_s32 = src_col_point[idx].y;
        uint16_t sx_w = sx_s32 & ((1 << MAT_ACC) - 1);
        uint16_t sy_w = sy_s32 & ((1 << MAT_ACC) - 1);
        int16_t sx_p = sx_s32 >> MAT_ACC;
        int16_t sy_p = sy_s32 >> MAT_ACC;
        sx_p = CLIP(sx_p, S16_MIN, S16_MAX);
        sy_p = CLIP(sy_p, S16_MIN, S16_MAX);

        bool out_bound[4];
        bilinear_bound_check(src->sbox_x, src->sbox_y, src->sbox_w, src->sbox_h,
                             sx_p, sy_p, out_bound);

        //attention !!! fix monotonicity error caused by interger deviation.
        if (i > 0) {
            int cur_x = sx_p;
            int cur_y = sy_p;
            if ((cur_y < last_y) && (cur_y + 1 == last_y)) {
                printf("Error: dst(%d,%d) -- cur(%d,%d) last(%d,%d)\n", dx, dy, cur_x, cur_y, last_x, last_y);
            }

            if ((cur_y == last_y) && (cur_x < last_x) && (cur_x + 1 == last_x)) {
                printf("Error: dst(0x%x,0x%x) -- cur(0x%x,0x%x) last(0x%x,0x%x)\n", dx, dy, cur_x, cur_y, last_x, last_y);
            }
        }

        //check
        {// only for check hit all 64x64 or not
            int hit_x = dst_col_point[idx].x - dst->sbox_x;
            int hit_y = dst_col_point[idx].y - dst->sbox_y;
            hit[hit_x][hit_y]++;
            if ((hit_x > dst->sbox_w) || (hit_y > dst->sbox_h)) {
                printf("[Error] : overflow!\n");
            }

            if (i > 0) {
                int cur_x = sx_p;
                int cur_y = sy_p;
                if ((cur_y == last_y) &&
                    (cur_x < last_x) &&
                    (cur_y > src->sbox_y) &&
                    (cur_y < src->sbox_y + src->sbox_h) &&
                    (cur_x > src->sbox_x) &&
                    (cur_x < src->sbox_x + src->sbox_w)
                    ) {
                    printf("[Error] :(%d,%d) x not increase, cur_x =%d, last_x=%d\n", dx, dy, cur_x, last_x);
                }

                if ((cur_y < last_y) &&
                    (cur_y > src->sbox_y) &&
                    (cur_y < src->sbox_y + src->sbox_h) &&
                    (cur_x > src->sbox_x) &&
                    (cur_x < src->sbox_x + src->sbox_w)
                    ) {
                    printf("%d, %d, %d, %d\n", mono_y, (mono_y & 0x7F), dst->sbox_y, dy);
                    printf("[Error] :(%d,%d) y not increase, cur_y =%d, last_y=%d\n", dx, dy, cur_y, last_y);
                }
            }
            last_x = sx_p;
            last_y = sy_p;
        }

        uint32_t p00_idx = ((sy_p - src->sbox_y) * src->stride0 +
                            (sx_p - src->sbox_x) * bpp);
        uint32_t p01_idx = p00_idx + bpp;
        uint32_t p10_idx = p00_idx + src->stride0;
        uint32_t p11_idx = p10_idx + bpp;

        int dst_idx = (dy - dst->sbox_y) * dst->stride0 +
            (dx - dst->sbox_x) * bpp;
        for (int k = 0; k < bpp; k++) {
            uint8_t p00 = out_bound[0] ? zero_point : src->base0[p00_idx + k];
            uint8_t p01 = out_bound[1] ? zero_point : src->base0[p01_idx + k];
            uint8_t p10 = out_bound[2] ? zero_point : src->base0[p10_idx + k];
            uint8_t p11 = out_bound[3] ? zero_point : src->base0[p11_idx + k];
            uint8_t val = bilinear_u8(p00, p01, p10, p11, sx_w, sy_w);
            if((is_abgr_order==0) & (k==3)){
                dst->base0[dst_idx + k] = nv2bgr_alpha;
            }else if((is_abgr_order==1) & (k==0)){
                dst->base0[dst_idx + k] = nv2bgr_alpha;
            }
            else{
                dst->base0[dst_idx + k] = val;
            }
#ifdef MDL_DEBUG
            if ((dx == debug_dx) && (dy == debug_dy)) {
                printf("dut0(%d,%d),src(%d,%d) weight:%d, %d, %d,%d,%d,%d, -- %d,%d,%d,%d -- %d -- %f, %f\n",
                       dx, dy, sx_p, sy_p, sx_w, sy_w, out_bound[0], out_bound[1], out_bound[2], out_bound[3], p00, p01, p10, p11, val, (float)sx_s32/(1 << MAT_ACC), (float)sy_s32/(1 << MAT_ACC));
            }
#endif
        }
        col_point_update(dst->sbox_x, dst->sbox_y, dst->sbox_w, dst->sbox_h, matrix,
                         idx, mono[idx], src_col_point + idx, dst_col_point + idx,
                         mask, limit);
    }

    //////////////////////////////////////////////////////////////////////////////////
    // un-unified
    for (int i = 0; i < ununified_size; i++) {
        int idx = get_min_point_ununified(src_col_point, mask, mono);
        assert((mono[idx] == 1) || (mono[idx] == 2));
        int dx = dst_col_point[idx].x;
        int dy = dst_col_point[idx].y;
        int32_t sx_s32 = src_col_point[idx].x;
        int32_t sy_s32 = src_col_point[idx].y;
        uint16_t sx_w = sx_s32 & ((1 << MAT_ACC) - 1);
        uint16_t sy_w = sy_s32 & ((1 << MAT_ACC) - 1);
        int16_t sx_p = sx_s32 >> MAT_ACC;
        int16_t sy_p = sy_s32 >> MAT_ACC;
        bool out_bound[4];
        bilinear_bound_check(src->sbox_x, src->sbox_y, src->sbox_w, src->sbox_h,
                             sx_p, sy_p, out_bound);

        //attention !!! fix monotonicity error caused by interger deviation.
        if (i > 0) {
            int cur_x = sx_p;
            int cur_y = sy_p;
            if ((cur_y < last_y) && (cur_y > src->sbox_y) && (cur_y + 1 == last_y)) {
                printf("Error: dst(%d,%d) -- cur(%d,%d) last(%d,%d)\n", dx, dy, cur_x, cur_y, last_x, last_y);
            }

            if ((cur_y == last_y) && (cur_x > last_x) && (cur_x - 1 == last_x)) {
                printf("Error: dst(%d,%d) -- cur(%d,%d) last(%d,%d)\n", dx, dy, cur_x, cur_y, last_x, last_y);
            }

        }

        {// only for check hit all 64x64 or not
            int hit_x = dst_col_point[idx].x - dst->sbox_x;
            int hit_y = dst_col_point[idx].y - dst->sbox_y;
            hit[hit_x][hit_y]++;
            if ((hit_x > dst->sbox_w) || (hit_y > dst->sbox_h)) {
                printf("[Error] : overflow!\n");
            }

            if (i > 0) {
                int cur_x = sx_p;
                int cur_y = sy_p;
                if ((cur_y == last_y) &&
                    (cur_x > last_x) &&
                    (cur_x > src->sbox_x) && (cur_x < src->sbox_x + src->sbox_w) &&
                    (cur_y > src->sbox_y) && (cur_y < src->sbox_y + src->sbox_h)
                    ) {
                    printf("[Error] :(%d,%d) x not decrease, cur_x =%d, last_x=%d\n", dx, dy, cur_x, last_x);
                }

                if ((cur_y < last_y) && (cur_y > src->sbox_y) &&
                    (cur_x > src->sbox_x) && (cur_x < src->sbox_x + src->sbox_w) &&
                    (cur_y > src->sbox_y) && (cur_y < src->sbox_y + src->sbox_h)
                    ) {
                    printf("[Error] :(%d,%d) y not increase, cur_y =%d, last_y=%d\n", dx, dy, cur_y, last_y);
                }
            }
            last_x = sx_p;
            last_y = sy_p;
        }

        uint32_t p00_idx = ((sy_p - src->sbox_y) * src->stride0 +
                            (sx_p - src->sbox_x) * bpp);
        uint32_t p01_idx = p00_idx + bpp;
        uint32_t p10_idx = p00_idx + src->stride0;
        uint32_t p11_idx = p10_idx + bpp;

        int dst_idx = (dy - dst->sbox_y) * dst->stride0 +
            (dx - dst->sbox_x) * bpp;
        for (int k = 0; k < bpp; k++) {
            uint8_t p00 = out_bound[0] ? zero_point : src->base0[p00_idx + k];
            uint8_t p01 = out_bound[1] ? zero_point : src->base0[p01_idx + k];
            uint8_t p10 = out_bound[2] ? zero_point : src->base0[p10_idx + k];
            uint8_t p11 = out_bound[3] ? zero_point : src->base0[p11_idx + k];
            uint8_t val = bilinear_u8(p00, p01, p10, p11, sx_w, sy_w);
            if((is_abgr_order==0) & (k==3)){
                dst->base0[dst_idx + k] = nv2bgr_alpha;
            }
            else if((is_abgr_order==1) & (k==0)){
                dst->base0[dst_idx + k] = nv2bgr_alpha;
            }
            else{
                dst->base0[dst_idx + k] = val;
            }
#ifdef MDL_DEBUG
            if ((dx == debug_dx) && (dy == debug_dy)) {
                printf("dut1(%d,%d),src(%d,%d) weight:%d, %d, %d,%d,%d,%d, -- %d,%d,%d,%d -- %d\n",
                       dx, dy, sx_p, sy_p, sx_w, sy_w, out_bound[0], out_bound[1], out_bound[2], out_bound[3], p00, p01, p10, p11, val);
            }
#endif
        }

        col_point_update(dst->sbox_x, dst->sbox_y, dst->sbox_w, dst->sbox_h, matrix,
                         idx, mono[idx], src_col_point + idx, dst_col_point + idx,
                         mask, limit);
    }
    {//check hit
        int error = 0;
        for (int y = 0; y < dst->sbox_h; y++) {
            for (int x = 0; x < dst->sbox_w; x++) {
                if (hit[x][y] > 1) {
                    printf("[Error]: hit %d times (%d,%d)\n",
                           hit[x][y], dst->sbox_x + x, dst->sbox_y + y);
                } else if (hit[x][y] != 1) {
                    printf("[Error]: not hit! (%d,%d)\n",
                           dst->sbox_x + x, dst->sbox_y + y);
                    error++;
                }
            }
        }
        if (error) {
            printf("Error: not hit all 64x64!\n");
        } else {
            //printf("all hit!\n");
        }
    }
}

void bs_perspective_box_rgb_simple(bs_box_s *src, bs_box_s *dst,
                                   int32_t *matrix, uint8_t zero_point,
                                   uint32_t is_abgr_order, const uint8_t nv2bgr_alpha)
{
    const uint8_t bpp = 4;
    uint8_t *src_base = src->base0;

    int16_t sx_p, sy_p;
    uint16_t sx_w, sy_w;
    bool out_bound[4];
    uint16_t dx, dy, k;
    int dy_last = dst->sbox_h + dst->sbox_y;
    int dx_last = dst->sbox_w + dst->sbox_x;

    for (dy = dst->sbox_y; dy < dy_last; dy++) {
        for (dx = dst->sbox_x; dx < dx_last; dx++) {
            //caculate src
            perspective_dst2src(matrix, dx, dy, &sx_p, &sy_p, &sx_w, &sy_w);
            //judge over bound or not
            bilinear_bound_check(src->sbox_x, src->sbox_y, src->sbox_w, src->sbox_h,
                                 sx_p, sy_p, out_bound);
            uint32_t p00_idx = ((sy_p - src->sbox_y) * src->stride0 +
                                (sx_p - src->sbox_x) * bpp);
            uint32_t p01_idx = p00_idx + bpp;
            uint32_t p10_idx = p00_idx + src->stride0;
            uint32_t p11_idx = p10_idx + bpp;
            int dst_idx = (dy - dst->sbox_y) * dst->stride0 +
                (dx - dst->sbox_x) * bpp;
            for (int k = 0; k < bpp; k++) {
                uint8_t p00 = out_bound[0] ? zero_point : src->base0[p00_idx + k];
                uint8_t p01 = out_bound[1] ? zero_point : src->base0[p01_idx + k];
                uint8_t p10 = out_bound[2] ? zero_point : src->base0[p10_idx + k];
                uint8_t p11 = out_bound[3] ? zero_point : src->base0[p11_idx + k];
                uint8_t val = bilinear_u8(p00, p01, p10, p11, sx_w, sy_w);
                if ((is_abgr_order == 0) & (k == 3)) {
                    dst->base0[dst_idx + k] = nv2bgr_alpha;
                } else if ((is_abgr_order == 1) & (k == 0)) {
                    dst->base0[dst_idx + k] = nv2bgr_alpha;
                } else {
                    dst->base0[dst_idx + k] = val;
                }
#ifdef MDL_DEBUG
                if ((dx == debug_dx) && (dy == debug_dy)) {
                    printf("dut0(%d,%d),src(%d,%d) weight:%d, %d, %d,%d,%d,%d, -- %d,%d,%d,%d -- %d\n",
                           dx, dy, sx_p, sy_p, sx_w, sy_w, out_bound[0], out_bound[1], out_bound[2], out_bound[3], p00, p01, p10, p11, val);
                }
#endif
            }
        }
    }
}

void yuv2rgb(uint8_t y, uint8_t u, uint8_t v,
             const uint32_t *coef, const uint8_t *offset,
             const uint8_t nv2bgr_alpha,
             int order, uint8_t *rgba)
{
    int16_t y_m = y - offset[0];
    int16_t u_m = u - offset[1];
    int16_t v_m = v - offset[1];
    int64_t b32 = ((int64_t)coef[0] * y_m + (int64_t)coef[1] * u_m +
                   (int64_t)coef[2] * v_m + (1 << (BS_CSCA - 1)));
    int64_t g32 = ((int64_t)coef[3] * y_m - (int64_t)coef[4] * u_m -
                   (int64_t)coef[5] * v_m + (1 << (BS_CSCA - 1)));
    int64_t r32 = ((int64_t)coef[6] * y_m + (int64_t)coef[7] * u_m +
                   (int64_t)coef[8] * v_m + (1 << (BS_CSCA - 1)));
    uint8_t a = nv2bgr_alpha;//fixme
    uint8_t b = CLIP(b32, 0, (1 << (BS_CSCA + 8)) - 1) >> BS_CSCA;//must do clip
    uint8_t g = CLIP(g32, 0, (1 << (BS_CSCA + 8)) - 1) >> BS_CSCA;//must do clip
    uint8_t r = CLIP(r32, 0, (1 << (BS_CSCA + 8)) - 1) >> BS_CSCA;//must do clip
    switch (order) {
    case 0://BGRA
        rgba[0] = b;
        rgba[1] = g;
        rgba[2] = r;
        rgba[3] = a;
        break;
    case 1:
        rgba[0] = g;
        rgba[1] = b;
        rgba[2] = r;
        rgba[3] = a;
        break;
    case 2:
        rgba[0] = r;
        rgba[1] = b;
        rgba[2] = g;
        rgba[3] = a;
        break;
    case 3:
        rgba[0] = b;
        rgba[1] = r;
        rgba[2] = g;
        rgba[3] = a;
        break;
    case 4:
        rgba[0] = g;
        rgba[1] = r;
        rgba[2] = b;
        rgba[3] = a;
        break;
    case 5:
        rgba[0] = r;
        rgba[1] = g;
        rgba[2] = b;
        rgba[3] = a;
        break;
    case 8:
        rgba[0] = a;
        rgba[1] = b;
        rgba[2] = g;
        rgba[3] = r;
        break;
    case 9:
        rgba[0] = a;
        rgba[1] = g;
        rgba[2] = b;
        rgba[3] = r;
        break;
    case 10:
        rgba[0] = a;
        rgba[1] = r;
        rgba[2] = b;
        rgba[3] = g;
        break;
    case 11:
        rgba[0] = a;
        rgba[1] = b;
        rgba[2] = r;
        rgba[3] = g;
        break;
    case 12:
        rgba[0] = a;
        rgba[1] = g;
        rgba[2] = r;
        rgba[3] = b;
        break;
    case 13:
        rgba[0] = a;
        rgba[1] = r;
        rgba[2] = g;
        rgba[3] = b;
        break;
    default:
        fprintf(stderr, "[Error][%s]: not support this order(%d)\n", __func__, order);
    }
}

static void rgba2virchn(uint8_t *src, uint32_t src_stride,
                        uint16_t width, uint16_t height,
                        uint8_t knl_w, uint8_t knl_h,
                        uint8_t knl_stride_w, uint8_t knl_stride_h,
                        uint8_t zero,
                        uint8_t pad_top,
                        uint8_t pad_bottom,
                        uint8_t pad_left,
                        uint8_t pad_right,
                        uint8_t *dst,
                        uint32_t dst_stride
    )
{
    int byte_num = knl_w * knl_h * 3;
    int chn_group_num = (byte_num + 31) / 32;

    int dst_w = (width + pad_left + pad_right - knl_w) / knl_stride_w + 1;
    int dst_h = (height + pad_top + pad_bottom - knl_h) / knl_stride_h + 1;

    for (int cg_idx = 0; cg_idx < chn_group_num; cg_idx++) {
        for (int y = 0; y < dst_h; y++) {
            for (int x = 0; x < dst_w; x++) {
                for (int c = 0; c < 32; c++) {
                    int knl_x = ((cg_idx * 32 + c) / 3) % knl_w;
                    int knl_y = ((cg_idx * 32 + c) / 3) / knl_w;
                    int chn = (cg_idx * 32 + c) % 3;
                    int src_st_x = x * knl_stride_w - pad_left;
                    int src_st_y = y * knl_stride_h - pad_top;
                    int src_idx = (src_st_y + knl_y) * src_stride + \
                        (src_st_x + knl_x) * 4 + chn;
                    int dst_idx = c + x * 32 + y * dst_stride + \
                        cg_idx * dst_stride * dst_h;

                    if ((cg_idx * 32 + c >= byte_num) ||
                        (src_st_x + knl_x < 0) ||
                        (src_st_y + knl_y < 0) ||
                        (src_st_x + knl_x >= width) ||
                        (src_st_y + knl_y >= height)) {
                        dst[dst_idx] = zero;
                    } else {
                        dst[dst_idx] = src[src_idx];
                    }
                }
            }
        }
    }
}

void bst_nv2bgr(uint8_t *src_y, uint8_t* src_c, uint32_t src_s,
                uint8_t *dst, uint32_t dst_s, int width, int height,
                uint32_t nv2bgr_order, uint32_t *nv2bgr_coef,
                uint8_t *nv2bgr_ofst, uint8_t nv2bgr_alpha)
{
    uint8_t *rgb_buffer = dst;
    int x, y;
    for (y = 0; y < height; y += 2) {
        for (x = 0; x < width; x += 2) {
            uint8_t pix_y0 = src_y[src_s * y + x];
            uint8_t pix_y1 = src_y[src_s * y + x + 1];
            uint8_t pix_y2 = src_y[src_s * (y + 1) + x];
            uint8_t pix_y3 = src_y[src_s * (y + 1) + (x + 1)];
            uint8_t pix_u  = src_c[src_s * y/2 + x];
            uint8_t pix_v  = src_c[src_s * y/2 + x + 1];
            uint8_t *dst_ptr = rgb_buffer + dst_s * y + x * 4;
            yuv2rgb(pix_y0, pix_u, pix_v, nv2bgr_coef, nv2bgr_ofst, nv2bgr_alpha,
                    nv2bgr_order, dst_ptr);
            yuv2rgb(pix_y1, pix_u, pix_v, nv2bgr_coef, nv2bgr_ofst, nv2bgr_alpha,
                    nv2bgr_order, dst_ptr + 4);
            yuv2rgb(pix_y2, pix_u, pix_v, nv2bgr_coef, nv2bgr_ofst, nv2bgr_alpha,
                    nv2bgr_order, dst_ptr + dst_s);
            yuv2rgb(pix_y3, pix_u, pix_v, nv2bgr_coef, nv2bgr_ofst, nv2bgr_alpha,
                    nv2bgr_order, dst_ptr + dst_s + 4);
            //printf("%d, %d, %d, %d, %d, %d -- %d, %d, %d, %d\n", pix_y0, pix_y1, pix_y2, pix_y3, pix_u, pix_v, dst_ptr[0], dst_ptr[1], dst_ptr[2], dst_ptr[3]);
        }
    }
}

int normal2kernel(uint32_t width,
                  uint32_t height,
                  uint8_t kernel_size,
                  uint8_t kernel_xstride,
                  uint8_t kernel_ystride,
                  uint32_t zero_point,
                  uint8_t pad_left,
                  uint8_t pad_right,
                  uint8_t pad_top,
                  uint8_t pad_bottom,
                  uint8_t *src,
                  uint32_t src_line_stride,
                  uint8_t *dst,
                  uint32_t dst_line_stride,
                  uint32_t dst_plane_stride)
{
    uint8_t kw = ((kernel_size == 3) ? 7 :
                  (kernel_size == 2) ? 5 :
                  (kernel_size == 1) ? 3 : 1);
    uint8_t kh = kw;
    uint8_t chn_num_valid = kw * kh * 3;
    uint8_t plane_num = (chn_num_valid + 31) >> 5;
    uint8_t chn_num = plane_num * 32;
    uint8_t zero0 = (zero_point >> 0) & 0xFF;
    uint8_t zero1 = (zero_point >> 8) & 0xFF;
    uint8_t zero2 = (zero_point >> 16) & 0xFF;
    uint8_t zero3 = (zero_point >> 24) & 0xFF;
    int32_t kernel_idx = 0;
    int32_t dst_w = (width + pad_left + pad_right - kw) / kernel_xstride + 1;
    int32_t dst_h = (height + pad_top + pad_bottom - kh) / kernel_ystride + 1;
    for (int y = 0; y < dst_h; y++) {
        for (int x = 0; x < dst_w; x++) {
            for (int c_idx = 0; c_idx < chn_num; c_idx++) {
                int pixel_kernel_i = c_idx / 3;
                int channel_pixel_i = c_idx % 3;
                int pixel_kernel_x = pixel_kernel_i % kw;
                int pixel_kernel_y = pixel_kernel_i / kw;
                int pixel_frame_x = x*kernel_xstride + pixel_kernel_x;
                int pixel_frame_y = y*kernel_ystride + pixel_kernel_y;
                int pixel_frame_x1 = pixel_frame_x - pad_left;
                int pixel_frame_y1 = pixel_frame_y - pad_top;
                int pixel_frame_i1 = pixel_frame_y1*(src_line_stride/4) + pixel_frame_x1;
                uint8_t is_dummy_edge  = ((pixel_frame_x1 < 0) ||
                                          (pixel_frame_x1 >= width) ||
                                          (pixel_frame_y1 < 0) ||
                                          (pixel_frame_y1 >= height));
                uint8_t is_dummy_chnn  = (c_idx >= chn_num_valid);
                int dst_idx = ((c_idx / 32) * dst_plane_stride +
                               y * dst_line_stride + x * 32 + (c_idx % 32));
                int src_idx = pixel_frame_i1 * 4 + channel_pixel_i;
                dst[dst_idx] = (is_dummy_chnn ? zero3 :
                                (is_dummy_edge & (channel_pixel_i == 0)) ? zero0 :
                                (is_dummy_edge & (channel_pixel_i == 1)) ? zero1 :
                                (is_dummy_edge & (channel_pixel_i == 2)) ? zero2 :
                                src[src_idx]);
                bst_osum += dst[dst_idx];
            }
        }
    }
    return 0;
}

void bst_nv12_to_nv12(uint8_t *src_y, uint8_t *src_c, uint32_t src_line_stride,
                      uint8_t *dst_y, uint8_t *dst_c, uint32_t dst_line_stride,
                      uint32_t width, uint32_t height)
{
    int src_y_offset = 0;
    int src_c_offset = 0;
    int dst_y_offset = 0;
    int dst_c_offset = 0;
    for (int i = 0; i < height / 2; i++) {
        //odd/even line
        for (int l = 0; l < 2; l++) {
            memcpy(dst_y + dst_y_offset , src_y + src_y_offset, width);
            dst_y_offset += dst_line_stride;
            src_y_offset += src_line_stride;
        }

        memcpy(dst_c + dst_c_offset , src_c + src_c_offset, width);
        dst_c_offset += dst_line_stride;
        src_c_offset += src_line_stride;
    }
}

void bst_bgr_to_bgr(uint8_t *src, uint32_t src_line_stride,
                    uint8_t *dst, uint32_t dst_line_stride,
                    uint32_t width, uint32_t height, uint32_t nv2bgr_order, uint8_t nv2bgr_alpha)
{
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int k = 0; k < 4; k++) {
                int src_idx = y * src_line_stride + x * 4 + k;
                int dst_idx = y * dst_line_stride + x * 4 + k;
                if (nv2bgr_order & 0x8) {
                    //src_idx = y * src_line_stride + x * 4 + k;
                    if (k == 0) {
                        dst[dst_idx] = nv2bgr_alpha;
                    } else {
                        dst[dst_idx] = src[src_idx];
                    }
                } else {
                    if (k == 3) {
                        dst[dst_idx] = nv2bgr_alpha;
                    } else {
                        dst[dst_idx] = src[src_idx];
                    }
                }

            }
        }
    }
}

static void bst_nv12_isum(uint8_t* y_base, uint8_t *c_base,
                          int w, int h, uint32_t stride)
{
    int x, y;
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            bst_isum += y_base[y * stride + x];
        }
    }
    for (y = 0; y < h/2; y++) {
        for (x = 0; x < w; x++) {
            bst_isum += c_base[y * stride + x];
        }
    }
}

static void bst_bgr_isum(uint8_t* base, int w, int h,
                         uint32_t stride)
{
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            for (int k = 0; k < 4; k++)
                bst_isum += base[y * stride + x * 4 + k];
        }
    }
}

static void bst_nv12_osum(uint8_t* y_base, uint8_t *c_base,
                          int w, int h, uint32_t stride)
{
    int x, y;
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            bst_osum += y_base[y * stride + x];
        }
    }
    for (y = 0; y < h/2; y++) {
        for (x = 0; x < w; x++) {
            bst_osum += c_base[y * stride + x];
        }
    }
}

static void bst_bgr_osum(uint8_t* base, int w, int h,
                         uint32_t stride)
{
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            for (int k = 0; k < 4; k++)
                bst_osum += base[y * stride + x * 4 + k];
        }
    }
}

bool bst_format_is_bgr(bst_hw_data_format_e format)
{
    if ((format & 0x3) == 0x1) {
        return true;
    } else {
        return false;
    }

}

void bst_cfg_check(bst_hw_once_cfg_s *cfg)
{
    if (cfg->dst_format == 3) {
        if ((cfg->kernel_xstride == 0) || (cfg->kernel_ystride == 0)) {
            fprintf(stderr, "[Error] : kernel_xstride = %0d, kernel_ystride = %0d\n", cfg->kernel_xstride, cfg->kernel_ystride);
        }
        if ((cfg->pad_left > 8) || (cfg->pad_right > 8) ||
            (cfg->pad_top > 8) || (cfg->pad_bottom > 8)) {
            fprintf(stderr, "[Error] : pad_left = %d, pad_right = %d, pad_top = %d, pad_bottom = %d\n",
                    cfg->pad_left, cfg->pad_right, cfg->pad_top, cfg->pad_bottom);
        }
        if ((cfg->kernel_size == 0) && (cfg->src_w > 4096) ||
            (cfg->kernel_size == 1) && (cfg->src_w > 2048) ||
            (cfg->kernel_size == 2) && (cfg->src_w > 1024) ||
            (cfg->kernel_size == 3) && (cfg->src_w > 1024)) {
            fprintf(stderr, "[Error] : kernel_size = %0d, width = %0d is not support\n", cfg->kernel_size, cfg->src_w);
        }
    }
}

void bst_mdl(bst_hw_once_cfg_s *cfg)
{
    bst_cfg_check(cfg);

    uint8_t order = (cfg->dst_format >> 4) & 0xF;
    // caculate isum
    if (cfg->src_format == BST_HW_DATA_FM_NV12) {
        bst_nv12_isum(cfg->src_base0, cfg->src_base1, cfg->src_w, cfg->src_h,
                      cfg->src_line_stride);
    } else if (bst_format_is_bgr(cfg->src_format)) {
        bst_bgr_isum(cfg->src_base0, cfg->src_w, cfg->src_h,
                     cfg->src_line_stride);
    } else {
        assert(0);
    }

    // select
    if ((cfg->src_format == BST_HW_DATA_FM_NV12) &&
        (cfg->dst_format == BST_HW_DATA_FM_NV12)) {
        bst_nv12_to_nv12(cfg->src_base0, cfg->src_base1, cfg->src_line_stride,
                         cfg->dst_base0, cfg->dst_base1, cfg->dst_line_stride,
                         cfg->src_w, cfg->src_h);
    } else if (bst_format_is_bgr(cfg->src_format) &&
               bst_format_is_bgr(cfg->dst_format)) {
        bst_bgr_to_bgr(cfg->src_base0, cfg->src_line_stride,
                       cfg->dst_base0, cfg->dst_line_stride,
                       cfg->src_w, cfg->src_h, order, cfg->nv2bgr_alpha);
    } else if ((cfg->src_format == BST_HW_DATA_FM_NV12) &&
               bst_format_is_bgr(cfg->dst_format)) {
        bst_nv2bgr(cfg->src_base0, cfg->src_base1, cfg->src_line_stride,
                   cfg->dst_base0, cfg->dst_line_stride,
                   cfg->src_w, cfg->src_h, order,
                   cfg->nv2bgr_coef, cfg->nv2bgr_ofst, cfg->nv2bgr_alpha);
    } else if ((cfg->dst_format == BST_HW_DATA_FM_VBGR) ||
               (cfg->dst_format == BST_HW_DATA_FM_VRGB)) {// virchn
        if (bst_format_is_bgr(cfg->src_format)) {
            normal2kernel(cfg->src_w, cfg->src_h, cfg->kernel_size,
                          cfg->kernel_xstride, cfg->kernel_ystride,
                          cfg->zero_point,
                          cfg->pad_left,
                          cfg->pad_right,
                          cfg->pad_top,
                          cfg->pad_bottom,
                          cfg->src_base0,
                          cfg->src_line_stride,
                          cfg->dst_base0,
                          cfg->dst_line_stride,
                          cfg->dst_plane_stride);
        } else { //nv12->kernel
            uint8_t *tmp = (uint8_t *)malloc(cfg->src_h * cfg->src_w * 4);
            if (tmp == NULL)
                fprintf(stderr, "error: malloc failed\n");
            bst_nv2bgr(cfg->src_base0, cfg->src_base1, cfg->src_line_stride,
                       tmp, cfg->src_w * 4, cfg->src_w, cfg->src_h,
                       order, cfg->nv2bgr_coef, cfg->nv2bgr_ofst,
                       cfg->nv2bgr_alpha);

            normal2kernel(cfg->src_w, cfg->src_h, cfg->kernel_size,
                          cfg->kernel_xstride, cfg->kernel_ystride,
                          cfg->zero_point,
                          cfg->pad_left,
                          cfg->pad_right,
                          cfg->pad_top,
                          cfg->pad_bottom,
                          tmp, //normal_val,
                          cfg->src_w * sizeof(uint32_t),
                          cfg->dst_base0,
                          cfg->dst_line_stride,
                          cfg->dst_plane_stride);
            free(tmp);
        }
    }

    // caculate osum
    if (cfg->dst_format == BST_HW_DATA_FM_NV12) {
        bst_nv12_osum(cfg->dst_base0, cfg->dst_base1,
                      cfg->dst_w, cfg->dst_h, cfg->dst_line_stride);
    } else if (cfg->dst_format & 0x1) { //bgr sets
        bst_bgr_osum(cfg->dst_base0, cfg->dst_w, cfg->dst_h,
                     cfg->dst_line_stride);
    } else {
        //fixme virchn
    }
}

void nv12_to_rgb(bs_box_s *src, bs_box_s *dst,
                 uint32_t rgb_order,
                 const uint32_t *coef,
                 const uint8_t *offset,
                 const uint8_t nv2bgr_alpha)
{
    assert(src->stride0 == src->stride1);
    int x, y;
    for (y = 0; y < src->sbox_h; y += 2) {
        for (x = 0; x < src->sbox_w; x += 2) {
            uint8_t pix_y0 = src->base0[src->stride0 * y + x];
            uint8_t pix_y1 = src->base0[src->stride0 * y + x + 1];
            uint8_t pix_y2 = src->base0[src->stride0 * (y + 1) + x];
            uint8_t pix_y3 = src->base0[src->stride0 * (y + 1) + (x + 1)];
            uint8_t pix_u  = src->base1[src->stride1 * y/2 + x];
            uint8_t pix_v  = src->base1[src->stride1 * y/2 + x + 1];
            uint8_t *dst_ptr = dst->base0 + dst->stride0 * y + x * 4;
            yuv2rgb(pix_y0, pix_u, pix_v, coef, offset, nv2bgr_alpha,
                    rgb_order, dst_ptr);
            yuv2rgb(pix_y1, pix_u, pix_v, coef, offset, nv2bgr_alpha,
                    rgb_order, dst_ptr + 4);
            yuv2rgb(pix_y2, pix_u, pix_v, coef, offset, nv2bgr_alpha,
                    rgb_order, dst_ptr + dst->stride0);
            yuv2rgb(pix_y3, pix_u, pix_v, coef, offset, nv2bgr_alpha,
                    rgb_order, dst_ptr + dst->stride0 + 4);
        }
    }
}

static void bs_resize_chn_base(bs_box_s *src, bs_box_s *dst,
                               uint8_t bpp,
                               int32_t *coef,
                               uint8_t zero_point,
                               uint32_t is_abgr_order,
                               const uint8_t nv2bgr_alpha)
{
    printf("%s\n", __func__);
    int dy_last = dst->sbox_h + dst->sbox_y;
    int dx_last = dst->sbox_w + dst->sbox_x;
    uint16_t dx, dy, k;
    for (dy = dst->sbox_y; dy < dy_last; dy++) {
        for (dx = dst->sbox_x; dx < dx_last; dx++) {
            int32_t sx = dx * coef[0] + coef[2];
            int32_t sy = dy * coef[1] + coef[3];
            int16_t sx_p = (sx >> MAT_ACC);
            int16_t sy_p = (sy >> MAT_ACC);
            uint16_t sx_w = sx & ((1 << MAT_ACC) - 1);
            uint16_t sy_w = sy & ((1 << MAT_ACC) - 1);

            bool out_bound[4];
            bilinear_bound_check(src->sbox_x, src->sbox_y, src->sbox_w, src->sbox_h,
                                 sx_p, sy_p, out_bound);

            uint32_t p00_idx = ((sy_p - src->sbox_y) * src->stride0 +
                                (sx_p - src->sbox_x) * bpp);
            uint32_t p01_idx = p00_idx + bpp;
            uint32_t p10_idx = p00_idx + src->stride0;
            uint32_t p11_idx = p10_idx + bpp;
            // printf("index:0x%x,0x%x,0x%x,0x%x\n",p00_idx,p01_idx,p10_idx,p11_idx);

            int dst_idx = (dy - dst->sbox_y) * dst->stride0 +
                (dx - dst->sbox_x) * bpp;
            for (k = 0; k < bpp; k++) {
                uint8_t val = 0;
                uint8_t p00 = out_bound[0] ? zero_point : src->base0[p00_idx + k];
                uint8_t p01 = out_bound[1] ? zero_point : src->base0[p01_idx + k];
                uint8_t p10 = out_bound[2] ? zero_point : src->base0[p10_idx + k];
                uint8_t p11 = out_bound[3] ? zero_point : src->base0[p11_idx + k];
                if ((bpp == 4) | (bpp == 32)) {
                    val = bilinear_u8(p00, p01, p10, p11, sx_w, sy_w);
                } else if (bpp == 8) {//2bit
                    val = bilinear_u2(p00, p01, p10, p11, sx_w, sy_w);
                } else if (bpp == 16) {//4bit
                    val = bilinear_u4(p00, p01, p10, p11, sx_w, sy_w);
                }
                if((is_abgr_order==0) & (bpp == 4) & (k==3)){
                    dst->base0[dst_idx + k] = nv2bgr_alpha;
                }else if((is_abgr_order==1) & (bpp == 4) & (k==0)){
                    dst->base0[dst_idx + k] = nv2bgr_alpha;
                }else{
                    dst->base0[dst_idx + k] = val;
                }
                /*if(dy == 0x100){
                  printf("dut1(0x%x,0x%x),src(0x%x,0x%x) weight:0x%x, 0x%x, 0x%x,0x%x,0x%x,0x%x, -- 0x%x,0x%x,0x%x,0x%x -- 0x%x\n",
                  dx, dy, sx_p, sy_p, sx_w, sy_w, out_bound[0], out_bound[1], out_bound[2], out_bound[3], p00, p01, p10, p11, val);
                  }*/
            }
        }
    }
}

static void bs_resize_line_chn_base(bs_box_s *src, bs_box_s *dst,
                                    uint8_t bpp, int32_t *coef,
                                    uint16_t wbox_w, uint16_t wbox_h,
                                    uint8_t zero_point,
                                    uint32_t is_abgr_order, const uint8_t nv2bgr_alpha)
{
    assert(src->base0 != NULL);
    assert(dst->base0 != NULL);
    int src_sbox_x = 0;
    int src_sbox_y = 0;
    uint16_t src_sbox_x_last = src->sbox_w + src_sbox_x - 1;
    uint16_t src_sbox_y_last = src->sbox_h + src_sbox_y - 1;
    int dy_last = dst->sbox_h + dst->sbox_y;
    int dx_last = dst->sbox_w + dst->sbox_x;
    uint16_t dx, dy, k;

    for (dy = dst->sbox_y; dy < dy_last; dy++) {
        for (dx = dst->sbox_x; dx < dx_last; dx++) {
            int32_t sx_init = dx * coef[0] + coef[2];
            int32_t sy_init = dy * coef[1] + coef[3];
            int32_t sx = sx_init;
            int32_t sy = sy_init;
            int16_t sx_p = (sx >> MAT_ACC);
            int16_t sy_p = (sy >> MAT_ACC);
            uint16_t sx_w = sx & ((1 << MAT_ACC) - 1);
            uint16_t sy_w = sy & ((1 << MAT_ACC) - 1);

            int16_t sx_p0 = sx_p;
            int16_t sy_p0 = sy_p;
            int16_t sx_p1 = sx_p0 + 1;
            int16_t sy_p1 = sy_p0 + 1;

            bool out_bound[4] = {false};
            if ((sx_p0 < src_sbox_x) || (sx_p0 > src_sbox_x_last) ||
                (sx_p0 + src->sbox_x > (wbox_w - 1)) ||
                (sx_p0 + src->sbox_x < 0) ||
                (sy_p0 < src_sbox_y) || (sy_p0 > src_sbox_y_last) ||
                (sy_p0 + src->sbox_y > (wbox_h - 1)) ||
                (sy_p0 + src->sbox_y < 0)) {//fixme
                out_bound[0] = true;
            }

            if ((sx_p1 < src_sbox_x) || (sx_p1 > src_sbox_x_last) ||
                (sx_p1 + src->sbox_x > (wbox_w - 1)) ||
                (sx_p1 + src->sbox_x < 0) ||
                (sy_p0 < src_sbox_y) || (sy_p0 > src_sbox_y_last) ||
                (sy_p0 + src->sbox_y > (wbox_h - 1)) ||
                (sy_p0 + src->sbox_y < 0)) {
                out_bound[1] = true;
            }

            if ((sx_p0 < src_sbox_x) || (sx_p0 > src_sbox_x_last) ||
                (sx_p0 + src->sbox_x > (wbox_w - 1)) ||
                (sx_p0 + src->sbox_x < 0) ||
                (sy_p1 < src_sbox_y) || (sy_p1 > src_sbox_y_last) ||
                (sy_p1 + src->sbox_y > (wbox_h - 1)) ||
                (sy_p1 + src->sbox_y < 0)) {
                out_bound[2] = true;
            }

            if ((sx_p1 < src_sbox_x) || (sx_p1 > src_sbox_x_last) ||
                (sx_p1 + src->sbox_x > (wbox_w - 1)) ||
                (sx_p1 + src->sbox_x < 0) ||
                (sy_p1 < src_sbox_y) || (sy_p1 > src_sbox_y_last) ||
                (sy_p1 + src->sbox_y > (wbox_h - 1)) ||
                (sy_p1 + src->sbox_y < 0)) {
                out_bound[3] = true;
            }

            uint32_t p00_idx = ((sy_p + src->sbox_y) * src->stride0 +
                                (sx_p + src->sbox_x) * bpp);
            uint32_t p01_idx = p00_idx + bpp;
            uint32_t p10_idx = p00_idx + src->stride0;
            uint32_t p11_idx = p10_idx + bpp;
            int dst_idx = (dy - dst->sbox_y) * dst->stride0 +
                (dx - dst->sbox_x) * bpp;
            for (k = 0; k < bpp; k++) {
                uint8_t val = 0;
                uint8_t p00 = out_bound[0] ? zero_point : src->base0[p00_idx + k];
                uint8_t p01 = out_bound[1] ? zero_point : src->base0[p01_idx + k];
                uint8_t p10 = out_bound[2] ? zero_point : src->base0[p10_idx + k];
                uint8_t p11 = out_bound[3] ? zero_point : src->base0[p11_idx + k];
                if ((bpp == 4) | (bpp == 32)) {
                    val = bilinear_u8(p00, p01, p10, p11, sx_w, sy_w);
                } else if (bpp == 8) {//2bit
                    val = bilinear_u2(p00, p01, p10, p11, sx_w, sy_w);
                } else if (bpp == 16) {//4bit
                    val = bilinear_u4(p00, p01, p10, p11, sx_w, sy_w);
                }
                if((is_abgr_order==0) & (bpp == 4) & (k==3)){
                    dst->base0[dst_idx + k] = nv2bgr_alpha;
                }else if((is_abgr_order==1) & (bpp == 4) & (k==0)){
                    dst->base0[dst_idx + k] = nv2bgr_alpha;
                }else{
                    dst->base0[dst_idx + k] = val;
                }
                /*if(dy==32 & dx==0){
                  printf("src(0x%x,0x%x,0x%x,0x%x),coef:(0x%x,0x%x,0x%x,0x%x)dut1(0x%x,0x%x),src(0x%x,0x%x,0x%x,0x%x) weight:0x%x, 0x%x, 0x%x,0x%x,0x%x,0x%x, -- 0x%x,0x%x,0x%x,0x%x -- 0x%x\n",
                  src->sbox_x,src->sbox_y, src->sbox_w, src->sbox_h, coef[0], coef[1], coef[2], coef[3],dx, dy, sx_p, sy_p, sx, sy, sx_w, sy_w, out_bound[0], out_bound[1], out_bound[2], out_bound[3], p00, p01, p10, p11, val);
                  }*/
            }
        }
    }
}

static void bs_amplify_height(bs_box_s *src, bs_box_s *dst,
                              uint32_t y_gain_exp, uint8_t bpp,
                              uint8_t zero_point)
{
    uint16_t src_sbox_y_last = src->sbox_h + src->sbox_y - 1;
    uint16_t dx, dy, k;
    uint16_t dx_last = dst->sbox_w + dst->sbox_x;
    uint16_t dy_last = dst->sbox_h + dst->sbox_y;

    for (dy = dst->sbox_y; dy < dy_last; dy++) {
        for (dx = dst->sbox_x; dx < dx_last; dx++) {
            int16_t sx_p  = dx;
            int16_t sy_p  = dy >> y_gain_exp;
            uint16_t sy_w = dy << (MAT_ACC - y_gain_exp);
            int16_t sy_p0 = sy_p;
            int16_t sy_p1 = sy_p0 + 1;
            bool p00_over = false;
            bool p10_over = false;
            if ((sy_p0 < src->sbox_y) || (sy_p0 > src_sbox_y_last)) {
                p00_over = true;
            }
            if ((sy_p1 < src->sbox_y) || (sy_p1 > src_sbox_y_last)) {
                p10_over = true;
            }

            uint32_t p00_idx = ((sy_p - src->sbox_y) * src->stride0 +
                                (sx_p - src->sbox_x) * bpp);
            uint32_t p10_idx = p00_idx + src->stride0;

            int dst_idx = (dy - dst->sbox_y) * dst->stride0 +
                (dx - dst->sbox_x) * bpp;
            for (k = 0; k < bpp; k++) {
                uint8_t val = 0;
                uint8_t p00 = p00_over ? zero_point : src->base0[p00_idx + k];
                uint8_t p10 = p10_over ? zero_point : src->base0[p10_idx + k];
                if ((bpp == 4) || (bpp == 32)) {
                    val = bilinear_u8(p00, 0, p10, 0, 0, sy_w);
                } else if (bpp == 8) {//2bit
                    val = bilinear_u2(p00, 0, p10, 0, 0, sy_w);
                } else if (bpp == 16) {//4bit
                    val = bilinear_u4(p00, 0, p10, 0, 0, sy_w);
                }
                dst->base0[dst_idx + k] = val;
            }
        }
    }
}

static void bs_amplify_height_line(bs_box_s *src, bs_box_s *dst,
                                   uint32_t y_gain_exp, uint8_t bpp,
                                   uint8_t zero_point)
{
    uint16_t src_sbox_y_last = src->sbox_h + src->sbox_y - 1;
    int src_sbox_y = 0;
    uint16_t dx, dy, k;
    for (dy = 0; dy < dst->sbox_h; dy++) {
        for (dx = 0; dx < dst->sbox_w; dx++) {
            int16_t sx_p  = dx;
            int16_t sy_p  = dy >> y_gain_exp;
            //uint16_t sy_w = (dy * 32768 >> y_gain_exp) & 0x7FFF;
            uint16_t sy_w = dy << (MAT_ACC - y_gain_exp);
            int16_t sy_p0 = sy_p;
            int16_t sy_p1 = sy_p0 + 1;
            bool p00_over = false;
            bool p10_over = false;
            if ((sy_p0 < src_sbox_y) || (sy_p0 > src_sbox_y_last)) {
                p00_over = true;
            }
            if ((sy_p1 < src_sbox_y) || (sy_p1 > src_sbox_y_last)) {
                p10_over = true;
            }

            uint32_t p00_idx = ((sy_p - src->sbox_y) * src->stride0 +
                                (sx_p - src->sbox_x) * bpp);
            uint32_t p10_idx = p00_idx + src->stride0;

            int dst_idx = dy * dst->stride0 + dx * bpp;
            for (k = 0; k < bpp; k++) {
                uint8_t val = 0;
                uint8_t p00 = p00_over ? zero_point : src->base0[p00_idx + k];
                uint8_t p10 = p10_over ? zero_point : src->base0[p10_idx + k];
                if ((bpp == 4) || (bpp == 32)) {
                    val = bilinear_u8(p00, 0, p10, 0, 0, sy_w);
                } else if (bpp == 8) {//2bit
                    val = bilinear_u2(p00, 0, p10, 0, 0, sy_w);
                } else if (bpp == 16) {//4bit
                    val = bilinear_u4(p00, 0, p10, 0, 0, sy_w);
                }
                dst->base0[dst_idx + k] = val;
            }
        }
    }
}

static void bs_resize_box_chn(bs_box_s *src, bs_box_s *dst,
                              int32_t *coef, uint32_t bpp,
                              uint32_t y_gain_exp, uint8_t zero_point,
                              uint32_t is_abgr_order, const uint8_t nv2bgr_alpha)
{
    if (y_gain_exp == 0) {
        bs_resize_chn_base(src, dst, bpp, coef, zero_point, is_abgr_order, nv2bgr_alpha);
    } else {
        uint32_t y_gain = 1 << y_gain_exp;
        uint32_t amplify_buf_size = src->sbox_w * bpp * src->sbox_h * y_gain;
        uint8_t *amplify_buf = (uint8_t *)malloc(amplify_buf_size);
        if (amplify_buf == NULL) {
            fprintf(stderr, " [Error] : alloc space failed!\n");
        }
        memset(amplify_buf, 0, amplify_buf_size);
        bs_box_s tmp = {amplify_buf, NULL, src->sbox_w * bpp, src->sbox_w * bpp,
                        src->sbox_x, src->sbox_y * y_gain,
                        src->sbox_w, src->sbox_h * y_gain,
                        src->format, src->chn};
        bs_amplify_height(src, &tmp, y_gain_exp, bpp, zero_point);
        bs_resize_chn_base(&tmp, dst, bpp, coef, zero_point, is_abgr_order, nv2bgr_alpha);
        free(amplify_buf);
    }
}

void bs_resize_box(bs_box_s *src, bs_box_s *dst,
                   int32_t *scale_coef,
                   const uint32_t *nv2bgr_coef,
                   const uint8_t *nv2bgr_ofst,
                   const uint8_t nv2bgr_alpha,
                   uint32_t nv2bgr_order,
                   uint32_t bpp,
                   uint32_t y_gain_exp,
                   uint8_t zero_point)
{
    bs_box_s *rgb_src = src;
    bs_box_s nv12_to_rgb_src = *src;
    uint8_t *rgb_buffer = NULL;
    uint32_t is_abgr_order = 0;
    is_abgr_order = (nv2bgr_order & 0x8)>>3;//1--A[7:0]
    if (src->format == 0) { //src_nv12
        uint32_t rgb_buffer_size = src->sbox_w * src->sbox_h * 4;
        rgb_buffer = (uint8_t *)malloc(rgb_buffer_size);
        if (rgb_buffer == NULL) {
            fprintf(stderr,"error:malloc bgr_one_base is failed!\n");
        }
        nv12_to_rgb_src.base0 = rgb_buffer;
        nv12_to_rgb_src.base1 = NULL;
        nv12_to_rgb_src.stride0 = src->sbox_w * 4;
        nv12_to_rgb_src.stride1 = 0;
        nv12_to_rgb_src.format = 1;//fixme
        nv12_to_rgb_src.chn = 4;//fixme

        nv12_to_rgb(src, &nv12_to_rgb_src, nv2bgr_order, nv2bgr_coef, nv2bgr_ofst, nv2bgr_alpha);
        rgb_src = &nv12_to_rgb_src;
    }
    bs_resize_box_chn(rgb_src, dst, scale_coef, bpp, y_gain_exp,
                      zero_point, is_abgr_order, nv2bgr_alpha);
    free(rgb_buffer);
}

void bs_resize_line(bs_box_s *src, bs_box_s *dst,
                    const uint32_t *nv2bgr_coef,
                    const uint8_t *nv2bgr_ofst,
                    const uint8_t nv2bgr_alpha,
                    uint32_t nv2bgr_order,
                    uint32_t bpp,
                    uint32_t y_gain_exp,
                    uint32_t *frmc_box_base,
                    uint32_t frmc_box_num,
                    uint8_t **dst_box_base,
                    uint8_t zero_point)
{
    bs_box_s *final_src = src;
    bs_box_s amplify;
    bs_box_s nv12_to_rgb_src;
    uint8_t *rgb_buffer = NULL;
    uint8_t *amplify_buf = NULL;
    uint32_t is_abgr_order = 0;
    is_abgr_order = (nv2bgr_order & 0x8)>>3;//1--A[7:0]

    // convert nv12 to bgr
    if (src->format == 0) { //src_nv12
        uint32_t rgb_buffer_size = src->sbox_w * src->sbox_h * 4;
        rgb_buffer = (uint8_t *)malloc(rgb_buffer_size);
        if (rgb_buffer == NULL) {
            fprintf(stderr,"error:malloc bgr_one_base is failed!\n");
        }
        nv12_to_rgb_src.base0 = rgb_buffer;
        nv12_to_rgb_src.base1 = NULL;
        nv12_to_rgb_src.stride0 = src->sbox_w * 4;
        nv12_to_rgb_src.stride1 = 0;
        nv12_to_rgb_src.sbox_x = 0;
        nv12_to_rgb_src.sbox_y = 0;
        nv12_to_rgb_src.sbox_w = src->sbox_w;
        nv12_to_rgb_src.sbox_h = src->sbox_h;
        nv12_to_rgb_src.format = 1;//fixme
        nv12_to_rgb_src.chn = 4;//fixme

        nv12_to_rgb(src, &nv12_to_rgb_src, nv2bgr_order, nv2bgr_coef, nv2bgr_ofst, nv2bgr_alpha);
        final_src = &nv12_to_rgb_src;
    }

    // amplify src
    if (y_gain_exp > 0) {
        uint32_t y_gain = 1 << y_gain_exp;
        uint32_t amplify_buf_size = src->sbox_w * bpp * src->sbox_h * y_gain;
        amplify_buf = (uint8_t *)malloc(amplify_buf_size);
        if (amplify_buf == NULL) {
            fprintf(stderr, " [Error] : alloc space failed!\n");
        }
        memset(amplify_buf, 0, amplify_buf_size);
        amplify.base0 = amplify_buf;
        amplify.base1 = NULL;
        amplify.stride0 = src->sbox_w * bpp;
        amplify.stride1 = src->sbox_w * bpp;
        amplify.sbox_x = 0;
        amplify.sbox_y = 0;
        amplify.sbox_w = src->sbox_w;
        amplify.sbox_h = src->sbox_h * y_gain;
        amplify.format = src->format;
        amplify.chn = src->chn;
        bs_amplify_height_line(final_src, &amplify, y_gain_exp, bpp, zero_point);
        final_src = &amplify;
    }

    int idx;
    bs_box_s cur_dst = *dst;
    for (idx = 0; idx < frmc_box_num; idx++) {
        uint16_t box_x = frmc_box_base[idx * 6 + 0] & 0xFFFF;
        // uint16_t box_y = frmc_box_base[idx * 6 + 0] >> (16 - y_gain_exp); // amplify
        uint16_t box_y_t = frmc_box_base[idx * 6 + 0] >> 16;
        uint16_t box_y = box_y_t << y_gain_exp;
        uint16_t box_w = frmc_box_base[idx * 6 + 1] & 0xFFFF;
        // uint16_t box_h = frmc_box_base[idx * 6 + 1] >> (16 - y_gain_exp); // amplify
        uint16_t box_h_t = frmc_box_base[idx * 6 + 1] >> 16;
        uint16_t box_h = box_h_t << y_gain_exp;
        int32_t *scale_coef = (int32_t *)(&frmc_box_base[idx * 6 + 2]);
        uint8_t *box_base = final_src->base0;
        bs_box_s cur_src = {box_base, NULL, final_src->stride0, 0,
                            box_x, box_y, box_w, box_h, final_src->format, final_src->chn};
        cur_dst.base0 = (uint8_t *)(dst_box_base[idx]);
        bs_resize_line_chn_base(&cur_src, &cur_dst, bpp, scale_coef, final_src->sbox_w, final_src->sbox_h, zero_point, is_abgr_order, nv2bgr_alpha);
    }
    free(amplify_buf);
    free(rgb_buffer);
}

inline static void affine_dst2src(const int32_t *matrix,
                                  const int32_t dx, const int32_t dy,
                                  int16_t *x_p, int16_t *y_p,
                                  uint16_t *x_w, uint16_t *y_w)
{
    int64_t src_x = (int64_t)matrix[MSCALEX] * dx +
        (int64_t)matrix[MSKEWX] * dy + matrix[MTRANSX];
    int64_t src_y = (int64_t)matrix[MSKEWY] * dx +
        (int64_t)matrix[MSCALEY] * dy + matrix[MTRANSY];

    int32_t s32_max = 2147483647;
    int32_t s32_min = -2147483648;
    int32_t s32_sx = CLIP(src_x, s32_min, s32_max);
    int32_t s32_sy = CLIP(src_y, s32_min, s32_max);

    *x_p = s32_sx >> MAT_ACC;
    *y_p = s32_sy >> MAT_ACC;
    *x_w = s32_sx & ((1<<MAT_ACC) - 1);
    *y_w = s32_sy & ((1<<MAT_ACC) - 1);
}

/**
 * Only support RGB, RGB -> RGB, bpp is always 4
 */
static void bs_affine_box_rgb(bs_box_s *src, bs_box_s *dst,
                              int32_t *matrix, uint8_t zero_point,
                              uint32_t is_abgr_order, const uint8_t nv2bgr_alpha)
{
    const uint8_t bpp = 4;
    uint16_t dx_last = dst->sbox_w + dst->sbox_x;
    uint16_t dy_last = dst->sbox_h + dst->sbox_y;
    int16_t sx_p, sy_p;
    uint16_t sx_w, sy_w;
    uint16_t dx, dy, k;
    for (dx = dst->sbox_x; dx < dx_last; dx++) {//fixme
        for (dy = dst->sbox_y; dy < dy_last; dy++) {//fixme
            affine_dst2src(matrix, dx, dy, &sx_p, &sy_p, &sx_w, &sy_w);
            bool out_bound[4];
            bilinear_bound_check(src->sbox_x, src->sbox_y, src->sbox_w, src->sbox_h,
                                 sx_p, sy_p, out_bound);
            uint32_t p00_idx = ((sy_p - src->sbox_y) * src->stride0 +
                                (sx_p - src->sbox_x) * bpp);
            uint32_t p01_idx = p00_idx + bpp;
            uint32_t p10_idx = p00_idx + src->stride0;
            uint32_t p11_idx = p10_idx + bpp;
            for ( k = 0; k < bpp; k++) {
                uint8_t p00 = out_bound[0] ? zero_point : src->base0[p00_idx + k];
                uint8_t p01 = out_bound[1] ? zero_point : src->base0[p01_idx + k];
                uint8_t p10 = out_bound[2] ? zero_point : src->base0[p10_idx + k];
                uint8_t p11 = out_bound[3] ? zero_point : src->base0[p11_idx + k];
                uint8_t val = bilinear_u8(p00, p01, p10, p11, sx_w, sy_w);
                int dst_idx = (dy - dst->sbox_y) * dst->stride0 +
                    (dx - dst->sbox_x) * bpp + k;

                if ((is_abgr_order==0) & (k==3)) {
                    dst->base0[dst_idx] = nv2bgr_alpha;
                } else if ((is_abgr_order==1) & (k==0)) {
                    dst->base0[dst_idx] = nv2bgr_alpha;
                } else {
                    dst->base0[dst_idx] = val;
                }
            }
        }
    }
}

/**
 * Only support NV12, YUV444 -> NV12
 */
static void bs_affine_box_nv12(bs_box_s *src, bs_box_s *dst,
                               int32_t *matrix, const uint8_t *offset)
{
    bool is_perspective = !((matrix[6] == 0) &&
                            (matrix[7] == 0) &&
                            (matrix[8] == (1 << MAT_ACC)));

    uint16_t dx, dy;
    uint16_t dx_last = dst->sbox_w + dst->sbox_x;
    uint16_t dy_last = dst->sbox_h + dst->sbox_y;
    int16_t sx_p, sy_p;
    uint16_t sx_w, sy_w;
    for (dy = dst->sbox_y; dy < dy_last; dy++) {
        for (dx = dst->sbox_x; dx < dx_last; dx++) {
            if (is_perspective) {
                perspective_dst2src(matrix, dx, dy, &sx_p, &sy_p, &sx_w, &sy_w);
            } else {
                affine_dst2src(matrix, dx, dy, &sx_p, &sy_p, &sx_w, &sy_w);
            }
            bool out_bound[4];
            bilinear_bound_check(src->sbox_x, src->sbox_y, src->sbox_w, src->sbox_h,
                                 sx_p, sy_p, out_bound);
            uint32_t p00_idx = ((sy_p - src->sbox_y) * src->stride0 +
                                (sx_p - src->sbox_x) * 1);
            uint32_t p01_idx = p00_idx + 1;
            uint32_t p10_idx = p00_idx + src->stride0;
            uint32_t p11_idx = p10_idx + 1;

            uint8_t p00 = out_bound[0] ? offset[0] : src->base0[p00_idx];
            uint8_t p01 = out_bound[1] ? offset[0] : src->base0[p01_idx];
            uint8_t p10 = out_bound[2] ? offset[0] : src->base0[p10_idx];
            uint8_t p11 = out_bound[3] ? offset[0] : src->base0[p11_idx];
            //y
            uint8_t y_val = bilinear_u8(p00, p01, p10, p11, sx_w, sy_w);
            int dst_y_idx = (dy - dst->sbox_y) * dst->stride0 + (dx - dst->sbox_x);
            dst->base0[dst_y_idx] = y_val;

            //uv
            p00_idx = ((sy_p - src->sbox_y) * src->stride1 +
                       (sx_p - src->sbox_x) * 2);
            p01_idx = p00_idx + 1 * 2;
            p10_idx = p00_idx + src->stride1;
            p11_idx = p10_idx + 1 * 2;

            p00 = out_bound[0] ? offset[1] : src->base1[p00_idx];
            p01 = out_bound[1] ? offset[1] : src->base1[p01_idx];
            p10 = out_bound[2] ? offset[1] : src->base1[p10_idx];
            p11 = out_bound[3] ? offset[1] : src->base1[p11_idx];

            uint8_t u_val = bilinear_u8(p00, p01, p10, p11, sx_w, sy_w);
            int dst_u_idx = ((dy - dst->sbox_y) / 2) * dst->stride0 +
                (dx - dst->sbox_x)/2 * 2;
            dst->base1[dst_u_idx] = u_val;

            p00 = out_bound[0] ? offset[1] : src->base1[p00_idx + 1];
            p01 = out_bound[1] ? offset[1] : src->base1[p01_idx + 1];
            p10 = out_bound[2] ? offset[1] : src->base1[p10_idx + 1];
            p11 = out_bound[3] ? offset[1] : src->base1[p11_idx + 1];
            uint8_t v_val = bilinear_u8(p00, p01, p10, p11, sx_w, sy_w);
            dst->base1[dst_u_idx + 1] = v_val;
        }
    }
}

/**
 * NV12 -> YUV444, Y....Y, UVUVUV...UV
 */
static void nv12_to_yuv444(bs_box_s *src, bs_box_s *dst)
{
    int x, y;
    for (y = 0; y < src->sbox_h; y++) {
        memcpy(dst->base0 + y * dst->stride0,
               src->base0 + y * src->stride0, src->sbox_w);
        int16_t *dst_uv_ptr = (int16_t *)(dst->base1 + y * dst->stride1);
        int16_t *src_uv_ptr = (int16_t *)(src->base1 + (y / 2) * src->stride0);
        for (x = 0; x < src->sbox_w; x++) {
            dst_uv_ptr[x] = src_uv_ptr[x/2];
        }
    }
}

static void bs_affine_box_nv12_to_nv12(bs_box_s *src, bs_box_s *dst,
                                       int32_t *matrix,
                                       const uint32_t *coef, const uint8_t *offset,
                                       uint8_t mono_x, uint8_t mono_y,
                                       int8_t *extreme_point)
{
    uint8_t *tmp_base0 = (uint8_t *)malloc(src->sbox_w * src->sbox_h * 3);
    if (tmp_base0 == NULL) {
        fprintf(stderr, "[Error] : alloc space failed!\n");
    }
    uint8_t *tmp_base1 = tmp_base0 + src->sbox_w * src->sbox_h;
    bs_box_s tmp = {tmp_base0, tmp_base1, src->sbox_w, src->sbox_w * 2,
                    src->sbox_x, src->sbox_y, src->sbox_w, src->sbox_h,
                    src->format, src->chn};
    nv12_to_yuv444(src, &tmp);

    bs_affine_box_nv12(&tmp, dst, matrix, offset);
    free(tmp_base0);
}

static void bs_affine_box_rgb_to_rgb(bs_box_s *src, bs_box_s *dst,
                                     int32_t *matrix,
                                     uint8_t zero_point,
                                     uint32_t is_abgr_order,
                                     const uint8_t nv2bgr_alpha,
                                     uint8_t mono_x, uint8_t mono_y,
                                     int8_t *extreme_point)
{
    bool is_perspective = !((matrix[6] == 0) &&
                            (matrix[7] == 0) &&
                            (matrix[8] == (1 << MAT_ACC)));

    if (is_perspective) {
#if 0
        bs_perspective_box_rgb_complex(src, dst, matrix, zero_point,
                                       is_abgr_order, nv2bgr_alpha,
                                       mono_x, mono_y, extreme_point);
#else
        bs_perspective_box_rgb_simple(src, dst, matrix, zero_point,
                                      is_abgr_order, nv2bgr_alpha);
#endif
    } else {
#if 0
        bs_affine_box_rgb_complex(src, dst, matrix, zero_point,
                                  is_abgr_order, nv2bgr_alpha);
#else
        bs_affine_box_rgb(src, dst, matrix, zero_point,
                          is_abgr_order, nv2bgr_alpha);
#endif
    }
}

static void bs_affine_box_nv12_to_rgb(bs_box_s *src, bs_box_s *dst,
                                      int32_t *matrix,
                                      const uint32_t *coef, const uint8_t *offset,
                                      const uint8_t nv2bgr_alpha,
                                      uint32_t nv2bgr_order, uint8_t zero_point,
                                      uint8_t mono_x, uint8_t mono_y,
                                      int8_t *extreme_point)
{
    uint32_t rgb_buffer_size = src->sbox_w * src->sbox_h * 4;
    uint8_t *rgb_buffer = (uint8_t *)malloc(rgb_buffer_size);
    if (rgb_buffer == NULL) {
        fprintf(stderr,"error:malloc bgr_one_base is failed!\n");
    }

    bs_box_s src_rgb = {rgb_buffer, NULL, src->sbox_w * 4, 0,
                        src->sbox_x, src->sbox_y, src->sbox_w, src->sbox_h,
                        src->format, src->chn};
    nv12_to_rgb(src, &src_rgb, nv2bgr_order, coef, offset, nv2bgr_alpha);
    bool is_perspective = !((matrix[6] == 0) &&
                            (matrix[7] == 0) &&
                            (matrix[8] == (1 << MAT_ACC)));
    uint32_t is_abgr_order = 0;
    is_abgr_order = (nv2bgr_order & 0x8)>>3;//1--A[7:0]
    if (is_perspective) {
#if 0
        bs_perspective_box_rgb_complex(&src_rgb, dst, matrix, zero_point,
                                       is_abgr_order, nv2bgr_alpha,
                                       mono_x, mono_y, extreme_point);
#else
        bs_perspective_box_rgb_simple(&src_rgb, dst, matrix, zero_point,
                                      is_abgr_order, nv2bgr_alpha);
#endif
    } else {
        bs_affine_box_rgb_complex(&src_rgb, dst, matrix, zero_point, is_abgr_order,
                                  nv2bgr_alpha);
    }
    free(rgb_buffer);
}

void bs_affine_box(bs_box_s *src, bs_box_s *dst, uint8_t bpp,
                   int32_t *matrix, const uint32_t *coef, const uint8_t *offset,
                   const uint8_t nv2bgr_alpha,
                   int32_t nv2bgr_order, uint8_t zero_point,
                   uint8_t mono_x, uint8_t mono_y, int8_t *extreme_point)
{
    if ((src->format == 0) && (dst->format == 0)) {//nv12 to nv12
        bs_affine_box_nv12_to_nv12(src, dst, matrix, coef, offset,
                                   mono_x, mono_y, extreme_point);
    } else if ((src->format == 0) && (dst->format == 1)) {//nv12 to rgb
        bs_affine_box_nv12_to_rgb(src, dst, matrix, coef, offset, nv2bgr_alpha,
                                  nv2bgr_order, zero_point,
                                  mono_x, mono_y, extreme_point);
    } else if ((src->format == 1) && (dst->format == 1)) {//rgb to rgb
        uint32_t is_abgr_order = 0;
        is_abgr_order = (nv2bgr_order & 0x8)>>3;//1--A[7:0]
        bs_affine_box_rgb_to_rgb(src, dst, matrix, zero_point, is_abgr_order, nv2bgr_alpha,
                                 mono_x, mono_y, extreme_point);
    } else {
        assert(0);
    }
}

static inline void bscaler_frmc_calc_isum(uint8_t *frmc_ybase_src,
                                          uint8_t *frmc_cbase_src,
                                          uint32_t frmc_w_src,
                                          uint32_t frmc_h_src,
                                          uint32_t frmc_ps_src,
                                          uint32_t frmc_format_src,
                                          uint32_t bpp)
{
    int x, y;
    int byte_num;
    uint32_t bpp_sel;
    if (frmc_format_src == 0) {
        bpp_sel = 1;
    } else {
        bpp_sel = bpp;
    }
    for (y = 0; y < frmc_h_src; y++) {
        for (x = 0; x < frmc_w_src; x++) {
            for (byte_num = 0; byte_num < bpp_sel; byte_num++) {
                bsc_isum += frmc_ybase_src[y * frmc_ps_src + x * bpp_sel + byte_num];
            }
        }
    }

    if (frmc_format_src == 0) {
        for (y = 0; y < frmc_h_src/2; y++) {
            for (x = 0; x < frmc_w_src; x++) {
                bsc_isum += frmc_cbase_src[y * frmc_ps_src + x * bpp_sel + byte_num];
            }
        }
    }
}

static inline void bscaler_frmc_calc_osum(uint8_t **frmc_base_dst,
                                          uint32_t box_num,
                                          // uint8_t *frmc_cbase_dst,
                                          uint32_t frmc_w_dst,
                                          uint32_t frmc_h_dst,
                                          uint32_t frmc_ps_dst,
                                          uint32_t frmc_format_dst,
                                          uint32_t bpp)
{
    uint32_t x, y, z;
    if (frmc_format_dst == 0) { //nv12
        uint8_t *frmc_ybase_dst = frmc_base_dst[0];
        uint8_t *frmc_cbase_dst = frmc_base_dst[1];
        for (y = 0; y < frmc_h_dst; y++) {
            for (x = 0; x < frmc_w_dst; x++) {
                bsc_osum += frmc_ybase_dst[y*frmc_ps_dst + x];
            }
        }

        for (y = 0; y < frmc_h_dst/2; y++) {
            for (x = 0; x < frmc_w_dst; x++) {
                bsc_osum += frmc_cbase_dst[y*frmc_ps_dst + x];
            }
        }
    } else { //bgr/chn
        int bid = 0;
        for(bid=0; bid<box_num; bid++){
            uint8_t * frmc_chn_base = frmc_base_dst[bid];
            for (y = 0; y < frmc_h_dst; y++) {
                for (x = 0; x < frmc_w_dst; x++) {
                    for (z = 0; z < bpp; z++) {
                        bsc_osum += frmc_chn_base[y * frmc_ps_dst + x * bpp + z];
                    }
                }
            }
        }
    }
}

void bsc_cfg_check(bsc_hw_once_cfg_s *cfg)
{
    assert(cfg->src_base0 != NULL);
    assert(cfg->dst_base != NULL);
    assert(cfg->dst_base[0] != NULL);

    if (cfg->src_format == BSC_HW_DATA_FM_NV12) {
        assert(cfg->src_base1 != NULL);
    }
    if (cfg->dst_format == BSC_HW_DATA_FM_NV12) {
        assert(cfg->dst_base[1] != NULL);
    }

    if (!cfg->box_mode) {//line mode
        assert(cfg->boxes_info != NULL);
        for (int i = 0; i < cfg->box_num; i++) {
            assert(cfg->dst_base[i] != NULL);
        }
    }

    if ((cfg->src_format == BSC_HW_DATA_FM_NV12) &&
        (cfg->dst_format != BSC_HW_DATA_FM_NV12)) {
        assert(cfg->coef != NULL);
        assert(cfg->offset != NULL);
    }

    //Update ...
    //TODO
}

void bsc_mdl(bsc_hw_once_cfg_s *cfg)
{
    // check cfg
    bsc_cfg_check(cfg);

    uint32_t *boxes_info = NULL;
    uint32_t src_format = cfg->src_format & 0x1;
    uint32_t dst_format = cfg->dst_format & 0x1;
    uint32_t bpp_mode = (cfg->src_format >> 5) & 0x3;
    uint32_t nv2bgr_order = (cfg->dst_format/*src_format*/ >> 1) & 0xF;
    bs_box_s src;
    src.base0 = cfg->src_base0;
    src.base1 = cfg->src_base1;
    src.stride0 = cfg->src_line_stride;
    src.stride1 = cfg->src_line_stride;
    src.sbox_x = cfg->src_box_x;
    src.sbox_y = cfg->src_box_y;
    src.sbox_w = cfg->src_box_w;
    src.sbox_h = cfg->src_box_h;
    src.format = src_format;

    bs_box_s dst;
    dst.base0 = cfg->dst_base[0];
    dst.base1 = cfg->dst_base[1];
    dst.stride0 = cfg->dst_line_stride;
    dst.stride1 = cfg->dst_line_stride;
    dst.sbox_x = cfg->dst_box_x;
    dst.sbox_y = cfg->dst_box_y;
    dst.sbox_w = cfg->dst_box_w;
    dst.sbox_h = cfg->dst_box_h;
    dst.format = dst_format;
#ifdef MDL_DEBUG
    //    printf("[MDL]Matrix:\n");
    //    printf("[%d, %d, %d]\n", cfg->matrix[0], cfg->matrix[1], cfg->matrix[2]);
    //    printf("[%d, %d, %d]\n", cfg->matrix[3], cfg->matrix[4], cfg->matrix[5]);
    //    printf("[%d, %d, %d]\n", cfg->matrix[6], cfg->matrix[7], cfg->matrix[8]);
#endif

    int32_t scale_coef[4] = {cfg->matrix[0], cfg->matrix[4],
                             cfg->matrix[2], cfg->matrix[5]};
    uint32_t bpp = 1 << (bpp_mode + 2);
    bscaler_frmc_calc_isum(cfg->src_base0, cfg->src_base1,
                           cfg->src_box_w, cfg->src_box_h,
                           cfg->src_line_stride,
                           src_format, bpp);
    if ((cfg->affine == 0) && (cfg->box_mode == 1)) { //resize box
        bs_resize_box(&src, &dst, scale_coef, cfg->coef, cfg->offset, cfg->nv2bgr_alpha,
                      nv2bgr_order, bpp, cfg->y_gain_exp,
                      cfg->zero_point);
    } else if ((cfg->affine == 0) && (cfg->box_mode == 0)) { //resize line
        bs_resize_line(&src, &dst, cfg->coef, cfg->offset, cfg->nv2bgr_alpha, nv2bgr_order,
                       bpp, cfg->y_gain_exp, cfg->boxes_info,
                       cfg->box_num, cfg->dst_base,
                       cfg->zero_point);
    } else if ((cfg->affine == 1)) { //affine box
        bs_affine_box(&src, &dst, bpp,
                      cfg->matrix, cfg->coef, cfg->offset, cfg->nv2bgr_alpha,
                      nv2bgr_order, cfg->zero_point,
                      cfg->mono_x, cfg->mono_y, cfg->extreme_point);
    } else {
        assert(0);
    }

    bscaler_frmc_calc_osum(cfg->dst_base, cfg->box_num,
                           cfg->dst_box_w, cfg->dst_box_h,
                           cfg->dst_line_stride,
                           dst_format, bpp);
}

uint32_t get_bst_isum()
{
    return bst_isum;
}

uint32_t get_bst_osum()
{
    return bst_osum;
}

uint32_t get_bsc_isum()
{
    return bsc_isum;
}

uint32_t get_bsc_osum()
{
    return bsc_osum;
}

