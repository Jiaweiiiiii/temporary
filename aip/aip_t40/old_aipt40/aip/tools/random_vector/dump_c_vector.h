/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : dump_c_vector.h
 * Authors    : jmqi@joshua
 * Create Time: 2020-06-26:10:33:40
 * Description:
 *
 */

#ifndef __DUMP_C_VECTOR_H__
#define __DUMP_C_VECTOR_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "bscaler_hal.h"

void dump_c_vector(bsc_hw_once_cfg_s *cfg, uint32_t seed, char *note);
void dump_c_chain_vector(uint32_t frame, bsc_hw_once_cfg_s *cfg,
                         bs_chain_ret_s *chain_info, uint32_t seed, char *note);

#ifdef __cplusplus
}
#endif
#endif /* __DUMP_C_VECTOR_H__ */

