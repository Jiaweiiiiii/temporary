#ifndef __LOCAL_MEMMGR_H__
#define __LOCAL_MEMMGR_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int Local_HeapInit(void *heap, unsigned int size, void **list);
int Local_HeapDeInit(void **list);

void *Local_Alloc(void *heap, unsigned int nbytes);

int Local_Dealloc(void *heap, void *address);

void *Local_Calloc(void *heap, unsigned int size, unsigned int n);

void *Local_alignAlloc(void *heap, unsigned int align, unsigned int size);

void Local_Dump_List(void *heap);

int Local_ReservedSize(void *heap);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif
