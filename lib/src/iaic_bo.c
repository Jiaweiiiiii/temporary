#include <stdio.h>
#include "iaic.h"
#include "iaic_list.h"

// BO
iaic_ret_code_t create_bo_node(iaic_ctx_t *ctx, size_t size)
{
	aip_v20_ioctl_bo_t ioctl_bo;
	ioctl_bo.op = AIP_BO_OP_APPEND;
	ioctl_bo.size = size;
	ioctl_bo.handle = 0;
	IAIC_IOCTL(ctx->aip_fd, IOCTL_AIP_V20_BO, &ioctl_bo);
	if (!ioctl_bo.handle) {
		perror("[IAIC] BO application failed");
		return -1;
	}

	block_t *new_block = (block_t *)malloc(sizeof(block_t));
	new_block->size = ioctl_bo.size;
	new_block->idle = 1;
	new_block->is_bo = 1;
	new_block->brother = 1;
	new_block->handle = ioctl_bo.handle;
	new_block->paddr = ioctl_bo.paddr;
	new_block->vaddr = mmap(NULL, size,
			PROT_READ | PROT_WRITE,
			MAP_SHARED, ctx->mem_fd,
			(off_t)ioctl_bo.paddr);
	pthread_mutex_lock(&ctx->mutex_lock);
	aip_list_add_tail(&new_block->node, &ctx->bo_list);
	pthread_mutex_unlock(&ctx->mutex_lock);

	return IAIC_CODE_OK;
}


iaic_ret_code_t iaic_fast_alloc_bo(iaic_ctx_t *ctx, size_t size, iaic_bo_t *bo)
{
	size_t align_size = (size + (64 - 1)) & ~(64 - 1);

	block_t *curr_bo = NULL;
	pthread_mutex_lock(&ctx->mutex_lock);
	aip_list_for_each(curr_bo, &ctx->bo_list, block_t, node) {
		if (curr_bo->idle && curr_bo->size >= align_size) {
			size_t remain_size = curr_bo->size - align_size;
			if (remain_size < 0x200) {
				break;
			} else {
				block_t *new_bo = (block_t *)malloc(sizeof(block_t));
				new_bo->size = curr_bo->size - align_size;
				curr_bo->size = align_size;
				new_bo->idle = 1;
				new_bo->is_bo = 0;
				new_bo->handle = curr_bo->handle;
				new_bo->paddr = curr_bo->paddr + align_size;
				new_bo->vaddr = curr_bo->vaddr + align_size;
				aip_list_add(&new_bo->node, &curr_bo->node);
				break;
			}
		}
	}
	curr_bo->idle = 0;

	if (&curr_bo->node == &ctx->bo_list) {
		printf("IAIC BO: Not enough space allocated to BO.\n");
		printf("IAIC BO: Max size available[0x%x], required size[0x%x].\n",
				iaic_get_bo_free(ctx), align_size);
		if (create_bo_node(ctx, size)) {
			return IAIC_CODE_BO_CREATE_FAIL;
		}
		curr_bo = list_last_entry(&ctx->bo_list, block_t, node);
		curr_bo->idle = 0;
		printf("IAIC BO: Apply for a new area from CMA for use.\n");
	}
	pthread_mutex_unlock(&ctx->mutex_lock);

	bo->size = size;
	bo->esize = curr_bo->size;
	bo->handle = curr_bo->handle;
	bo->paddr = curr_bo->paddr;
	bo->vaddr = curr_bo->vaddr;
	bo->evaddr = curr_bo->vaddr;

	return IAIC_CODE_OK;
}

iaic_ret_code_t iaic_fast_free_bo(iaic_ctx_t *ctx, iaic_bo_t *bo)
{
	int ret = 0;
	int found = 0;

	if (!bo->handle)
		return IAIC_CODE_OK;


	block_t *curr_bo = NULL;
	block_t *prev_bo = NULL;
	block_t *next_bo = NULL;
	pthread_mutex_lock(&ctx->mutex_lock);
	aip_list_for_each(curr_bo, &ctx->bo_list, block_t, node) {
		if (curr_bo->paddr == bo->paddr) {
			found = 1;
			break;
		}
	}
	if (!found) {
		pthread_mutex_unlock(&ctx->mutex_lock);
		printf("Can not find this bo!\n");
		return -1;
	}
	prev_bo = aip_list_prev_entry(&curr_bo->node, block_t, node);
	next_bo = aip_list_next_entry(&curr_bo->node, block_t, node);
	curr_bo->idle = 1;

	while (prev_bo->idle) {
		if (&prev_bo->node == &ctx->bo_list ||
				curr_bo->is_bo) {
			break;
		}
		prev_bo->size += curr_bo->size;
		aip_list_del(&curr_bo->node);
		free(curr_bo);
		curr_bo = prev_bo;
		prev_bo = aip_list_prev_entry(&curr_bo->node, block_t, node);
	}

	while (next_bo->idle) {
		if (&next_bo->node == &ctx->bo_list) {
			break;
		}
		curr_bo->size += next_bo->size;
		aip_list_del(&next_bo->node);
		free(next_bo);
		next_bo = aip_list_next_entry(&curr_bo->node, block_t, node);
	}
	pthread_mutex_unlock(&ctx->mutex_lock);

	if (curr_bo->is_bo) {
		int aip_fd = ctx->aip_fd;
		aip_v20_ioctl_bo_t ioctl_bo;
		ioctl_bo.op = AIP_BO_OP_DESTROY;
		ioctl_bo.handle = curr_bo->handle;
		ioctl_bo.paddr = curr_bo->paddr;
		ioctl_bo.size = ctx->max_mem_size;
		ret = IAIC_IOCTL(aip_fd, IOCTL_AIP_V20_BO, &ioctl_bo);
		if (!ioctl_bo.handle || ret != 0) {
			perror("[IAIC] BO free failed");
			return IAIC_CODE_BO_DESTROY_FAIL;
		}
		if (bo->vaddr != MAP_FAILED){
			munmap(bo->vaddr, ctx->max_mem_size);
		}
		free(curr_bo);
	}

	return IAIC_CODE_OK;
}

iaic_ret_code_t iaic_alloc_bo(iaic_ctx_t *ctx, size_t size, iaic_bo_t *bo)
{
	int fd = ctx->aip_fd;
	aip_v20_ioctl_bo_t ioctl_bo;

	ioctl_bo.op = AIP_BO_OP_APPEND;
	ioctl_bo.size = size;
	ioctl_bo.handle = 0;

	IAIC_IOCTL(fd, IOCTL_AIP_V20_BO, &ioctl_bo);
	if (!ioctl_bo.handle) {
		perror("[IAIC] BO create failed");
		return IAIC_CODE_BO_CREATE_FAIL;
	}

	bo->size = size;
	bo->handle = ioctl_bo.handle;
	bo->paddr = ioctl_bo.paddr;

#ifdef IAIC_BO_EXT_MAP
	if ((long)bo->paddr < PAGE_SIZE) {
		perror("[IAIC] BO create extend failed");
		iaic_destroy_bo(ctx, bo);
		return IAIC_CODE_BO_CREATE_EXT_FAIL;
	}
	bo->esize = bo->size + PAGE_SIZE * 2;
	bo->evaddr = mmap(NULL, bo->esize,
			PROT_READ | PROT_WRITE,
			MAP_SHARED, ctx->mem_fd,
			(off_t)(bo->paddr - PAGE_SIZE));
	bo->vaddr = bo->evaddr + PAGE_SIZE;
#else
#error("IAIC BO MAP ERROR.");
#endif

	if (bo->vaddr == MAP_FAILED) {
		perror("[IAIC] BO create mmap failed");
		iaic_destroy_bo(ctx, bo);
		return IAIC_CODE_BO_CREATE_MMAP_FAIL;
	}

	return IAIC_CODE_OK;
}

iaic_ret_code_t iaic_free_bo(iaic_ctx_t *ctx, iaic_bo_t *bo)
{
	int __attribute__((__unused__)) ret;
	int fd = ctx->aip_fd;
	aip_v20_ioctl_bo_t ioctl_bo;

	if (!bo->handle)
		return IAIC_CODE_OK;

	ioctl_bo.op = AIP_BO_OP_DESTROY;
	ioctl_bo.handle = bo->handle;
	ioctl_bo.size = bo->size;
	ioctl_bo.paddr = bo->paddr;

	ret = IAIC_IOCTL(fd, IOCTL_AIP_V20_BO, &ioctl_bo);
	if (!ioctl_bo.handle) {
		perror("[IAIC] BO destroy failed");
		return IAIC_CODE_BO_DESTROY_FAIL;
	}

	if (bo->vaddr != MAP_FAILED) {
#ifdef IAIC_BO_EXT_MAP
		munmap(bo->evaddr, bo->esize);
#else
#error("IAIC BO MAP ERROR.");
#endif
	}

	bo->handle = 0;
	bo->vaddr = MAP_FAILED;

	return IAIC_CODE_OK;
}

int iaic_get_bo_free(iaic_ctx_t *ctx)
{
	block_t *last_bo = NULL;
	last_bo = list_last_entry(&ctx->bo_list, block_t, node);
	while (!last_bo->idle && &last_bo->node != &ctx->bo_list) {
		last_bo = list_last_entry(&last_bo->node, block_t, node);
	}

	return last_bo->size;
}
