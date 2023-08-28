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

/* #define USER_ORAM_I */
/* #define USER_ORAM_O */

int aip_covert_test(uint32_t task_len);
#define TASK_LEN	128
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

int aip_covert_test(uint32_t len)
{
	int ret = 0;
	FILE *fp;
	int i, j;
	uint32_t task_off;
	char name[32] = {0};
	uint32_t num_t = 0;

	task_info_s task_info;
	uint32_t task_len;
	uint32_t src_w, src_h;
	uint32_t dst_w, dst_h;
	uint32_t dst_size, src_size;
	uint32_t src_bpp, dst_bpp;
	uint8_t *src_base, *src_base1;
	uint8_t *dst_base, *dst_base1;
	uint8_t *opencv_base, *opencv_base1;

while(1)
{
	src_w = 256;
	src_h = 256;
	dst_w = src_w;
	dst_h = src_h;

	src_bpp = 12;
	dst_bpp = 32;
	src_size = src_w * src_h * src_bpp >> 3;
	dst_size = dst_w * dst_h * dst_bpp >> 3;
	src_base = (uint8_t *)ddr_memalign(32, src_size);
	dst_base = (uint8_t *)ddr_memalign(32, dst_size);

	memset(src_base, 0x00, src_size);
	memset(dst_base, 0x00, dst_size);

	fp = fopen("./1111_resize.bmp.nv12","r");
	if(fp == NULL) {
		perror("Covert fopen picture Error");
		exit(-1);
	}
	fread(src_base, 32, src_size/32, fp);
	fclose(fp);
	__aie_flushcache_dir((void *)src_base, src_size, NNA_DMA_TO_DEVICE);

	data_info_s *src =  (data_info_s *)malloc(sizeof(data_info_s));
	src->base = __aie_get_ddr_paddr((uint32_t)src_base);
	src->base1 = src->base + src_w * src_h;
	src->format = BS_DATA_NV12;
	src->width = src_w;
	src->height = src_h;
	src->line_stride = src_w;
	src->locate = BS_DATA_RMEM;

	data_info_s *dst = (data_info_s *)malloc(sizeof(data_info_s));
	dst->base = __aie_get_ddr_paddr((uint32_t)dst_base);
	dst->base1 = 0;
	dst->format = BS_DATA_BGRA;
	dst->width = dst_w;
	dst->height = dst_h;
	dst->line_stride = dst_w * dst_bpp >> 3;
	dst->locate = BS_DATA_RMEM;
	task_len = len;
	__aie_flushcache_dir((void *)dst_base, dst_size, NNA_DMA_FROM_DEVICE);
	bs_covert_cfg(src, dst, &nv2bgr_coef[0], &nv2bgr_ofst[0], &task_info);
	for(i = 0; i < dst_h;) {
		task_off = dst_h - i;
		if(task_len <= task_off) {
			task_len = task_len;
		} else {
			task_len = task_off;
		}
		i = i + task_len;
		task_info.task_len = task_len;
		bs_covert_step_start(&task_info, dst->base, BS_DATA_RMEM);
		dst->base = dst->base + task_len * dst->line_stride;
	bs_covert_step_wait();
	}
	bs_covert_step_exit();
	printf("aip   %d\n", __LINE__);
	free(dst);
	free(src);
	ddr_free(dst_base);
	ddr_free(src_base);
}

	return 0;
}

int main(int argc, char *argv[])
{

	int ret = 0;
	int chain_num = 1;


	nna_cache_attr_t desram_cache_attr = NNA_UNCACHED_ACCELERATED;
	nna_cache_attr_t oram_cache_attr = NNA_UNCACHED_ACCELERATED;
	nna_cache_attr_t ddr_cache_attr = NNA_CACHED;
	ret = __aie_mmap(0x1000000, 1, desram_cache_attr, oram_cache_attr, ddr_cache_attr);

	aip_covert_test(TASK_LEN);

	__aie_munmap();

	return 0;
}
