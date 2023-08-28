/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : bscaler_mdl_api.h
 * Authors    : jmqi@ingenic
 * Create Time: 2021-01-14:15:43:23
 * Description:
 *
 */

#ifndef __BSCALER_MDL_API_H__
#define __BSCALER_MDL_API_H__
#include "bscaler_api.h"

#ifdef __cplusplus
extern "C" {
#endif

int bs_perspective_mdl(const data_info_s *src,
                       const int box_num, const data_info_s *dst,
                       const box_affine_info_s *boxes,
                       const uint32_t *coef, const uint32_t *offset);

int bs_affine_mdl(const data_info_s *src,
                  const int box_num, const data_info_s *dst,
                  const box_affine_info_s *boxes,
                  const uint32_t *coef, const uint32_t *offset);

int bs_resize_mdl(const data_info_s *src,
                  const int box_num, const data_info_s *dst,
                  const box_resize_info_s *boxes,
                  const uint32_t *coef, const uint32_t *offset);

int bs_covert_mdl(const data_info_s *src, const data_info_s *dst,
                  const uint32_t *coef, const uint32_t *offset,
                  const task_info_s *task_info);

#ifdef __cplusplus
}
#endif
#endif /* __BSCALER_MDL_API_H__ */

