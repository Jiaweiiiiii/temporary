/*
 * =======================================================================
 *       Filename:  aipv20_resize.c
 *
 *    Description: This test case is used to verify whether the results
 *                 of the AIP_F and the opencv on processing FMU2 images
 *                 are consistent.
 *
 *        Created:  06/19/2023 04:47:57 PM
 *       Compiler:  mips-linux-gnu-gcc & -static & -muclibc
 *
 *         Author: jwzhang 
 * =======================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <unistd.h>
#include "iaic.h"

void format_set(aip_v20_ioctl_f_format_t format,
		int *src_bpp, int *dst_bpp, int *chn);

int aipv20_resize(aip_v20_ioctl_f_format_t format, int box_num)
{
	iaic_ctx_t iaic_ctx;
	iaic_bo_t src_bo;
	iaic_bo_t dst_bo;
	iaic_bo_t node_bo;
	int src_w, src_h, dst_w, dst_h, chn;
	int box_w, box_h, box_x, box_y;
	int src_bpp, dst_bpp, src_stride, dst_stride, src_size, dst_size;
	FILE *img_fp;
	int ret, i;
	int dst_total_size, node_size;
	int scale_x, scale_y;
	aip_v20_f_node_t *fnode;
	int src_pbase_y, src_pbase_uv, offset_y, offset_uv;
	uint64_t job_seqno, job_timeout_ns;
	int64_t job_wait_time;

	src_w = 200;
	src_h = 200;
	
	box_w = src_w;
	box_h = src_h;
	box_x = 0;
	box_y = 0;

	dst_w = 100;
	dst_h = 100;

	
	format_set(format, &src_bpp, &dst_bpp, &chn);
	if(format == AIP_F_FORMAT_NV12) {
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

	// Init ctx.
	ret = iaic_ctx_init(&iaic_ctx, 0x100000);
	if (ret) {
		printf("iaic_ctx_init failed, ret = %d\n", ret);
		return -ret;
	}

	// Create a BO(buff object) to save image.
	ret = iaic_create_bo(&iaic_ctx, src_size, &src_bo);
	if (ret) {
		perror("src_bo failed");
		return -ret;
	}
	//img_fp = fopen("./model/day_w1024_h576.nv12","r");
	img_fp = fopen("./model/FMU2_w200_h200.data","r");
	if(img_fp == NULL) {
		perror("image fopen failed");
		return -1;
	}
	fread(src_bo.vaddr, 32, src_size/32, img_fp);
	if(fclose(img_fp) < 0){
		perror("image fclose failed");
		return -1;
	}

	// Create a BO for the next operation of saving processed image.
	dst_total_size = dst_size * box_num;
	ret = iaic_create_bo(&iaic_ctx, dst_total_size, &dst_bo);
	if (ret) {
		perror("dst_bo failed");
		return -1;
	}

	// The BO used to save nodes.
	node_size = sizeof(aip_v20_f_node_t) * box_num;
	ret = iaic_create_fnode(&iaic_ctx, node_size, &node_bo);
	if (ret) {
		perror("node_bo failed");
		return -1;
	}

	// Node Data
	scale_x = (float)box_w / (float)dst_w;
	scale_y = (float)box_h / (float)dst_h;

	src_pbase_y = (uint32_t)src_bo.paddr;
	src_pbase_uv = src_pbase_y + src_w * src_h;
	offset_y = src_stride * box_y + box_x;
	offset_uv = src_stride * box_y / 2 + box_x;

	fnode = (aip_v20_f_node_t *)node_bo.vaddr;
	for (i = 0; i < box_num; i++) {
		fnode[i].timeout[0] = 0xffffffff;
		fnode[i].scale_x[0] = (uint32_t)(scale_x * 65536);
		fnode[i].scale_y[0] = (uint32_t)(scale_y * 65536);
		fnode[i].trans_x[0] = (int32_t)((scale_x * 0.5 - 0.5) * 65536) + 16;
		fnode[i].trans_y[0] = (int32_t)((scale_y * 0.5 - 0.5) * 65536) + 16;
		fnode[i].src_base_y[0] = src_pbase_y + offset_y;
		fnode[i].src_base_uv[0] = src_pbase_uv + offset_uv;
		fnode[i].dst_base_y[0] = (uint32_t)dst_bo.paddr + (i * dst_size);
		fnode[i].dst_base_uv[0] = fnode[i].dst_base_y[0] + dst_w * dst_h;
		fnode[i].src_size[0] = box_h<<16 | box_w;
		fnode[i].dst_size[0] = dst_h<<16 | dst_w;
		fnode[i].stride[0] = dst_stride<<16 | src_stride;
	}

	// Run AIP_F
	ret = iaic_aip_f_chain(&iaic_ctx,
			format,
			AIP_DAT_POS_DDR,
			AIP_DAT_POS_DDR,
			&node_bo,
			box_num,
			NULL,
			&job_seqno);
	if (ret) {
		printf("Failed to submit IO request, ret = %d\n", ret);
		return -1;
	}

	// Waiting for AIP_F execution to complete
	job_timeout_ns = -1ll;
	ret = iaic_aip_f_wait(&iaic_ctx, job_seqno, job_timeout_ns, &job_wait_time);
	if (ret) {
		printf("Failed to wait, ret = %d\n", ret);
		return -1;
	}


	// Output Image
#if 0
	void *dst_vbase = NULL;
	dst_vbase = dst_bo.vaddr;
	FILE *dst_fp;
	char name [32];
	for (i = 0; i < box_num; i++){
		sprintf(name, "%s%d", "./dst_image", i);
		dst_fp = fopen(name, "w+");
		if(dst_fp == NULL) {
			perror("dst_image fopen");
			break;
		}
		fwrite(dst_vbase + (dst_size * i), 32, dst_size/32, dst_fp);
		//printf("Successfully obtained image[%d] information\n", i);
		if(fclose(dst_fp) != 0){
			perror("dst_image fclose");
			break;
		}
	}
#endif

	// other testing

	// Release resources
	ret = iaic_destroy_bo(&iaic_ctx, &node_bo);
	ret = iaic_destroy_bo(&iaic_ctx, &dst_bo);
	ret = iaic_destroy_bo(&iaic_ctx, &src_bo);
	ret = iaic_ctx_destroy(&iaic_ctx);
	return ret;
}


int main(int argc, char *argv[])
{
	int ret;
	aip_v20_ioctl_f_format_t format;
	int box_num;

	format = AIP_F_FORMAT_FEATURE2;
	box_num = 1;


	ret = aipv20_resize(format, box_num);
	if (ret != 0)
		printf("aipv20_resize failed!\n");

	return 0;
}

void format_set(aip_v20_ioctl_f_format_t format,
		int *src_bpp, int *dst_bpp, int *chn)
{
	switch (format) {
	case AIP_F_FORMAT_Y:
		*src_bpp = 8;
		*dst_bpp = 8;
		*chn = 1;
		break;
	case AIP_F_FORMAT_C:
		*src_bpp = 8;
		*dst_bpp = 8;
		*chn = 2;
		break;
	case AIP_F_FORMAT_BGRA:
		*src_bpp = 8;
		*dst_bpp = 8;
		*chn = 4;
		break;
	case AIP_F_FORMAT_FEATURE2:
		*src_bpp = 2;
		*dst_bpp = 2;
		*chn = 32;
		break;
	case AIP_F_FORMAT_FEATURE4:
		*src_bpp = 4;
		*dst_bpp = 4;
		*chn = 32;
		break;
	case AIP_F_FORMAT_FEATURE8:
		*src_bpp = 8;
		*dst_bpp = 8;
		*chn = 32;
		break;
	case AIP_F_FORMAT_FEATURE8_H:
		*src_bpp = 8;
		*dst_bpp = 8;
		*chn = 16;
		break;
	case AIP_F_FORMAT_NV12:
		*src_bpp = 12;
		*dst_bpp = 12;
		*chn = 1;
		break;
	default:
		printf("Unsupported image format.\n");
	}
}
