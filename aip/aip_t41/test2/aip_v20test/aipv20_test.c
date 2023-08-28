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

	/* Required parameters for timing */
    struct timeval start_time, end_time;
    double total_time;

	/* open the device file */
    aipv20_fd = open("/dev/aip2.0", O_RDWR);
    if (aipv20_fd < 0) {
        perror("Failed to open device file");
        goto err_fd;
    }

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
        goto err_bo_src;
    }
	src_base = (uint8_t *)mmap(NULL, src_size, PROT_READ | PROT_WRITE, MAP_SHARED, aipv20_fd, (off_t)ioctl_bo_src_img.handle);
    memset(src_base, 0x00, src_size);

	/* apply for dst_image space by ioctl_bo */
	dst_img_size = dst_size * box_num;
    ioctl_bo_dst_img.op = AIP_BO_OP_APPEND;
    ioctl_bo_dst_img.size = dst_img_size;
    ioctl_bo_dst_img.handle = 0; // Invalid configuration, aviod large random value
    ioctl_bo_dst_img.paddr = NULL; // Invalid configuration, aviod wild pointer
    err = aip_bo(aipv20_fd, &ioctl_bo_dst_img);
    if (err) {
        perror("Failed to bo(buffer object) IO request");
        goto err_bo_dst;
    }
	dst_base = (uint8_t *)mmap(NULL, dst_img_size, PROT_READ | PROT_WRITE, MAP_SHARED, aipv20_fd, (off_t)ioctl_bo_dst_img.handle);
    memset(dst_base, 0x00, dst_img_size);
    
	image_fp = fopen("./model/day-T40XP_273_w1024h576_w1024_h576.nv12","r");
    if(image_fp == NULL) {
        printf("aip_f fopen failed!\n");
        goto err_fnode;
    }
    fread(src_base, 32, src_size/32, image_fp);
    if(fclose(image_fp) < 0){
        printf("aip_f flcose faile!\n");
        goto err_fnode;
    }

	src_base_y = (uint32_t)ioctl_bo_src_img.paddr;
    src_base_uv = src_base_y + src_w * src_h;
    offset_y    = src_stride * box_y + box_x;
    offset_uv   = src_stride * box_y / 2 + box_x;

	dst_base_y = (uint32_t *)malloc(box_num * sizeof(uint32_t));
	dst_base_uv = (uint32_t *)malloc(box_num * sizeof(uint32_t));

	/* apply for fnode space by ioctl_bo */
	fnode_size = sizeof(aip_v20_f_node_t) * box_num;
    ioctl_bo.op = AIP_BO_OP_APPEND;
    ioctl_bo.size = fnode_size;
    ioctl_bo.handle = 0; // Invalid configuration, aviod large random value
    ioctl_bo.paddr = NULL; // Invalid configuration, aviod wild pointer
    err = aip_bo(aipv20_fd, &ioctl_bo);
    if (err) {
        perror("Failed to bo(buffer object) IO request");
        goto err_fnode;
    }

	/* apply for node space and configure information */
	fnode = (aip_v20_f_node_t *)mmap(NULL, fnode_size, PROT_READ | PROT_WRITE, MAP_SHARED, aipv20_fd, (off_t)ioctl_bo.handle);
	for (int i = 0; i < box_num; i++){
		dst_base_y[i] = (uint32_t)ioctl_bo_dst_img.paddr + (i * dst_size);
		dst_base_uv[i] = dst_base_y[i] + dst_w * dst_h;

		fnode[i].timeout[0] = 0xffffffff;
		fnode[i].scale_x[0] = (uint32_t)(scale_x_f * 65536);
		fnode[i].scale_y[0] = (uint32_t)(scale_y_f * 65536);
		fnode[i].trans_x[0] = (int32_t)((scale_x_f * 0.5 - 0.5) * 65536) + 16;
		fnode[i].trans_y[0] = (int32_t)((scale_y_f * 0.5 - 0.5) * 65536) + 16;
		fnode[i].src_base_y[0] = src_base_y + offset_y;
		fnode[i].src_base_uv[0] = src_base_uv + offset_uv;
		fnode[i].dst_base_y[0] = dst_base_y[i];
		fnode[i].dst_base_uv[0] = dst_base_uv[i];
		fnode[i].src_size[0] = box_h<<16 | box_w;;
		fnode[i].dst_size[0] = dst_h<<16 | dst_w;
		fnode[i].stride[0] = dst_stride<<16 | src_stride;;
	}

	gettimeofday(&start_time, NULL);
	/* aipv20_submit */
    ioctl_submit.op = AIP_CH_OP_F;
    ioctl_submit.oprds.f.format = AIP_F_FORMAT_NV12; // NV12
    ioctl_submit.oprds.f.pos_in = AIP_DAT_POS_DDR;  // SRC_DDR
    ioctl_submit.oprds.f.pos_out = AIP_DAT_POS_DDR; // DST_DDR
    ioctl_submit.oprds.f.node_handle = ioctl_bo.handle;
    ioctl_submit.oprds.f.node_num = box_num;
    ioctl_submit.oprds.f.seqno = 0; // Invalid configuration, aviod large random value
    err = aip_submit(aipv20_fd, &ioctl_submit);
    if (err) {
        perror("Failed to submit IO request");
        goto err_free;
    }

	/* aipv20_wait */
    ioctl_wait.op = AIP_CH_OP_F;
    ioctl_wait.seqno = ioctl_submit.oprds.f.seqno;
    ioctl_wait.timeout_ns = 1 * 1000 * 1000 * 1000; // 1s
    err = aip_wait(aipv20_fd, &ioctl_wait);
    if (err) {
        perror("Failed to wait IO request\n");
        goto err_free;
    }

	gettimeofday(&end_time, NULL);
	total_time = (double)((end_time.tv_sec * 1000000 + end_time.tv_usec) - (start_time.tv_sec * 1000000 + start_time.tv_usec)) /1000.0;
    printf("The aipv20_resize runs for %.3lf ms\n", total_time);

#if 0
	/* Get target image */
	for (int i = 0; i < box_num; i++){
		FILE *dst_fp[i];
		char name [32][i];
		sprintf(name[i], "%s%d", "./dst_image", i);
		dst_fp[i] = fopen(name[i], "w+");
		if(dst_fp[i] == NULL) {
			printf("aip_f[%d] fopen failed!\n", i);
			goto err_free;
		}
		fwrite(dst_base + (dst_size * i), 32, dst_size/32, dst_fp[i]);
		printf("Successfully obtained image[%d] information\n", i);
		if(fclose(dst_fp[i]) < 0){ 
			printf("aip_f[%d] flcose faile!\n", i);
			goto err_free;
		}
	}
#endif

	/* destroy fnode space */
	munmap(fnode, fnode_size);
	ioctl_bo.op = AIP_BO_OP_DESTROY;
    err = aip_bo(aipv20_fd, &ioctl_bo);
    if (err) {
        perror("Failed to bo(buffer object) IO request");
        goto err_free;
    }

	free(dst_base_uv);
	free(dst_base_y);

	/* destroy dst_img space */
	munmap(dst_base, dst_img_size);
	ioctl_bo_dst_img.op = AIP_BO_OP_DESTROY;
    err = aip_bo(aipv20_fd, &ioctl_bo_dst_img);
    if (err) {
        perror("Failed to bo(buffer object) IO request");
        goto err_free;
    }

	/* destroy src_img space */
	munmap(src_base, src_size);
	ioctl_bo_src_img.op = AIP_BO_OP_DESTROY;
    err = aip_bo(aipv20_fd, &ioctl_bo_src_img);
    if (err) {
        perror("Failed to bo(buffer object) IO request");
        goto err_free;
    }

	close(aipv20_fd);
    return 0;

err_free:
	munmap(fnode, fnode_size);
	/* destroy fnode space */
	ioctl_bo.op = AIP_BO_OP_DESTROY;
    err = aip_bo(aipv20_fd, &ioctl_bo);
    if (err) {
        perror("Failed to bo(buffer object) IO request");
        goto err_free;
    }
	free(dst_base_uv);
	free(dst_base_y);
err_fnode:
	munmap(dst_base, dst_img_size);
	/* destroy dst_img space */
	ioctl_bo_dst_img.op = AIP_BO_OP_DESTROY;
    err = aip_bo(aipv20_fd, &ioctl_bo_dst_img);
    if (err) {
        perror("Failed to bo(buffer object) IO request");
        goto err_free;
    }
err_bo_dst:
	munmap(src_base, src_size);
	/* destroy src_img space */
	ioctl_bo_src_img.op = AIP_BO_OP_DESTROY;
    err = aip_bo(aipv20_fd, &ioctl_bo_src_img);
    if (err) {
        perror("Failed to bo(buffer object) IO request");
        goto err_free;
    }
err_bo_src:
	close(aipv20_fd);
err_fd:
	return -1;
}

    
int main()
{
	aipv20_resize();
	return 0;
}


