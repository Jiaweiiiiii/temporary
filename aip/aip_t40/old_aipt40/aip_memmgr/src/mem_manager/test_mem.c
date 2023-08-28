#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "LocalMemMgr.h"
#include "oram_mem.h"
#include "../drivers/aie_mmap.h"

int main(int argc, char **argv)
{
	void *total = NULL;
	void *heap = NULL;
	unsigned int totalsize = 4*1024*1024;
	void *addr[64];
	int ret = 0;
	total = malloc(totalsize);
	if(!total){
		printf("test failed\n");
		return 0;
	}
	heap = malloc(totalsize);
	if(!heap){
		printf("test failed\n");
		goto out;
	}
	ret = Local_HeapInit(total, totalsize, heap);
	if(ret == 0){
		printf("Failed Local_HeapInit\n");
		goto out1;
	}
#if 0
	ret = oram_memory_init(heap, totalsize);
	if(ret == 0){
		printf("Failed Local_HeapInit\n");
		goto out1;
	}
#endif
	ret = __aie_mmap(0, 1, NNA_UNCACHED_ACCELERATED, NNA_UNCACHED_ACCELERATED, NNA_CACHED);
	/*ret = __aie_mmap(0, 1, NNA_UNCACHED_ACCELERATED, NNA_CACHED, NNA_CACHED);*/
	if (0 != ret) {
		printf("aie init failed.\n");
		goto out1;
	}
	addr[0] = Local_Alloc(total, 40*1024);
	addr[1] = Local_Alloc(total, 140*1024);
	addr[2] = Local_Alloc(total, 240*1024);
	addr[3] = Local_Alloc(total, 340*1024);
	/*Local_Dump_List(total);*/
	memset(addr[0], 0, 40*1024);
	Local_Dealloc(total, addr[1]);
	addr[1] = Local_Alloc(total, 540*1024);
	Local_Dealloc(total, addr[2]);
	addr[2] = Local_Alloc(total, 340*1024);
	memset(addr[2], 0, 40*1024);
	/*Local_Dump_List(total);*/
	Local_Dealloc(total, addr[0]);
	/*Local_Dump_List(total);*/
	addr[0] = Local_Alloc(total, 10*1024);
	/*Local_Dump_List(total);*/
	printf("11\n");
#if 1
	addr[10] = oram_malloc(10*1024);
	printf("2\n");
	addr[11] = oram_malloc(10*1024);
	oram_free(addr[10]);
	addr[10] = oram_malloc(100*1024);
	oram_free(addr[10]);
	oram_free(addr[11]);
#endif
	Local_HeapDeInit(total);
	/*Local_HeapDeInit(heap);*/
out1:
	free(heap);
out:
	free(total);
	return 0;
}
