#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "iaic.h"


typedef struct block {
    void* mem_ptr;
    size_t size;
    struct block* next;
} block_t;

typedef struct {
    block_t* head;
    block_t* tail;
    size_t size;
    pthread_mutex_t lock;
} mem_pool_t;

typedef struct {
	iaic_bo_t bo;
	size_t size;
	int align;
	void *vaddr;
} iaic_mem_t;

int my_malloc(iaic_ctx_t *ctx, mem_pool_t *pool, iaic_mem_t *mem)
{
	pthread_mutex_lock(&pool->lock);
	int align_size = ((mem->size + mem->align - 1) / mem->align) * mem->align;
    void* ptr = NULL;
    block_t* node = pool->head;
	DL_APPEND(pool->head, node)

	while (node) {
		size_t offset = (size_t)node->mem_ptr % mem->align;
		if (offset != 0) {
			node->mem_ptr = (char*)node->mem_ptr + mem->align - offset;
			node->size -= mem->align - offset;
		}
		if (node->size >= align_size) {
            ptr = (void*)(((size_t)node->mem_ptr + mem->align - 1) / mem->align * mem->align);
            node->size -= (char*)ptr - (char*)node->mem_ptr + align_size;
            node->mem_ptr = (char*)ptr + align_size;
			break;
		}
		node = node->next;
	}
	if(!ptr) {
		size_t new_size = 0x1000;
		iaic_create_bo(ctx, new_size, &mem->bo);
	   	block_t *new_node = (block_t *)mem->bo.vaddr; 
		if (new_node) {
			new_node->mem_ptr = (char *)new_node + sizeof(block_t);
			new_node->size = new_size - sizeof(block_t);
			new_node->next = NULL;
			if (!pool->head) {
				pool->head = new_node;
			} else {
				pool->tail->next = new_node;
			}
			pool->tail = new_node;
			pool->size += new_node->size;
            ptr = (void*)(((size_t)new_node->mem_ptr + mem->align - 1) / mem->align * mem->align);
            new_node->size -= (char*)ptr - (char*)new_node->mem_ptr + align_size;
            new_node->mem_ptr = (char*)ptr + align_size;
		}
	}
	mem->vaddr = ptr;
	pthread_mutex_unlock(&pool->lock);
	return 0;

}

void my_free(iaic_ctx_t *ctx, mem_pool_t *pool, iaic_mem_t *mem)
{
	if (!mem->vaddr)
			return;
	pthread_mutex_lock(&pool->lock);
	void *ptr = mem->vaddr;
    block_t* node = pool->head;
	while (node) {
		if (node->mem_ptr <= ptr && node->mem_ptr + node->size > ptr) {
			if (ptr == node->mem_ptr + node->size - sizeof(block_t)) {
				node->size += sizeof(block_t);
			} else if (ptr == node->mem_ptr) {
				node->size += sizeof(block_t);
				node->mem_ptr = (char *)ptr - sizeof(block_t);
			} else {
				block_t *new_node = (block_t *)((char *)ptr - sizeof(block_t));
				new_node->next = node->next;
				new_node->size = (char *)node - (char *)ptr;
				new_node->mem_ptr = ptr;
				node->next = new_node;
				node->size = (char *)ptr - (char *)node->mem_ptr;
			}
			pool->size += sizeof(block_t);
			break;
		}
		node = node->next;
	}
	pthread_mutex_unlock(&pool->lock);
	if (pool->size >= 0x1000) {
		pthread_mutex_lock(&pool->lock);
		node = pool->head;
		while (node) {
			block_t* next = node->next;
			iaic_destroy_bo(ctx, &mem->bo);
			node = next;
		}
		pool->head = NULL;
		pool->tail = NULL;
		pool->size = 0;
		pthread_mutex_unlock(&pool->lock);
	}

}

int aipv20_ddr()
{
	int ret;
	iaic_ctx_t ctx;
	ret = iaic_ctx_init(&ctx);

	mem_pool_t pool = {
		.head = NULL,
		.tail = NULL,
		.size = 0,
		.lock = PTHREAD_MUTEX_INITIALIZER
	};

	iaic_mem_t mem1 = {
		.size = 33,
		.align = 32,
		.vaddr = NULL
	};
	ret = my_malloc(&ctx, &pool, &mem1);

	iaic_mem_t mem2 = {
		.size = 33,
		.align = 32,
		.vaddr = NULL
	};
	ret = my_malloc(&ctx, &pool, &mem2);

	iaic_mem_t mem3 = {
		.size = 33,
		.align = 32,
		.vaddr = NULL
	};
	ret = my_malloc(&ctx, &pool, &mem3);


	*(int *)(mem1.vaddr) = 111;
	*(int *)(mem2.vaddr) = 222;
	*(int *)(mem3.vaddr) = 333;
	printf("vaddr = %p, value = %d\n", mem1.vaddr, *(int *)(mem1.vaddr));
	printf("vaddr = %p, value = %d\n", mem2.vaddr, *(int *)(mem2.vaddr));
	printf("vaddr = %p, value = %d\n", mem3.vaddr, *(int *)(mem3.vaddr));


	my_free(&ctx, &pool, &mem3);
	my_free(&ctx, &pool, &mem2);
	my_free(&ctx, &pool, &mem1);


	ret = iaic_ctx_destroy(&ctx);
	return ret;
}

int main()
{
	int ret;
	ret = aipv20_ddr();
	if (ret < 0)
		printf("aipv20_ddr failed!\n");
	return 0;
}
