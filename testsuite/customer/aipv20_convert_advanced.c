/*
 * =====================================================================================
 *       Filename:  aipv20_test.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  07/28/2023 04:37:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME ().
 *   Organization:
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include "iaic.h"

struct oram {
	void *vaddr;
	uint32_t paddr;
	size_t size;
};

int aipv20_t_dma()
{
	int ret, i, j, task_h;
	FILE *img_fp;
	iaic_ctx_t ctx;
	iaic_bo_t src_bo;
	iaic_bo_t dst_bo;
	iaic_t_node_t tnode;
	uint64_t job_seqno;
	uint32_t dst_oram_paddr, dst_bo_paddr;

	uint32_t src_w, src_h, src_bpp, dst_bpp, src_stride, dst_stride;
	uint32_t box_w, box_h, box_x, box_y, box_num;
	size_t src_size, dst_size;
	uint32_t src_pbase_y, src_pbase_uv, offset_y, offset_uv;
	uint32_t nv2bgr_coef[9] = {
		1220542, 2116026, 0,
		1220542, 409993, 852492,
		1220542, 0, 1673527
	};

	struct oram oram1, oram2;

	src_w = 1024;
	src_h = 576;

	box_w = src_w;
	box_h = src_h;
	box_x = 0;
	box_y = 0;
	box_num = 1;

	src_bpp = 12;
	dst_bpp = 32;
	src_stride = src_w;
	dst_stride = box_w * dst_bpp >> 3;
	src_size = src_stride * src_h * src_bpp >> 3;
	dst_size = dst_stride * box_h;

	ret = iaic_ctx_init(&ctx, 0x600000);
	if (ret) {
		printf("iaic_ctx_init failed, ret = %d\n", ret);
		return -1;
	}

	printf("***************************************************\n");
	printf("oram:vbase[%p],pbase[%p],size[0x%x]\n",
			ctx.nna_status.oram_vbase,
			ctx.nna_status.oram_pbase,
			ctx.nna_status.oram_size);
	printf("dma_io:vbase[%p],pbase[%p]\n",
			ctx.nna_status.dma_vbase,
			ctx.nna_status.dma_pbase);
	printf("desram:vbase[%p],pbase[%p]\n",
			ctx.nna_status.dma_desram_vbase,
			ctx.nna_status.dma_desram_pbase);

	ret = iaic_create_bo(&ctx, src_size, &src_bo);
	if (ret) {
		perror("src_bo failed");
		return -1;
	}
	img_fp = fopen("./model/day_w1024_h576.nv12","r");
	if(img_fp == NULL) {
		perror("image fopen failed");
		return -1;
	}
	fread(src_bo.vaddr, 32, src_size/32, img_fp);
	if(fclose(img_fp) < 0){
		perror("image fclose failed");
		return -1;
	}

	ret = iaic_create_bo(&ctx, dst_size, &dst_bo);
	if (ret) {
		perror("dst_bo failed");
		return -1;
	}
	oram1.vaddr = ctx.nna_status.oram_vbase;
	oram1.paddr = (uint32_t)ctx.nna_status.oram_pbase;
	oram1.size = ctx.nna_status.oram_size>>1;
	oram2.vaddr = ctx.nna_status.oram_vbase + oram1.size;
	oram2.paddr = (uint32_t)ctx.nna_status.oram_pbase + oram1.size;
	oram2.size = ctx.nna_status.oram_size - oram1.size;
	printf("***************************************************\n");
	printf("sr_bo:vaddr[%p],paddr[%p],size[0x%x]\n",
			src_bo.vaddr, src_bo.paddr, src_bo.size);
	printf("oram1:vaddr[%p],paddr[0x%x],size[0x%x]\n",
			oram1.vaddr, oram1.paddr, oram1.size);
	printf("oram2:vaddr[%p],paddr[0x%x],size[0x%x]\n",
			oram2.vaddr, oram2.paddr, oram2.size);
	printf("ds_bo:vaddr[%p],paddr[%p],size[0x%x]\n",
			dst_bo.vaddr, dst_bo.paddr, dst_bo.size);
	src_pbase_y = (uint32_t)src_bo.paddr;
	src_pbase_uv = src_pbase_y + src_w * src_h;
	offset_y = src_stride * box_y + box_x;
	offset_uv = src_stride * box_y / 2 + box_x;
	task_h = 24;
	tnode.src_ybase  = src_pbase_y + offset_y;
	tnode.src_cbase  = src_pbase_uv + offset_uv;
	tnode.dst_base   = (uint32_t)dst_bo.paddr;
	tnode.src_h      = box_h;
	tnode.src_w      = box_w;
	tnode.src_stride = src_stride;
	tnode.dst_stride = dst_stride;
	tnode.task_len   = task_h;

	uint32_t task_size = dst_stride * task_h;
	printf("***************************************************\n");
	printf("task_size[0x%x]\n", task_size);
	printf("***************************************************\n");

	ret = iaic_convert_config(&ctx,
			AIP_DAT_POS_DDR,
			AIP_DAT_POS_ORAM,
			AIP_ORDER_BGRA,
			255, 16, 128,
			nv2bgr_coef,
			(void *)(&tnode),
			NULL, &job_seqno);
	if (ret) {
		printf("Failed to submit IO request, ret = %d\n", ret);
		return -1;
	}

	i = 1;
	for (j = 0; j < box_h; j += task_h) {
		if (task_h > (box_h - j) - task_h)
			task_h = box_h - j;

		dst_bo_paddr = (uint32_t)dst_bo.paddr + (dst_stride * j);
		if (i == 1) {
			dst_oram_paddr = oram1.paddr;
			i = 2;
		} else {
			dst_oram_paddr = oram2.paddr;
			i = 1;
		}
		memset(ctx.nna_status.oram_vbase, 0x00, ctx.nna_status.oram_size);
		iaic_convert_start(&ctx, dst_oram_paddr, task_h);
		iaic_convert_wait(&ctx);

		iaic_nndma_wch0_start(&ctx, dst_bo_paddr, dst_oram_paddr, task_size);
		iaic_nndma_wch0_wait;


	}
	ret = iaic_convert_exit(&ctx);
	if (ret) printf("aip_t wait faild\n");

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

	ret = iaic_destroy_bo(&ctx, &dst_bo);
	ret = iaic_destroy_bo(&ctx, &src_bo);
	ret = iaic_ctx_destroy(&ctx);
	return 0;
}

int main(int argc, char *argv[])
{
	int ret;
	ret = aipv20_t_dma();
	if (ret < 0)
		printf("aipv20_convert failed!\n");

	return 0;
}

