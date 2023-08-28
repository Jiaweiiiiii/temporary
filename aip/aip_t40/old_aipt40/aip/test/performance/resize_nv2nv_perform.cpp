/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : resize_nv12_to_nv12.cpp
 * Authors    : jmqi@taurus
 * Create Time: 2020-04-20:11:04:27
 * Description:
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <sys/time.h>
#include "image_process.h"//not necessary
#include "bscaler_api.h"
#include "bscaler_hal.h"
#include "aie_mmap.h"

/*int box[33][4] = {
    { 0,  0, 640, 480},
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
*/
int box[1][4] ={
    {0 , 0, 256,256}
};

int main(int argc, char** argv)
{
    int ret = 0;
    bs_version();
    float time_use = 0;
    struct timeval start;
    struct timeval end;
    gettimeofday(&start, NULL);

    nna_cache_attr_t desram_cache_attr = NNA_UNCACHED_ACCELERATED;
    nna_cache_attr_t oram_cache_attr = NNA_UNCACHED_ACCELERATED;
    nna_cache_attr_t ddr_cache_attr = NNA_CACHED;
    if ((ret = __aie_mmap(0x4000000, 1, desram_cache_attr, oram_cache_attr, ddr_cache_attr)) == 0) { //64MB
        printf ("nna box_base virtual generate failed !!\n");
        return 0;
    }
    const uint32_t coef[9] = {1220542, 0, 1673527,
                              1220542, 409993, 852492,
                              1220542, 2116026, 0};
    const uint32_t offset[2] = {16, 128};

    if (argc < 4) {
        fprintf(stderr, "%s [image_path]/entertainers_2048x1080.nv12 src_w src_h dst_w dst_h\n", argv[1]);
        exit(1);
    }

    int img_w, img_h, img_chn;
    img_w = 2048;// 2048;
    img_h = 1080;//1080;

    printf("[BSCALER]: load image ...\n");
    FILE *fpi;
    fpi = fopen(argv[1], "rb+");
    if (fpi == NULL) {
        printf("open %s failed!", argv[1]);
    }
    int img_size = img_w * img_h * 3 / 2;
    uint8_t *img = (uint8_t *)malloc(img_size);
    if (img == NULL) {
        printf("malloc image failed!\n");
    }

    size_t cnt = fread(img, 1, img_size, fpi);
    if (cnt != img_size) {
        printf("fread failed!\n");
    }
    printf("[BSCALER]: load image success\n");

    int box_num = 1;//33;
    int dst_w = atoi(argv[4]);
    int dst_h = atoi(argv[5]);
    int dst_size = dst_w * dst_h * box_num * 3 / 2;
    uint8_t *dst_base = (uint8_t *)ddr_memalign(8, dst_size);

    if (dst_base == NULL) {
        fprintf(stderr, "[BSCALER]: dst alloc failed!\n");
    }
    memset(dst_base, 255, dst_size);

    int src_size = img_w * img_h * 3 / 2;
    uint8_t *src_base = (uint8_t *)ddr_memalign(8, src_size);
    if (src_base == NULL) {
        fprintf(stderr, "[BSCALER]: src alloc failed!\n");
    }
    memset(src_base, 0, src_size);
    memcpy(src_base, img, src_size);

    data_info_s src;
    src.base = src_base;
    src.base1 = NULL;//fixme
    src.format = BS_DATA_NV12;
    src.chn = 1;
    src.width = img_w;
    src.height = img_h;
    src.line_stride =img_w;
    src.locate = BS_DATA_NMEM;

    data_info_s *dst = (data_info_s *)malloc(sizeof(data_info_s) * box_num);

    for (int i = 0; i < box_num; i++) {
        dst[i].base = dst_base + i * dst_w * dst_h * 3 / 2;
        dst[i].base1 = NULL;//fixme
        dst[i].format = BS_DATA_NV12;
        dst[i].chn = 1;
        dst[i].height = dst_h;
        dst[i].width = dst_w;
        dst[i].line_stride = dst_w;
        dst[i].locate = BS_DATA_NMEM;
    }

    box_resize_info_s *infos = (box_resize_info_s *)malloc(sizeof(box_affine_info_s) * box_num);
    for (int i = 0; i < box_num; i++) {
        int src_x = box[i][0];
        int src_y = box[i][1];
        int src_w = atoi(argv[2]);//box[i][2];
        int src_h = atoi(argv[3]);//box[i][3];

        infos[i].box.x = src_x;
        infos[i].box.y = src_y;
        infos[i].box.w = src_w;
        infos[i].box.h = src_h;
        infos[i].wrap = 0;
        infos[i].zero_point = 0;
    }

    if (ddr_cache_attr & NNA_CACHED) {
        __aie_flushcache((void *)src_base, src_size);
    }

    gettimeofday(&end, NULL);
    time_use = (float)((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec)) / 1000.0f;
    printf("stage 1 cost:%.3fms\n",time_use);

    printf("[BSCALER]: bscaler resize start\n");
    bs_resize_start(&src, box_num, dst, infos, coef, offset);
    printf("[BSCALER]: bscaler wait finish ...\n");

     gettimeofday(&end, NULL);
    time_use = (float)((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec)) / 1000.0f;
    printf("stage 2 cost:%.3fms\n",time_use);

    bs_resize_wait();
    printf("[BSCALER]: bscaler resize finish ..\n");

     gettimeofday(&end, NULL);
    time_use = (float)((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec)) / 1000.0f;
    printf("stage 3 cost:%.3fms\n",time_use);

    if (ddr_cache_attr & NNA_CACHED) {
        __aie_flushcache((void *)dst_base, dst_size);
    }

     gettimeofday(&end, NULL);
    time_use = (float)((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec)) / 1000.0f;
    printf("total time cost:%.3fms\n",time_use);

    char str[128];
    sprintf(str,"resize_nv2nv_%d_%d_%.1fms.nv12",img_w,dst_w,time_use);
    FILE *fpo;
    fpo = fopen(str, "wb+");
    if (fpo == NULL) {
        fprintf(stderr, "Open resize_out.nv12 failed!\n");
    }
    fwrite(dst_base, 1, dst_size, fpo);

    ddr_free(dst_base);
    ddr_free(src_base);
    free(dst);
    free(infos);
    __aie_munmap();
    return 0;
}
