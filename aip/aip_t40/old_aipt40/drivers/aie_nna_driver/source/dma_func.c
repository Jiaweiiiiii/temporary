/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : dma_func.c
 * Authors    : jzhang@elim.ic.jz.com
 * Create Time: 2020-03-02:20:53:18
 * Description:
 * Common DMA functions
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "oram_mem.h"
#include "aie_mmap.h"
#include "aie_nndma.h"
#include "dma_func.h"

#define FLUSH_CACHE_INTERNEL

#define SIZE_LIMIT (1<<(12+6))
#define SIZE_BIG_LIMIT (1<<(12+5+6))

/* DMA a number of data blocks from DDR to ORAM.
   optional allocate ORAM, setup DMA and run it immidiately. Used to get W/BT

   do_oralloc       input: alloc ORAM
   blk_num:         number of data blocks
   ddr_ptr[blk_num] input: ddr points of these blocks
   or_ptr[blk_num]  input or output: ORAM point of these blocks, may alloc here
   size[blk_num]    input: size of these blocks
*/
void w_d2o_orasetrun(int do_oralloc, int blk_num,
             uint8_t ** ddr_ptr, uint8_t ** or_ptr, uint32_t * size) {
  //get desram mmap addr and its RAM entry number
  uint32_t des_addr = __aie_get_desram_W_ping();
  uint32_t des_entry = __aie_get_desram_ptr(des_addr);
#ifdef FLUSH_CACHE_INTERNEL
  uint32_t total_size = 0, ddr_linked = 1;
  uint8_t *start_flush_ddr_ptr = NULL;
  uint32_t is_ddr_cahced = __aie_get_ddr_cache_attr() & NNA_CACHED;
#endif

  for (int i_b=0; i_b<blk_num; i_b++) {
    if (do_oralloc)
      or_ptr[i_b] = (uint8_t*)oram_memalign(64, size[i_b]);
#ifdef __AIE_VALID_CHECK__
    if ((uint32_t)ddr_ptr[i_b] & 0x3f || (uint32_t)or_ptr[i_b] & 0x3f ||
    (uint32_t)size[i_b] & 0x3f || (uint32_t)size[i_b] > SIZE_BIG_LIMIT) {
      printf("Error: %s:%d invalid data\n",__func__,__LINE__);
      exit(-1);
    }
#endif
    uint32_t ddr_paddr = __aie_get_ddr_paddr((uint32_t)ddr_ptr[i_b]);
    uint32_t or_offset = __aie_get_oram_offset((uint32_t)or_ptr[i_b]);
    __aie_push_nndma_bignode_maylast(i_b==blk_num-1, &des_addr,
                     or_offset, ddr_paddr, size[i_b]);
#ifdef FLUSH_CACHE_INTERNEL
    if (is_ddr_cahced) {
        if ((i_b < blk_num - 1) && ((ddr_ptr[i_b + 1] - ddr_ptr[i_b] - size[i_b]) <= 0)) {
            ddr_linked = 1;
            total_size += size[i_b];
            if (!start_flush_ddr_ptr) {
                start_flush_ddr_ptr = ddr_ptr[i_b];
            }
        } else if (ddr_linked && start_flush_ddr_ptr && total_size > 0) {
            __aie_flushcache(start_flush_ddr_ptr, total_size);
            ddr_linked = 0;
            total_size = 0;
            start_flush_ddr_ptr = NULL;
        } else {
            __aie_flushcache(ddr_ptr[i_b], size[i_b]);
            ddr_linked = 0;
            total_size = 0;
            start_flush_ddr_ptr = NULL;
        }
    }
#endif
#ifdef __AIE_INFO__
    printf("Info-0:w/1:bt: i:%d ddr:%x=>%x or:%x=>%x size:%x\n",
       i_b, ddr_ptr[i_b],ddr_paddr, or_ptr[i_b],or_offset, size[i_b]);
#endif
  }
  //__aie_set_nndma_unlink(&des_addr);

  //run the DMA
  __aie_run_nndma_W(des_entry);

  return;
}

/* setup a series of input feature DMA from DDR to rolling back ORAM,
   optional allocate ORAM. Used to get IF. All input except specified

   do_oralloc         alloc ORAM for orbuf of every ifp
   line_size          line size of 1 ifp in byte
   blk_lnum          line number of a block, 1 DMA to move
   blk0_lnum_more     line number difference of first block from other blocks
   orbuf_lnum          line number in ORAM buffer 1-ifp
   total_lnum          total line number to be moved
   ifp_num          ifp number
   qifp_num          quater size ifp number, which is half line size and half
              line number compare to normal ones. If >0, its in the
              first qifp_num IFP of the total ifp_num IFP
   ddr_ptr[ifp_num]   ddr point of every ifp
   or_ptr[ifp_num]    input/output: ORAM point of ifp's orbuf, may alloc here
   des_entry[des_num] output: entry number of every DMA chain, malloc here
   return des_num     des_entry number which is the DMA chain or move number
 */
int if_d2rbo_oraset(int do_oralloc, uint32_t line_size,
            int blk_lnum, int blk0_lnum_more, int orbuf_lnum,
            int total_lnum, int ifp_num, int qifp_num,
            int *qorbuf_size_p, uint8_t **ddr_ptr, uint8_t **or_ptr,
            uint32_t **des_entry_p) {

#ifdef __AIE_VALID_CHECK__
  if ((line_size & 0x3f) || blk0_lnum_more < 0 || total_lnum<1 ||
      qifp_num < 0 || qifp_num > ifp_num || ifp_num <= 0 ||
      qifp_num && (blk_lnum%2 || total_lnum%2 ||
           (orbuf_lnum - orbuf_lnum % blk_lnum)%2)) {
      printf("Error: %s:%d invalid data\n",__func__,__LINE__);
      exit(-1);
  }
#endif

  int des_num;
  if (total_lnum <= blk0_lnum_more)
    des_num = 1;
  else {
    des_num = (total_lnum - blk0_lnum_more) / blk_lnum;
    if ((total_lnum - blk0_lnum_more) % blk_lnum)
      des_num++;
  }

  uint32_t *des_entry = (uint32_t*)malloc(sizeof(uint32_t)*des_num);
  if (des_entry == NULL) {
    printf("Error: %s:%d malloc fail\n",__func__,__LINE__);
    exit(-1);
  }
  *des_entry_p = des_entry;

  uint32_t orbuf_size = line_size * orbuf_lnum;

  //all Q IFP needed variables
  uint32_t qline_size, qorbuf_size;
  int qblk_lnum, qtotal_lnum, qorbuf_lnum, qdes_num = 0, qblk0_lnum_more;
  if (qifp_num) {
    //q line_size also need to align to 64 byte
    qline_size = (line_size/2 + 63)/64 * 64;
    qtotal_lnum = total_lnum / 2;
    qblk_lnum = blk_lnum / 2;
    qorbuf_lnum = (orbuf_lnum + 2) / 2;
    qorbuf_size = qline_size * qorbuf_lnum;

    qblk0_lnum_more = (blk0_lnum_more+1)/2;
    if (qtotal_lnum <= qblk0_lnum_more)
      qdes_num = 1;
    else {
      qdes_num = (qtotal_lnum - qblk0_lnum_more) / qblk_lnum;
      if ((qtotal_lnum - qblk0_lnum_more) % qblk_lnum) qdes_num ++;
    }
  }

#ifdef FLUSH_CACHE_INTERNEL
  uint32_t is_ddr_cahced = __aie_get_ddr_cache_attr() & NNA_CACHED;
  uint32_t b_flush_all = 0;
  uint32_t total_tranfer_size = qifp_num*(qorbuf_size+qline_size) +
      (ifp_num-qifp_num)*orbuf_size;
  if (total_tranfer_size >= L2C_SIZE) {
      b_flush_all = 1;
  }
#endif
  //allocate ORAM/des_entry
  if (do_oralloc) {
    uint8_t* ptr = (uint8_t*)oram_memalign(64,
                       qifp_num*(qorbuf_size+qline_size) +
                       (ifp_num-qifp_num)*orbuf_size);
    if (ptr == NULL) {
      printf("Error: %s:%d ORAM malloc fail\n",__func__,__LINE__);
      exit(-1);
    }
    int i_fp=0;
    for (; i_fp<(ifp_num-qifp_num); i_fp++) {
      or_ptr[i_fp] = ptr; ptr +=  orbuf_size;}
    for (; i_fp<ifp_num; i_fp++) {
      or_ptr[i_fp] = ptr; ptr += (qorbuf_size+qline_size);}
  }
#ifdef __AIE_VALID_CHECK__
  if (line_size * (blk_lnum + blk0_lnum_more) > SIZE_LIMIT) {
    printf("Error: %s:%d invalid data\n",__func__,__LINE__);
    exit(-1);
  }
  for (int i_fp=0; i_fp<ifp_num; i_fp++)
    if ((uint32_t)ddr_ptr[i_fp] & 0x3f || (uint32_t)or_ptr[i_fp] & 0x3f) {
      printf("Error: %s:%d invalid data\n",__func__,__LINE__);
      printf("\t %d: %x %x\n",i_fp,ddr_ptr[i_fp],or_ptr[i_fp]);
      exit(-1);
    }
#endif

  //setup des chain. start from the begining of the orbuf
#define SET_SIZE(Q) {                            \
    if (i_dc == 0)                            \
      block_size = Q##line_size * (Q##blk_lnum + Q##blk0_lnum_more);    \
    else if (i_dc == Q##des_num-1)                    \
      block_size = Q##line_size * Q##total_lnum - Q##done_size;        \
    else block_size = Q##line_size * Q##blk_lnum;            \
    size_bef_rb = 0;                            \
    offset = Q##orbuf_offset;                        \
    if (Q##orbuf_offset + block_size > Q##orbuf_size) {            \
      size_bef_rb = Q##orbuf_size - Q##orbuf_offset;            \
      offset = 0;                            \
    }                                    \
    size = block_size - size_bef_rb;                    \
  }
#define GEN_DMA_DES(Q,last) {                        \
    if (size_bef_rb) {                            \
      ddr_paddr = __aie_get_ddr_paddr((uint32_t)(ddr_ptr[i_fp]+        \
                         Q##done_size));    \
      or_offset = __aie_get_oram_offset((uint32_t)(or_ptr[i_fp]+    \
                           Q##orbuf_offset));    \
      __aie_push_nndma_node(&des_addr, or_offset, ddr_paddr, size_bef_rb); \
    }                                    \
    ddr_paddr = __aie_get_ddr_paddr((uint32_t)(ddr_ptr[i_fp]+        \
                           Q##done_size+size_bef_rb)); \
    or_offset = __aie_get_oram_offset((uint32_t)(or_ptr[i_fp]+offset));    \
    __aie_push_nndma_node_maylast(i_fp==ifp_num-1 || (last), &des_addr,    \
                  or_offset, ddr_paddr, size);        \
  }
#define PRINT_INFO(Q) \
  printf("Info-if: %d-%d ddr:%x+%x=>%x or:%x+%x(%x)=>%x size:%x\n",    \
     i_dc,i_fp, ddr_ptr[i_fp],Q##done_size,ddr_paddr,        \
     or_ptr[i_fp],Q##orbuf_offset,Q##orbuf_size,or_offset, block_size)
#define SIZE_UPDATE(Q) {            \
    Q##done_size += block_size;            \
    Q##orbuf_offset += block_size;        \
    if (Q##orbuf_offset >= Q##orbuf_size)    \
      Q##orbuf_offset -= Q##orbuf_size;        \
  }

#ifdef FLUSH_CACHE_INTERNEL
#define DDR_FLUSH_CACHE(last_ifp_num) { \
    if (is_ddr_cahced && !b_flush_all) { \
        if ((i_fp < (last_ifp_num)) && ((ddr_ptr[i_fp + 1] - ddr_ptr[i_fp] - block_size) <= 0)) { \
            total_size += block_size; \
            ddr_linked = 1; \
            if (!start_flush_ddr_ptr) { \
                start_flush_ddr_ptr = ddr_ptr[i_fp]; \
            } \
        } else if (ddr_linked && start_flush_ddr_ptr && total_size > 0) { \
            __aie_flushcache(start_flush_ddr_ptr, total_size); \
            total_size = 0; \
            ddr_linked = 0; \
            start_flush_ddr_ptr = NULL; \
        } else { \
            __aie_flushcache(ddr_ptr[i_fp], block_size); \
            total_size = 0; \
            ddr_linked = 0; \
            start_flush_ddr_ptr = NULL; \
        } \
    } \
}
#endif

  int orbuf_offset = 0, qorbuf_offset = 0;
  int done_size = 0, qdone_size = 0;
  uint32_t ddr_paddr, or_offset;
#ifdef FLUSH_CACHE_INTERNEL
  uint32_t total_size = 0, ddr_linked = 1;
  uint8_t *start_flush_ddr_ptr = NULL;
#endif

  uint32_t des_addr = __aie_get_desram_IF_ping();
  for (int i_dc=0; i_dc<des_num; i_dc++) {
    des_entry[i_dc] = __aie_get_desram_ptr(des_addr);
    //printf("DMA dma-entry: %d:%d,%x\n",i_dc,des_entry[i_dc],des_addr);

    int i_fp=0, offset;
    uint32_t size, size_bef_rb;
    uint32_t block_size;

    //for normal IFP
    if (ifp_num-qifp_num) SET_SIZE();
    for (; i_fp<(ifp_num-qifp_num); i_fp++)
      {
    GEN_DMA_DES(,(i_fp==ifp_num-qifp_num-1 && i_dc>=qdes_num));
#ifdef FLUSH_CACHE_INTERNEL
    DDR_FLUSH_CACHE(ifp_num-qifp_num-1);
#endif
#ifdef __AIE_INFO__
    PRINT_INFO();
#endif
      }
    if (ifp_num-qifp_num) SIZE_UPDATE();

    //for q IFP
    if (qifp_num) SET_SIZE(q);
    for (; i_fp<ifp_num; i_fp++) if (i_dc < qdes_num) {
    GEN_DMA_DES(q,0);
#ifdef FLUSH_CACHE_INTERNEL
    DDR_FLUSH_CACHE((ifp_num-1));
#endif
#ifdef __AIE_INFO__
    PRINT_INFO(q);
#endif
      }
    if (qifp_num) SIZE_UPDATE(q);
  }
#ifdef FLUSH_CACHE_INTERNEL
  if (is_ddr_cahced && b_flush_all) {
      __aie_flushcache(ddr_ptr[0], total_tranfer_size);
  }
#endif

  return ifp_num-qifp_num ? des_num : qdes_num;
}

/* setup a series of output feature DMA from ORAM to DDR,
   optional allocate ORAM. Used to send OF. All input except specified

   blk_size          block size in byte for every one ofp DMA
   blk_num          block number for all run
   ofp_size           1 OFP size, which should <= blk_size*blk_num
                      > blk_size*(blk_num-1), =total_lnum*line_size

   do_oralloc         alloc ORAM for orbuf for all ofp
   line_size          line size of 1 ofp in byte
   blk_lnum          line number of a block, 1 DMA to move
   buf_bnum          block number in the ORAM buffer, usually 2 or 3
   total_lnum          total line number to be moved
   ofp_num          ofp number
   ddr_ptr            ddr point of all ofp
   or_ptr             input/output: ORAM point of orbuf, may alloc here
   des_entry[des_num] output: entry number of every DMA chain, malloc here
   return des_num
 */
int of_o2d_oraset(int do_oralloc, uint32_t line_size, int blk_lnum,
          int buf_bnum, int total_lnum, int ofp_num,
          uint8_t *ddr_ptr, uint8_t **or_ptr_p,
          uint32_t **des_entry_p) {

  //ORAM buffer size for 1 ofp and all ofp
  uint32_t blk_size = line_size * blk_lnum;
  uint32_t ofp_size = line_size * total_lnum;
  uint32_t orbuf_size = blk_size * buf_bnum;
  uint32_t total_orbuf_size = orbuf_size * ofp_num;
#ifdef FLUSH_CACHE_INTERNEL
  uint32_t is_ddr_cahced = __aie_get_ddr_cache_attr() & NNA_CACHED;
#endif

  int des_num = total_lnum / blk_lnum;
  int blklast_lnum = total_lnum % blk_lnum;
  if (blklast_lnum) des_num++;

  uint32_t *des_entry = (uint32_t*)malloc(sizeof(uint32_t)*des_num);
  if (des_entry == NULL) {
    printf("Error: %s:%d malloc fail\n",__func__,__LINE__);
    exit(-1);
  }
  *des_entry_p = des_entry;

  //allocate ORAM
  uint8_t *or_ptr;
  if (do_oralloc) {
    or_ptr = (uint8_t*)oram_memalign(64, total_orbuf_size);
    if (or_ptr == NULL) {
      printf("Error: %s:%d ORAM malloc fail\n",__func__,__LINE__);
      exit(-1);
    }
    *or_ptr_p = or_ptr;
  } else or_ptr = *or_ptr_p;

#ifdef __AIE_VALID_CHECK__
  if (line_size & 0x3f || blk_size > SIZE_LIMIT ||
      (uint32_t)(or_ptr) & 0x3f || (uint32_t)ddr_ptr & 0x3f){
    printf("Error: %s:%d invalid data\n",__func__,__LINE__);
    exit(-1);
  }
#endif

  //setup des chain. start from the begining of the orbuf
  int i_bb = 0, done_size = 0;
  uint32_t ddr_paddr, or_offset;
  uint32_t des_addr;
  des_addr = __aie_get_desram_OF_ping();
  for (int i_dc=0; i_dc<des_num; i_dc++) {
    des_entry[i_dc] = __aie_get_desram_ptr(des_addr);
    //printf("DMA dma-entry: %d:%d,%x\n",i_dc,des_entry[i_dc],des_addr);

    int orbuf_offset = i_bb * blk_size;
    i_bb++; if (i_bb == buf_bnum) i_bb = 0;
    uint32_t size = blk_size;
    if (i_dc == des_num-1 && blklast_lnum) size = line_size*blklast_lnum;
    for (int i_fp=0; i_fp<ofp_num; i_fp++) {
      int or_ofp_offset  = i_fp * orbuf_size;
      int ddr_ofp_offset = i_fp * ofp_size;
      ddr_paddr = __aie_get_ddr_paddr((uint32_t)(ddr_ptr+ddr_ofp_offset+
                         done_size));
      or_offset = __aie_get_oram_offset((uint32_t)(or_ptr+or_ofp_offset+
                           orbuf_offset));
      //printf("des: %x\n",des_addr);
      __aie_push_nndma_node_maylast(i_fp==ofp_num-1, &des_addr,
                    or_offset, ddr_paddr, size);
      //printf("des: %x\n",des_addr);
#ifdef __AIE_INFO__
      printf("Info-of: i:%d%d ddr:%x+%x=>%x or:%x+%x(%x)=>%x size:%x\n",
         i_dc,i_fp, ddr_ptr+ddr_ofp_offset,done_size,ddr_paddr,
         or_ptr+or_ofp_offset,orbuf_offset,orbuf_size,or_offset, size);
#endif
    }
    done_size += size;

    //end this DMA chain
    //__aie_set_nndma_unlink(&des_addr);
  }

#ifdef FLUSH_CACHE_INTERNEL
  if (is_ddr_cahced) {
      __aie_flushcache(ddr_ptr, done_size);
  }
#endif

  return des_num;
}

void aie_run_nndma_rdchn(int rd_chn, uint32_t ptr, int total_size)
{
    volatile uint32_t data = 0;
    uint32_t dma_io_vbase =  __aie_get_nndma_io_vbase();

    *(volatile uint32_t *)(dma_io_vbase + rd_chn * 4) = (0x1<<31 | ((total_size >> 6) << 12) | ptr);
    data = *(volatile uint32_t *)(dma_io_vbase);
}

void aie_run_nndma_wrchn(int wr_chn, uint32_t ptr, int total_size)
{
    volatile uint32_t data = 0;
    uint32_t dma_io_vbase = __aie_get_nndma_io_vbase();

    *(volatile int *)(dma_io_vbase + 0x10 + wr_chn * 4) = (0x1<<31 | ((total_size >> 6) << 12) | ptr);
    data = *(volatile uint32_t *)(dma_io_vbase);
}

void aie_wait_nndma_rdchn(int rd_chn)
{
  switch(rd_chn) {
    case 0:
      i_rdhwr(8);
      break;
    case 1:
      i_rdhwr(9);
      break;
    case 2:
      i_rdhwr(10);
      break;
  }
}

void aie_wait_nndma_wrchn(int wr_chn)
{
  switch(wr_chn) {
    case 0:
      i_rdhwr(11);
      break;
    case 1:
      i_rdhwr(12);
      break;
  }
}

int check_rd_wr_chn_same(uint32_t ddr_vaddr_in, uint32_t ddr_vaddr_out, int block_size)
{
  uint32_t *p1 = (uint32_t *)ddr_vaddr_in;
  uint32_t *p2 = (uint32_t *)ddr_vaddr_out;
  int i = 0;

  printf("start to check ddr_vaddr_in=%x, ddr_vaddr_out=%x\n", ddr_vaddr_in, ddr_vaddr_out);
  for (i = 0; i < block_size / 4; i++) {
    if (p1[i] != p2[i]) {
      printf("i=%d, check ddr_vaddr_in=%x(value=%x), ddr_vaddr_out=%x(value=%x) failed\n", i, p1 + i, p1[i], p2 + i, p2[i]);
      return -1;
    }
  }
  printf("check ddr_vaddr_in=%x, ddr_vaddr_out=%x success\n", ddr_vaddr_in, ddr_vaddr_out);

  return 0;
}
