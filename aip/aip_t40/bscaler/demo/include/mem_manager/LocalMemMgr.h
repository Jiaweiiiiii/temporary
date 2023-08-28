#ifndef __LOCAL_MEMMGR_H__
#define __LOCAL_MEMMGR_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

extern int Local_HeapInit (void *heap, unsigned int size);

extern void *Local_Alloc (void *heap, unsigned int nbytes);

extern void Local_Dealloc(void *heap, void *address);

extern void *Local_Calloc (void *heap, unsigned int size, unsigned int n);

extern void *Local_Realloc (void *heap, void *address, unsigned int nbytes);

extern void *Local_alignAlloc (void *heap, unsigned int align, unsigned int size);

extern void Local_Dump_List(void *heap);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
