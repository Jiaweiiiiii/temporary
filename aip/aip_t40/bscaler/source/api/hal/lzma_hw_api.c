#ifndef __LZMA_TC_H__
#define __LZMA_TC_H__
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "lzma_hw_api.h"

#ifdef CSE_SIM_ENV
void *lzma_base_vaddr = NULL;
static void *lzma_base_paddr = NULL;
static int lzma_base_size = 0;
void lzma_mem_init(){
    int memfd = open("/dev/mem", O_RDWR | O_SYNC);
    if (memfd  < 0) {
        printf("open /dev/mem failed:%s\n", strerror(errno));
        goto err_open_mem;
    }
    lzma_base_paddr = (void *)LZMA_BASE;
    lzma_base_size = 0x00001000;
    lzma_base_vaddr = mmap(NULL, lzma_base_size, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, (off_t)lzma_base_paddr);
    if (lzma_base_vaddr == MAP_FAILED) {
        printf("mmap paddr=%p size=0x%08x failed:%s\n", lzma_base_paddr, lzma_base_size, strerror(errno));
        goto err_mmap_nnlzma;
    }
err_open_mem:
err_mmap_nnlzma:
    close(memfd);
    memfd = -1;
}
#endif

#if 0
void *lzma_malloc(uint32_t align, uint32_t size)
{
    void *addr;
#ifdef EYER_SIM_ENV
    addr = malloc(align + size);
#elif CSE_SIM_ENV
    addr = ddr_malloc(align + size);
#else
    addr = (void *)cMalloc(NULL, align + size, 1);
#endif
    if (addr == NULL)
        fprintf(stderr, "error: lzma malloc failed!\n");
    return (void *)(((uint32_t)addr + align - 1) & (~(align - 1)));
}

void lzma_free(void *p2)
{
    void *p1 = ((void**)p2)[-1];
#ifdef EYER_SIM_ENV
    free(p1);
#elif CSE_SIM_ENV
    ddr_free(p1);
#else
    cFree(p1);
#endif
}
#endif

void lzma_write_reg(uint32_t reg, uint32_t val) {
#ifdef EYER_SIM_ENV
    write_reg(LZMA_BASE+reg, val);
#elif CSE_SIM_ENV
    *(volatile unsigned int*)(lzma_base_vaddr + reg) = val;
#else
    CpuWrite(LZMA_BASE+reg, val);
#endif
}

uint32_t lzma_read_reg(uint32_t reg, uint32_t val) {
#ifdef EYER_SIM_ENV
    return (read_reg(LZMA_BASE+reg, 0));
#elif CSE_SIM_ENV
    return (*(volatile unsigned int*)(lzma_base_vaddr + reg));
#else
    return (CpuRead(LZMA_BASE+reg));
#endif
}

uint32_t lzma_cfg(LZMA_CFG_S *cfg) {
    while (((lzma_read_reg(LZMA_CTRL, 0) >> 31) & 0x1) != 1) { //switch to lzma.
        lzma_write_reg(LZMA_CTRL, 1<<31);
    }
    lzma_write_reg(LZMA_CTRL, 2); //ram initial.
    while (lzma_read_reg(LZMA_CTRL, 2) & 0x2); //wait ram initial done.
#ifdef EYER_SIM_ENV
    lzma_write_reg(LZMA_BS_BASE, (uint32_t)cfg->bs_base);
    lzma_write_reg(LZMA_DST_BASE, cfg->dst_base);
#elif CSE_SIM_ENV
    lzma_write_reg(LZMA_BS_BASE, (uint32_t)__aie_get_ddr_paddr((uint32_t)cfg->bs_base));
    lzma_write_reg(LZMA_DST_BASE, (uint32_t)__aie_get_ddr_paddr((uint32_t)cfg->dst_base));
#else
    lzma_write_reg(LZMA_BS_BASE, (uint32_t)cGetPaddr(cfg->bs_base));
    lzma_write_reg(LZMA_DST_BASE, (uint32_t)cGetPaddr(cfg->dst_base));
#endif
    lzma_write_reg(LZMA_BS_SIZE, cfg->bs_size*sizeof(uint8_t));
    printf("lzmadec...\n");
    lzma_write_reg(LZMA_CTRL, 1);
}

#endif
