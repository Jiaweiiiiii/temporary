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

void opencv_resize_fmu(uint8_t *src_base, int src_w, int src_h, int src_chn,
		uint8_t *dst_base, int dst_w, int dst_h, int dst_chn, int type);

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
#if 1
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
	iaic_bo_t dst_bo2;
	ret = iaic_create_bo(&iaic_ctx, dst_total_size, &dst_bo2);
	if (ret) {
		perror("dst_bo failed");
		return -1;
	}

	opencv_resize_fmu((uint8_t *)src_bo.vaddr, src_w, src_h, chn,
			(uint8_t *)dst_bo2.vaddr, dst_w, dst_h, chn, 2);


	char *dst_vbase1 = NULL;
	char *dst_vbase2 = NULL;
	dst_vbase1 = (char *)dst_bo.vaddr;
	dst_vbase2 = (char *)dst_bo2.vaddr;

	for (i = 0; i < dst_size; i++) {
		if (dst_vbase1[i] != dst_vbase2[i]) {
			printf("[%d]value1=%d, value2=%d, base1=0x%x, base2=0x%x\n", i,
					dst_vbase1[i], dst_vbase2[i], (int)&dst_vbase1[i], (int)&dst_vbase2[i]);
			break;
		}
	}

	if (i == dst_size) {
		printf("RIGHT:Same numerical value.\n");
	} else {
		printf("ERROR:Different values.\n");
	}

	// Release resources
	ret = iaic_destroy_bo(&iaic_ctx, &dst_bo2);
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

void opencv_resize_fmu(uint8_t *src_base, int src_w, int src_h, int src_chn,
		uint8_t *dst_base, int dst_w, int dst_h, int dst_chn, int type) {
    double inv_scale_x = (double)src_w / (double)dst_w;
    double inv_scale_y = (double)src_h / (double)dst_h;
    int trans_x = (int)((inv_scale_x * 0.5 - 0.5) * 65536);
    int trans_y = (int)((inv_scale_y * 0.5 - 0.5) * 65536);
    int scale_x = inv_scale_x * 65536;
    int scale_y = inv_scale_y * 65536;
    int32_t src_fx = trans_x; // src float x
    int32_t src_fy = trans_y; // src float y
    int src_stride;
    int dst_stride;
    int bpp;
    if (type == 2) {
        src_stride = src_w * src_chn / 4;
        dst_stride = dst_w * dst_chn / 4;
        bpp = 8;
    } else if (type == 4) {
        src_stride = src_w * src_chn / 2;
        dst_stride = dst_w * dst_chn / 2;
        bpp = 16;
    } else if (type == 8) {
        src_stride = src_w * src_chn;
        dst_stride = dst_w * dst_chn;
        bpp = 32;
    } else if (type == 16) {
        src_stride = src_w * src_chn;
        dst_stride = dst_w * dst_chn;
        bpp = 16;
    }

    int dst_y = 0;
    for (; dst_y < dst_h; dst_y++) {
        src_fx = trans_x;
        int dst_x = 0;
        for (; dst_x < dst_w; dst_x++) {
            int src_fx_round = src_fx + 16;
            int src_fy_round = src_fy + 16;
            int src_x = (src_fx_round) >> 16;
            int src_y = (src_fy_round) >> 16;
            int w_x1 = (src_fx_round & 0xFFFF) >> 5;
            int w_y1 = (src_fy_round & 0xFFFF) >> 5;
            int w_x0 = 2048 - w_x1;
            int w_y0 = 2048 - w_y1;
            int src_x0 = src_x;
            int src_x1 = src_x + 1;
            int src_y0 = src_y;
            int src_y1 = src_y + 1;
            if (src_x < 0) {
                src_x0 = 0;
                src_x1 = 0;
            }
            if (src_y < 0) {
                src_y0 = 0;
                src_y1 = 0;
            }

            if (src_x >= src_w - 1) {
                src_x0 = src_w - 1;
                src_x1 = src_w - 1;
            }
            if (src_y >= src_h - 1) {
                src_y0 = src_h - 1;
                src_y1 = src_h - 1;
            }
            int k = 0;
            for (; k < bpp; k++) {
                int d0_idx = 0;
                int d1_idx = 0;
                int d2_idx = 0;
                int d3_idx = 0;
                uint8_t d0 = 0;
                uint8_t d1 = 0;
                uint8_t d2 = 0;
                uint8_t d3 = 0;
                {
                    d0_idx = src_y0 * src_stride + src_x0 * bpp + k;
                    d1_idx = src_y0 * src_stride + src_x1 * bpp + k;
                    d2_idx = src_y1 * src_stride + src_x0 * bpp + k;
                    d3_idx = src_y1 * src_stride + src_x1 * bpp + k;
                    d0 = src_base[d0_idx];
                    d1 = src_base[d1_idx];
                    d2 = src_base[d2_idx];
                    d3 = src_base[d3_idx];
                }
                if (type == 8 || type == 16) {
                    uint32_t v0 = (d0 * w_x0 + d1 * w_x1) >> 4;
                    uint32_t v1 = (d2 * w_x0 + d3 * w_x1) >> 4;
                    uint32_t res = (((v0 * w_y0) >> 16) + ((v1 * w_y1) >> 16) + 2) >> 2;
                    int dst_idx = dst_y * dst_stride + dst_x * bpp + k;
                    dst_base[dst_idx] = (uint8_t)(res & 0xFF);
                } else if (type == 2) {
                    uint8_t res = 0;
                    for (int b = 0; b < 4; b++) {
                        uint8_t d0_2b = (d0 >> (b * 2)) & 0x3;
                        uint8_t d1_2b = (d1 >> (b * 2)) & 0x3;
                        uint8_t d2_2b = (d2 >> (b * 2)) & 0x3;
                        uint8_t d3_2b = (d3 >> (b * 2)) & 0x3;
                        uint32_t v0 = (d0_2b * w_x0 + d1_2b * w_x1) >> 4;
                        uint32_t v1 = (d2_2b * w_x0 + d3_2b * w_x1) >> 4;
                        uint32_t res_2b = (((v0 * w_y0) >> 16) + ((v1 * w_y1) >> 16) + 2) >> 2;
                        res |= res_2b << (b * 2);
                    }
                    int dst_idx = dst_y * dst_stride + dst_x * bpp + k;
                    dst_base[dst_idx] = (uint8_t)(res & 0xFF);
                } else if (type == 4) {
                    uint8_t res = 0;
                    for (int b = 0; b < 2; b++) {
                        uint8_t d0_4b = (d0 >> (b * 4)) & 0xF;
                        uint8_t d1_4b = (d1 >> (b * 4)) & 0xF;
                        uint8_t d2_4b = (d2 >> (b * 4)) & 0xF;
                        uint8_t d3_4b = (d3 >> (b * 4)) & 0xF;
                        uint32_t v0 = (d0_4b * w_x0 + d1_4b * w_x1) >> 4;
                        uint32_t v1 = (d2_4b * w_x0 + d3_4b * w_x1) >> 4;
                        uint32_t res_4b = (((v0 * w_y0) >> 16) + ((v1 * w_y1) >> 16) + 2) >> 2;
                        res |= res_4b << (b * 4);
                    }
                    int dst_idx = dst_y * dst_stride + dst_x * bpp + k;
                    dst_base[dst_idx] = (uint8_t)(res & 0xFF);
                }
            }
            src_fx += scale_x;
        }
        src_fy += scale_y;
    }
}

