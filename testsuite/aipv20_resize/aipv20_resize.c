#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "iaic.h"



int aipv20_resize(int run_cnt)
{

	int ret, i;
	FILE *img_fp;
	iaic_ctx_t iaic_ctx;
	iaic_bo_t src_bo;
	iaic_bo_t dst_bo;
	iaic_bo_t node_bo;
	uint64_t job_seqno, job_timeout_ns;
	int64_t job_wait_time;
	uint32_t src_w, src_h, dst_w, dst_h, src_bpp, dst_bpp, src_stride, dst_stride;
	uint32_t box_w, box_h, box_x, box_y, box_num;
	size_t src_size, dst_size, dst_total_size, node_size;
	float scale_x, scale_y;
	void *src_vbase = NULL;
	aip_v20_f_node_t *fnode;
	uint32_t src_pbase_y, src_pbase_uv, offset_y, offset_uv;

	src_w = 1024;
	src_h = 576;

	box_w = src_w;
	box_h = src_h;
	box_x = 0;
	box_y = 0;
	box_num = 3;

	dst_w = 512;
	dst_h = 512;

	src_bpp = 12;
	dst_bpp = 12;
	src_stride = src_w;
	dst_stride = dst_w;
	src_size = src_stride * src_h * src_bpp >> 3;
	dst_size = dst_stride * dst_h * dst_bpp >> 3;

	scale_x = (float)box_w / (float)dst_w;
	scale_y = (float)box_h / (float)dst_h;

	node_size = sizeof(aip_v20_f_node_t) * box_num;
	dst_total_size = dst_size * box_num;

	ret = iaic_ctx_init(&iaic_ctx, 0x400000);
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
	ret = iaic_create_fnode(&iaic_ctx, node_size, &node_bo);
	if (ret) {
		perror("node_bo failed");
		return -1;
	}

	src_vbase = src_bo.vaddr;

	img_fp = fopen("./model/day_w1024_h576.nv12","r");
	if(img_fp == NULL) {
		perror("aip_f_image fopen");
		return -1;
	}
	fread(src_vbase, 32, src_size/32, img_fp);
	if(fclose(img_fp) < 0){
		perror("aip_f_image fclose");
		return -1;
	}

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


	for (i = 0; i < run_cnt; i++) {

	ret = iaic_aip_f_chain(&iaic_ctx,
			AIP_F_FORMAT_NV12,
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

	job_timeout_ns = -1ll;
	ret = iaic_aip_f_wait(&iaic_ctx, job_seqno, job_timeout_ns, &job_wait_time);
	if (ret) {
		printf("Failed to wait, ret = %d\n", ret);
		return -1;
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
		//printf("Successfully obtained image[%d] information\n", i);
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
	int run_cnt = 1;

	ret = aipv20_resize(run_cnt);

	if (ret < 0)
		printf("aipv20_resize failed!\n");

	return 0;
}
