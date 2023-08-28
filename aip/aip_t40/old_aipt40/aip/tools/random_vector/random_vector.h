/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : random_vector.h
 * Authors    : jmqi@joshua
 * Create Time: 2020-06-23:17:24:50
 * Description:
 *
 */

#ifndef __RANDOM_VECTOR_H__
#define __RANDOM_VECTOR_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "bscaler_hal.h"

bool bsc_random(bsc_hw_once_cfg_s *cfg, int md_sel);

void bst_random(bst_hw_once_cfg_s *cfg);

bs_chain_ret_s  bscaler_chain_init(uint32_t frame, bsc_hw_once_cfg_s *frmc_cfg);

bs_chain_ret_s  bscaler_chain_t_init(uint32_t frame, bst_hw_once_cfg_s *frmc_cfg);

#endif /* __RANDOM_VECTOR_H__ */

