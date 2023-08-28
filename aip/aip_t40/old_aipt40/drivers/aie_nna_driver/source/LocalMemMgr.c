
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define FREE            0
#define RESERVED        1
/* Memory block header = at lest 16 bytes = 4 previous + 4 next + 4 status + ... + 4 align */
#define SIZE_HEADER     16
#define local_prev(i)   (*((uintptr_t *)((uintptr_t)i)))
#define local_next(i)   (*((uintptr_t *)(uintptr_t)i + 1))
#define local_status(i) (*((uintptr_t *)(uintptr_t)i + 2))
#define local_size(i)           (local_next(i) - (uintptr_t)i - SIZE_HEADER)
#define local_setalign(i,y)  (*((uintptr_t *)(uintptr_t)i - 1)) = y
#define local_getalign(i)    (*((uintptr_t *)(uintptr_t)i - 1))

/* if going to split free block, need at least 4 bytes in new free part */

#define MIN_FREE_BYTES 64

//static unsigned char memory[MEM_LENGHT] __attribute__ ((aligned (MIN_FREE_BYTES)));
#define local_first(x) (*(uintptr_t *)(uintptr_t)x)       /* stores address of first byte of heap*/
#define local_last(x)  (*((uintptr_t *)(uintptr_t)x + 1)) /* store addr of last byte of heap+1 */

/*
   Local Heap Initiale
   note: heap is 4 byte aligned
*/
int Local_HeapInit(void *heap, unsigned int size)
{
  uintptr_t addr = (uintptr_t)heap;

  if ((addr % 4) != 0)
    return 0;

  if (size < 16)
    return 0;

  if (MIN_FREE_BYTES > 8)
    local_first (heap) = (uintptr_t)((unsigned char *)heap + MIN_FREE_BYTES);
  else
    local_first (heap) = (uintptr_t)((unsigned char *)heap + 8);

  local_last (heap) = (uintptr_t)((unsigned char *)heap + size) / 4 * 4;

  local_prev (local_first(heap)) = 0;
  local_next (local_first(heap)) = 0;
  local_status (local_first(heap)) = FREE;

  return 1;

}
/*end heapInit*/

static int currentNodeAlloc(void *heap, void *addr, unsigned int nbytes)
{
  unsigned int free_size;
  /*handle case of current block being the last*/

  if (local_next(addr) == 0)
  {
    free_size = local_last(heap) - (uintptr_t)addr - SIZE_HEADER;
  }
  else
  {
    free_size = local_size (addr);
  }

  /*either split current block, use entire current block, or fail*/

  if (free_size >= nbytes + SIZE_HEADER + MIN_FREE_BYTES)
  {
    uintptr_t old_next;
    uintptr_t old_block;
    uintptr_t new_block;

    old_next = local_next (addr);
    old_block = (uintptr_t)addr;

    /*fix up original block*/

    local_next (addr) = (uintptr_t)addr + SIZE_HEADER + nbytes;
    new_block = local_next (addr);
    local_status (addr) = RESERVED;

    /*set up new free block*/

    addr = (void *)local_next (addr);
    local_next (addr) = old_next;
    local_prev (addr) = old_block;
    local_status (addr) = FREE;

    /*right nieghbor must point to new free block*/
    if (local_next (addr) !=0)
    {
      addr = (void *)local_next (addr);
      local_prev (addr) = new_block;
    }
    return (TRUE);
  }
  else
    if (free_size >= nbytes)
    {
      local_status (addr) = RESERVED;
      return (TRUE);
    }

  return (FALSE);
}
/*end currentNodeAlloc*/

void Local_Dump_List(void *heap)
{
  int i = 0;
  void *addr = (void *)local_first (heap);

  while (local_next (addr) != 0)
  {
    printf("[%5d]:local_status (%p)=0x%x, local_next(%p)=0x%x, local_prev(%p)=0x%x\n", i++, addr, local_status (addr), addr, local_next(addr), addr, local_prev(addr));
    addr = (void *)local_next (addr);
  }
}

static uintptr_t Local_Alloc_Inner (void *heap, unsigned int nbytes)
{
  int ret;
  void *addr;

  nbytes = ((nbytes  + MIN_FREE_BYTES - 1) / MIN_FREE_BYTES )  * MIN_FREE_BYTES;

  addr = (void *)local_first (heap);

  if (local_status (addr) == FREE)
  {
    ret = currentNodeAlloc (heap, addr, nbytes);
    if (ret == TRUE)
      return ((uintptr_t)addr + SIZE_HEADER);
  }

  while (local_next (addr) != 0)
  {
    addr = (void *)local_next (addr);
    if (local_status (addr) == FREE)
    {
      ret = currentNodeAlloc (heap, addr, nbytes);
      if (ret == TRUE)
        return ((uintptr_t)addr + SIZE_HEADER);
    }
  }

  return 0;
}

void *Local_Alloc (void *heap, unsigned int nbytes)
{
  void *addr = (void *)Local_Alloc_Inner (heap, nbytes);
  if (addr != NULL)
    local_setalign (addr, 0);

  return addr;
}
/*end Local_Alloc*/

static void Local_Dealloc_Inner (void *heap, void *address)
{
  uintptr_t block;
  uintptr_t lblock;
  uintptr_t rblock;

  block = (uintptr_t)address - SIZE_HEADER;
  lblock = local_prev (block);
  rblock = local_next (block);

  /*
  4 cases: FFF->F, FFR->FR, RFF->RF, RFR
  always want to merge free blocks
  */

  if ((lblock != 0) && (local_status (lblock) == FREE)
      && (rblock != 0) && (local_status (rblock) == FREE))
  {
    local_next (lblock) = local_next (rblock);
    local_status (lblock) = FREE;
    if (local_next(rblock) != 0)
      local_prev (local_next (rblock)) = lblock;
  }
  else
    if ((lblock != 0) && (local_status (lblock) == FREE))
    {
      local_next (lblock) = local_next (block);
      local_status (lblock) = FREE;
      if (local_next (block) != 0)
        local_prev (local_next (block)) = lblock;
    }
    else
      if ((rblock != 0) && (local_status (rblock) == FREE))
      {
        local_next (block) = local_next (rblock);
        local_status (block) = FREE;
        if (local_next (rblock) != 0)
          local_prev (local_next (rblock)) = block;
      }
      else
        local_status (block) = FREE;
}

/*Note: disaster will strike if fed wrong address*/

void Local_Dealloc(void *heap, void *address)
{
  /*
  4 cases: FFF->F, FFR->FR, RFF->RF, RFR
  always want to merge free blocks
  */
  address = (void *)((uintptr_t)address - local_getalign (address));
  Local_Dealloc_Inner (heap, address);

  return;
}
/*end Local_Dealloc*/

void *Local_Realloc (void *heap, void *address, unsigned int nbytes)
{
  uintptr_t rr, addr;
  unsigned int oldsize, bsize, align, len;
  uintptr_t block, rblock, rrblock;

  oldsize = nbytes;

  nbytes = ((nbytes  + MIN_FREE_BYTES - 1) / MIN_FREE_BYTES )  * MIN_FREE_BYTES;

  rr = (uintptr_t) address;

  if (nbytes == 0)
  {
    Local_Dealloc (heap, address);
    return 0;
  }

  if (address == NULL)
  {
    addr = Local_Alloc_Inner (heap, nbytes);
    if (addr != 0)
      local_setalign (addr, 0);
    return (void *)addr;
  }

  align = local_getalign (address);
  len = (nbytes + align  + MIN_FREE_BYTES - 1) & (~(MIN_FREE_BYTES - 1));

  address = (void *)((uintptr_t)address - local_getalign (address) - SIZE_HEADER);
  bsize = local_size (address);

  if (nbytes <= bsize - align)
    return (void *)rr;

  rblock = local_next (address);
  if ((rblock != 0) && (local_status (rblock) == FREE))
  {
    bsize += local_size(rblock);
    if (bsize >= nbytes + align)
    {
      rrblock = local_next (rblock);
      local_next (address) = (uintptr_t)address + len + SIZE_HEADER;
      block = local_next (address);
      local_prev (block) = (uintptr_t)address;
      local_next (block) = rrblock;
      local_status (block) = FREE;

      return (void *)rr;
    }
  }

  addr = Local_Alloc_Inner(heap, len);

  if (addr == 0)
    return NULL;

  addr += align;
  local_setalign (addr, align);
  memcpy ((void *)addr, (void *)rr, bsize);
  Local_Dealloc (heap, (void *)rr);

  return (void *)addr;
}

void *Local_Calloc (void *heap, unsigned int size, unsigned int n)
{
  void *rr;

  rr = Local_Alloc (heap, size * n);
  if (rr != NULL)
    memset(rr, 0, size * n);

  return rr;
}

void *Local_alignAlloc (void *heap, unsigned int align, unsigned int size)
{
  uintptr_t addr, addr2 = 0;

  addr = Local_Alloc_Inner (heap, size + align);

  if (addr != 0)
  {
    addr2 = (addr +  align - 1) & ~(align - 1);
    local_setalign (addr2, addr2 - addr);
  }

  return (void *)addr2;
} /*end alloc*/

