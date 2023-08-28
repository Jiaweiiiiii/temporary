#include <stdio.h>
#include <string.h>
#include <math.h>
#include "iaic.h"



int aipv20_perspective()
{

	int ret, i;
	FILE *img_fp;
	iaic_ctx_t iaic_ctx;
	iaic_bo_t src_bo;
	iaic_bo_t dst_bo;
	iaic_bo_t node_bo;
	uint64_t job_seqno, job_timeout_ns;
	uint32_t src_w, src_h, dst_w, dst_h, src_bpp, dst_bpp, src_stride, dst_stride;
	uint32_t box_w, box_h, box_x, box_y, box_num;
	size_t src_size, dst_size, dst_total_size, node_size;
	void *src_vbase = NULL;
	iaic_p_node_t *pnode;
	uint32_t src_pbase_y, src_pbase_uv, offset_y, offset_uv;
	int64_t ax,bx,cx,ay,by,cy,az,bz,cz;
	double Dxx, A11, A22, b1, b2;
	double t0, t1, t2, t3, dst_m[9] = {0};

	float matrix[9] = {
		0.5, 0, 0,
		0, 0.8888, 0,
		0, 0, 1.0
	};

	uint32_t nv2bgr_coef[9] = {
		1220542, 2116026, 0,
		1220542, 409993, 852492,
		1220542, 0, 1673527
	};

	src_w = 1024;
	src_h = 576;
	dst_w = 512;
	dst_h = 512;

	box_num = 1;
	box_w = src_w;
	box_h = src_h;
	box_x = 0;
	box_y = 0;

	src_bpp = 12;
	dst_bpp = 32;
	src_stride = src_w;
	dst_stride = dst_w * dst_bpp >> 3;
	src_size = src_stride * src_h * src_bpp >> 3;
	dst_size = dst_stride * dst_h;

	node_size = sizeof(iaic_p_node_t) * box_num;
	dst_total_size = dst_size * box_num;

	ret = iaic_ctx_init(&iaic_ctx, 0x200000);
	if (ret) {
		printf("iaic_ctx_init failed, ret = %d\n", ret);
		return -1;
	}
	ret = iaic_create_bo(&iaic_ctx, src_size, &src_bo);
	if (ret) {
		perror("src_bo failed");
		return -1;
	}
	ret = iaic_create_bo(&iaic_ctx, dst_total_size, &dst_bo);
	if (ret) {
		perror("dst_bo failed");
		return -1;
	}
	ret = iaic_create_pnode(&iaic_ctx, node_size, &node_bo);
	if (ret) {
		perror("node_bo failed");
		return -1;
	}

	src_vbase = src_bo.vaddr;

	img_fp = fopen("./model/day_w1024_h576.nv12","r");
	if(img_fp == NULL) {
		perror("image fopen failed");
		return -1;
	}
	fread(src_vbase, 32, src_size/32, img_fp);
	if(fclose(img_fp) < 0){
		perror("image fclose failed");
		return -1;
	}

	src_pbase_y = (uint32_t)src_bo.paddr;
	src_pbase_uv = src_pbase_y + src_w * src_h;
	offset_y = src_stride * box_y + box_x;
	offset_uv = src_stride * box_y / 2 + box_x;

	if (fabs(matrix[6]) < 1e-10 && fabs(matrix[7]) < 1e-10) {
		Dxx = matrix[0] * matrix[4] - matrix[1] * matrix[3];
		Dxx = Dxx != 0? 1.0/Dxx : 0;
		A11 = matrix[4] * Dxx , A22 = matrix[0] * Dxx;
		matrix[0] = A11;
		matrix[1] *= -Dxx;
		matrix[3] *= -Dxx;
		matrix[4] = A22;
		b1 = -matrix[0] * matrix[2] - matrix[1] * matrix[5];
		b2 = -matrix[3] * matrix[2] - matrix[4] * matrix[5];
		matrix[2] = b1;
		matrix[5] = b2;

		ax = (int64_t)round(matrix[0] * 65536);
    	bx = (int64_t)round(matrix[1] * 65536);
    	cx = (int64_t)round(matrix[2] * 65536);
    	ay = (int64_t)round(matrix[3] * 65536);
    	by = (int64_t)round(matrix[4] * 65536);
    	cy = (int64_t)round(matrix[5] * 65536);
    	az = (int64_t)round(matrix[6] * 65536);
    	bz = (int64_t)round(matrix[7] * 65536);
    	cz = (int64_t)round(matrix[8] * 65536);
	} else {
		t0 = matrix[0] * (matrix[4] * matrix[8] - matrix[5] * matrix[7]);
		t1 = matrix[1] * (matrix[3] * matrix[8] - matrix[5] * matrix[6]);
		t2 = matrix[2] * (matrix[3] * matrix[7] - matrix[4] * matrix[6]);
		t3 = t0 - t1 + t2;

		if(t3 != 0.0) {
			float def = 1.0 / t3;
			dst_m[0] = (matrix[4] * matrix[8] - matrix[5] * matrix[7]) * def;
			dst_m[1] = (matrix[2] * matrix[7] - matrix[1] * matrix[8]) * def;
			dst_m[2] = (matrix[1] * matrix[5] - matrix[2] * matrix[4]) * def;

			dst_m[3] = (matrix[5] * matrix[6] - matrix[3] * matrix[8]) * def;
			dst_m[4] = (matrix[0] * matrix[8] - matrix[2] * matrix[6]) * def;
			dst_m[5] = (matrix[2] * matrix[3] - matrix[0] * matrix[5]) * def;

			dst_m[6] = (matrix[3] * matrix[7] - matrix[4] * matrix[6]) * def;
			dst_m[7] = (matrix[1] * matrix[6] - matrix[0] * matrix[7]) * def;
			dst_m[8] = (matrix[0] * matrix[4] - matrix[1] * matrix[3]) * def;
		}

		ax = (int64_t)round(dst_m[0] * 65536 * 1024); //26bit fixed poind
		bx = (int64_t)round(dst_m[1] * 65536 * 1024);
		cx = (int64_t)round(dst_m[2] * 65536 * 1024);
		ay = (int64_t)round(dst_m[3] * 65536 * 1024);
		by = (int64_t)round(dst_m[4] * 65536 * 1024);
		cy = (int64_t)round(dst_m[5] * 65536 * 1024);
		az = (int64_t)round(dst_m[6] * 65536 * 1024);
		bz = (int64_t)round(dst_m[7] * 65536 * 1024);
		cz = (int64_t)round(dst_m[8] * 65536 * 1024);
	}



	pnode = (iaic_p_node_t *)node_bo.vaddr;
	for (i = 0; i < box_num; i++) {
		pnode[i].timeout[0]      = 0xffffffff;
		pnode[i].src_base_y[0]   = src_pbase_y + offset_y;
		pnode[i].src_base_uv[0]  = src_pbase_uv + offset_uv;
		pnode[i].src_stride[0]   = src_stride;
		pnode[i].dst_base[0]     = (uint32_t)dst_bo.paddr + (i * dst_size);;
		pnode[i].dst_stride[0]   = dst_stride;
		pnode[i].dst_size[0]     = dst_h<<16 | dst_w;
		pnode[i].src_size[0]     = box_h<<16 | box_w;
		pnode[i].dummy_val[0]    = 0x80801010;
		pnode[i].coef0[0]        = (ax & 0xffffffff);
		pnode[i].coef1[0]        = ((bx & 0xffff) << 16) | ((ax & 0xffff00000000) >> 32);
		pnode[i].coef2[0]        = (bx & 0xffffffff0000) >> 16;
		pnode[i].coef3[0]        = (cx & 0xffffffff);
		pnode[i].coef4[0]        = (cx & 0x7ffffff00000000) >> 32;
		pnode[i].coef5[0]        = (ay & 0xffffffff);
		pnode[i].coef6[0]        = ((by & 0xffff) << 16) | ((ay & 0xffff00000000) >> 32);
		pnode[i].coef7[0]        = (by & 0xffffffff0000) >> 16;
		pnode[i].coef8[0]        = (cy & 0xffffffff);
		pnode[i].coef9[0]        = (cy & 0x7ffffff00000000) >> 32;
		pnode[i].coef10[0]       = (az & 0xffffffff);
		pnode[i].coef11[0]       = ((bz & 0xffff) << 16) | ((az & 0xffff00000000) >> 32);
		pnode[i].coef12[0]       = (bz & 0xffffffff0000) >> 16;
		pnode[i].coef13[0]       = (cz & 0xffffffff);
		pnode[i].coef14[0]       = (cz & 0x7ffffff00000000) >> 32;

	}

	ret = iaic_aip_p_chain(&iaic_ctx,
			AIP_P_MODE_NV12_TO_BGRA,
			AIP_DAT_POS_DDR,
			AIP_DAT_POS_DDR,
			AIP_ORDER_BGRA,
			125, 16, 128,
			nv2bgr_coef,
			&node_bo,
			box_num,
			NULL,
			&job_seqno);
	if (ret) {
		printf("Failed to submit IO request, ret = %d\n", ret);
		return -1;
	}

	job_timeout_ns = -1ll;
	ret = iaic_aip_p_wait(&iaic_ctx, job_seqno, job_timeout_ns, NULL);
	if (ret) {
		printf("Failed to wait, ret = %d\n", ret);
	}

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
		printf("Successfully obtained image[%d] information\n", i);
		if(fclose(dst_fp) != 0){
			perror("dst_image fclose");
			break;
		}
	}
#endif

	ret = iaic_destroy_bo(&iaic_ctx, &node_bo);
	ret = iaic_destroy_bo(&iaic_ctx, &dst_bo);
	ret = iaic_destroy_bo(&iaic_ctx, &src_bo);

	ret = iaic_ctx_destroy(&iaic_ctx);


	return ret;
}

int main()
{
	int ret;
	ret = aipv20_perspective();
	if (ret < 0)
		printf("aipv20_resize failed!\n");
	return 0;
}
