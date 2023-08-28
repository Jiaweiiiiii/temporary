
#include "alloc_manager.h"
#include "LocalMemMgr.h"
#include <stdio.h>

//#define DUMP_ORAM_MALLOC_FREE_LIST

static void *mp_memory = NULL;
static unsigned int mp_memory_size = 0;
static void *list = NULL;
static void *heap = NULL;

int oram_memory_init(void *mp, unsigned int mp_size) {
    mp_memory_size = mp_size;
    mp_memory = mp;
	list = mp;
	heap = malloc(sizeof(Alloc));
    int ret = Local_HeapInit(mp_memory, mp_memory_size, &list);
	 *(unsigned int*)heap = (unsigned int)list;
	return ret;
}

void oram_memory_deinit(void) {
    mp_memory = NULL;
    mp_memory_size = 0;
	free(heap);
	list = NULL;
	heap = NULL;
}

void *oram_malloc(unsigned int size) {
#ifdef DUMP_ORAM_MALLOC_FREE_LIST
    printf("--------------before:%s(%d) start------------\n", __func__, __LINE__);
    Local_Dump_List(heap);
    printf("--------------before:%s(%d) end------------\n", __func__, __LINE__);
#endif
    void *ret = Local_Alloc(heap, size);
	if(ret == NULL){
		printf("%s: alloc is failed\n",__func__);
	}
#ifdef DUMP_ORAM_MALLOC_FREE_LIST
    printf("--------------after:%s(%d):ret=%p start------------\n", __func__, __LINE__, ret);
    Local_Dump_List(heap);
    printf("--------------after:%s(%d):ret=%p end------------\n", __func__, __LINE__, ret);
#endif
    return ret;
}

void oram_free(void *addr) {
#ifdef DUMP_ORAM_MALLOC_FREE_LIST
    printf("--------------before:%s(%d):addr=%p start------------\n", __func__, __LINE__, addr);
    Local_Dump_List(heap);
    printf("--------------before:%s(%d):addr=%p end------------\n", __func__, __LINE__, addr);
#endif
    if (addr != NULL)
        if(Local_Dealloc(heap, addr) == 0)
			printf("%s: ###### Dealloc is failed\n",__func__);
#ifdef DUMP_ORAM_MALLOC_FREE_LIST
    printf("--------------after:%s(%d):addr=%p start------------\n", __func__, __LINE__, addr);
    Local_Dump_List(heap);
    printf("--------------after:%s(%d):addr=%p end------------\n", __func__, __LINE__, addr);
#endif
}

void *oram_calloc(unsigned int size, unsigned int n) {
    void *ret = Local_Calloc(heap, size, n);
	if(ret == NULL){
		printf("%s: alloc is failed\n",__func__);
	}
    return ret;
}
#if 0
void *oram_realloc(void *addr, unsigned int size) {
    void *ret = Local_Realloc(mp_memory, addr, size);
    return ret;
}
#endif

#if 0
void *oram_memalign(unsigned int align, unsigned int size) {
#ifdef DUMP_ORAM_MALLOC_FREE_LIST
    printf("--------------before:%s(%d) start------------\n", __func__, __LINE__);
    Local_Dump_List(heap);
    printf("--------------before:%s(%d) end------------\n", __func__, __LINE__);
#endif
    void *ret = Local_alignAlloc(heap, align, size);
	if(ret == NULL){
		printf("%s: alloc is failed\n",__func__);
	}
#ifdef DUMP_ORAM_MALLOC_FREE_LIST
    printf("--------------after:%s(%d):ret=%p start------------\n", __func__, __LINE__, ret);
    Local_Dump_List(heap);
    printf("--------------after:%s(%d):ret=%p end------------\n", __func__, __LINE__, ret);
#endif
    return ret;
}
#endif
