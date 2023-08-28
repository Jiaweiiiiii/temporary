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

#include "ingenic_aip.h"
#include "aie_mmap.h"
#include "Matrix.h" // get_matrixs()

#include <sys/mman.h> //mmap
#include <sys/time.h>


#define AIP_LOCK                _IOWR('P', 3, int)
#define AIP_UNLOCK              _IOWR('P', 4, int)


unsigned int nv2bgr_ofst[2] = {0, 0}; 
unsigned int nv2bgr_coef[9] = { 
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
};

int aie_mmp_set()
{
    nna_cache_attr_t desram_cache_attr = NNA_UNCACHED_ACCELERATED;
    nna_cache_attr_t oram_cache_attr = NNA_UNCACHED_ACCELERATED;
    nna_cache_attr_t ddr_cache_attr = NNA_CACHED;
    return __aie_mmap(0x600000, 1, desram_cache_attr, oram_cache_attr, ddr_cache_attr);
}

void format_set(bs_data_format_e format, uint32_t *src_bpp, uint32_t *dst_bpp, uint8_t *chn, uint8_t *bw)
{
    if(format == BS_DATA_Y) {
        *src_bpp = 8;
        *dst_bpp = 8;
        *chn = 1;
        *bw = 0;
    } else if(format == BS_DATA_UV) {
        *src_bpp = 8;
        *dst_bpp = 8;
        *chn = 2;
        *bw = 1;
    } else if(format <= BS_DATA_ARGB && format >= BS_DATA_BGRA){
        *src_bpp = 8;
        *dst_bpp = 8;
        *chn = 4;
        *bw = 2;
    } else if(format == BS_DATA_FMU2) {
        *src_bpp = 2;
        *dst_bpp = 2;
        *chn = 32;
        *bw = 3;
    } else if(format == BS_DATA_FMU4) {
        *src_bpp = 4;
        *dst_bpp = 4;
        *chn = 32;
        *bw = 4;
    } else if(format == BS_DATA_FMU8) {
        *src_bpp = 8;
        *dst_bpp = 8;
        *chn = 32;
        *bw = 5;
    } else if(format == BS_DATA_FMU8_H) {
        *src_bpp = 8;
        *dst_bpp = 8;
        *chn = 16;
        *bw = 6;
    } else if(format == BS_DATA_NV12) {
        *src_bpp = 12;
        *dst_bpp = 12;
        *chn = 1;
        *bw = 7;
    }

}

int aip_resize_test(bs_data_format_e format, uint32_t box_num)
{

    FILE *fp;
    int run_cnt = 1;
    int i,j,k;

    uint32_t src_w, src_h;
    uint32_t dst_w, dst_h;
    uint32_t dst_size, src_size;
    uint32_t src_bpp, dst_bpp;
    uint32_t src_stride, dst_stride;
    uint8_t chn, bw;
    uint8_t *src_base;
    uint8_t *dst_base;

    src_w = 1024;
    src_h = 576;
    dst_w = 512;
    dst_h = 512;

    format_set(format, &src_bpp, &dst_bpp, &chn, &bw);

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

    src_base = (uint8_t *)ddr_memalign(32, src_size);
    dst_base = (uint8_t *)ddr_memalign(32, dst_size * box_num);
    memset(src_base, 0x00, src_size);
    memset(dst_base, 0x00, dst_size * box_num);

    for(i = 0; i < run_cnt; i++){
        fp = fopen("./model/day-T40XP_273_w1024h576_w1024_h576.nv12","r");
		if(fp == NULL) {
            printf("aip_f fopen failed!\n");
            return -1;
        }
        fread(src_base, 32, src_size/32, fp);
        if(fclose(fp) < 0){
            printf("aip_f flcose faile!\n");
            return -1;
        }
        __aie_flushcache_dir((void *)src_base, src_size, NNA_DMA_TO_DEVICE);
        box_resize_info_s *boxes = (box_resize_info_s *)malloc(sizeof(box_resize_info_s) * box_num);
        for(j = 0; j < box_num; j++) {
            boxes[j].box.x = 0;
            boxes[j].box.y = 0;
            boxes[j].box.w = src_w;
            boxes[j].box.h = src_h;
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
        for(k = 0; k < box_num; k++) {
            dst[k].base = __aie_get_ddr_paddr((uint32_t)dst_base) + i * dst_size;
            dst[k].base1 = dst[i].base + dst_w * dst_h;
            dst[k].format = format;
            dst[k].width = dst_w;
            dst[k].height = dst_h;
            dst[k].line_stride = dst_stride;
            dst[k].locate = BS_DATA_NMEM;
        }

        __aie_flushcache_dir((void *)dst_base, dst_size * box_num, NNA_DMA_FROM_DEVICE);


		//int aipfd = open("/dev/jzaip_f",O_RDWR | O_SYNC);
		//int ret = ioctl(aipfd, AIP_LOCK, NULL);
		//if (ret < 0) {
		//	perror("ioctl f_lock");
		//}


	struct timeval start_time, end_time;
	double total_time;
	gettimeofday(&start_time, NULL);

        if(ingenic_aip_resize_process(src, box_num, dst, boxes, &nv2bgr_coef[0], &nv2bgr_ofst[0])){
            printf("ingenic_aip_resize_process execution failed !\n");
        }
        else{
            printf("ingenic_aip_resize_process execution completed.\n");
        }

	gettimeofday(&end_time, NULL);
	long time = start_time.tv_sec * 1000000 + start_time.tv_usec;
	time = end_time.tv_sec * 1000000 + end_time.tv_usec - time;
	total_time = time *1.0 / 1000;
	printf("Running time of resize is %.03lf ms\n", total_time);

		//ret = ioctl(aipfd, AIP_UNLOCK, NULL);
		//if (ret < 0) {
		//	perror("ioctl f_unlock");
		//}
#if 0
        char name[32] = {0};
        sprintf(name, "%s%d", "./dst_image", 2);
        fp = fopen(name, "w+");
        fwrite(dst_base, 32, dst_size/32, fp);
        fclose(fp);
#endif

        free(dst);
        free(src);
        free(boxes);
        ddr_free(dst_base);
        ddr_free(src_base);
    }
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

	ret = aip_resize_test(BS_DATA_NV12, 10);
    
	ingenic_aip_deinit();
	__aie_munmap();


    return 0;
}

    



