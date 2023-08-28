/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : platform.h
 * Authors    : jmqi@ingenic.st.jz.com
 * Create Time: 2020-07-30:14:32:35
 * Description:
 *
 */

#ifndef __PLATFORM_H__
#define __PLATFORM_H__
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>

#if (defined EYER_SIM_ENV)
#include "eyer_driver.h"
#define BSCALER_BASE                0x13090000
#define __place_k0_data__
#elif (defined FPGA_TEST_ENV)
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <float.h>
#include "aie_mmap.h"
//#include "LocalMemMgr.h"
#include "drivers/oram_mem.h"
#include "mem_manager/ddr_mem.h"
#include "soc_nna.h"
#elif (defined CSE_SIM_ENV)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <float.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include "drivers/aie_mmap.h"
#include "drivers/soc_nna.h"
//#include "LocalMemMgr.h"
#include "mem_manager/oram_mem.h"
#include "mem_manager/ddr_mem.h"
#elif (defined CHIP_SIM_ENV)
#ifdef SRC_CPU
#define BSCALER_BASE                0xB3090000
#else
#define BSCALER_BASE                0x13090000
#define __place_k0_data__
#endif
#endif

void *pf_malloc(uint32_t reg, uint32_t val);
void pf_free(void *ptr);

void pf_write_reg(uint32_t reg, uint32_t val);

uint32_t pf_read_reg(uint32_t reg);

uint32_t pf_va_2_pa(void *viraddr);

int pf_printf(const char *fmt, ...);

#endif /* __PLATFORM_H__ */

