/*********************************************************
 * File Name   : aip_resize_demo.c
 * Author      : jwzhang
 * Mail        : kevin.jwzhang@ingenic.com
 * Created Time: 2023-03-1 16:45
 ********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

#include "ingenic_aip.h"
#include "aie_mmap.h"
//#include "Matrix.h" // get_matrixs()

int aie_mmp_set()
{
    nna_cache_attr_t desram_cache_attr = NNA_UNCACHED_ACCELERATED;
    nna_cache_attr_t oram_cache_attr = NNA_UNCACHED_ACCELERATED;
    nna_cache_attr_t ddr_cache_attr = NNA_CACHED;
    return __aie_mmap(0x1000000, 1, desram_cache_attr, oram_cache_attr, ddr_cache_attr);
}



unsigned int nv2bgr_ofst[2] = {16, 128};
unsigned int nv2bgr_coef[9] = {
    1220542, 2116026, 0,
	1220542, 409993, 852492,
	1220542, 0, 1673527
};

int aip_convert_test()
{
	FILE *fp;
	task_info_s task_info;
	uint32_t task_len;
	uint32_t task_off;
	uint32_t src_w, src_h;
	uint32_t dst_w, dst_h;
	uint32_t dst_size, src_size;
	uint32_t src_bpp, dst_bpp;
	uint8_t *src_base,*dst_base;

	src_w = 1024;
	src_h = 576;
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

	fp = fopen("./model/day-T40XP_273_w1024h576_w1024_h576.nv12","r");
	if(fp == NULL) {
		perror("Covert fopen picture Error");
		return -1;
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
	dst->format = BS_DATA_RGBA;
	dst->width = dst_w;
	dst->height = dst_h;
	dst->line_stride = dst_w * dst_bpp >> 3;
	dst->locate = BS_DATA_RMEM;
	
	struct timeval start_time, end_time;
	double total_time;
	gettimeofday(&start_time, NULL);

	//task_len = 2;
	task_len = dst_h;
	task_info.task_len = task_len;
	__aie_flushcache_dir((void *)dst_base, dst_size, NNA_DMA_FROM_DEVICE);
	bs_covert_cfg(src, dst, &nv2bgr_coef[0], &nv2bgr_ofst[0], &task_info);
	for(int i = 0; i < dst_h;) {
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
#if 0
	char name[32] = {0};
	sprintf(name, "%s%d", "./dst_image", 1);
	fp = fopen(name, "w+");
	fwrite(dst_base, 32, dst_size/32, fp);
	fclose(fp);
#endif
	bs_covert_step_exit();

	gettimeofday(&end_time, NULL);
	long time = start_time.tv_sec * 1000000 + start_time.tv_usec;
	time = end_time.tv_sec * 1000000 + end_time.tv_usec - time;
	total_time = time *1.0 / 1000;
	printf("Running time of create_bo is %.03lf ms\n", total_time);


	free(dst);
	free(src);
	ddr_free(dst_base);
	ddr_free(src_base);

	return 0;
}


int main(int argc, char *atgv[])
{
    int ret = 0;


    ret = aie_mmp_set();
    if(ret < 0){
        printf("aie_mmp_set() failed!\n");
        return 0;
    }

    ingenic_aip_init();

    ret = aip_convert_test();

    ingenic_aip_deinit();
    __aie_munmap();


    return 0;
}

    



