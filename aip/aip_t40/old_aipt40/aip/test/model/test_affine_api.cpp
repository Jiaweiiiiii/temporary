/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : test_affine_api.cpp
 * Authors    : jmqi@taurus
 * Create Time: 2020-04-20:11:04:27
 * Description: test api and model
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Matrix.h"//not necessary
#include "image_process.h"//not necessary
#include "bscaler_api.h"
#include "bscaler_hal.h"

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

int main(int argc, char** argv)
{
    uint32_t seed = (uint32_t)time(NULL);
    srand(seed);
    printf("seed = 0x%08x\n", seed);
    const uint32_t coef[9] = {1220542, 0, 1673527,
                              1220542, 409993, 852492,
                              1220542, 2116026, 0};

    const uint32_t offset[2] = {16, 128};

    if (argc < 2) {
        fprintf(stderr, "%s [image_path]/entertainers.jpg\n", argv[1]);
        exit(1);
    }

    int img_w, img_h, img_chn;
    //uint8_t *img = stbi_load("../image/entertainers.jpg", &img_w, &img_h, &img_chn, 4);
    uint8_t *img = stbi_load(argv[1], &img_w, &img_h, &img_chn, 4);
    if (img == NULL) {
        fprintf(stderr, "Open input image failed!\n");
        exit(1);
    }

    int box_num = 33;
    int dst_w = 96;
    int dst_h = 96;
    int dst_size = dst_w * dst_h * 4 * box_num;
    uint8_t *dst_base = (uint8_t *)malloc(dst_size);
    if (dst_base == NULL) {
        fprintf(stderr, "alloc failed!\n");
    }
    memset(dst_base, 255, dst_size);

    uint8_t *src_base = (uint8_t *)malloc(img_w * img_h * 4);
    if (src_base == NULL) {
        fprintf(stderr, "alloc failed!\n");
    }
    memset(src_base, 255, img_w * img_h * 4);
    memcpy(src_base, img, img_w * img_h * 4);

    const data_info_s src = {src_base, NULL, BS_DATA_BGRA,
                             4, img_w, img_h, img_w * 4};
    data_info_s *dst = (data_info_s *)malloc(sizeof(data_info_s) * box_num);
    for (int i = 0; i < box_num; i++) {
        dst[i].base = dst_base + i * dst_w * dst_h * 4;
        dst[1].base1 = NULL;
        dst[i].format = BS_DATA_BGRA;
        dst[i].chn = 4;
        dst[i].height = dst_h;
        dst[i].width = dst_w;
        dst[i].line_stride = dst_w * 4;
    }

    box_affine_info_s *infos = (box_affine_info_s *)malloc(sizeof(box_affine_info_s) * box_num);

    for (int i = 0; i < box_num; i++) {
        int src_x = box[i][0];
        int src_y = box[i][1];
        int src_w = box[i][2];
        int src_h = box[i][3];

        CV::Matrix trans;
        float angle = rand() % 360;
        //float angle = (rand() % 3) * 30;
        //float angle = 30;
        printf("angle = %f\n", angle);
        trans.setRotate(angle, (float)(src_w - 1)/2, (float)(src_h - 1)/2);
        trans.postScale((float)dst_w / (float)src_w,
                        (float)dst_h / (float)src_h);
        float matrix[9];
        trans.get9(matrix);

        infos[i].box.x = src_x;
        infos[i].box.y = src_y;
        infos[i].box.w = src_w;
        infos[i].box.h = src_h;
        infos[i].matrix[0] = matrix[0];
        infos[i].matrix[1] = matrix[1];
        infos[i].matrix[2] = matrix[2];
        infos[i].matrix[3] = matrix[3];
        infos[i].matrix[4] = matrix[4];
        infos[i].matrix[5] = matrix[5];
        infos[i].matrix[6] = matrix[6];
        infos[i].matrix[7] = matrix[7];
        infos[i].matrix[8] = matrix[8];
        infos[i].wrap = 0;
        infos[i].zero_point = 0;
    }

    bs_affine_mdl(&src, box_num, dst, infos, coef, offset);
    printf("============ api finish =============\n");
    stbi_write_jpg("out.jpg", dst_w, dst_h * box_num, 4, dst_base, 99);

    free(dst_base);
    free(src_base);
    free(dst);
    free(infos);
    stbi_image_free(img);
    return 0;
}
