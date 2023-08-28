#include "aie_mmap.h"
#include "mem_manager/ddr_mem.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define ORAM_TEST_SIZE (0 * 1024 * 1024)
#define NMEM_TEST_SIZE (0 * 1024 * 1024)
#define DRIVERS_VERSION (0x00000041)
#define CMD_LINE "/proc/cmdline"

/* IOCTL MACRO DEFINE PLACE */
#define SOC_NNA_MAGIC 'c'
#define IOCTL_SOC_NNA_MALLOC _IOWR(SOC_NNA_MAGIC, 0, int)
#define IOCTL_SOC_NNA_FREE _IOWR(SOC_NNA_MAGIC, 1, int)
#define IOCTL_SOC_NNA_FLUSHCACHE _IOWR(SOC_NNA_MAGIC, 2, int)
#define IOCTL_SOC_NNA_SETUP_DES _IOWR(SOC_NNA_MAGIC, 3, int)
#define IOCTL_SOC_NNA_RDCH_START _IOWR(SOC_NNA_MAGIC, 4, int)
#define IOCTL_SOC_NNA_WRCH_START _IOWR(SOC_NNA_MAGIC, 5, int)
#define IOCTL_SOC_NNA_VERSION _IOWR(SOC_NNA_MAGIC, 6, int)
#define NMEM_INIT _IOWR(SOC_NNA_MAGIC, 7, int)
#define NNA_LOCK _IOWR(SOC_NNA_MAGIC, 8, int)
#define NNA_UNLOCK _IOWR(SOC_NNA_MAGIC, 9, int)
#define NMEM_UNINIT _IOWR(SOC_NNA_MAGIC, 10, int)

typedef struct soc_nna_buf {
    void *vaddr;
    void *paddr;
    int size;
} soc_nna_buf_t;

struct flush_cache_info {
    unsigned int addr;
    unsigned int len;
    unsigned int dir;
};

struct buf {
    uint32_t version_buf;
    uint32_t nmem_extension_buf;
    uint32_t nmem_paddr;
    uint32_t nmem_size;
};

struct user_nmem {
    int size;
    int paddr;
};

struct nmem_size_buf {
    uint32_t version_buf;
    uint32_t nmem_extension_buf;
    uint32_t nmem_paddr;
    uint32_t nmem_size;
};

static int __memfd = -1;
static int __nnafd = -1;
static int __nnalockfd = -1;
static int __b_use_rmem = 0;
static int __multiprocess_choose = 0;

void *__nndma_io_vbase = NULL;
void *__nndma_desram_vbase = NULL;
void *__oram_vbase = NULL;
void *__ddr_vbase = NULL;
void *__ddr_pbase = NULL;
void *__nndma_fastio_vbase = NULL;
unsigned int __nndma_desram_cache_attr = 0;
unsigned int __oram_vbase_cache_attr = 0;
unsigned int __ddr_vbase_cache_attr = 0;

static soc_nna_buf_t __ddr_nna_buf;
struct user_nmem unmem = {0};

void *l2cache_size_vaddr = NULL;
uint32_t l2cache_size_gpio = 0;
unsigned int l2cache_size = 0;
unsigned int oram_real_size = 0;
unsigned int oram_base = 0;

static int get_nmem_info(void **pRmemAddr, int *pRmemSize) {
    uint32_t addr = 0;
    int size = 0;
    char buf[512] = "";
    char *p = NULL;

    FILE *fb = fopen(CMD_LINE, "r");
    if (fb == NULL) {
        printf("%s open file (%s) error\n", __func__, CMD_LINE);
        goto err_fopen_cmdline;
    }

    if (fread(buf, 1, sizeof(buf), fb) <= 0) {
        printf("%s fread (%s) error\n", __func__, CMD_LINE);
        goto err_fread_cmdline;
    }

    if ((p = strstr(buf, "nmem")) == NULL) {
        printf("%s fread (%s) error\n", __func__, CMD_LINE);
        goto err_strstr_rmem;
    }

    char *atsignpos = strchr(p, '@');
    if (*(atsignpos - 1) == 'M') {
        sscanf(p, "nmem=%dM@%x", &size, &addr);
        size = size * 1024 * 1024;
    } else if (*(atsignpos - 1) == 'K') {
        sscanf(p, "nmem=%dK@%x", &size, &addr);
        size = size * 1024;
    } else {
        goto err_rmem_unit;
    }

    *pRmemAddr = (void *)addr;
    *pRmemSize = size;

    if (!addr || !size) {
        printf("CMD Line Nmem Size:%d, Addr:0x%08x is invalide\n", size, addr);
        goto err_inv_kmem_addr_or_len;
    }

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

static int get_oram_l2cache(int __memfd) {
    oram_base = 0x12600000;
    /*oramcache size*/
    int oram_size = 0;
    l2cache_size_vaddr =
        mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, __memfd, 0x12200000);
    if (l2cache_size_vaddr == NULL) {
        printf("get l2cache_size_vaddr failed\n");
        goto err_mmap_l2c_size;
    }

    l2cache_size_gpio = (*(unsigned long *)(l2cache_size_vaddr + 0x060));
    switch ((l2cache_size_gpio & 0x1c00) >> 10) {
    case 1:
        l2cache_size = 128 * 1024;
        oram_size = 1024 * 1024 - l2cache_size;
        break;
    case 2:
        l2cache_size = 256 * 1024;
        oram_size = 1024 * 1024 - l2cache_size;
        break;
    case 3:
        l2cache_size = 512 * 1024;
        oram_size = 1024 * 1024 - l2cache_size;
        break;
    case 4:
        l2cache_size = 1024 * 1024;
        oram_size = 1024 * 1024 - l2cache_size;
        break;
    default:
        break;
    }

    oram_base += l2cache_size;
    oram_size = 512 * 1024 - l2cache_size;

    return oram_size;
err_mmap_l2c_size:
    munmap(l2cache_size_vaddr, 0x1000);
    return -1;
}

int __aie_mmap(int ddr_mem_size, int b_use_rmem, nna_cache_attr_t desram_cache_attr,
               nna_cache_attr_t oram_cache_attr, nna_cache_attr_t ddr_cache_attr) {
    int ret = 0;
    int mmap_fd = -1;
    int rmemSize = 0;
    int val = 0;
    uint32_t oram_extend = ORAM_TEST_SIZE;
    uint32_t nmem_extend = NMEM_TEST_SIZE;

    printf("DRIVERS-version T41..\n");
    __b_use_rmem = b_use_rmem;
    __multiprocess_choose = ddr_mem_size;
    __nndma_desram_cache_attr = desram_cache_attr;
    __oram_vbase_cache_attr = oram_cache_attr;
    __ddr_vbase_cache_attr = ddr_cache_attr;

    // if use __memfd, all memory mmaped uncached
    __memfd = open("/dev/mem", O_RDWR | O_SYNC);
    if (__memfd < 0) {
        printf("Error: /dev/mem open failed: %s\n", strerror(errno));
        goto err_open_mem;
    }

    __nnafd = open("/dev/soc-nna", O_RDWR | (__ddr_vbase_cache_attr & NNA_CACHED ? 0 : O_SYNC));
    if (__nnafd < 0) {
        printf("Error: /dev/soc-nna open failed:%s\n", strerror(errno));
        goto err_open_soc_nna;
    }

    if (__multiprocess_choose != 0) {
        __nnalockfd = open("/dev/nna_lock", O_RDWR);
        if (__nnalockfd < 0) {
            printf("Error: /dev/nna_lock open failed:%s\n", strerror(errno));
            goto err_open_nna_lock;
        }
    }

    oram_real_size = get_oram_l2cache(__memfd);
    if (oram_real_size < 0) {
        printf("Error: give oram size failed failed:%s\n", strerror(errno));
        goto err_oram_real_size;
    }

    // NNDMA
    __nndma_io_vbase = mmap(NULL, NNDMA_IO_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, __memfd,
                            (off_t)NNDMA_IO_BASE);
    if (__nndma_io_vbase == MAP_FAILED) {
        printf("Error: NNDMA IO mmap paddr=0x%x size=0x%x failed:%s\n", NNDMA_IO_BASE,
               NNDMA_IO_SIZE, strerror(errno));
        goto err_mmap_nndma_io;
    }

    mmap_fd = __nndma_desram_cache_attr & NNA_UNCACHED_ACCELERATED ? __nnafd : __memfd;
    __nndma_desram_vbase = mmap(NULL, NNDMA_DESRAM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
                                mmap_fd, (off_t)NNDMA_DESRAM_BASE);
    if (__nndma_desram_vbase == MAP_FAILED) {
        printf("Error: NNDMA DESRAM mmap paddr=0x%x size=0x%x failed:%s\n", NNDMA_DESRAM_BASE,
               NNDMA_DESRAM_SIZE, strerror(errno));
        goto err_mmap_nndma_desram;
    }

    // ORAM
    mmap_fd = __oram_vbase_cache_attr & NNA_UNCACHED_ACCELERATED ? __nnafd : __memfd;
    __oram_vbase = mmap(NULL, oram_extend + oram_real_size + oram_extend, PROT_READ | PROT_WRITE,
                        MAP_SHARED, mmap_fd, (off_t)oram_base - oram_extend);
    if (__oram_vbase == MAP_FAILED) {
        printf("Error: ORAM mmap paddr=%d size=0x%08x failed:%s\n",
	oram_base, oram_real_size, strerror(errno));
        goto err_mmap_oram;
    }
    __oram_vbase += oram_extend;

    val = get_nmem_info(&__ddr_nna_buf.paddr, &rmemSize);
    if ((val < 0)) {
        printf("get_nmem_info failed\n");
        goto err_get_rmem_info;
    }
    if (b_use_rmem) {
        if (ddr_mem_size == 0) {
            ddr_mem_size = rmemSize;
            __ddr_nna_buf.size = ddr_mem_size;
            __ddr_pbase = __ddr_nna_buf.paddr;
        } else {
            unmem.size = ddr_mem_size;
            ret = ioctl(__nnalockfd, NMEM_INIT, &unmem);
            if (ret < 0) {
                printf("Error: DDR Malloc size=%d failed:%s\n", ddr_mem_size, strerror(errno));
                goto err_get_rmem_info;
            }
            __ddr_pbase = (void *)unmem.paddr;
            ddr_mem_size = unmem.size;
        }
    } else {
        ret = ioctl(__nnafd, IOCTL_SOC_NNA_MALLOC, &__ddr_nna_buf);
        if (ret < 0) {
            printf("Error: DDR Malloc size=%d failed:%s\n", __ddr_nna_buf.size, strerror(errno));
            goto err_ioctl_nna_malloc;
        }
    }

    struct nmem_size_buf buf;
    buf.version_buf = DRIVERS_VERSION;
    buf.nmem_extension_buf = nmem_extend;
    buf.nmem_paddr = (int)__ddr_pbase;
    buf.nmem_size = ddr_mem_size;
    ret = ioctl(__nnafd, IOCTL_SOC_NNA_VERSION, &buf);
    if (ret < 0) {
        printf("Warning : The version number is not obtained. Please upgrade the "
               "soc-nna!\n");
        oram_extend = 0;
        nmem_extend = 0;
        goto err_ioctl_nna_version;
    } else {
        if (DRIVERS_VERSION != buf.version_buf) {
            printf("The soc-nna version is %08x drivers_version is %d\n", buf.version_buf,
                   DRIVERS_VERSION);
        } else {
            printf("The soc-nna version is %08x\n", buf.version_buf);
        }
    }

    mmap_fd = __ddr_vbase_cache_attr & NNA_UNCACHED ? __memfd : __nnafd;
    __ddr_vbase = mmap(NULL, nmem_extend + ddr_mem_size + nmem_extend, PROT_READ | PROT_WRITE,
                       MAP_SHARED, mmap_fd, (off_t)__ddr_pbase - nmem_extend);
    if (__ddr_vbase == MAP_FAILED) {
        printf("Error: DDR mmap paddr=%p size=0x%08x failed:%s\n", __ddr_pbase, ddr_mem_size,
               strerror(errno));
        goto err_mmap_ddr_nna;
    }
    __ddr_vbase += nmem_extend;

    __nndma_fastio_vbase = mmap(NULL, FASTIO_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, __memfd,
                                (off_t)((intptr_t)__ddr_pbase & 0x0fffffff));
    if (__nndma_fastio_vbase == MAP_FAILED) {
        printf("Error: fastio mmap paddr=%d size=0x%08x failed:%s\n",
               ((intptr_t)__ddr_pbase & 0x0fffffff), FASTIO_SIZE, strerror(errno));
        goto err_mmap_fastio;
    }
	
	//printf("__nndma_fastio_vbase = %p", __nndma_fastio_vbase);	

    ret = ddr_memory_init(__ddr_vbase, ddr_mem_size);
    if (!ret) {
        printf("DDR Memory init failed !!\n");
        goto err_ddr_memory_init;
    }

    return 0;

/*
err_oram_memory_init:
    ddr_memory_deinit();
*/
err_ddr_memory_init:
    munmap(__nndma_fastio_vbase, FASTIO_SIZE);
err_mmap_fastio:
    munmap(__ddr_vbase, ddr_mem_size);
err_mmap_ddr_nna:
    if (__b_use_rmem) {
        if (__multiprocess_choose != 0) {
            ioctl(__nnalockfd, NMEM_UNINIT, &unmem);
        }
    } else {
        ioctl(__nnafd, IOCTL_SOC_NNA_FREE, &__ddr_nna_buf);
    }
err_ioctl_nna_malloc:
err_get_rmem_info:
    munmap(__oram_vbase, oram_real_size);
err_mmap_oram:
    munmap(__nndma_desram_vbase, NNDMA_DESRAM_SIZE);
err_mmap_nndma_desram:
    munmap(__nndma_io_vbase, NNDMA_IO_SIZE);
err_mmap_nndma_io:
    munmap(l2cache_size_vaddr, 0x1000);
err_oram_real_size:
    close(__nnalockfd);
    __nnalockfd = -1;
err_open_nna_lock:
    close(__nnafd);
    __nnafd = -1;
err_open_soc_nna:
    close(__memfd);
    __memfd = -1;
err_open_mem:
err_ioctl_nna_version:
    return -1;
}

int __aie_munmap() {
    int ret = -1;

    ddr_memory_deinit();
    munmap(__nndma_fastio_vbase, FASTIO_SIZE);
    munmap(__ddr_vbase, __ddr_nna_buf.size);
    if (__b_use_rmem) {
        if (__multiprocess_choose != 0) {
            ret = ioctl(__nnalockfd, NMEM_UNINIT, &unmem);
            if (ret < 0) {
                printf("NMEM_UNINIT failed\n");
                return -1;
            }
        }
    } else {
        ioctl(__nnafd, IOCTL_SOC_NNA_FREE, &__ddr_nna_buf);
    }
    munmap(__oram_vbase, oram_real_size);
    munmap(__nndma_desram_vbase, NNDMA_DESRAM_SIZE);
    munmap(__nndma_io_vbase, NNDMA_IO_SIZE);
    if (__multiprocess_choose != 0) {
        close(__nnalockfd);
        __nnalockfd = -1;
    }
    close(__nnafd);
    __nnafd = -1;
    close(__memfd);
    __memfd = -1;

    return 0;
}

int __aie_lock() {
    if (__nnalockfd < 0) {
        printf("Error: __nnalockfd = %d is wrong\n", __nnalockfd);
        return -1;
    }
    int ret = ioctl(__nnalockfd, NNA_LOCK, NULL);
    if (ret < 0) {
        printf("Error: nna_lock failed:%s\n", strerror(errno));
        return -1;
    }
    return 0;
}
int __aie_unlock() {
    if (__nnalockfd < 0) {
        printf("Error: __nnalockfd = %d is wrong\n", __nnalockfd);
        return -1;
    }
    int ret = ioctl(__nnalockfd, NNA_UNLOCK, NULL);
    if (ret < 0) {
        printf("Error: nna_unlock failed:%s\n", strerror(errno));
        return -1;
    }
    return 0;
}

int __aie_flushcache(void *ddr_mem_vaddr, int ddr_mem_size) {
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

int __aie_flushcache_dir(void *ddr_mem_vaddr, int ddr_mem_size, enum nna_dma_data_direction dir) {
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

int __aie_get_oram_size() { return oram_real_size; }

soc_mem_buf_t __aie_get_oram_info() {
    soc_mem_buf_t info;
    info.vaddr = __oram_vbase;
    info.paddr = (void *)oram_base;
    info.size = oram_real_size;
    return info;
}
