/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : nna_dma_va_gen.c
 * Authors    : xqwu@aram.ic.jz.com
 * Create Time: 2019-05-29:12:10:39
 * Description:
 *
 */
#define _GNU_SOURCE

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#ifndef USE_SIM
#include <sys/mman.h>
#include <sys/ioctl.h>
#endif

#include "nna_dma_memory_v1.h"
#include "oram_mem.h"
#include "ddr_mem.h"
#include "soc_nna.h"

#define DEBUG_NNA_DMA_MEMORY

unsigned int *dma_rcfg_vaddr[3] = {NULL, NULL, NULL};
unsigned int *dma_wcfg_vaddr[2] = {NULL, NULL};
unsigned int *desram_rvaddr;
unsigned int *desram_wvaddr;

#ifdef USE_SIM
#else
static pthread_mutex_t nna_init_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif
static bool b_nna_memory_init = false;

static int memfd = -1;
void *nndma_base_vaddr = NULL;
void *nnoram_base_vaddr = NULL;
static void *nndma_base_paddr = NULL;
static int nndma_base_size = 0;

static void *oram_base_vaddr = NULL;
static void *oram_base_paddr = NULL;
static int oram_base_size = 0;

static int nnafd = -1;
static struct soc_nna_buf ddr_nna_buf;
static void *ddr_base_vaddr = NULL;
static void *ddr_base_paddr = NULL;

static int ddr_base_size = 0;

#ifdef USE_SIM
int nndma_memory_init(int max_ddr_size)
{
  int ret = 0;
  nndma_base_paddr = (void *)NNA_DMA_IO_BASE;
  nndma_base_size = NNA_DMA_IO_SIZE;
  nndma_base_vaddr = nndma_base_paddr + 0xa0000000;
  nnoram_base_vaddr = nndma_base_paddr;

#ifdef DEBUG_NNA_DMA_MEMORY
  printf ("NNDMA Memory init success: nndma_base_vanndma=%p, nndma_base_panndma=%p, nndma_base_size=0x%08x\n", nndma_base_vaddr, nndma_base_paddr, nndma_base_size);
#endif

  //-nndma cfg reg va addr
  dma_rcfg_vaddr[0] = (unsigned int *)(NNA_DMA_RCFG_VA_ADDR(0));
  dma_rcfg_vaddr[1] = (unsigned int *)(NNA_DMA_RCFG_VA_ADDR(1));
  dma_rcfg_vaddr[2] = (unsigned int *)(NNA_DMA_RCFG_VA_ADDR(2));
  dma_wcfg_vaddr[0] = (unsigned int *)(NNA_DMA_WCFG_VA_ADDR(0));
  dma_wcfg_vaddr[1] = (unsigned int *)(NNA_DMA_WCFG_VA_ADDR(1));
#ifdef DEBUG_NNA_DMA_MEMORY
  printf("dma_rcfg_vaddr=(%p,%p,%p), dma_wcfg_vaddr=(%p,%p)\n",
      dma_rcfg_vaddr[0], dma_rcfg_vaddr[1], dma_rcfg_vaddr[2],
      dma_wcfg_vaddr[0], dma_wcfg_vaddr[1]);
#endif

  //-desram va addr for driver
  desram_rvaddr = (unsigned int *)NNA_DESRAM_BASE_VA_ADDR;
//  desram_wvaddr = desram_rvaddr + 512;
  desram_wvaddr = desram_rvaddr + 2048;
#ifdef DEBUG_NNA_DMA_MEMORY
  printf("desram_rvaddr=%p, desram_wvaddr=%p\n", desram_rvaddr, desram_wvaddr);
#endif

  oram_base_vaddr = (void *)NNA_ORAM_BASE_VA_ADDR;
  oram_base_paddr = (void *)(NNA_ORAM_STR_ADDR + NNA_DMA_IO_BASE);
  oram_base_size = 0xE0000;

  ret =  oram_memory_init(oram_base_vaddr, oram_base_size);
  if(!ret) {
    printf ("Oram Memory init failed !!\n");
    goto err_oram_memory_init;
  }
#ifdef DEBUG_NNA_DMA_MEMORY
  printf ("ORAM Memory init success: oram_base_vaoram=%p, oram_base_paoram=%p, oram_base_size=0x%08x\n", oram_base_vaddr, oram_base_paddr, oram_base_size);
#endif

  ddr_base_paddr = 0x0e000000;
  ddr_base_size = max_ddr_size;
  ddr_base_vaddr = ddr_base_paddr + 0xa0000000;

  ret = ddr_memory_init(ddr_base_vaddr, ddr_base_size);
  if (!ret) {
    printf ("DDR Memory init failed !!\n");
    goto err_ddr_memory_init;
  }
#ifdef DEBUG_NNA_DMA_MEMORY
  printf ("DDR Memory init success: ddr_base_vaddr=%p, ddr_base_paddr=%p, ddr_base_size=0x%08x\n", ddr_base_vaddr, ddr_base_paddr, ddr_base_size);
#endif

  b_nna_memory_init = true;

  return 1;

err_ddr_memory_init:
  oram_memory_deinit();
err_oram_memory_init:
  b_nna_memory_init = false;
  return 0;
}

int nndma_memory_deinit()
{
  if (b_nna_memory_init) {
    b_nna_memory_init = false;
    ddr_memory_deinit();
    nnafd = -1;
    oram_memory_deinit();
  }
}
#else
int nndma_memory_init(int max_ddr_size)
{
  int ret = 0;
  cpu_set_t cpuset;

  pthread_mutex_lock(&nna_init_mutex);
  if (b_nna_memory_init) {
    printf("nndma_memory_init has been done\n");
    pthread_mutex_unlock(&nna_init_mutex);
    return 1;
  }

  CPU_ZERO(&cpuset);
  CPU_SET(0, &cpuset);
  ret = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
  if (ret != 0) {
    printf("pthread_setaffinity_np failed\n", strerror(ret));
    goto err_pthread_setaffinity_np;
  }

  memfd = open("/dev/mem", O_RDWR | O_SYNC);
  if (memfd  < 0) {
    printf("open /dev/mem failed:%s\n", strerror(errno));
    goto err_open_mem;
  }

  nndma_base_paddr = (void *)NNA_DMA_IO_BASE;
  nndma_base_size = NNA_DMA_IO_SIZE;
  nndma_base_vaddr = mmap(NULL, nndma_base_size, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, (off_t)nndma_base_paddr);
  if (nndma_base_vaddr == MAP_FAILED) {
    printf("mmap paddr=%p size=0x%08x failed:%s\n", nndma_base_paddr, nndma_base_size, strerror(errno));
    goto err_mmap_nndma;
  }

#ifdef DEBUG_NNA_DMA_MEMORY
  printf ("NNDMA Memory init success: nndma_base_vanndma=%p, nndma_base_panndma=%p, nndma_base_size=0x%08x\n", nndma_base_vaddr, nndma_base_paddr, nndma_base_size);
#endif

  //-nndma cfg reg va addr
  dma_rcfg_vaddr[0] = (unsigned int *)(NNA_DMA_RCFG_VA_ADDR(0));
  dma_rcfg_vaddr[1] = (unsigned int *)(NNA_DMA_RCFG_VA_ADDR(1));
  dma_rcfg_vaddr[2] = (unsigned int *)(NNA_DMA_RCFG_VA_ADDR(2));
  dma_wcfg_vaddr[0] = (unsigned int *)(NNA_DMA_WCFG_VA_ADDR(0));
  dma_wcfg_vaddr[1] = (unsigned int *)(NNA_DMA_WCFG_VA_ADDR(1));
#ifdef DEBUG_NNA_DMA_MEMORY
  printf("dma_rcfg_vaddr=(%p,%p,%p), dma_wcfg_vaddr=(%p,%p)\n",
      dma_rcfg_vaddr[0], dma_rcfg_vaddr[1], dma_rcfg_vaddr[2],
      dma_wcfg_vaddr[0], dma_wcfg_vaddr[1]);
#endif

  //-desram va addr for driver
  desram_rvaddr = (unsigned int *)NNA_DESRAM_BASE_VA_ADDR;
//  desram_wvaddr = desram_rvaddr + 512;
  desram_wvaddr = desram_rvaddr + 2048;
#ifdef DEBUG_NNA_DMA_MEMORY
  printf("desram_rvaddr=%p, desram_wvaddr=%p\n", desram_rvaddr, desram_wvaddr);
#endif

  oram_base_vaddr = (void *)NNA_ORAM_BASE_VA_ADDR;
  oram_base_paddr = (void *)(NNA_ORAM_STR_ADDR + NNA_DMA_IO_BASE);
  oram_base_size = 0xE0000;

  ret =  oram_memory_init(oram_base_vaddr, oram_base_size);
  if(!ret) {
    printf ("Oram Memory init failed !!\n");
    goto err_oram_memory_init;
  }
#ifdef DEBUG_NNA_DMA_MEMORY

  printf ("ORAM Memory init success: oram_base_vaoram=%p, oram_base_paoram=%p, oram_base_size=0x%08x\n", oram_base_vaddr, oram_base_paddr, oram_base_size);
#endif

  int nnafd = open("/dev/soc_nna", O_RDWR | O_SYNC);
  if (nnafd < 0) {
    printf("open /dev/soc_nna failed:%s\n", strerror(errno));
    goto err_open_soc_nna;
  }

  ddr_nna_buf.size = max_ddr_size;
  ret = ioctl(nnafd, IOCTL_SOC_NNA_MALLOC, &ddr_nna_buf);
  if (ret < 0) {
    printf("DDR Malloc size=%d failed:%s\n", ddr_nna_buf.size, strerror(errno));
    goto err_ioctl_nna_malloc;
  }

  ddr_base_paddr = ddr_nna_buf.paddr;
  ddr_base_size = ddr_nna_buf.size;
  ddr_base_vaddr = mmap(NULL, ddr_base_size, PROT_READ | PROT_WRITE, MAP_SHARED, nnafd, (off_t)ddr_base_paddr);
  if (nndma_base_vaddr == MAP_FAILED) {
    printf("mmap paddr=%p size=0x%08x failed:%s\n", nndma_base_paddr, nndma_base_size, strerror(errno));
    goto err_mmap_ddr_nna;
  }

  ret = ddr_memory_init(ddr_base_vaddr, ddr_base_size);
  if (!ret) {
    printf ("DDR Memory init failed !!\n");
    goto err_ddr_memory_init;
  }
#ifdef DEBUG_NNA_DMA_MEMORY
  printf ("DDR Memory init success: ddr_base_vaddr=%p, ddr_base_paddr=%p, ddr_base_size=0x%08x\n", ddr_base_vaddr, ddr_base_paddr, ddr_base_size);
#endif

  b_nna_memory_init = true;
  pthread_mutex_unlock(&nna_init_mutex);

  return 1;

err_ddr_memory_init:
  munmap(ddr_base_vaddr, ddr_base_size);
err_mmap_ddr_nna:
  ioctl(nnafd, IOCTL_SOC_NNA_FREE, &ddr_nna_buf);
err_ioctl_nna_malloc:
  close(nnafd);
  nnafd = -1;
err_open_soc_nna:
  oram_memory_deinit();
err_oram_memory_init:
  munmap(nndma_base_vaddr, nndma_base_size);
err_mmap_nndma:
  close(memfd);
  memfd = -1;
err_open_mem:
err_pthread_setaffinity_np:
  b_nna_memory_init = false;
  pthread_mutex_unlock(&nna_init_mutex);
  return 0;
}

int nndma_memory_deinit()
{
  pthread_mutex_lock(&nna_init_mutex);
  if (b_nna_memory_init) {
    b_nna_memory_init = false;
    ddr_memory_deinit();
    munmap(ddr_base_vaddr, ddr_base_size);
    ioctl(nnafd, IOCTL_SOC_NNA_FREE, &ddr_nna_buf);
    close(nnafd);
    nnafd = -1;
    oram_memory_deinit();
    munmap(nndma_base_vaddr, nndma_base_size);
    close(memfd);
    memfd = -1;
  }
  pthread_mutex_unlock(&nna_init_mutex);
}
#endif

void *nndma_oram_vir_to_phy(void *vaddr)
{
    return nndma_base_paddr + (vaddr - nndma_base_vaddr);
}

void *nndma_oram_phy_to_vir(void *paddr)
{
  return nndma_base_vaddr + (paddr - nndma_base_paddr);
}

void *nndma_ddr_vir_to_phy(void *vaddr)
{
    return ddr_base_paddr + (vaddr - ddr_base_vaddr);
}

void *nndma_ddr_phy_to_vir(void *paddr)
{
    return ddr_base_vaddr + (paddr - ddr_base_paddr);
}

//void *nndma_memory_vir_to_phy(void *vaddr)
//{
//  if ((vaddr >= nndma_base_vaddr) && (vaddr < nndma_base_vaddr + nndma_base_size)) {
//    return nndma_base_paddr + (vaddr - nndma_base_vaddr);
//  } else if ((vaddr >= ddr_base_vaddr) && (vaddr < ddr_base_vaddr + ddr_base_size)) {
//    return ddr_base_paddr + (vaddr - ddr_base_vaddr);
//  }
//
//  return NULL;
//}

void *nndma_memory_phy_to_vir(void *paddr)
{
  if ((paddr >= nndma_base_paddr) && (paddr < nndma_base_paddr + nndma_base_size)) {
    return nndma_base_vaddr + (paddr - nndma_base_paddr);
  } else if ((paddr >= ddr_base_paddr) && (paddr < ddr_base_paddr + ddr_base_size)) {
    return ddr_base_vaddr + (paddr - ddr_base_paddr);
  }

  return NULL;
}
