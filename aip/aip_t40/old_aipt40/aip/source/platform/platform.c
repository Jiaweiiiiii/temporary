/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : platform.c
 * Authors    : jmqi@ingenic.st.jz.com
 * Create Time: 2020-06-30:14:57:42
 * Description:
 *             Common platform, support eyer, FPGA/Palladium, SDVP.
 *             can not support two IP run at the same time.
 */

#include "platform.h"

#if ( defined(CHIP_SIM_ENV) && defined(SRC_CPU) )
#define SDVP_KSEG0_BUFFER_SIZE (2048*1024*10) //20M
__place_k0_data__ uint8_t __ALN128__ SDVP_KSEG0_BUFFER[SDVP_KSEG0_BUFFER_SIZE];
uint32_t SDVP_KSEG0_BUFFER_ST = 0;
uint32_t SDVP_KSEG0_BUFFER_CUR = 0;
uint32_t SDVP_KSEG0_BUFFER_END = 0;
#endif

/***************************************************************************
 @EYER Environment Function
***************************************************************************/
#ifdef EYER_SIM_ENV
static void *malloc_eyer(size_t align, size_t size)
{
    int offset = align - 1 + sizeof(void *);
    void *p1 = malloc(size + offset);
    if (p1 == NULL) {
        fprintf(stderr, "[Error][%s]: eyer malloc space failed!\n", __func__);
        exit(1);
    }
    void **p2 = (void **)(((size_t)p1 + offset) & (~(align - 1)));
    p2[-1] = p1;
    return p2;
}

static void free_eyer(void *p2)
{
    void *p1 = ((void**)p2)[-1];
    free(p1);
}
#endif

/***************************************************************************
 @SDVP Environment Function
***************************************************************************/
#ifdef CHIP_SIM_ENV

#ifdef SRC_CPU
#define JZ4780b_CACHE_SET()                     \
    {                                           \
        uint32_t tmp;                           \
        tmp = i_mfc0(CP0_ERRCTL) | 0x20000000;  \
        i_mtc0(tmp, CP0_ERRCTL);                \
        i_nop; i_nop; i_nop; i_nop; i_nop;      \
    }
#else // Virtual CPU
#define JZ4780b_CACHE_SET()
#endif

static void *sdvp_malloc(size_t align, size_t size)
{
    void *addr;
#ifdef SRC_CPU
    //real CPU @SDVP
    SDVP_KSEG0_BUFFER_ST = (uint32_t)SDVP_KSEG0_BUFFER;
    SDVP_KSEG0_BUFFER_END = (uint32_t)SDVP_KSEG0_BUFFER + SDVP_KSEG0_BUFFER_SIZE;

    /* First used sdvp_malloc */
    if (0 == SDVP_KSEG0_BUFFER_CUR) {
        SDVP_KSEG0_BUFFER_CUR = SDVP_KSEG0_BUFFER_ST;
    }

    if (align == 1) {
        addr = (void *)SDVP_KSEG0_BUFFER_CUR;
    } else {
        addr = (void *)((SDVP_KSEG0_BUFFER_CUR + align - 1) & ~(align - 1));
    }

    uint32_t high_4bits = ((uint32_t)addr >> 28) & 0xF;
    if ((high_4bits != 0x8) && (high_4bits != 0x9)) {
        pf_printf("[Error]: sdvp malloc virtual address is illegal(0x%08x)!\n", addr);
    }

    SDVP_KSEG0_BUFFER_CUR = (uint32_t)addr + size;

    if (SDVP_KSEG0_BUFFER_CUR > SDVP_KSEG0_BUFFER_END) {
        pf_printf("[Error]: sdvp(real cpu) malloc failed, out of kseg0 range!\n");
    }
#else
    //virtual CPU @SDVP
    addr = (void *)cMalloc(NULL, size, align);
    if (addr == NULL) {
        pf_printf("[Error]: sdvp(virtual cpu) malloc failed!\n");
    }
#endif
    return (void *)addr;
}
#endif

/***************************************************************************
 @FPGA Environment Function
***************************************************************************/
#ifdef FPGA_TEST_ENV
#define RESERVE_MEM_LEN  64
static void alloc_memdev()
{
    uint32_t vaddr, paddr = (256 - RESERVE_MEM_LEN) * 1024 * 1024;

    // get paddr
    uint32_t fd = open("/dev/rmem", O_RDWR);
    if (fd >= 0) {
        vaddr= (uint32_t)mmap(NULL, /*length*/RESERVE_MEM_LEN*1024*1024,
                              PROT_READ | PROT_WRITE, MAP_SHARED, fd, paddr);
        if (vaddr <= 0) {
            pf_printf("mmap error %s\n", __func__);
        }
        close(fd);
    } else {
        pf_printf("open /dev/rmem failed!\n");
    }

    /* Set the memory device info.  */
    if (paddr == 0) {
        pf_printf("[error]: Can not get address of the reserved %dM memory.\n", RESERVE_MEM_LEN);
    } else {
        memdev[memdev_count].vaddr = vaddr;
        memdev[memdev_count].paddr = paddr;
        memdev[memdev_count].totalsize = (RESERVE_MEM_LEN*1024*1024);
        memdev[memdev_count].usedsize = 0;
        memdev_count++;
        pf_printf("Dev alloc: vaddr=0x%08x, paddr=0x%08x, size=0x%x\n",
                  vaddr, paddr, (RESERVE_MEM_LEN*1024*1024));
    }
}

void *fpga_malloc(size_t align, size_t size)
{
    int i, alloc_size, used_size, total_size;
    uint32_t vaddr;

    /* alloc the memory.  */
    for (i = 0; i < MEM_ALLOC_DEV_NUM; i++) {
        if (i >= memdev_count) {
            alloc_memdev();
        }
        // handle align
        used_size = memdev[i].usedsize;
        used_size = (used_size + align - 1) & ~ (align - 1);
        memdev[i].usedsize = used_size;
        // get memory size for allocated.  */
        total_size = memdev[i].totalsize;
        alloc_size = total_size - used_size;

        if (alloc_size >= size) {
/*             if((((memdev[i].vaddr & 0xFFFFFF) + used_size)&0x1C00000) != */
/*                (((memdev[i].vaddr & 0xFFFFFF) + used_size + size) & 0x1C00000)){ */
/*                 vaddr = ((memdev[i].vaddr + used_size + size) & 0xFFC00000); */
/*                 memdev[i].usedsize = size + (vaddr - memdev[i].vaddr); */
/*             }else{ */
            memdev[i].usedsize = used_size + size;
            vaddr = memdev[i].vaddr + used_size;
/*         } */
            return (void *)vaddr;
        } else {
            pf_printf("[Error] : fpga malloc out of range\n");
            exit(1);
        }
    }
}
#endif

/***************************************************************************
Common Function
***************************************************************************/
static void *malloc_align(size_t align, size_t size)
{
    int offset = align - 1 + sizeof(void *);
    void *p1 = malloc(size + offset);
    if (p1 == NULL) {
        fprintf(stderr, "[Error][%s]: eyer malloc space failed!\n", __func__);
        exit(1);
    }
    void **p2 = (void **)(((size_t)p1 + offset) & (~(align - 1)));
    p2[-1] = p1;
    return p2;
}

static void free_align(void *p2)
{
    void *p1 = ((void**)p2)[-1];
    free(p1);
}

void *pf_malloc(uint32_t align, uint32_t size)
{
    if (align == 0) {
        pf_printf("[Warning]: align can not be 0!\n");
    }
    void *addr = NULL;

#if (defined EYER_SIM_ENV)
    addr = malloc_eyer(align, size);
#elif (defined CHIP_SIM_ENV)
    addr = sdvp_malloc(align, size);
#elif (defined FPGA_TEST_ENV)
    addr = fpag_malloc(align, size);
#else
    addr = malloc_align(align, size);
#endif
    return addr;
}

void pf_free(void *ptr)
{

}
uint32_t pf_va_2_pa(void *vaddr)
{
    uint32_t paddr;

#ifdef EYER_SIM_ENV
    paddr = (uint32_t)vaddr;
#endif

#ifdef CHIP_SIM_ENV
#ifdef SRC_CPU
    paddr = (uint32_t)vaddr & 0x1FFFFFFF;
#else
    {//virtual CPU @SDVP
        uint32_t *src = (uint32_t *)vaddr;
        uint32_t *vaddr2;
        uint32_t *dst;
        uint32_t i;
        paddr = va_2_pa(vaddr);
        /*if (paddr == 0) {
            vaddr2 = (uint32_t *)pf_malloc(align, len);
            dst = (uint32_t *) vaddr2;
            for(i=0; i<len/4; i++){
                dst[i] = src[i];
            }
            paddr = va_2_pa((void *)vaddr2);
            if (paddr == 0) {
                pf_printf(LOG_ERR, "[error]: va2pa failed!\n");
            }
            }*/
    }
#endif
#endif

#ifdef FPGA_TEST_ENV
    uint32_t *src = (uint32_t *)vaddr;
    uint32_t *dst;
    uint32_t i;
    uint32_t *vaddr2;

    for(i = 0; i < memdev_count; i++){
        if((uint32_t)vaddr >= memdev[i].vaddr && (uint32_t)vaddr
           < (memdev[i].vaddr + memdev[i].totalsize)){
            paddr = memdev[i].paddr + ((uint32_t)vaddr - memdev[i].vaddr);
        }else{
            pf_printf("[WARNING]: %s, virtual address must be raw/ref, else will be wrong!\n", __func__);
            vaddr2 = (uint32_t *)pf_malloc(align, len);
            dst = (uint32_t *) vaddr2;
            uint32_t ii;
            for(ii=0; ii<len/4; ii++){
                dst[ii] = src[ii];
            }
            if((uint32_t)vaddr2 >= memdev[i].vaddr && (uint32_t)vaddr
               < (memdev[i].vaddr + memdev[i].totalsize)){
                paddr = memdev[i].paddr + ((uint32_t)vaddr2 - memdev[i].vaddr);
            }else{
                pf_printf("[error]: va2pa failed!\n");
            }
        }
    }
#endif
    return paddr;
}

uint32_t pf_read_reg(uint32_t reg)
{
    uint32_t ret = 0;
#if (defined EYER_SIM_ENV)
    ret = read_reg(reg, 0);
#elif (defined CHIP_SIM_ENV)
    ret = CpuRead(reg);
#elif (defined FPGA_TEST_ENV)
    ret = (*((volatile uint32_t *)(reg)));
#endif
    printf("[%s]:0x%08x -- 0x%08x\n", __func__, reg, ret);
    return ret;
}

void pf_write_reg(uint32_t reg, uint32_t val)
{
    printf("[%s]:0x%08x -- 0x%08x\n", __func__, reg, val);
#if (defined EYER_SIM_ENV)
    write_reg(reg, val);
#elif (defined CHIP_SIM_ENV)
    CpuWrite(reg, val);
#elif (defined FPGA_TEST_ENV)
    (*((volatile uint32_t *)(reg)) = (val));
#endif
}

void pf_flush_cache()
{
#ifdef CHIP_SIM_ENV
#ifdef SRC_CPU
#define CACHE_FLUSH_BASE 0x80000000
    uint32_t i, va;
    va = CACHE_FLUSH_BASE;
    for(i = 0; i < 1024; i++){
        i_cache(0x1, va, 0);
        va += 32;
    }
    i_sync();
#endif
#endif

#ifdef FPGA_TEST_ENV
    tcsm_fd = open("/dev/tcsm", O_RDWR);
    if (tcsm_fd < 0) {
        pf_printf("open /dev/tcsm error.\n");
        exit(1);
    }
    jz_flush_cache(NULL, 600 * 1024);
    close(tcsm_fd);
#endif
}

int pf_printf(const char *fmt, ...)
{
    int ret = 0;
    va_list args;
    va_start(args, fmt);

#if ( defined(CHIP_SIM_ENV) && defined(SRC_CPU) )
    uint32_t i;
    uint8_t msg[1000];
    uint32_t scnt;
    scnt = vsprintf_h(msg, fmt, args);
    for (i = 0; i < scnt; i++) {
        CpuWrite(UART0_BASE + 0xFC, msg[i]);
    }
#else
    ret = vfprintf(stderr, fmt, args);
#endif
    va_end(args);
    return ret;
}

int pf_init()
{
#if (defined EYER_SIM_ENV)
    eyer_system_ini(0);// eyer environment initialize.
    eyer_reg_segv();   // simulation error used
    eyer_reg_ctrlc();  // simulation error used
#elif (defined FPGA_TEST_ENV)
    ... TODO;
#elif (defined CHIP_SIM_ENV)
    bscaler_mem_init();
    int ret = NULL;
    if ((ret = __aie_mmap(0x4000000)) == NULL) { //64MB
        printf("nna box_base virtual generate failed !!\n");
        return -1;
    }
#endif
    return 0;
}

int pf_dut_init(uint32_t dut_io_base, uint32_t size)
{
//     /* open and map flash device */
//    vae_fd = open("/dev/mem", O_RDWR | O_SYNC);
//    if (vae_fd < 0) {
//         show(LOG_ERR, "open /dev/mem error.\n");
//         exit(1);
//    }
//
//    cpm_base = (uint32_t)mmap((void *)0, CPM_SIZE, PROT_READ | PROT_WRITE,
//                               MAP_SHARED, vae_fd, 0x10000000);
//    potato_base = (uint32_t)mmap((void *)0, POTATO_SIZE, PROT_READ | PROT_WRITE,
//                                  MAP_SHARED, vae_fd, 0x13300000);
//    dut0_base = potato_base;
//
}

int pf_deinit()
{
#ifdef EYER_SIM_ENV
    eyer_stop();
#endif
}
