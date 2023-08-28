/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : common.h
 * Authors    : ljxie@taurus
 * Create Time: 2023-05-18:15:13:35
 * Description:
 *
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#include "mxu3.h"
#include "nna.h"
//#include "nna_insn.h"
#include "nna_app.h"
#include "iaic.h"

#define NEW 1

#define MAC_VWR0  VWR1
#define MAC_VWR1  VWR2
#define FWR_VWR0  VWR3
#define FWR_VWR1  VWR4
#define WWR_VWR   VWR5
#define TLX_VWR   VWR6
#define TLY_VWR   VWR7
#define WRA_ADD   VWR8

#define __sync()                                    \
    __asm__ __volatile__(".set    push\n\t"         \
                         ".set    noreorder\n\t"    \
                         ".set    mips2\n\t"        \
                         "sync\n\t"                 \
                         ".set    pop"              \
                         : /* no output */          \
                         : /* no input */           \
                         : "memory")

#define __fast_iob()                                                    \
    __asm__ __volatile__(".set    push\n\t"                             \
                         ".set    noreorder\n\t"                        \
                         "lw    $0,%0\n\t"                              \
                         "nop\n\t"                                      \
                         ".set    pop"                                  \
                         : /* no output */                              \
                         : "m"(*(unsigned int *)__nndma_fastio_vbase)   \
                         : "memory")

#define fast_iob()                              \
    do {                                        \
        __sync();                               \
    } while (0)
        //__fast_iob();                          

typedef struct {
    uint32_t ic, ih, iw, ibit;
    uint32_t oc, oh, ow, obit;
    uint32_t kx, ky, sx, sy, wbit;
    uint32_t if_ddr_ptr, w_ddr_ptr, of_ddr_ptr; // DDR
    uint32_t if_oram_ptr, w_oram_ptr, of_oram_ptr; // ORAM
} TSparameter;

#endif /* __COMMON_H__ */

