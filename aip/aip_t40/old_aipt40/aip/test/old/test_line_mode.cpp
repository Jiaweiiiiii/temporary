/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : test_line_mode.c
 * Authors    : jmqi@taurus
 * Create Time: 2020-04-20:12:05:26
 * Description:
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "image_process.h"

#include "bscaler_api.h"
#include "bscaler_hw_api.h"

int box[33][4] = {
    { 970,  502, 36, 40},
    {  92,  642, 52, 62},
    { 744,  628, 52, 64},
    {1554,  560, 48, 60},
    {1750,  630, 48, 58},
    { 400,  622, 50, 62},
    {1222,  456, 46, 56},
    {1834,  484, 46, 58},
    {1262,  616, 46, 58},
    { 510,  446, 48, 60},
    { 760,  484, 48, 60},
    { 274,  494, 48, 60},
    {1602,  640, 48, 60},
    {1470,  634, 48, 60},
    {1906,  646, 52, 64},
    { 548,  632, 46, 58},
    {1122,  636, 50, 62},
    { 448,  476, 44, 56},
    { 622,  478, 48, 60},
    { 174,  510, 48, 60},
    {1408,  496, 48, 60},
    { 894,  508, 48, 60},
    {1554,  474, 46, 56},
    {1714,  566, 48, 60},
    {1042,  542, 46, 58},
    { 354,  478, 44, 56},
    {1280,  506, 46, 58},
    { 270,  632, 48, 60},
    {1686,  510, 42, 52},
    { 990,  634, 48, 60},
    {1168,  524, 44, 56},
    {1092,  498, 44, 54},
    {1858,  548, 46, 56},
};

int main(int argc, char **argv)
{
    const uint32_t coef[9] = {1220542, 1673527, 0,
                              1220542, 409993, 852492,
                              1220542, 2116026, 0};
    const uint32_t offset[2] = {16, 128};

    // read image
    int img_w, img_h, img_chn;
    uint8_t *img = stbi_load(argv[1], &img_w, &img_h, &img_chn, 4);

    // alloc space for output
    uint32_t box_num = 33;
    uint32_t dst_w = 64;
    uint32_t dst_h = 64;
    int dst_size = dst_w * dst_h * 4 * box_num;
    uint8_t *dst_base = (uint8_t *)malloc(dst_size);
    bool affine = false;
    bool box_mode = false;
    bs_hw_data_format_e src_format = BS_HW_DATA_FM_BGRA;
    bs_hw_data_format_e dst_format = BS_HW_DATA_FM_BGRA;

    // only for resize line mode, affine, box_mode must be false
    bs_hw_once_cfg_s cfg = {img, NULL, src_format, (uint32_t)img_w * 4,
                            0, 0, (uint32_t)img_w, (uint32_t)img_h,
                            dst_base, NULL, dst_format, dst_w * 4,
                            0, 0, dst_w, dst_h,
                            affine, box_mode, 0, 0,
                            coef[0], coef[1], coef[2],
                            coef[3], coef[4], coef[5],
                            coef[6], coef[7], coef[8],
                            offset[0], offset[1],
                            0, 0, 0, 0, 0, 0,
                            box_num,
                            NULL,
                            0, 0};

    cfg.boxes_info = (uint32_t *)malloc(box_num * 7 * 4);//6 word per box
    if (cfg.boxes_info == NULL) {
        fprintf(stderr, "%s:[%d] alloc space failed!\n", __FILE__, __LINE__);
        return -1;
    }

    for (int i = 0; i < box_num; i++) {
        cfg.boxes_info[i * 7 + 0] = ((box[i][0] & 0xFFFF) << 16 |
                                     (box[i][1] & 0xFFFF));
        cfg.boxes_info[i * 7 + 1] = ((box[i][2] & 0xFFFF) << 16 |
                                     (box[i][3] & 0xFFFF));
        cfg.boxes_info[i * 7 + 2] = (int32_t)((box[i][2] * 32768 / dst_w) );//scale_x
        cfg.boxes_info[i * 7 + 3] = (int32_t)((box[i][3] * 32768 / dst_h) );//scale_y
        cfg.boxes_info[i * 7 + 4] = 0;
        cfg.boxes_info[i * 7 + 5] = 0;
        cfg.boxes_info[i * 7 + 6] = (uint32_t)(dst_base + i * dst_w * 4 * dst_h);
    }

    bscaler_mdl_wrap(&cfg);

    stbi_write_png(argv[2], dst_w, dst_h * box_num, 4, dst_base, dst_w * 4);
    free(dst_base);
    free(cfg.boxes_info);
    return 0;
}
