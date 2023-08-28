#include "ddr_mem.h"
#include "LocalMemMgr.h"
#include <mutex>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include "alloc_manager.h"

static uint32_t mp_memory_used = 0;
static void *mp_memory = NULL;
static unsigned int mp_memory_size = 0;
static void *list = NULL;
static void *heap = NULL;
std::mutex mem_mutex;
int buffer = 0, max_size = 0;
//#define VENUS_MEM_MSG
int ddr_memory_init(void *mp, unsigned int mp_size) {
	std::unique_lock<std::mutex> lock(mem_mutex);
	mp_memory_size = mp_size;
	mp_memory = mp;
	mp_memory_used = 0;
	heap = malloc(sizeof(Alloc));
	int ret = Local_HeapInit(mp_memory, mp_memory_size, &list);
	*(unsigned int*)heap = (unsigned int)list;

	return ret;
}

void ddr_memory_deinit(void) {
	std::unique_lock<std::mutex> lock(mem_mutex);
	mp_memory = NULL;
	free(heap);
	list = NULL;
	heap = NULL;
	mp_memory_size = 0;
}

void nmem_memory(unsigned int size, char n) {
	FILE *fd_t = fopen("/tmp/nmem_memory.txt", "w+");
	if (fd_t == NULL) {
		printf("open /tmp/nmem_memory.txt is error\n");
	}
	if (n == '+') {
		//       printf("buffer %d + size %d = %d\n", buffer, size, buffer + size);
		buffer += size;
	} else if (n == '-') {
		//        printf("buffer %d - size %d = %d\n", buffer, size, buffer - size);
		buffer -= size;
	}
	if (buffer > max_size) {
		max_size = buffer;
	}
	fprintf(fd_t, "nmem total = %dK\n", mp_memory_size / 1024);
	fprintf(fd_t, "nmem used  = %dK\n", ((max_size + 64 * 1024) / 1024 + (4 - 1)) / 4 * 4);
	fprintf(fd_t, "nmem free  = %dK\n",
			(mp_memory_size / 1024) - (((max_size + 64 * 1024) / 1024 + (4 - 1)) / 4 * 4));
	fprintf(fd_t, "#### %%%0.2f ####\n",
			(float)(((max_size + 64 * 1024) / 1024 + (4 - 1)) / 4 * 4) / (mp_memory_size / 1024) *
			100);
	fclose(fd_t);
	fd_t = NULL;
}
/*
   struct ddr_memory_info get_ddr_memory_info(void *info) {
   std::unique_lock<std::mutex> lock(mem_mutex);
   struct ddr_memory_info *info_1 = (struct ddr_memory_info *)info;
   info_1->ddr_head_ptr = (unsigned int)((uint64_t)mp_memory);
   info_1->ddr_total_size = mp_memory_size;
   info_1->ddr_used_size = mp_memory_used; // Local_ReservedSize(mp_memory);
   printf("mp_memory_used = %d\n", mp_memory_used);
   return *info_1;
   }
   */
struct ddr_memory_info get_ddr_memory_info(void) {
	std::unique_lock<std::mutex> lock(mem_mutex);
	struct ddr_memory_info info;
	info.ddr_head_ptr = (unsigned int)((uint64_t)mp_memory);
	info.ddr_total_size = mp_memory_size;
	info.ddr_used_size = mp_memory_used; // Local_ReservedSize(mp_memory);
	return info;
}
void *ddr_malloc(unsigned int size) {
	std::unique_lock<std::mutex> lock(mem_mutex);
	void *ret = Local_Alloc(heap, size);
	if (ret) {
		mp_memory_used += size;
#ifdef VENUS_MEM_MSG
		nmem_memory(size, '+');
#endif
	}else{
		printf("%s: alloc is failed\n",__func__);
	}
	return ret;
}

void ddr_free(void *addr) {
	std::unique_lock<std::mutex> lock(mem_mutex);
	if (addr != NULL) {
		int nbytes = Local_Dealloc(heap, addr);
		if(nbytes == 0){
			printf("%s: Dealloc is failed\n",__func__);
		}
		mp_memory_used -= nbytes;
#ifdef VENUS_MEM_MSG
		nmem_memory(nbytes, '-');
#endif
	}
}

void *ddr_calloc(unsigned int size, unsigned int n) {
	std::unique_lock<std::mutex> lock(mem_mutex);
	void *ret = Local_Calloc(heap, size, n);
	if (ret) {
		mp_memory_used += size * n;
#ifdef VENUS_MEM_MSG
		nmem_memory(size * n, '+');
#endif
	}else{
		printf("%s: alloc is failed\n",__func__);
	}
	return ret;
}

#if 0
void *ddr_realloc(void *addr, unsigned int size) {
	std::unique_lock<std::mutex> lock(mem_mutex);
	void *ret = Local_Realloc(mp_memory, addr, size);
	if (ret) {
		mp_memory_used += size;
	}
	return ret;
}
#endif
void *ddr_memalign(unsigned int align, unsigned int size) {
	std::unique_lock<std::mutex> lock(mem_mutex);
	void *ret = Local_alignAlloc(heap, align, size);
	if (ret) {
		mp_memory_used += size;
#ifdef VENUS_MEM_MSG
		nmem_memory(size, '+');
#endif
	}else{
		printf("%s: alloc is failed\n",__func__);
	}
	return ret;
}
