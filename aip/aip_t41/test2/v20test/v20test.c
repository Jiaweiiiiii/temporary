/*********************************************************
 * File Name   : aip_resize_demo.c
 * Author      : jwzhang
 * Mail        : kevin.jwzhang@ingenic.com
 * Created Time: 2023-03-1 16:45
 ********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h> //mmap
#include <sys/time.h>

#include "include/test.h"
#include "include/jz_aip_v20_ioctl.h"

int aip_submit(int fd,  aip_v20_ioctl_submit_t *args)
{
    int ret;
    if (args->op > AIP_CH_OP_F)
        return -1; 
    ret = ioctl(fd, IOCTL_AIP_V20_SUBMIT, args);   
    return ret;
}

int aip_wait(int fd, aip_v20_ioctl_wait_t *args)
{
    int ret;
    ret = ioctl(fd, IOCTL_AIP_V20_WAIT, args); 
    return ret;
}

int aip_bo(int fd, aip_v20_ioctl_bo_t *args)
{
    int ret;
    ret = ioctl(fd, IOCTL_AIP_V20_BO, args); 
    return ret;
}

int aipv20_resize()
{
	int aipv20_fd; 
    int err;

	/* Required parameters for ioctl */
	aip_v20_f_node_t *fnode;
	aip_v20_ioctl_bo_t ioctl_bo_src_img;
	aip_v20_ioctl_bo_t ioctl_bo_dst_img;
	aip_v20_ioctl_bo_t ioctl_bo;
	aip_v20_ioctl_submit_t ioctl_submit;
	aip_v20_ioctl_wait_t ioctl_wait;

	/* Picture information parameters */
    FILE *image_fp;
    uint32_t src_w, src_h, dst_w, dst_h;
    uint32_t src_size,dst_size;
    uint32_t src_bpp, dst_bpp;
    uint32_t src_stride;
    uint32_t dst_stride;
    uint8_t *src_base = NULL, *dst_base = NULL;
    uint32_t src_base_y, src_base_uv;
    uint32_t *dst_base_y = NULL, *dst_base_uv = NULL;
    uint32_t offset_y, offset_uv;
    int box_x, box_y, box_h, box_w, box_num;
	float scale_x_f, scale_y_f;
	int fnode_size, dst_img_size;

	FILE *mem_fp;
	char mem_buf[256];
	long mem_avail = 0;
//======================================================
	mem_fp = fopen("/proc/meminfo", "r");
	if (mem_fp == NULL) {
		perror("Failed to open /proc/meminfo");
	}
	while (fgets(mem_buf, sizeof(mem_buf), mem_fp)) {
		if (sscanf(mem_buf, "MemAvailable: %ld", &mem_avail) == 1) {
			printf("Start:Available memory: %ld KB\n", mem_avail);
			break;
		}
	}
	fclose(mem_fp);
//======================================================

	/* open the device file */
    aipv20_fd = open("/dev/aip2.0", O_RDWR);
    if (aipv20_fd < 0) {
        perror("Failed to open device file");
    }

//======================================================
	mem_fp = fopen("/proc/meminfo", "r");
	if (mem_fp == NULL) {
		perror("Failed to open /proc/meminfo");
	}
	while (fgets(mem_buf, sizeof(mem_buf), mem_fp)) {
		if (sscanf(mem_buf, "MemAvailable: %ld", &mem_avail) == 1) {
			printf("Open:Available memory: %ld KB\n", mem_avail);
			break;
		}
	}
	fclose(mem_fp);
//======================================================

	/* Set original and target image information */
    src_w = 1024;
    src_h = 576;
    dst_w = 512;
    dst_h = 512;
    src_bpp = 12;
    dst_bpp = 12;
    box_num = 5;
    src_stride =  src_w;
    dst_stride =  dst_w;
    src_size = src_stride * src_h * src_bpp >> 3;
    dst_size = dst_stride * dst_h * dst_bpp >> 3;

	/* Set box information */
    box_x = 0;
    box_y = 0;
    box_w = src_w;
    box_h = src_h;

	/* resize ratio */
    scale_x_f = (float)box_w / (float)dst_w;
    scale_y_f = (float)box_h / (float)dst_h;

	/* apply for src_image space by ioctl_bo */
    ioctl_bo_src_img.op = AIP_BO_OP_APPEND;
    ioctl_bo_src_img.size = src_size;
    ioctl_bo_src_img.handle = 0; // Invalid configuration, aviod large random value
    ioctl_bo_src_img.paddr = NULL; // Invalid configuration, aviod wild pointer
    err = aip_bo(aipv20_fd, &ioctl_bo_src_img);
    if (err) {
        perror("Failed to bo(buffer object) IO request");
    }
	src_base = (uint8_t *)mmap(NULL, src_size, PROT_READ | PROT_WRITE, MAP_SHARED, aipv20_fd, (off_t)ioctl_bo_src_img.handle);
    memset(src_base, 0x00, src_size);

//======================================================
	mem_fp = fopen("/proc/meminfo", "r");
	if (mem_fp == NULL) {
		perror("Failed to open /proc/meminfo");
	}
	while (fgets(mem_buf, sizeof(mem_buf), mem_fp)) {
		if (sscanf(mem_buf, "MemAvailable: %ld", &mem_avail) == 1) {
			printf("Bo_src:Available memory: %ld KB\n", mem_avail);
			break;
		}
	}
	fclose(mem_fp);
//======================================================

	/* apply for dst_image space by ioctl_bo */
	dst_img_size = dst_size * box_num;
    ioctl_bo_dst_img.op = AIP_BO_OP_APPEND;
    ioctl_bo_dst_img.size = dst_img_size;
    ioctl_bo_dst_img.handle = 0; // Invalid configuration, aviod large random value
    ioctl_bo_dst_img.paddr = NULL; // Invalid configuration, aviod wild pointer
    err = aip_bo(aipv20_fd, &ioctl_bo_dst_img);
    if (err) {
        perror("Failed to bo(buffer object) IO request");
    }
	dst_base = (uint8_t *)mmap(NULL, dst_img_size, PROT_READ | PROT_WRITE, MAP_SHARED, aipv20_fd, (off_t)ioctl_bo_dst_img.handle);
    memset(dst_base, 0x00, dst_img_size);
    
	image_fp = fopen("./model/day-T40XP_273_w1024h576_w1024_h576.nv12","r");
    if(image_fp == NULL) {
        printf("aip_f fopen failed!\n");
    }
    fread(src_base, 32, src_size/32, image_fp);
    if(fclose(image_fp) < 0){
        printf("aip_f flcose faile!\n");
    }

//======================================================
	mem_fp = fopen("/proc/meminfo", "r");
	if (mem_fp == NULL) {
		perror("Failed to open /proc/meminfo");
	}
	while (fgets(mem_buf, sizeof(mem_buf), mem_fp)) {
		if (sscanf(mem_buf, "MemAvailable: %ld", &mem_avail) == 1) {
			printf("Bo_dst:Available memory: %ld KB\n", mem_avail);
			break;
		}
	}
	fclose(mem_fp);
//======================================================

	src_base_y = (uint32_t)ioctl_bo_src_img.paddr;
    src_base_uv = src_base_y + src_w * src_h;
    offset_y    = src_stride * box_y + box_x;
    offset_uv   = src_stride * box_y / 2 + box_x;

	dst_base_y = (uint32_t *)malloc(box_num * sizeof(uint32_t));
	dst_base_uv = (uint32_t *)malloc(box_num * sizeof(uint32_t));

//======================================================
	mem_fp = fopen("/proc/meminfo", "r");
	if (mem_fp == NULL) {
		perror("Failed to open /proc/meminfo");
	}
	while (fgets(mem_buf, sizeof(mem_buf), mem_fp)) {
		if (sscanf(mem_buf, "MemAvailable: %ld", &mem_avail) == 1) {
			printf("malloc:Available memory: %ld KB\n", mem_avail);
			break;
		}
	}
	fclose(mem_fp);
//======================================================

	free(dst_base_uv);
	free(dst_base_y);

//======================================================
	mem_fp = fopen("/proc/meminfo", "r");
	if (mem_fp == NULL) {
		perror("Failed to open /proc/meminfo");
	}
	while (fgets(mem_buf, sizeof(mem_buf), mem_fp)) {
		if (sscanf(mem_buf, "MemAvailable: %ld", &mem_avail) == 1) {
			printf("free_ma:Available memory: %ld KB\n", mem_avail);
			break;
		}
	}
	fclose(mem_fp);
//======================================================

	/* destroy dst_img space */
	munmap(dst_base, dst_img_size);
	ioctl_bo_dst_img.op = AIP_BO_OP_DESTROY;
    err = aip_bo(aipv20_fd, &ioctl_bo_dst_img);
    if (err) {
        perror("Failed to bo(buffer object) IO request");
    }

//======================================================
	mem_fp = fopen("/proc/meminfo", "r");
	if (mem_fp == NULL) {
		perror("Failed to open /proc/meminfo");
	}
	while (fgets(mem_buf, sizeof(mem_buf), mem_fp)) {
		if (sscanf(mem_buf, "MemAvailable: %ld", &mem_avail) == 1) {
			printf("ds_dst:Available memory: %ld KB\n", mem_avail);
			break;
		}
	}
	fclose(mem_fp);
//======================================================

	/* destroy src_img space */
	munmap(src_base, src_size);
	ioctl_bo_src_img.op = AIP_BO_OP_DESTROY;
    err = aip_bo(aipv20_fd, &ioctl_bo_src_img);
    if (err) {
        perror("Failed to bo(buffer object) IO request");
    }

	close(aipv20_fd);

//======================================================
	mem_fp = fopen("/proc/meminfo", "r");
	if (mem_fp == NULL) {
		perror("Failed to open /proc/meminfo");
	}
	while (fgets(mem_buf, sizeof(mem_buf), mem_fp)) {
		if (sscanf(mem_buf, "MemAvailable: %ld", &mem_avail) == 1) {
			printf("ds_src:Available memory: %ld KB\n", mem_avail);
			break;
		}
	}
	fclose(mem_fp);
//======================================================
	return 0;
}

    
int main()
{
	aipv20_resize();
	return 0;
}


