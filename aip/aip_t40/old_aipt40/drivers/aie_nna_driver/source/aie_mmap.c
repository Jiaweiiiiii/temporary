/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : aie_mmap.c
 * Authors    : yzhai@aram.ic.jz.com
 * Create Time: 2019-11-13:17:29:36
 * Description:
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
//#define USE_SIM
#ifndef USE_SIM
#include <sys/mman.h>
#include <sys/ioctl.h>
#endif
#include <string.h>
#include <unistd.h>
#include "oram_mem.h"
#include "aie_mmap.h"

#define _GNU_SOURCE

/*
 * USE_RMEM: ddr memory space comes from rmem, else ddr memory comes from system memory; For from system memory need more time
 * to more larger ddr size, so advice to open USE_RMEM
 */
#define USE_RMEM

/* IOCTL MACRO DEFINE PLACE */
#define SOC_NNA_MAGIC               'c'
#define IOCTL_SOC_NNA_MALLOC        _IOWR(SOC_NNA_MAGIC, 0, int)
#define IOCTL_SOC_NNA_FREE          _IOWR(SOC_NNA_MAGIC, 1, int)
#define IOCTL_SOC_NNA_FLUSHCACHE    _IOWR(SOC_NNA_MAGIC, 2, int)
#define IOCTL_SOC_NNA_SETUP_DES     _IOWR(SOC_NNA_MAGIC, 3, int)
#define IOCTL_SOC_NNA_RDCH_START    _IOWR(SOC_NNA_MAGIC, 4, int)
#define IOCTL_SOC_NNA_WRCH_START    _IOWR(SOC_NNA_MAGIC, 5, int)

typedef struct soc_nna_buf {
    void        *vaddr;
    void        *paddr;
    int         size;
} soc_nna_buf_t;

struct flush_cache_info {
    unsigned int    addr;
    unsigned int    len;
    unsigned int    dir;
};

static int __memfd = -1;
static int __nnafd = -1;
static int __b_use_rmem = 0;

void *__nndma_io_vbase = NULL;
void *__nndma_desram_vbase = NULL;
void *__nndbg_io_vbase = NULL;
void *__bscaler_io_vbase = NULL;
void *__oram_vbase = NULL;
void *__ddr_vbase = NULL;
void *__ddr_pbase = NULL;
void *__nndma_fastio_vbase = NULL;
unsigned int __nndma_desram_cache_attr = 0;
unsigned int __oram_vbase_cache_attr = 0;
unsigned int __ddr_vbase_cache_attr = 0;

static soc_nna_buf_t __ddr_nna_buf;

#ifdef USE_SIM
#define GET_SIM_VBASE(a)        ((void *)((a) + 0xa0000000))
#define DDR_SIM_PBASE           0x08000000
#define ORAM_SIM_VBASE          (0x30000000 + L2C_SIZE)
int __aie_mmap(int ddr_mem_size, int b_use_rmem, nna_cache_attr_t desram_cache_attr, nna_cache_attr_t oram_cache_attr, nna_cache_attr_t ddr_cache_attr)
{
	printf("###%d   %s\n", __LINE__, __func__);
    int32_t ret = 0;
    //NNDMA
    __nndma_io_vbase = GET_SIM_VBASE(NNDMA_IO_BASE);
    __nndma_desram_vbase = GET_SIM_VBASE(NNDMA_DESRAM_BASE);

    //NNDBG
    __nndbg_io_vbase = GET_SIM_VBASE(NNDBG_IO_BASE);

    //BSCALER
    __bscaler_io_vbase = GET_SIM_VBASE(BSCALER_IO_BASE);

    //ORAM
    __oram_vbase = (void *)ORAM_SIM_VBASE;

    //DDR
    __ddr_pbase = (void *)DDR_SIM_PBASE;
    __ddr_vbase = GET_SIM_VBASE(__ddr_pbase);

    ret = ddr_memory_init(__ddr_vbase, ddr_mem_size);
    if (!ret) {
        printf ("DDR Memory init failed !!\n");
        return 0;
    }
    ret =  oram_memory_init(__oram_vbase, ORAM_SIZE);
    if (!ret) {
        printf ("ORAM Memory init failed !!\n");
        return 0;
    }

    return 1;
}
#else
#define CMD_LINE "/proc/cmdline"
static int get_rmem_info(void **pRmemAddr, int *pRmemSize)
{
    uint32_t addr = 0;
    int size = 0;
    char buf[512] = "";
    char *p = NULL;

    FILE *fb = fopen(CMD_LINE, "r");
    if(fb == NULL) {
        printf("%s open file (%s) error\n", __func__, CMD_LINE);
        goto err_fopen_cmdline;
    }

    if (fread(buf, 1, sizeof(buf), fb) <= 0) {
        printf("%s fread (%s) error\n", __func__, CMD_LINE);
        goto err_fread_cmdline;
    }
#if 1
    if ((p = strstr(buf, "nmem")) == NULL) {
        printf("%s fread (%s) error\n", __func__, CMD_LINE);
        goto err_strstr_rmem;
    }

    char *atsignpos = strchr(p, '@');
    if (*(atsignpos - 1) == 'M') {
        sscanf(p, "nmem=%dM@%x", &size, &addr);
        size = size * 1024 * 1024;
        printf("[M] size: %d\n", size);
    } else if (*(atsignpos - 1) == 'K') {
        sscanf(p, "nmem=%dK@%x", &size, &addr);
        size = size * 1024;
        printf("[K] size: %d\n", size);
    } else {
        goto err_rmem_unit;
    }
#endif
#if 0
    if ((p = strstr(buf, "rmem")) == NULL) {
        printf("%s fread (%s) error\n", __func__, CMD_LINE);
        goto err_strstr_rmem;
    }

    char *atsignpos = strchr(p, '@');
    if (*(atsignpos - 1) == 'M') {
        sscanf(p, "rmem=%dM@%x", &size, &addr);
        size = size * 1024 * 1024;
        printf("[M] size: %d\n", size);
    } else if (*(atsignpos - 1) == 'K') {
        sscanf(p, "rmem=%dK@%x", &size, &addr);
        size = size * 1024;
        printf("[K] size: %d\n", size);
    } else {
        goto err_rmem_unit;
    }
#endif

    *pRmemAddr = (void *)addr;
    *pRmemSize = size;

    if (!addr || !size) {
        printf("CMD Line Rmem Size:%d, Addr:0x%08x is invalide\n", size, addr);
        goto err_inv_kmem_addr_or_len;
    }

    printf("CMD Line Rmem Size:%x, Addr:0x%08x\n", size, addr);

    fclose(fb);

    return 0;

err_inv_kmem_addr_or_len:
err_rmem_unit:
err_strstr_rmem:
err_fread_cmdline:
    fclose(fb);
err_fopen_cmdline:
    return -1;
}

int __aie_mmap(int ddr_mem_size, int b_use_rmem, nna_cache_attr_t desram_cache_attr, nna_cache_attr_t oram_cache_attr, nna_cache_attr_t ddr_cache_attr)
{
    int ret = 0;
    int mmap_fd = -1;

    printf("__aie_mmap start\n");
    __b_use_rmem = b_use_rmem;
    __nndma_desram_cache_attr = desram_cache_attr;
    __oram_vbase_cache_attr = oram_cache_attr;
    __ddr_vbase_cache_attr = ddr_cache_attr;

    // if use __memfd, all memory mmaped uncached
    __memfd = open("/dev/mem", O_RDWR | O_SYNC);
    if(__memfd < 0) {
        printf("Error: /dev/mem open failed: %s\n", strerror(errno));
        goto err_open_mem;
    }

    __nnafd = open("/dev/soc-nna", O_RDWR | (__ddr_vbase_cache_attr & NNA_CACHED ? 0 : O_SYNC));
    if(__nnafd < 0) {
        printf("Error: /dev/soc-nna open failed:%s\n", strerror(errno));
        goto err_open_soc_nna;
    }

    //NNDMA
    __nndma_io_vbase = mmap(NULL, NNDMA_IO_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, __memfd, (off_t)NNDMA_IO_BASE);
    if (__nndma_io_vbase == MAP_FAILED) {
        printf("Error: NNDMA IO mmap paddr=0x%x size=0x%x failed:%s\n", NNDMA_IO_BASE, NNDMA_IO_SIZE, strerror(errno));
        goto err_mmap_nndma_io;
    }
    printf("mmaped __nndma_io_vbase=%p, NNDMA_IO_BASE=0x%x, NNDMA_IO_SIZE=0x%x uncached successed\n", __nndma_io_vbase, NNDMA_IO_BASE, NNDMA_IO_SIZE);

    mmap_fd = __nndma_desram_cache_attr & NNA_UNCACHED_ACCELERATED ? __nnafd : __memfd;
    __nndma_desram_vbase = mmap(NULL, NNDMA_DESRAM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mmap_fd, (off_t)NNDMA_DESRAM_BASE);
    if (__nndma_desram_vbase == MAP_FAILED) {
        printf("Error: NNDMA DESRAM mmap paddr=0x%x size=0x%x failed:%s\n", NNDMA_DESRAM_BASE, NNDMA_DESRAM_SIZE, strerror(errno));
        goto err_mmap_nndma_desram;
    }
    printf("mmaped __nndma_desram_vbase=%p, NNDMA_DESRAM_BASE=0x%x, NNDMA_DESRAM_SIZE=0x%x %s successed\n", __nndma_desram_vbase, NNDMA_DESRAM_BASE, NNDMA_DESRAM_SIZE,
            mmap_fd == __nnafd ? "uncached accelerated" : "uncached");

    //NNDBG
    __nndbg_io_vbase = mmap(NULL, NNDBG_IO_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, __memfd, (off_t)NNDBG_IO_BASE);
    if (__nndbg_io_vbase == MAP_FAILED) {
        printf("Error: NNDBG IO mmap paddr=0x%x size=0x%x failed:%s\n", NNDBG_IO_BASE, NNDBG_IO_SIZE, strerror(errno));
        goto err_mmap_nndbg_io;
    }
    printf("mmaped __nndbg_io_vbase=%p, NNDBG_IO_BASE=0x%x, NNDBG_IO_SIZE=0x%x uncached success\n", __nndbg_io_vbase, NNDBG_IO_BASE, NNDBG_IO_SIZE);

    //BSCALER
    __bscaler_io_vbase = mmap(NULL, BSCALER_IO_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, __memfd, (off_t)BSCALER_IO_BASE);
    if (__bscaler_io_vbase == MAP_FAILED) {
        printf("Error: BSCALER IO mmap paddr=0x%x size=0x%x failed:%s\n", BSCALER_IO_BASE, BSCALER_IO_SIZE, strerror(errno));
        goto err_mmap_bscaler_io;
    }
    printf("mmaped __bscaler_io_vbase=%p, BSCALER_IO_BASE=0x%x, BSCALER_IO_SIZE=0x%x uncached successed\n", __bscaler_io_vbase, BSCALER_IO_BASE, BSCALER_IO_SIZE);

    //ORAM
    mmap_fd = __oram_vbase_cache_attr & NNA_UNCACHED_ACCELERATED ? __nnafd : __memfd;
    __oram_vbase = mmap(NULL, ORAM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mmap_fd, (off_t)ORAM_BASE);//(0x12600000+L2C_SIZE)       
    if (__oram_vbase == MAP_FAILED) {
        printf("Error: ORAM mmap paddr=%p size=0x%08x failed:%s\n", ORAM_BASE, ORAM_SIZE, strerror(errno));
        goto err_mmap_oram;
    }
    printf("mmaped __oram_vbase=%p, ORAM_BASE=0x%x, ORAM_SIZE=0x%x %s successed\n", __oram_vbase, ORAM_BASE, ORAM_SIZE,
            mmap_fd == __nnafd ? "uncached accelerated" : "uncached");

    __ddr_nna_buf.size = ddr_mem_size;
    printf("%s(%d):want to allocate __ddr_nna_buf.size=%d\n", __func__, __LINE__, __ddr_nna_buf.size);
    if (b_use_rmem) {
        int rmemSize = 0;
        int val = get_rmem_info(&__ddr_nna_buf.paddr, &rmemSize);
        printf("val:%d\n", val);
        printf("ddr_mem_size:%d\n", ddr_mem_size);
        printf("rmemSize:%d\n", rmemSize);
        printf("__ddr_nna_buf.paddr: %08x\n", __ddr_nna_buf.paddr);
        if ((val < 0) || (rmemSize < ddr_mem_size)) {
            printf("get_rmem_info failed\n");
            goto err_get_rmem_info;
        }
    } else {
        ret = ioctl(__nnafd, IOCTL_SOC_NNA_MALLOC, &__ddr_nna_buf);
        if (ret < 0) {
            printf("Error: DDR Malloc size=%d failed:%s\n", __ddr_nna_buf.size, strerror(errno));
            goto err_ioctl_nna_malloc;
        }
    }

    __ddr_pbase = __ddr_nna_buf.paddr;
    printf("__ddr_pbase: %08x\n", __ddr_pbase);
    mmap_fd = __ddr_vbase_cache_attr & NNA_UNCACHED ? __memfd : __nnafd;
    __ddr_vbase = mmap(NULL, ddr_mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, mmap_fd, (off_t)__ddr_pbase);
    if (__ddr_vbase == MAP_FAILED) {
        printf("Error: DDR mmap paddr=%p size=0x%08x failed:%s\n", __ddr_pbase, ddr_mem_size, strerror(errno));
        goto err_mmap_ddr_nna;
    }
    printf("mmaped __ddr_vbase=%p, __ddr_pbase=%p, ddr_mem_size=0x%x %s successed\n", __ddr_vbase, __ddr_pbase, ddr_mem_size,
            __ddr_vbase_cache_attr & NNA_CACHED ? "cached" : (__ddr_vbase_cache_attr & NNA_UNCACHED_ACCELERATED ? "uncached accelerated" : "uncached"));

    /* __nndma_fastio_vbase must be an uncached address which phyaddr should be start with 0x0******* value */
    __nndma_fastio_vbase = mmap(NULL, FASTIO_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, __memfd, (off_t)((intptr_t)__ddr_pbase & 0x0fffffff));
    if (__nndma_fastio_vbase == MAP_FAILED) {
        printf("Error: fastio mmap paddr=%p size=0x%08x failed:%s\n", ((intptr_t)__ddr_pbase & 0x0fffffff), FASTIO_SIZE, strerror(errno));
        goto err_mmap_fastio;
    }
    printf("mmaped __nndma_fastio_vbase=%p, paddr=%p, FASTIO_SIZE=0x%x uncached success\n", __nndma_fastio_vbase, ((intptr_t)__ddr_pbase & 0x0fffffff), FASTIO_SIZE);

    ret = ddr_memory_init(__ddr_vbase, ddr_mem_size);
    if (!ret) {
        printf ("DDR Memory init failed !!\n");
        goto err_ddr_memory_init;
    }
    ret =  oram_memory_init(__oram_vbase, ORAM_SIZE);
    if (!ret) {
        printf ("ORAM Memory init failed !!\n");
        goto err_oram_memory_init;
    }

    return 1;

err_oram_memory_init:
    ddr_memory_deinit();
err_ddr_memory_init:
    munmap(__nndma_fastio_vbase, FASTIO_SIZE);
err_mmap_fastio:
    munmap(__ddr_vbase, ddr_mem_size);
err_mmap_ddr_nna:
err_get_rmem_info:
    if (!__b_use_rmem) {
        ioctl(__nnafd, IOCTL_SOC_NNA_FREE, &__ddr_nna_buf);
    }
err_ioctl_nna_malloc:
    munmap(__oram_vbase, ORAM_SIZE);
err_mmap_oram:
    munmap(__bscaler_io_vbase, BSCALER_IO_SIZE);
err_mmap_bscaler_io:
    munmap(__nndbg_io_vbase, NNDBG_IO_SIZE);
err_mmap_nndbg_io:
    munmap(__nndma_desram_vbase, NNDMA_DESRAM_SIZE);
err_mmap_nndma_desram:
    munmap(__nndma_io_vbase, NNDMA_IO_SIZE);
err_mmap_nndma_io:
    close(__nnafd);
    __nnafd = -1;
err_open_soc_nna:
    close(__memfd);
    __memfd = -1;
err_open_mem:
    return 0;
}

int __aie_munmap()
{
    oram_memory_deinit();
    ddr_memory_deinit();
    munmap(__nndma_fastio_vbase, FASTIO_SIZE);
    munmap(__ddr_vbase, __ddr_nna_buf.size);
    if (!__b_use_rmem) {
        ioctl(__nnafd, IOCTL_SOC_NNA_FREE, &__ddr_nna_buf);
    }
    munmap(__oram_vbase, ORAM_SIZE);
    munmap(__bscaler_io_vbase, BSCALER_IO_SIZE);
    munmap(__nndbg_io_vbase, NNDBG_IO_SIZE);
    munmap(__nndma_desram_vbase, NNDMA_DESRAM_SIZE);
    munmap(__nndma_io_vbase, NNDMA_IO_SIZE);
    close(__nnafd);
    __nnafd = -1;
    close(__memfd);
    __memfd = -1;
    return 1;
}
#endif

int __aie_flushcache(void *ddr_mem_vaddr, int ddr_mem_size)
{
    struct flush_cache_info info = {
        .addr = (unsigned int)ddr_mem_vaddr,
        .len = (unsigned int)ddr_mem_size,
        .dir = 0,
    };

    if (ioctl(__nnafd, IOCTL_SOC_NNA_FLUSHCACHE, &info) < 0) {
        printf("flush cache %p(%d) failed\n", ddr_mem_vaddr, ddr_mem_size);
        return -1;
    }

    return 0;
}

int __aie_flushcache_dir(void *ddr_mem_vaddr, int ddr_mem_size, enum nna_dma_data_direction dir)
{
    struct flush_cache_info info = {
        .addr = (unsigned int)ddr_mem_vaddr,
        .len = (unsigned int)ddr_mem_size,
        .dir = dir,
    };

    if (ioctl(__nnafd, IOCTL_SOC_NNA_FLUSHCACHE, &info) < 0) {
        printf("flush cache %p(%d) failed\n", ddr_mem_vaddr, ddr_mem_size);
        return -1;
    }

    return 0;
}
