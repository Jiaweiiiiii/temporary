/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : platform.h
 * Authors    : jmqi@joshua
 * Create Time: 2020-06-24:12:26:57
 * Description:
 *
 */

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

#if (!defined(EYER_SIM_ENV) && !defined(FPGA_SIM_ENV))
#define CHIP_SIM_ENV
#endif

#ifdef EYER_SIM_ENV
#include "eyer_driver.h"
#include <stdint.h>
#endif

#ifdef CSE_SIM_ENV
#include <stdint.h>
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
#include "nna_dma_memory.h"
#include "oram_mem.h"
#include "ddr_mem.h"
#include "soc_nna.h"
#endif

#ifdef CHIP_SIM_ENV
#include "../reg_map/bscaler_map.h"
#ifdef SRC_CPU
#include <instructions.h>
#include <test.h>
#include <exp_supp.h>
#include <debug.h>
#include "uint.h"
#else //Virtual CPU
#include "ingenic_cpu.h"
#include <time.h>
#include "uint.h"
#endif
#endif

#define __ALN4__ __attribute__ ((aligned(4)))
#define __ALN128__ __attribute__ ((aligned(0x80)))
#define __ALN256__ __attribute__ ((aligned(0x100)))

#ifdef CHIP_SIM_ENV
#ifdef SRC_CPU
#include <instructions.h>
#include <test.h>
#include <exp_supp.h>
#include <debug.h>
#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#define int32_t  int
#define int16_t  short
#else // Virtual CPU
#include <stdlib.h>
#define __place_k0_data__
#include "ingenic_cpu.h"
#endif // SRC_CPU
#endif // CHIP_SIM_ENV

#ifdef __cplusplus
extern "C" {
#endif

int plat_printf(const char *fmt, ...);

char plat_strcmp(const char *str1, const char *str2);

void *plat_malloc(size_t align, size_t size);

void plat_free(void *ptr);

uint32_t plat_va_2_pa(void *vaddr);

uint32_t plat_read_reg(uint32_t reg);

void plat_write_reg(uint32_t reg, uint32_t val);

void plat_flush_cache();

int platform_init();

int platform_deinit();

#ifdef __cplusplus
}
#endif
#endif /* __PLATFORM_H__ */

