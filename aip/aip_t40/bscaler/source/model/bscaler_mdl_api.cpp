/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : bscaler_mdl_api.cpp
 * Authors    : jmqi@ingenic
 * Create Time: 2021-01-14:15:15:41
 * Description:
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <vector>
#include <string.h>

#include "matrix.h"
#include "bscaler_mdl.h"
#include "bscaler_mdl_api.h"
#include "bscaler_api.h"
#include "bscaler_segmentation.h"

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
/**
 * fake generate chain, wrap bscaler software model.
 */
static void fake_bs_chain_stuff(std::vector<bsc_hw_once_cfg_s> &bs_cfgs)
{
    for (int i = 0; i < bs_cfgs.size(); i++) {
        bsc_hw_once_cfg_s *cfg = &(bs_cfgs[i]);
        assert(cfg != NULL);
        cfg->nv2bgr_alpha = ALPHA;
        bsc_mdl(cfg);//do bscaler
    }
}

int bs_resize_mdl(const data_info_s *src,
                  const int box_num, const data_info_s *dst,
                  const box_resize_info_s *boxes,
                  const uint32_t *coef, const uint32_t *offset)
{
    std::vector<bsc_hw_once_cfg_s> bs_cfgs;
    /**********************************************************************
     1. resize box process, pick up box of box part mode
    **********************************************************************/
    std::vector<std::pair<int, const box_resize_info_s *>> res_resize_boxes;
    for (int i = 0; i < box_num; i++) {
        bool resize_split = (dst->width > BS_RESIZE_W) || (dst->height > BS_RESIZE_H);
        const box_resize_info_s *cur_box = &(boxes[i]);
        const data_info_s *cur_dst = &(dst[i]);
        if (resize_split) {
            resize_box_split(bs_cfgs, cur_box, src, cur_dst, coef, offset);
        } else {
            res_resize_boxes.push_back(std::make_pair(i, cur_box));
        }
    }

    /**********************************************************************
     2. resize line mode process
    **********************************************************************/
    bs_resize_line_cfg_stuff(bs_cfgs, res_resize_boxes, src, dst, coef, offset);

    /**********************************************************************
     3. stuff chain
    **********************************************************************/
    fake_bs_chain_stuff(bs_cfgs);
}

int bs_affine_mdl(const data_info_s *src,
                  const int box_num, const data_info_s *dst,
                  const box_affine_info_s *boxes,
                  const uint32_t *coef, const uint32_t *offset)
{
    /**********************************************************************
     1. classification, affine or resize
    **********************************************************************/
    std::vector<int> affine_boxes;
    std::vector<int> resize_boxes;
    for (int i = 0; i < box_num; i++) {
#if 0
        if (affine_is_scale_translate(boxes[i].matrix)) {
            resize_boxes.push_back(i);
        } else {
            affine_boxes.push_back(i);
        }
#else
    affine_boxes.push_back(i);
#endif
    }

    /**********************************************************************
     2. affine box process
    **********************************************************************/
    std::vector<bsc_hw_once_cfg_s> bs_cfgs;
    for (int i = 0; i < affine_boxes.size(); i++) {
        int idx = affine_boxes[i];
        box_affine_info_s cur_box = boxes[idx];
        data_info_s cur_dst = dst[idx];
        bool affine_split = (cur_dst.width > 64) || (cur_dst.height > 64);
        if (affine_split) {
            affine_box_split(bs_cfgs, &cur_box, src, &cur_dst, coef, offset);
        } else {
            bs_affine_cfg_stuff(bs_cfgs, &cur_box, src, &cur_dst, coef, offset);
        }
    }

    /**********************************************************************
     3. resize box process
    **********************************************************************/
    for (int i = 0; i < resize_boxes.size(); i++) {
        bool resize_split = (dst->width > BS_RESIZE_W) || (dst->height > BS_RESIZE_H);
        int idx = resize_boxes[i];
        box_affine_info_s cur_box = boxes[idx];
        data_info_s cur_dst = dst[idx];
        if (resize_split) {
            affine_resize_box_split(bs_cfgs, &cur_box, src, &cur_dst, coef, offset);
        } else {
            bs_affine_resize_cfg_stuff(bs_cfgs, &cur_box, src, &cur_dst, coef, offset);
        }
    }

    /**********************************************************************
     4. stuff chain
    **********************************************************************/
    fake_bs_chain_stuff(bs_cfgs);
}

int bs_perspective_mdl(const data_info_s *src,
                       const int box_num, const data_info_s *dst,
                       const box_affine_info_s *boxes,//fixme
                       const uint32_t *coef, const uint32_t *offset)
{
    std::vector<bsc_hw_once_cfg_s> bs_cfgs;
    /**********************************************************************
     1. classification, perspective or affine or resize
    **********************************************************************/
    std::vector<int> perspective_boxes;
    std::vector<int> affine_boxes;
    std::vector<int> resize_boxes;
    for (int i = 0; i < box_num; i++) {
#if 0
        if (affine_is_scale_translate(boxes[i].matrix)) {
            resize_boxes.push_back(i);
        } else {
            if (IS_ZERO(boxes[i].matrix[MPERSP0]) &&
                IS_ZERO(boxes[i].matrix[MPERSP1]) &&
                IS_ONE(boxes[i].matrix[MPERSP2])) {//affine
                affine_boxes.push_back(i);
            } else {//perspective
                perspective_boxes.push_back(i);
            }
        }
#else
        perspective_boxes.push_back(i);
#endif
    }

    /**********************************************************************
     2. perspective box process
    **********************************************************************/
    for (int i = 0; i < perspective_boxes.size(); i++) {
        int idx = perspective_boxes[i];
        box_affine_info_s cur_box = boxes[idx];
        data_info_s cur_dst = dst[idx];
        bool perspective_split = (cur_dst.width > 64) || (cur_dst.height > 64);
        if (perspective_split) {
            perspective_box_split(bs_cfgs, &cur_box, src, &cur_dst, coef, offset);
        } else {
            bs_perspective_cfg_stuff(bs_cfgs, &cur_box, src, &cur_dst, coef, offset);
        }
    }

    /**********************************************************************
     3. affine box process
    **********************************************************************/
    for (int i = 0; i < affine_boxes.size(); i++) {
        int idx = affine_boxes[i];
        box_affine_info_s cur_box = boxes[idx];
        data_info_s cur_dst = dst[idx];
        bool affine_split = (cur_dst.width > 64) || (cur_dst.height > 64);
        if (affine_split) {
            affine_box_split(bs_cfgs, &cur_box, src, &cur_dst, coef, offset);
        } else {
            bs_affine_cfg_stuff(bs_cfgs, &cur_box, src, &cur_dst, coef, offset);
        }
    }

    /**********************************************************************
     3. resize box process
    **********************************************************************/
    for (int i = 0; i < resize_boxes.size(); i++) {
        bool resize_split = (dst->width > BS_RESIZE_W) || (dst->height > BS_RESIZE_H);
        int idx = resize_boxes[i];
        box_affine_info_s cur_box = boxes[idx];
        data_info_s cur_dst = dst[idx];
        if (resize_split) {
            affine_resize_box_split(bs_cfgs, &cur_box, src, &cur_dst, coef, offset);
        } else {
            bs_affine_resize_cfg_stuff(bs_cfgs, &cur_box, src, &cur_dst, coef, offset);
        }
    }

    /**********************************************************************
     4. stuff chain
    **********************************************************************/
    fake_bs_chain_stuff(bs_cfgs);
}

int bs_covert_mdl(const data_info_s *src, const data_info_s *dst,
                  const uint32_t *coef=global_default_coef,
                  const uint32_t *offset=global_default_offset,
                  const task_info_s *task_info=global_default_task_info)
{
    assert(src->format != BS_DATA_FMU2);
    assert(src->format != BS_DATA_FMU4);
    assert(src->format != BS_DATA_FMU8);
    assert(src->format != BS_DATA_VBGR);
    assert(src->format != BS_DATA_VRGB);

    bst_hw_once_cfg_s cfg;
    cfg.src_format = bs_format_to_bst(src->format);
    cfg.src_w = src->width;
    cfg.src_h = src->height;
    cfg.src_base0 = (uint8_t *)src->base;
    cfg.src_base1 = NULL;
    cfg.src_line_stride = src->line_stride;
    if (src->format == BS_DATA_NV12) {
        if (src->base1 == NULL)
            cfg.src_base1 = (uint8_t *)src->base + src->height * src->line_stride;
        else
            cfg.src_base1 = (uint8_t *)src->base1;
    }

    cfg.dst_format = bs_format_to_bst(dst->format);
    cfg.dst_w = dst->width;
    cfg.dst_h = dst->height;
    cfg.dst_base0 = (uint8_t *)dst->base;
    cfg.dst_base1 = NULL;
    if (dst->format == BS_DATA_NV12) {
        if (dst->base1 == NULL)
            cfg.dst_base1 = (uint8_t *)dst->base + dst->height * dst->line_stride;
        else
            cfg.dst_base1 = (uint8_t *)dst->base1;
    }
    cfg.dst_line_stride = dst->line_stride;

    //if ((dst->format == BS_DATA_VBGR) || (dst->format == BS_DATA_VRGB)) {
    //    if ((task_info->kw == 1) && (task_info->kh == 1)) {
    //        cfg.kernel_size = 0;
    //    } else if ((task_info->kw == 3) && (task_info->kh == 3)) {
    //        cfg.kernel_size = 1;
    //    } else if ((task_info->kw == 5) && (task_info->kh == 5)) {
    //        cfg.kernel_size = 2;
    //    } else if ((task_info->kw == 7) && (task_info->kh == 7)) {
    //        cfg.kernel_size = 3;
    //    } else {
    //        fprintf(stderr, "only support 3x3, 5x5, 7x7 kernel size yet!(%dx%d)\n",
    //                task_info->kw, task_info->kh);
    //        return -1;
    //    }
    //}

    cfg.kernel_xstride = 0;//task_info->k_stride_x;
    cfg.kernel_ystride = 0;//task_info->k_stride_y;
    cfg.zero_point = task_info->zero_point;
    cfg.pad_left = 0;//task_info->pad_left;
    cfg.pad_right = 0;//task_info->pad_right;
    cfg.pad_top = 0;//task_info->pad_top;
    cfg.pad_bottom = 0;//task_info->pad_bottom;

    cfg.task_len = task_info->task_len;//fixme
    cfg.dst_plane_stride = task_info->plane_stride;//fixme
    cfg.chain_bus = 0;//always in ddr
    cfg.ibuft_bus = src->locate;
    cfg.obuft_bus = dst->locate;

    cfg.nv2bgr_alpha = ALPHA;
    for (int i = 0; i < 9; i++) {
        cfg.nv2bgr_coef[i] = coef[i];
    }
    for (int i = 0; i < 2; i++) {
        cfg.nv2bgr_ofst[i] = offset[i];
    }

    cfg.irq_mask = 1;
    cfg.t_chain_irq_mask = 1;
    cfg.t_timeout_irq_mask = 1;
    cfg.t_timeout_val = 0;
    bst_mdl(&cfg);
}
