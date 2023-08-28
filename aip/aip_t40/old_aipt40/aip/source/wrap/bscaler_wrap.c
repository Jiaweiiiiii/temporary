/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : bscaler_wrap.c
 * Authors    : jmqi@ingenic.st.jz.com
 * Create Time: 2020-07-31:09:24:51
 * Description:
 *
 */
#include "platform.h"
#ifdef CSE_SIM_ENV
#include "drivers/aie_mmap.h"
#include "mem_manager/ddr_mem.h"
#endif
#define SYMBOL_EXPORT __attribute__ ((visibility("default")))

#define BSCALER_BASE            0x13090000
#ifdef CSE_SIM_ENV
void *bscaler_base_vaddr = NULL;
void *bscaler_base_paddr = NULL;
int bscaler_base_size = 0;

int intc_fd;
SYMBOL_EXPORT
void bscaler_mem_init()
{
    int memfd = open("/dev/mem", O_RDWR | O_SYNC);
    if (memfd  < 0) {
        printf("open /dev/mem failed:%s\n", strerror(errno));
        goto err_open_mem;
    }
    bscaler_base_paddr = (void *)BSCALER_BASE;
    bscaler_base_size = 0x00001000;
    bscaler_base_vaddr = mmap(NULL, bscaler_base_size, PROT_READ | PROT_WRITE,
                              MAP_SHARED, memfd, (off_t)bscaler_base_paddr);
    if (bscaler_base_vaddr == MAP_FAILED) {
        fprintf(stderr, "mmap paddr=%p size=0x%08x failed:%s\n",
                bscaler_base_paddr, bscaler_base_size, strerror(errno));
        goto err_mmap_nnbscaler;
    }
 err_open_mem:
 err_mmap_nnbscaler:
    close(memfd);
    memfd = -1;
}
#endif

SYMBOL_EXPORT
void bscaler_init()
{
#ifdef EYER_SIM_ENV
    eyer_system_ini(0);// eyer environment initialize.
    eyer_reg_segv();   // simulation error used
    eyer_reg_ctrlc();  // simulation error used
#elif CSE_SIM_ENV
    bscaler_mem_init();
    int ret = 0;
    if ((ret = __aie_mmap(0x4000000, 1, NNA_UNCACHED, NNA_UNCACHED, NNA_UNCACHED)) == 0) { //64MB
        printf ("nna box_base virtual generate failed !!\n");
    }
#endif
}

SYMBOL_EXPORT
void *bscaler_malloc(size_t align, size_t size)
{
    void *ptr = NULL;
#ifdef CSE_SIM_ENV
    ptr = ddr_memalign(align, size);
#else
    ptr = pf_malloc(align, size);
#endif
    if (ptr == NULL) {
        printf("[Error] : bscaler malloc failed!\n");
        return NULL;
    }
    return ptr;
}

SYMBOL_EXPORT
void bscaler_free(void *ptr)
{
#if (defined CSE_SIM_ENV)
    ddr_free(ptr);
#else
    pf_free(ptr);
#endif
}

SYMBOL_EXPORT
void *bscaler_malloc_oram(size_t align, size_t size)
{
    void *ptr = NULL;
#if (defined CSE_SIM_ENV)
    //ptr = oram_memalign(align, size);
#elif (defined EYER_SIM_ENV)
    ptr = pf_malloc(align, size);
#else
    //#error "not support alloc oram in chips environment."
#endif
    return ptr;
}

SYMBOL_EXPORT
void bscaler_free_oram(void *ptr)
{
#if (defined CSE_SIM_ENV)
    //oram_free(ptr);
#elif (defined EYER_SIM_ENV)
    pf_free(ptr);
#else
    //#error "not support alloc oram in chips environment."
#endif
}

SYMBOL_EXPORT
void bscaler_write_reg(uint32_t reg, uint32_t val)
{
#if (defined EYER_SIM_ENV)
    write_reg(BSCALER_BASE + reg, val);
#elif (defined CSE_SIM_ENV)
    *(volatile unsigned int*)(__bscaler_io_vbase + reg) = val;
#elif (defined CHIP_SIM_ENV)
    pf_write_reg(BSCALER_BASE + reg, val);
#endif
}

SYMBOL_EXPORT
uint32_t bscaler_read_reg(uint32_t reg, uint32_t val)
{
#if (defined EYER_SIM_ENV)
    return (read_reg(BSCALER_BASE + reg, 0));
#elif (defined CSE_SIM_ENV)
    return (*(volatile unsigned int*)(__bscaler_io_vbase + reg));
#elif (defined CHIP_SIM_ENV)
    return (pf_read_reg(BSCALER_BASE + reg));
#endif
}
