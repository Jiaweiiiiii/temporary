/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : resize_demo.cpp
 * Authors    : jmqi@taurus
 * Create Time: 2020-04-20:11:04:27
 * Description:
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "image_process.h"//not necessary
#include "bscaler_api.h"
#include "hal/bscaler_hal.h"
#include "aie_mmap.h"

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

	float time_use=0;
	struct timeval start;                                                                                                                                                        
	struct timeval end;
	FILE *fp; 
    int ret = 0;
    nna_cache_attr_t desram_cache_attr = NNA_UNCACHED_ACCELERATED;
    nna_cache_attr_t oram_cache_attr = NNA_UNCACHED_ACCELERATED;
    nna_cache_attr_t ddr_cache_attr = NNA_CACHED;
    if ((ret = __aie_mmap(0x4000000, 1, desram_cache_attr, oram_cache_attr, ddr_cache_attr)) == 0) { //64MB
        printf ("nna box_base virtual generate failed !!\n");
        return 0;
    }
    const uint32_t coef[9] = {1220542, 0, 1673527,
                              1220542, 409993, 852492,
                              1220542, 2116026, 0};//opencv
    const uint32_t offset[2] = {16, 128};

#if 0
    if (argc < 2) {
        fprintf(stderr, "%s [image_path]/entertainers.jpg\n", argv[1]);
        exit(1);
    }
    int img_w, img_h, img_chn;
    //argv[1] = "./entertainers.jpg";
    printf("[BSCALER]: load image ...\n");
    uint8_t *img = stbi_load(argv[1], &img_w, &img_h, &img_chn, 4);
    if (img == NULL) {
        fprintf(stderr, "[BSCALER]: Open input image failed!\n");
        exit(1);
	}
	printf("[BSCALER]: load image success\n");
#else
	int img_w = 512;
	int img_h = 512;
	int img_chn = 4;

#endif

    int box_num = 33;
    int dst_w = 96;
    int dst_h = 96;
    int dst_size = dst_w * dst_h * 4 * box_num;
    uint8_t *dst_base = (uint8_t *)ddr_memalign(8, dst_size);
        
    if (dst_base == NULL) {
        fprintf(stderr, "[BSCALER]: dst alloc failed!\n");
    }
    memset(dst_base, 255, dst_size);

    int src_size = img_w * img_h * 4;
    uint8_t *src_base = (uint8_t *)ddr_memalign(8, src_size);
    if (src_base == NULL) {
        fprintf(stderr, "[BSCALER]: src alloc failed!\n");
    }
    memset(src_base, 0, src_size);
	fp = fopen("./model/day_w1024_h576.nv12","r");
	if(fp == NULL) {
		perror("Resize fopen picture Error");
		exit(-1);
	}
	fread(src_base, 32, src_size/32, fp);
	fclose(fp);
    //memcpy(src_base, img, src_size);

    data_info_s src;
	src.base = src_base;
	src.format = BS_DATA_NV12;
	src.chn = (bs_data_format_e)4;
	src.height = img_w;
	src.width = img_h;
	src.line_stride = img_w * 4;
	src.locate = (bs_data_locate_e)0;

    data_info_s *dst = (data_info_s *)malloc(sizeof(data_info_s) * box_num);

    for (int i = 0; i < box_num; i++) {
        dst[i].base = dst_base + i * dst_w * dst_h * 4;
        dst[i].format = BS_DATA_BGRA;
        dst[i].chn = 4;
        dst[i].height = dst_h;
        dst[i].width = dst_w;
        dst[i].line_stride = dst_w * 4;
        dst[i].locate = (bs_data_locate_e)0;
    }

    box_resize_info_s *infos = (box_resize_info_s *)malloc(sizeof(box_affine_info_s) * box_num);
    for (int i = 0; i < box_num; i++) {
        int src_x = box[i][0];
        int src_y = box[i][1];
        int src_w = box[i][2];
        int src_h = box[i][3];

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
	gettimeofday(&start,NULL);
    printf("[BSCALER]: bscaler resize start\n");
    bs_resize_start(&src, box_num, dst, infos, coef, offset);
    printf("[BSCALER]: bscaler wait finish ...\n");
    bs_resize_wait();
    printf("[BSCALER]: bscaler resize finish ..\n");
	gettimeofday(&end,NULL);
	time_use=(end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);//微秒
	printf("time_use is %.10f\n",time_use);

    if (ddr_cache_attr & NNA_CACHED) {
        __aie_flushcache((void *)dst_base, dst_size);
    }

    stbi_write_jpg("resize_out.jpg", dst_w, dst_h * box_num, 4, dst_base, 90);
    ddr_free(dst_base);
    ddr_free(src_base);
    free(dst);
    free(infos);
    //stbi_image_free(img);
    __aie_munmap();
    return 0;
}
