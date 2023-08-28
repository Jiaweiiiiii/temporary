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

int aip_resize_test(bs_data_format_e format, uint32_t box_num);
int aip_perspective_test(bs_data_format_e format, uint32_t box_num);
int aip_affine_test(bs_data_format_e format, uint32_t box_num);
void *aip_version(void);

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

int aip_resize_test(bs_data_format_e format, uint32_t box_num)
{

	int ret = 0;
	FILE *fp;
	FILE *fp_man;
	int i, j;
	int index;
	char name[32] = {0};
	uint32_t num_t = 0;
	int count = 0;

	uint8_t chn , bw;
	uint32_t src_w, src_h;
	uint32_t dst_w, dst_h;
	uint32_t dst_size, src_size;
	uint32_t src_bpp, dst_bpp;
	uint32_t src_stride, dst_stride;
	uint8_t *src_base, *src_base1 , *src_base_3;
	uint8_t *dst_base, *dst_base1, * dst_base_3;
	uint8_t *opencv_base, *opencv_base1;
	int x, y, h, w;

	float time_use=0;
	struct timeval start;
	struct timeval end;

	//box_num = (rand() % 10) + 1;
	src_w = 1024;
	src_h = 576;
	dst_w = 512;
	dst_h = 512;
	src_size = src_w * src_h *4;
	dst_size = src_w * src_h *4;
	dst_base = (uint8_t *)ddr_memalign(32, dst_size * box_num);
	src_base = (uint8_t *)ddr_memalign(32, src_size);
	memset(src_base, 0x00, src_size);
	memset(dst_base, 0x00, dst_size * box_num);
	for(count = 0; count < 10; count++){
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
		} else if(format <= BS_DATA_ARGB && format >= BS_DATA_BGRA){
			src_bpp = 8;
			dst_bpp = 8;
			chn = 4;
			bw = 2;
		} else if(format == BS_DATA_FMU2) {
			src_bpp = 2;
			dst_bpp = 2;
			chn = 32;
			bw = 3;
		} else if(format == BS_DATA_FMU4) {
			src_bpp = 4;
			dst_bpp = 4;
			chn = 32;
			bw = 4;
		} else if(format == BS_DATA_FMU8) {
			src_bpp = 8;
			dst_bpp = 8;
			chn = 32;
			bw = 5;
		} else if(format == BS_DATA_FMU8_H) {
			src_bpp = 8;
			dst_bpp = 8;
			chn = 16;
			bw = 6;
		} else if(format == BS_DATA_NV12) {
			src_bpp = 12;
			dst_bpp = 12;
			chn = 1;
			bw = 7;
		}


		if(format == BS_DATA_NV12) {
			src_stride =  src_w;
			dst_stride =  dst_w;
			src_size = src_stride * src_h * src_bpp >> 3;
			dst_size = dst_stride * dst_h * dst_bpp >> 3;
		} else {
			src_stride =  src_w * chn * src_bpp >> 3;
			dst_stride =  dst_w * chn * dst_bpp >> 3;
			src_size = src_stride * src_h;
			dst_size = dst_stride * dst_h;
		}

		fp = fopen("./model/day_w1024_h576.nv12","r");
		if(fp == NULL) {
			perror("Resize fopen picture Error");
			exit(-1);
		}
		fread(src_base, 32, src_size/32, fp);
		int flocse_fp = fclose(fp);
		if(flocse_fp < 0)
		{
			return -1;
		}
		__aie_flushcache_dir((void *)src_base, src_size, NNA_DMA_TO_DEVICE);
		box_resize_info_s *boxes = (box_resize_info_s *)malloc(sizeof(box_resize_info_s) * box_num);
		for(i = 0; i < box_num; i++) {
			boxes[i].box.x = 0;	//box[index][0];
			boxes[i].box.y = 0;	//box[index][1];
			boxes[i].box.w = src_w;	//box[index][2];
			boxes[i].box.h = src_h;	//box[index][3];
		}

		data_info_s *src = (data_info_s *)malloc(sizeof(data_info_s));
		src->base = __aie_get_ddr_paddr((uint32_t)src_base);
		src->base1 = src->base + src_w * src_h;
		src->format = format;
		src->width = src_w;
		src->height = src_h;
		src->line_stride = src_stride;
		src->locate = BS_DATA_NMEM;

		data_info_s *dst = (data_info_s *)malloc(sizeof(data_info_s) * box_num);
		for(i = 0; i < box_num; i++) {
			dst[i].base = __aie_get_ddr_paddr((uint32_t)dst_base) + i * dst_size;
			dst[i].base1 = dst[i].base + dst_w * dst_h;
			dst[i].format = format;
			dst[i].width = dst_w;
			dst[i].height = dst_h;
			dst[i].line_stride = dst_stride;
			dst[i].locate = BS_DATA_NMEM;
		}

		__aie_flushcache_dir((void *)dst_base, dst_size * box_num, NNA_DMA_FROM_DEVICE);
		gettimeofday(&start,NULL);
		ingenic_aip_resize_process(src, box_num, dst, boxes, &nv2bgr_coef[0], &nv2bgr_ofst[0]);
		gettimeofday(&end,NULL);
		time_use=(end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);//微秒
		printf("time_use is %.10f\n",time_use); 
#if 0

		sprintf(name, "%s_%d", "./test_dst", 0);
		fp = fopen(name, "w+");
		fwrite(dst_base, 32, dst_size/32, fp);
		fclose(fp);
#endif

		free(dst);
		free(src);
		free(boxes);
		ddr_free(dst_base);
		ddr_free(src_base);
		//exit(-1);
	}
	//free(memcpy_size_man);
	return 0;
}

#if 1
int aip_perspective_test(bs_data_format_e format, uint32_t box_num)
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

	while(1)
	{
		src_w = 512;
		src_h = 512;
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
		opencv_base = (uint8_t *)ddr_memalign(32, dst_size * box_num);
		memset(src_base, 0x00, src_size);
		memset(dst_base, 0x00, dst_size * box_num);

		fp = fopen("./1111_resize.bmp.data","r");
		if(fp == NULL) {
			perror("Perspective fopen picture Error");
			exit(-1);
		}
		fread(src_base, 32, src_size/32, fp);
		fclose(fp);
		__aie_flushcache_dir((void *)src_base, src_size, NNA_DMA_TO_DEVICE);

		get_matrixs(&matrix[0], rand()%180, src_w, src_h, dst_w, dst_h, 1);
		box_affine_info_s *boxes = (box_affine_info_s *)malloc(sizeof(box_affine_info_s) * box_num);
		for(i = 0; i < box_num; i++) {
			boxes[i].box.x = 0;	//box[i][0];
			boxes[i].box.y = 0;	//box[i][1];
			boxes[i].box.w = src_w;	//box[i][2];
			boxes[i].box.h = src_h;	//box[i][3];
			boxes[i].zero_point = 0x0;
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

		__aie_flushcache_dir((void *)dst_base, dst_size * box_num, NNA_DMA_FROM_DEVICE);
		ingenic_aip_perspective_process(src, box_num, dst, boxes, &nv2bgr_coef[0], &nv2bgr_ofst[0]);
		//bs_perspective_wait();


		free(dst);
		free(src);
		free(boxes);
		ddr_free(dst_base);
		ddr_free(src_base);
	}

	return 0;
}

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
	float time_use=0;
	struct timeval start;                                                                                                                                                        
	struct timeval end;
	while(1)
	{
		src_w = 512;
		src_h = 512;
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
		opencv_base = (uint8_t *)ddr_memalign(32, dst_size * box_num);
		memset(src_base, 0x00, src_size);
		memset(dst_base, 0x00, dst_size * box_num);

		fp = fopen("./1111_resize.bmp.data","r");
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
		__aie_flushcache_dir((void *)dst_base, dst_size * box_num, NNA_DMA_FROM_DEVICE);
		gettimeofday(&start,NULL);
		ingenic_aip_affine_process(src, box_num, dst, boxes, &nv2bgr_coef[0], &nv2bgr_ofst[0]);
		//bs_affine_wait();
		gettimeofday(&end,NULL);
		time_use=(end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);//微秒
		printf("time_use is %.10f\n",time_use); 
		free(dst);
		free(src);
		free(boxes);
		ddr_free(dst_base);
		ddr_free(src_base);
	}

	return 0;
}
#endif
void * resize_1(void *arg)
{

	int chain_num = 1;
	aip_resize_test(BS_DATA_Y, chain_num);
}

void * resize_2(void *arg)
{

	int chain_num = 1;
	aip_resize_test(BS_DATA_BGRA, chain_num);
}

void * perspective(void *arg)
{

	int chain_num = 1;
	aip_perspective_test(BS_DATA_NV12, chain_num);
}

void * affine(void *arg)
{
	int chain_num = 1;
	aip_affine_test(BS_DATA_NV12, chain_num);

}

int main(int argc, char *argv[])
{


	int ret = 0;
	int chain_num = 1;
	void * version;
	pthread_t tid_1;
	//	pthread_t tid_2;
	//	pthread_t tid_3;
	//	pthread_t tid_4;

	nna_cache_attr_t desram_cache_attr = NNA_UNCACHED_ACCELERATED;
	nna_cache_attr_t oram_cache_attr = NNA_UNCACHED_ACCELERATED;
	nna_cache_attr_t ddr_cache_attr = NNA_CACHED;
	ret = __aie_mmap(0x1000000, 1, desram_cache_attr, oram_cache_attr, ddr_cache_attr);

	ingenic_aip_init();
	version = aip_version();
	if(version < 0)
	{
		printf("get version failed\n");
	}
	//printf("version = %x\n", version);
	printf("version = %s\n", (char *)version);
	//	pthread_create(&tid_1, NULL, resize_1, NULL);
	//	pthread_create(&tid_2, NULL, resize_2, NULL);
	//	pthread_create(&tid_3, NULL, perspective, NULL);
	//	pthread_create(&tid_4, NULL, affine, NULL);

	//	pthread_join(tid_1, NULL);
	//	pthread_join(tid_2, NULL);
	//	pthread_join(tid_3, NULL);
	//	pthread_join(tid_4, NULL);
	//	pthread_detach(tid_1);
	//	pthread_detach(tid_2);

	aip_resize_test(BS_DATA_NV12, chain_num);

	ingenic_aip_deinit();
	__aie_munmap();

	return 0;
}
