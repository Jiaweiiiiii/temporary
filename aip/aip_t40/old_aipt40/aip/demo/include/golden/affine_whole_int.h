/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : affine_whole_int.h
 * Authors    : jmqi@taurus
 * Create Time: 2020-05-11:17:16:35
 * Description:
 *
 */

#ifndef __AFFINE_WHOLE_INT_H__
#define __AFFINE_WHOLE_INT_H__

#include <stdint.h>
#include "bscaler_api.h"

#ifdef __cplusplus
extern "C" {
#endif

void affine_whole_int(const data_info_s *src,
                      const int box_num, const data_info_s *dst,
                      const box_affine_info_s *boxes,//fixme
                      const uint32_t *coef, const uint32_t *offset);
#ifdef __cplusplus
}
#endif
#endif /* __AFFINE_WHOLE_INT_H__ */
