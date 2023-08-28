/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : dump_t_vector.h
 * Authors    : jmqi@joshua
 * Create Time: 2020-06-26:16:38:54
 * Description:
 *
 */

#ifndef __DUMP_T_VECTOR_H__
#define __DUMP_T_VECTOR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "bscaler_hal.h"

void dump_t_vector(bst_hw_once_cfg_s *cfg, uint32_t seed, char* note);

#ifdef __cplusplus
}
#endif

#endif /* __DUMP_T_VECTOR_H__ */

