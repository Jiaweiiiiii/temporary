/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : test_perspective_api.cpp
 * Authors    : jmqi@taurus
 * Create Time: 2020-04-20:11:04:27
 * Description:
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Matrix.h"//not necessary
#include "image_process.h"//not necessary
#include "bscaler_api.h"
#include "bscaler_hal.h"

float pre_matrix[5][9] = {
    { //card0
        0.7432289094520868, 0.3437581877848567, -264.6641702678065,
        -0.08757221079416411, 0.5322170275729831, 17.43920406223504,
        0.0003323120648683242, 0.0002745154549321075, 1
    },
    { //card1
        0.4543057240815148, -0.03131428951546547, -10.80939450940996,
        -0.08699491144180858, 0.2956677276535477, 1.200874691642763,
        0.000327062308108619, -0.000159130143900859, 1
    },
    { //card2
        0.401960601521412, 0.1006454674597662, -77.88406010591774,
        -0.2734789798275858, 0.3457995719465359, 41.80398078520208,
        0.0003258940309528753, -0.0001701695719617651, 1
    },
    {//QR_code
        0.4808396418405373, -0.327522397222399, 0.6270549977433945,
        0.003929922590050981, 0.5495341755088036, -11.59130667935554,
        -0.0002289044767245844, 0.0003870534629624227, 1
    },
    { //sfz.jpg
        0.5614026216496186, -0.1122805243299237, 5.164904119176487,
        0.08803051543765936, 0.5422679750959821, -27.74369724533274,
        0.0001237909161920783, -0.0003093367976619777, 1
    }
};

int main(int argc, char** argv)
{
    uint32_t seed = (uint32_t)time(NULL);
    srand(seed);
    printf("seed = 0x%08x\n", seed);
    const uint32_t coef[9] = {1220542, 1673527, 0,
                              1220542, 409993, 852492,
                              1220542, 2116026, 0};

    const uint32_t offset[2] = {16, 128};

    if (argc < 2) {
        fprintf(stderr, "%s [image_path]/card2.jpg\n", argv[1]);
        exit(1);
    }

    int img_w, img_h, img_chn;
    uint8_t *img = stbi_load(argv[1], &img_w, &img_h, &img_chn, 4);
    if (img == NULL) {
        fprintf(stderr, "Open input image failed!\n");
        exit(1);
    }
    int box_num = 1;
    int dst_w = 172;
    int dst_h = 108;
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
    memset(src_base, 0, img_w * img_h * 4);
    memcpy(src_base, img, img_w * img_h * 4);
    printf("%d, %d, %d, %d\n", src_base[0], src_base[1], src_base[2], src_base[3]);

    const data_info_s src = {src_base, NULL, BS_DATA_BGRA,
                             4, img_h, img_w, img_w * 4};
    data_info_s *dst = (data_info_s *)malloc(sizeof(data_info_s) * box_num);
    for (int i = 0; i < box_num; i++) {
        dst[i].base = dst_base + i * dst_w * dst_h * 4;
        dst[i].base1 = NULL;
        dst[i].format = BS_DATA_BGRA;
        dst[i].chn = 4;
        dst[i].height = dst_h;
        dst[i].width = dst_w;
        dst[i].line_stride = dst_w * 4;
    }

    box_affine_info_s *infos = (box_affine_info_s *)malloc(sizeof(box_affine_info_s) * box_num);

    for (int i = 0; i < box_num; i++) {
        int src_x = 0;
        int src_y = 0;
        int src_w = img_w;
        int src_h = img_h;

        float *matrix = pre_matrix[4];// 0 - card0, 1 - card1, 2 - card2, 3 - QR_code, 4 --sfz

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

    bs_perspective_mdl(&src, box_num, dst, infos, coef, offset);
    printf("============ api finish =============\n");

#if 1
    int cnt = 0;
    for (int i=0; i<dst_h; i++) {
        for (int j=0; j<dst_w; j++) {
            if (cnt++ < 100)
                printf("(%d, %d)%d, %d, %d, %d\n", j, i,
                       dst_base[j*4 + i * dst_w * 4 + 0],
                       dst_base[j*4 + i * dst_w * 4 + 1],
                       dst_base[j*4 + i * dst_w * 4 + 2],
                       dst_base[j*4 + i * dst_w * 4 + 3]);
        }
    }
#endif
    stbi_write_jpg("out.jpg", dst_w, dst_h * box_num, 4, dst_base, 99);
    printf("============ write image finish =============\n");

    free(dst_base);
    free(src_base);
    free(dst);
    free(infos);
    stbi_image_free(img);
    return 0;
}
