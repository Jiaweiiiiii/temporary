/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : perspective_whole_float.h
 * Authors    : jmqi@taurus
 * Create Time: 2020-05-11:17:14:16
 * Description:
 *
 */

#ifndef __PERSPECTIVE_WHOLE_FLOAT_H__
#define __PERSPECTIVE_WHOLE_FLOAT_H__
#include <stdint.h>
#include "bscaler_api.h"

#ifdef __cplusplus
extern "C" {
#endif

void perspective_whole_float(const data_info_s *src,
                             const int box_num, const data_info_s *dst,
                             const box_affine_info_s *boxes,//fixme
                             const uint32_t *coef, const uint32_t *offset);
#ifdef __cplusplus
}
#endif
#endif /* __PERSPECTIVE_WHOLE_FLOAT_H__ */
