/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : bscaler_segmentation.h
 * Authors    : jmqi@ingenic
 * Create Time: 2021-01-14:15:18:32
 * Description:
 *
 */

#ifndef __BSCALER_SEGMENTATION_H__
#define __BSCALER_SEGMENTATION_H__
#include <assert.h>
#include <vector>

#include "bscaler_hal.h"
#include "bscaler_api.h"

#define DST_MAX_WIDTH           (8192)
#define ALPHA                   (0)

#ifdef __cplusplus
extern "C" {
#endif

extern const uint32_t global_default_coef[9];
extern const uint32_t global_default_offset[2];
extern const task_info_s global_default_task_info[1];

bst_hw_data_format_e bs_format_to_bst(bs_data_format_e src);

void resize_box_split(std::vector<bsc_hw_once_cfg_s> &bs_cfgs,
                      const box_resize_info_s *info,
                      const data_info_s *src, const data_info_s *dst,
                      const uint32_t *coef, const uint32_t *offset);

void affine_box_split(std::vector<bsc_hw_once_cfg_s> &bs_cfgs,
                      box_affine_info_s *info,
                      const data_info_s *src, data_info_s *dst,
                      const uint32_t *coef, const uint32_t *offset);

void perspective_box_split(std::vector<bsc_hw_once_cfg_s> &bs_cfgs,
                           box_affine_info_s *info,
                           const data_info_s *src, data_info_s *dst,
                           const uint32_t *coef, const uint32_t *offset);

void affine_resize_box_split(std::vector<bsc_hw_once_cfg_s> &bs_cfgs,
                             box_affine_info_s *info,
                             const data_info_s *src, data_info_s *dst,
                             const uint32_t *coef, const uint32_t *offset);

void bs_affine_cfg_stuff(std::vector<bsc_hw_once_cfg_s> &bs_cfgs,
                         box_affine_info_s *info,
                         const data_info_s *src, data_info_s *dst,
                         const uint32_t *coef, const uint32_t *offset);

void bs_affine_resize_cfg_stuff(std::vector<bsc_hw_once_cfg_s> &bs_cfgs,
                                box_affine_info_s *info,
                                const data_info_s *src, data_info_s *dst,
                                const uint32_t *coef, const uint32_t *offset);

void bs_resize_line_cfg_stuff(std::vector<bsc_hw_once_cfg_s> &bs_cfgs,
                              std::vector<std::pair<int, const box_resize_info_s *>> &res_resize_boxes,
                              const data_info_s *src, const data_info_s *dst,
                              const uint32_t *coef, const uint32_t *offset);

void bs_perspective_cfg_stuff(std::vector<bsc_hw_once_cfg_s> &bs_cfgs,
                              box_affine_info_s *info,
                              const data_info_s *src, data_info_s *dst,
                              const uint32_t *coef, const uint32_t *offset);

#ifdef __cplusplus
}
#endif
#endif /* __BSCALER_SEGMENTATION_H__ */

