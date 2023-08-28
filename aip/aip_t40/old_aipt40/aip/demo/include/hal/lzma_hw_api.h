#ifndef __LZMA_API_H__
#define __LZMA_API_H__

#ifdef EYER_SIM_ENV
#include "eyer_driver.h"
#include <stdint.h>
#elif CSE_SIM_ENV
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
#include "drivers/aie_mmap.h"
#include "drivers/soc_nna.h"
//#include "LocalMemMgr.h"
#include "mem_manager/oram_mem.h"
#include "mem_manager/ddr_mem.h"
#else
/*#ifdef SRC_CPU
#include <instructions.h>
#include <test.h>
#include <exp_supp.h>
#include <debug.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <ingenic_cpu.h>
#endif*/
#ifdef CHIP_SIM_ENV
#include "platform.h"
#else
#include <stdint.h>
#include <stdbool.h>
#endif

#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SRC_CPU
#define LZMA_BASE               0xB3090000
#else
#define LZMA_BASE               0x13090000
#endif

#define LZMA_CTRL               (0*4)
#define LZMA_BS_BASE            (1*4)
#define LZMA_BS_SIZE            (2*4)
#define LZMA_DST_BASE           (3*4)
#define LZMA_ICRC               (4*4)
#define LZMA_OCRC               (5*4)
#define LZMA_VERSION            (6*4)
#define LZMA_FINAL_SIZE         (7*4)

typedef struct LZMA_CFG_S {
  uint8_t         debug_en;
  //bitstream
  uint8_t         *bs_base;
  uint32_t        bs_size;
  uint16_t        *leftbyte_base;
  uint32_t        leftbyte_size;
  uint32_t        *ctx_base;
  uint32_t        ctx_size;
  uint8_t         *dst_base;
  uint32_t        dst_size;

  uint32_t        buf_idx;
} LZMA_CFG_S;

#ifdef CSE_SIM_ENV
void lzma_mem_init();
#endif
void *lzma_malloc(uint32_t align, uint32_t size);
  void lzma_free(void *p2);
void lzma_write_reg(uint32_t reg, uint32_t val);
uint32_t lzma_read_reg(uint32_t reg, uint32_t val);
uint32_t lzma_cfg(LZMA_CFG_S *cfg);

#ifdef __cplusplus
}
#endif
#endif
