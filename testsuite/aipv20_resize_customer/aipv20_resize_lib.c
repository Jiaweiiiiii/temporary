/*
 * =======================================================================
 *       Filename:  aipv20_resize_lib.c
 *
 *    Description:
 *
 *        Created:  07/10/2023 02:10 PM
 *       Compiler:  mips-linux-gnu-gcc & -static & -muclibc
 *
 *         Author: jwzhang 
 * =======================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <unistd.h>
#include "aipv20_resize_lib.h"


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

void set_bpp(aip_v20_ioctl_f_format_t format,
		int *bpp, int *chn)
{
	switch (format) {
	case AIP_F_FORMAT_Y:
		*bpp = 8;
		*chn = 1;
		break;
	case AIP_F_FORMAT_C:
		*bpp = 8;
		*chn = 2;
		break;
	case AIP_F_FORMAT_BGRA:
		*bpp = 8;
		*chn = 4;
		break;
	case AIP_F_FORMAT_FEATURE2:
		*bpp = 2;
		*chn = 32;
		break;
	case AIP_F_FORMAT_FEATURE4:
		*bpp = 4;
		*chn = 32;
		break;
	case AIP_F_FORMAT_FEATURE8:
		*bpp = 8;
		*chn = 32;
		break;
	case AIP_F_FORMAT_FEATURE8_H:
		*bpp = 8;
		*chn = 16;
		break;
	case AIP_F_FORMAT_NV12:
		*bpp = 12;
		*chn = 1;
		break;
	default:
		printf("Unsupported image format.\n");
	}
}



img_info_t img_read(iaic_ctx_t *ctx, char *str,
	size_t wid, size_t hei,
	aip_v20_ioctl_f_format_t format)
{
	int ret;
	FILE *img_fp;
	img_info_t src_img;
	size_t src_stride, src_size;
	int bpp, chn;

	set_bpp(format, &bpp, &chn);
	if(format == AIP_F_FORMAT_NV12) {
		src_stride =  wid;
		src_size = src_stride * hei * bpp >> 3;
	} else {
		src_stride =  wid * chn * bpp >> 3;
		src_size = src_stride * hei;
	}

	ret = iaic_create_bo(ctx, src_size, &src_img.bo);
	if (ret) {
		perror("src_bo failed");
		src_img.bo.vaddr = NULL;
		return src_img;
	}
	img_fp = fopen(str, "r");
	if(img_fp == NULL) {
		perror("image fopen failed");
		iaic_destroy_bo(ctx, &src_img.bo);
		src_img.bo.vaddr = NULL;
		return src_img;
	}
	fread(src_img.bo.vaddr, 32, src_size/32, img_fp);
	if(fclose(img_fp) < 0){
		perror("image fclose failed");
		iaic_destroy_bo(ctx, &src_img.bo);
		src_img.bo.vaddr = NULL;
		return src_img;
	}

	src_img.wid = wid;
	src_img.hei = hei;
	src_img.stride = src_stride;
	src_img.size = src_size;

	return src_img;
}

img_info_t img_resize(iaic_ctx_t *ctx, img_info_t src_img,
	size_t wid, size_t hei,
	aip_v20_ioctl_f_format_t format)
{
	int ret;
	img_info_t dst_img;
	size_t dst_stride, dst_size;
	int bpp, chn;
	iaic_bo_t node_bo;
	aip_v20_f_node_t *fnode;
	size_t node_size;
	float scale_x, scale_y;
	uint64_t src_pbase_y, src_pbase_uv;
	uint64_t dst_pbase_y, dst_pbase_uv;
	uint64_t job_seqno, job_timeout_ns;
	int64_t job_wait_time;

	set_bpp(format, &bpp, &chn);
	if(format == AIP_F_FORMAT_NV12) {
		dst_stride =  wid;
		dst_size = dst_stride * hei * bpp >> 3;
	} else {
		dst_stride =  wid * chn * bpp >> 3;
		dst_size = dst_stride * hei;
	}

	ret = iaic_create_bo(ctx, dst_size, &dst_img.bo);
	if (ret) {
		perror("dst_bo failed");
		dst_img.bo.vaddr = NULL;
		return src_img;
	}

	node_size = sizeof(aip_v20_f_node_t);
	ret = iaic_create_fnode(ctx, node_size, &node_bo);
	if (ret) {
		perror("node_bo failed");
		iaic_destroy_bo(ctx, &dst_img.bo);
		dst_img.bo.vaddr = NULL;
		return src_img;
	}

	scale_x = (float)src_img.wid / (float)wid;
	scale_y = (float)src_img.hei / (float)hei;

	src_pbase_y = (uint32_t)src_img.bo.paddr;
	src_pbase_uv = src_pbase_y + src_img.wid * src_img.hei;
	dst_pbase_y = (uint32_t)dst_img.bo.paddr;
	dst_pbase_uv = dst_pbase_y + wid * hei;

	fnode = (aip_v20_f_node_t *)node_bo.vaddr;
	fnode->timeout[0] = 0xffffffff;
	fnode->scale_x[0] = (uint32_t)(scale_x * 65536);
	fnode->scale_y[0] = (uint32_t)(scale_y * 65536);
	fnode->trans_x[0] = (int32_t)((scale_x *
			0.5 - 0.5) * 65536) + 16;
	fnode->trans_y[0] = (int32_t)((scale_y *
			0.5 - 0.5) * 65536) + 16;
	fnode->src_base_y[0] = src_pbase_y;
	fnode->src_base_uv[0] = src_pbase_uv;
	fnode->dst_base_y[0] = dst_pbase_y;
	fnode->dst_base_uv[0] = dst_pbase_uv;
	fnode->src_size[0] = src_img.hei<<16 | src_img.wid;
	fnode->dst_size[0] = hei<<16 | wid;
	fnode->stride[0] = dst_stride<<16 | src_img.stride;

	ret = iaic_aip_f_chain(ctx, format,
			AIP_DAT_POS_DDR,
			AIP_DAT_POS_DDR,
			node_bo.handle,
			1, NULL, &job_seqno);
	if (ret != 0) {
		printf("Failed to submit IO request, ret = %d\n", ret);
		iaic_destroy_bo(ctx, &dst_img.bo);
		dst_img.bo.vaddr = NULL;
		return src_img;
	}

	dst_img.wid = wid;
	dst_img.hei = hei;
	dst_img.stride = dst_stride;
	dst_img.size = dst_size;

	job_timeout_ns = -1ll;
	ret = iaic_aip_f_wait(ctx, job_seqno, job_timeout_ns, &job_wait_time);
	if (ret) {
		printf("Failed to wait, ret = %d\n", ret);
		iaic_destroy_bo(ctx, &dst_img.bo);
		dst_img.bo.vaddr = NULL;
		return src_img;
	}
	iaic_destroy_bo(ctx, &node_bo);

	return dst_img;
}

int img_show(img_info_t dst_img, char *str)
{
	FILE *dst_fp;
	void *dst_vbase = NULL;
	dst_vbase = dst_img.bo.vaddr;
	dst_fp = fopen(str, "w+");
	if(dst_fp == NULL) {
		perror("dst_image fopen");
		return -1;
	}
	fwrite(dst_vbase, 32, dst_img.size/32, dst_fp);
	if(fclose(dst_fp) != 0){
		perror("dst_image fclose");
		return -1;
	}

	return 0;
}
