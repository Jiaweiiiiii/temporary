#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include "iaic.h"



int nnav20_dma()
{
	int ret = 0;
	iaic_ctx_t iaic_ctx;
	iaic_bo_t bo;
	iaic_oram_t oram;

	volatile uint32_t *nnadma_desram;
	volatile uint32_t *nnadma_cfg;


	ret = iaic_ctx_init(&iaic_ctx);

	iaic_set_nice(&iaic_ctx, -19);
	if (ret) {
		printf("iaic_ctx_init failed, ret = %d\n", ret);
		return -1;
	}

	size_t op_size = 1024 * 8;
	ret = iaic_create_bo(&iaic_ctx, op_size, &bo);
	if (ret) {
		printf("bo alloc failed\n");
		goto unref;
	}
	//printf("bo paddr:0x%08x 0x%08x\n", (int)bo.vaddr, (int)bo.paddr);

	// Alloc ORAM
	ret = iaic_nna_oram_alloc(&iaic_ctx, op_size, &oram);
	if (ret) {
		printf("oram alloc failed\n");
		goto fail;
	}
	//printf("oram paddr:0x%08x offset:0x%08x\n",
	//		(int)oram.paddr,
	//		oram.offset);

	memset(bo.vaddr, 0xa, op_size);
	ret = iaic_cacheflush(&iaic_ctx, bo.vaddr, op_size, DMA_TO_DEVICE);

	iaic_nna_mutex_lock(&iaic_ctx);

	// NNA DMA
	nnadma_desram = (volatile uint32_t *)iaic_ctx.nna_status.dma_desram_vbase;
	nnadma_cfg = (volatile uint32_t *)iaic_ctx.nna_status.dma_vbase;

	size_t nnadma_length = (op_size >> 6) - 1;
	*nnadma_desram = ((uint32_t)(bo.paddr) & ~(0x3f)) | (((nnadma_length >> 12) & 0x1f) << 1);
	nnadma_desram++;
	*nnadma_desram = (oram.offset >> 6) | ((nnadma_length & 0xfff) << 20);
	*nnadma_cfg = 1<<31;
	iaic_fast_iob(&iaic_ctx);

	iaic_nndma_rch0_wait;

	// Check
	for (int i = 0; i < op_size; i++) {
		char res = ((char *)oram.vaddr)[i];
		if (res != 0xa) {
			printf("test0 result error i=%d 0x%x 0x%08x\n", i, res,
					(oram.offset + i));
			goto err;
		}
	}

err:
	iaic_nna_mutex_unlock(&iaic_ctx);
	ret = iaic_nna_oram_free(&iaic_ctx, &oram);
fail:
	ret = iaic_destroy_bo(&iaic_ctx, &bo);
unref:
	ret = iaic_ctx_destroy(&iaic_ctx);

	return ret;
}

int main(int argc, char**argv)
{
	int ret;

	ret = nnav20_dma();
	if (ret < 0)
		printf("nnav20_dma failed!\n");

	return 0;
}
