/*********************************************************
 * File Name   : aip_resize_test.c
 * Author      : Zhou Qi
 * Mail        : gavin.qzhou@ingenic.com
 * Created Time: 2022-06-01 14:36
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <sys/stat.h>
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>


#include "ingenic_aip.h"
#include "aie_mmap.h"
#include "bs_src.h"
#include "aip_p_mdl.h"
#include "aip_f_mdl.h"
#include "Matrix.h"

int aip_affine_test(bs_data_format_e format, uint32_t box_num);

unsigned int nv2bgr_ofst[2] = {16, 128};
unsigned int nv2bgr_coef[9] = {
	1220542, 2116026, 0,
	1220542, 409993, 852492,
	1220542, 0, 1673527
};
int box[][4] = {
	{0, 0, 256 , 256},
	{0, 100, 128, 128},
	{100, 0, 128, 128},
	{100, 100, 128, 128},
	{0, 0, 128, 128},
};
float matrix_g[][9] = {
	{
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	},
	{
		2, 0, 0,
		0, 2, 0,
		0, 0, 1
	},
	{
		1, 0, 0,
		0.5, 1, 0,
		0, 0, 1
	},

	{
		0.5, 0, 0,
		0, 0.5, 0,
		0, 0, 1
	},
};

int aip_affine_test(bs_data_format_e format, uint32_t box_num)
{
	int ret = 0;
	FILE *fp;
	int i, j;
	char name[32] = {0};
	uint32_t num_t = 0;

	uint8_t chn, bw;
	uint32_t src_w, src_h;
	uint32_t dst_w, dst_h;
	uint32_t dst_size, src_size;
	uint32_t src_bpp, dst_bpp;
	uint32_t src_stride, dst_stride;
	uint8_t *src_base, *src_base1;
	uint8_t *dst_base, *dst_base1;
	uint8_t *opencv_base, *opencv_base1;
	float matrix[9] = {0};
	float angel = 0;

	//while(1)
	{

		src_w = 1024;
		src_h = 576;
		dst_w = 512;
		dst_h = 512;
		if(format == BS_DATA_Y) {
			src_bpp = 8;
			dst_bpp = 8;
			chn = 1;
			bw = 0;
		} else if(format == BS_DATA_UV) {
			src_bpp = 8;
			dst_bpp = 8;
			chn = 2;
			bw = 1;
		} else if(format == BS_DATA_NV12) {
			src_bpp = 12;
			dst_bpp = 8;
			chn = 4;
			bw = 2;
		} else if(format <= BS_DATA_ARGB && format >= BS_DATA_BGRA) {
			src_bpp = 8;
			dst_bpp = 8;
			chn = 4;
			bw = 3;
		}

		if(format == BS_DATA_NV12) {
			src_stride =  src_w;
			dst_stride =  dst_w * chn;
			src_size = src_stride * src_h * 3 / 2;
			dst_size = dst_stride * dst_h;
		} else {
			src_stride =  src_w * chn;
			dst_stride =  dst_w * chn;
			src_size = src_stride * src_h;
			dst_size = dst_stride * dst_h;
		}

		src_base = (uint8_t *)ddr_memalign(32, src_size);
		dst_base = (uint8_t *)ddr_memalign(32, dst_size * box_num);
		memset(src_base, 0x00, src_size);
		memset(dst_base, 0x00, dst_size * box_num);

		fp = fopen("./model/day-T40XP_273_w1024h576_w1024_h576.nv12","r");
		if(fp == NULL) {
			perror("Affine fopen picture Error");
			exit(-1);
		}
		fread(src_base, 32, src_size/32, fp);
		fclose(fp);
		__aie_flushcache_dir((void *)src_base, src_size, NNA_DMA_TO_DEVICE);

		box_affine_info_s *boxes = (box_affine_info_s *)malloc(sizeof(box_affine_info_s) * box_num);
		for(i = 0; i < box_num; i++) {
			boxes[i].box.x = 0;	//box[i][0];
			boxes[i].box.y = 0;	//box[i][1];
			boxes[i].box.w = src_w;	//box[i][2];
			boxes[i].box.h = src_h;	//box[i][3];
			boxes[i].zero_point = 0x0;
			angel = rand()%180;
			get_matrixs(&matrix[0], angel, src_w, src_h, dst_w, dst_h, 0);
			memcpy(boxes[i].matrix, &matrix[0], sizeof(matrix));
		}

		data_info_s *src = (data_info_s *)malloc(sizeof(data_info_s));
		src->base = __aie_get_ddr_paddr((uint32_t)src_base);
		src->base1 = src->base + src_w * src_h;
		src->format = format;
		src->width = src_w;
		src->height = src_h;
		src->line_stride = src_stride;
		src->locate = BS_DATA_RMEM;

		data_info_s *dst = (data_info_s *)malloc(sizeof(data_info_s) * box_num);
		for(i = 0; i < box_num; i++) {
			dst[i].base = __aie_get_ddr_paddr((uint32_t)dst_base) + i* dst_size;
			dst[i].base1 = dst[i].base + dst_w * dst_h;
			dst[i].format = format;
			dst[i].width = dst_w;
			dst[i].height = dst_h;
			dst[i].line_stride = dst_stride;
			dst[i].locate = BS_DATA_RMEM;
		}


	struct timeval start_time, end_time;
	double total_time;
	gettimeofday(&start_time, NULL);

		__aie_flushcache_dir((void *)dst_base, dst_size * box_num, NNA_DMA_FROM_DEVICE);
		ingenic_aip_affine_process(src, box_num, dst, boxes, &nv2bgr_coef[0], &nv2bgr_ofst[0]);


	gettimeofday(&end_time, NULL);
	long time = start_time.tv_sec * 1000000 + start_time.tv_usec;
	time = end_time.tv_sec * 1000000 + end_time.tv_usec - time;
	total_time = time *1.0 / 1000;
	printf("Running time of create_bo is %.03lf ms\n", total_time);

		free(dst);
		free(src);
		free(boxes);
		ddr_free(dst_base);
		ddr_free(src_base);
	}

	return 0;
}

int main(int argc, char *argv[])
{

	int ret = 0;
	int chain_num = 10;

	nna_cache_attr_t desram_cache_attr = NNA_UNCACHED_ACCELERATED;
	nna_cache_attr_t oram_cache_attr = NNA_UNCACHED_ACCELERATED;
	nna_cache_attr_t ddr_cache_attr = NNA_CACHED;
	ret = __aie_mmap(0x1000000, 1, desram_cache_attr, oram_cache_attr, ddr_cache_attr);

	ingenic_aip_init();
	aip_affine_test(BS_DATA_NV12, chain_num);
	ingenic_aip_deinit();
	__aie_munmap();

	return 0;
}
