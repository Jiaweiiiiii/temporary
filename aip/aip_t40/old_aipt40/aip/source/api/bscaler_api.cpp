/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : bscaler_api.cpp
 * Authors    : jmqi@taurus
 * Create Time: 2020-04-20:09:29:03
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
#include <semaphore.h>

#include "matrix.h"
#include "bscaler_hal.h"
#include "bscaler_api.h"
#include "platform.h"
#include "bscaler_segmentation.h"
#ifndef RELEASE
#include "AutoTime.hpp"
#endif
#include "version.h"

#define SYMBOL_EXPORT __attribute__ ((visibility("default")))

#define USING_INTERRUPT         1
#define USING_INTERRUPT_BST     1
uint32_t *bsc_chain_base = NULL;
uint32_t *bst_chain_base = NULL;
static int __bscaler_intc_fd = -1;

static pthread_mutex_t bsc_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t bst_lock = PTHREAD_MUTEX_INITIALIZER;
static int bst_task_num;
/**
 * generate chain, wrap bscaler software model.
 */

#define BSCALER_IO_BASE 0x13090000
unsigned int jz_aip_readl(unsigned int base, unsigned int offset)
{
	unsigned int value = *(volatile unsigned int *)(base + offset);
	return value;
}

unsigned int jz_aip_writel(unsigned int base, unsigned int offset, unsigned int value)
{   
	*(volatile unsigned int *)(base + offset) = value;
	return 0;
} 

static void bs_chain_stuff(std::vector<bsc_hw_once_cfg_s> &bs_cfgs)
{
#if 0
    for (int i = 0; i < bs_cfgs.size(); i++) {
        bsc_hw_once_cfg_s *cfg = &(bs_cfgs[i]);
        bscaler_frmc_cfg(cfg);//do bscaler
    }
#else

    //cfg nv2bgr param
    bsc_hw_once_cfg_s *cfg_once = &(bs_cfgs[0]);
    assert(cfg_once != NULL);
    uint32_t nv2bgr_order = (cfg_once->dst_format >> 1) & 0xF;
    uint32_t nv2bgr_alpha = ALPHA;

    bscaler_common_param_cfg(cfg_once->coef, cfg_once->offset, nv2bgr_alpha);//fixme
    bsc_order_cfg(nv2bgr_order);

    //init chain info
    uint32_t box_max_num = 64;
    uint32_t box_dst_max_w = 64;
    uint32_t cfg_reg_max = 22;
    uint32_t bsc_chain_size = bs_cfgs.size() * (box_max_num + box_dst_max_w + cfg_reg_max) * 8;
    bsc_chain_base = (uint32_t *)bscaler_malloc(8, bsc_chain_size);
    if (bsc_chain_base == NULL) {
        fprintf(stderr,"error : malloc bsc_chain_base is failed ! \n ");
    }

    uint32_t *bsc_chain_ptr = bsc_chain_base;
    uint32_t chain_len =0;

    for(int i=0; i<bs_cfgs.size(); i++){//frame n
        bs_chain_ret_s  back;
        bsc_hw_once_cfg_s *bsc_cfg_once = &(bs_cfgs[i]);
        back = bscaler_frmc_chain_cfg(bsc_cfg_once, bsc_chain_ptr);//
        bsc_chain_ptr = back.bs_ret_base;
        chain_len = chain_len + back.bs_ret_len;
    }
    bs_chain_cfg_s chain_info;
    chain_info.bs_chain_base = bsc_chain_base;
    chain_info.bs_chain_len = chain_len;
#if USING_INTERRUPT
    chain_info.bs_chain_irq_mask = 0;
#else
    chain_info.bs_chain_irq_mask = 1;
#endif
    chain_info.c_timeout_irq_mask = 1;
    chain_info.chain_bus = 0;
    //start chain
#ifdef CSE_SIM_ENV
    if (__aie_get_ddr_cache_attr() & NNA_CACHED) {
        __aie_flushcache((void *)bsc_chain_base, bsc_chain_size);
    }
#endif
	


    bsc_chain_hw_cfg(&chain_info);

#endif
}

////////////////////////////////////////////////////////////////////////////////
/**
 * for application developer
 */

SYMBOL_EXPORT
int bs_covert_cfg(const data_info_s *src, const data_info_s *dst,
                  const uint32_t *coef,
                  const uint32_t *offset,
                  const task_info_s *task_info)
{
    pthread_mutex_lock(&bsc_lock);
    AUTOTIME;
#if USING_INTERRUPT_BST
    if (__bscaler_intc_fd < 0) {
        __bscaler_intc_fd = open("/dev/bscaler0", O_RDWR);
        if (__bscaler_intc_fd < 0) {
            fprintf(stderr,"open /dev/bscaler0 error.\n");
            exit(1);
        }
    }
#endif

    assert(src->format != BS_DATA_FMU2);
    assert(src->format != BS_DATA_FMU4);
    assert(src->format != BS_DATA_FMU8);
    assert(src->format != BS_DATA_VBGR);
    assert(src->format != BS_DATA_VRGB);
    assert(dst->format != BS_DATA_VBGR);
    assert(dst->format != BS_DATA_VRGB);

    bst_hw_once_cfg_s cfg;
    cfg.src_format = bs_format_to_bst(src->format);
    cfg.src_w = src->width;
    cfg.src_h = src->height;
    cfg.src_base0 = (uint8_t *)src->base;
    cfg.src_base1 = NULL;
    cfg.src_line_stride = src->line_stride;
    if (src->format == BS_DATA_NV12) {
        if (src->base1 == NULL) {
            cfg.src_base1 = (uint8_t *)src->base + src->height * src->line_stride;
        } else {
            cfg.src_base1 = (uint8_t *)src->base1;
        }
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
    cfg.ibuft_bus = src->locate; //0-ddr, 1-oram, fixme
    cfg.obuft_bus = dst->locate; //0-ddr, 1-oram, fixme
    cfg.nv2bgr_alpha = ALPHA;
    for (int i = 0; i < 9; i++) {
        cfg.nv2bgr_coef[i] = coef[i];
    }
    for (int i = 0; i < 2; i++) {
        cfg.nv2bgr_ofst[i] = offset[i];
    }

    cfg.isum = 0; //fixme
    cfg.osum = 0; //fixme
    cfg.irq_mask = 1; //fixme ??
    cfg.t_chain_irq_mask = 1; //fixme ??
    cfg.t_timeout_irq_mask = 1; //fixme ??
    cfg.t_timeout_val; //fixme ??

    bscaler_frmt_soft_reset();
    bscaler_common_param_cfg(cfg.nv2bgr_coef, cfg.nv2bgr_ofst, cfg.nv2bgr_alpha);
    bscaler_frmt_cfg(&cfg);

}

SYMBOL_EXPORT
int bs_covert_step_start(const task_info_s *task_info, const void *dst_ptr, const bs_data_locate_e locate)
{
    AUTOTIME;
    uint32_t phy_dst_base = 0;
    if (locate) {
        phy_dst_base = __aie_get_oram_paddr((uint32_t)dst_ptr);
    } else {
        phy_dst_base = __aie_get_ddr_paddr((uint32_t)dst_ptr);
    }

    bscaler_write_reg(BSCALER_FRMT_YBASE_DST, phy_dst_base);

#if USING_INTERRUPT_BST
    bscaler_write_reg(BSCALER_FRMT_TASK, (task_info->task_len << 16) | 1 | 0 << 1);
    if (__bscaler_intc_fd < 0) {
        __bscaler_intc_fd = open("/dev/bscaler0", O_RDWR);
        if (__bscaler_intc_fd < 0) {
           fprintf(stderr,"open /dev/bscaler0 error.\n");
            exit(1);
        }
    }
#else
    bscaler_write_reg(BSCALER_FRMT_TASK, (task_info->task_len << 16) | 1 | 1 << 1);
#endif
}

SYMBOL_EXPORT
int bs_covert_step_wait()
{
    AUTOTIME;
#if USING_INTERRUPT_BST
    uint32_t value;
    ioctl(__bscaler_intc_fd, 0x0, &value);
#else //pooling
    while ((bscaler_read_reg(BSCALER_FRMT_TASK, 0) & 0x1) != 0);
    //while ((bscaler_read_reg(BSCALER_FRMT_CTRL, 0) & 0x1) != 0);
#endif
    if ((bscaler_read_reg(BSCALER_FRMT_CTRL, 0) & 0x1) == 0) {
        pthread_mutex_unlock(&bsc_lock);
    }
}

SYMBOL_EXPORT
int bs_cropbox_start(const data_info_s *src,
                     const int box_num, const data_info_s *dst,
                     const uint32_t *coef, const uint32_t *offset)
{

}

SYMBOL_EXPORT
int bs_cropbox_wait()
{
    //TODO
    //polling or wait for interrupt
}

SYMBOL_EXPORT
int bs_affine_start(const data_info_s *src,
                    const int box_num, const data_info_s *dst,
                    const box_affine_info_s *boxes,
                    const uint32_t *coef, const uint32_t *offset)
{
     pthread_mutex_lock(&bsc_lock);
#if USING_INTERRUPT
    if (__bscaler_intc_fd < 0) {
        __bscaler_intc_fd = open("/dev/bscaler0", O_RDWR);
        if (__bscaler_intc_fd < 0) {
            fprintf(stderr,"open /dev/bscaler0 error.\n");
            exit(1);
        }
    }
#endif
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
    bs_chain_stuff(bs_cfgs);
}

SYMBOL_EXPORT
int bs_affine_wait()
{
#if USING_INTERRUPT

    uint32_t value;
    ioctl(__bscaler_intc_fd, 0x100, &value);
    close(__bscaler_intc_fd);
    __bscaler_intc_fd = -1;
#else //polling
    int fail_flag = 0;
    while ((bscaler_read_reg(BSCALER_FRMC_CHAIN_CTRL, 0) & 0x1) != 0) {
        fail_flag++;
        if (fail_flag >= 0x24000000) {
            fprintf(stderr,"[Error] : time out!!\n");
            break;
        }
    }
#endif
    //bscaler_frmc_soft_reset();
    //free chain_base
    bscaler_free(bsc_chain_base);
    pthread_mutex_unlock(&bsc_lock);

}

SYMBOL_EXPORT
int bs_perspective_start(const data_info_s *src,
                         const int box_num, const data_info_s *dst,
                         const box_affine_info_s *boxes,//fixme
                         const uint32_t *coef, const uint32_t *offset)
{
     pthread_mutex_lock(&bsc_lock);
#if USING_INTERRUPT
    if (__bscaler_intc_fd < 0) {
        __bscaler_intc_fd = open("/dev/bscaler0", O_RDWR);
        if (__bscaler_intc_fd < 0) {
            fprintf(stderr,"open /dev/bscaler0 error.\n");
            exit(1);
        }
    }
#endif

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
    bs_chain_stuff(bs_cfgs);
}

SYMBOL_EXPORT
int bs_perspective_wait()
{
#if USING_INTERRUPT
    uint32_t value;
    ioctl(__bscaler_intc_fd, 0x100, &value);
    close(__bscaler_intc_fd);
    __bscaler_intc_fd = -1;
#else //polling
    int fail_flag = 0;
    while ((bscaler_read_reg(BSCALER_FRMC_CHAIN_CTRL, 0) & 0x1) != 0) {
        fail_flag++;
        if (fail_flag >= 0x24000000) {
            fprintf(stderr,"[Error] : time out!!\n");
            break;
        }
    }
#endif
    //bscaler_frmc_soft_reset();
    //free chain_base
    bscaler_free(bsc_chain_base);
    pthread_mutex_unlock(&bsc_lock);

}

SYMBOL_EXPORT
int bs_resize_start(const data_info_s *src,
                    const int box_num, const data_info_s *dst,
                    const box_resize_info_s *boxes,
                    const uint32_t *coef, const uint32_t *offset)
{
    pthread_mutex_lock(&bsc_lock);
#if USING_INTERRUPT
    if (__bscaler_intc_fd < 0) {
        __bscaler_intc_fd = open("/dev/bscaler0", O_RDWR);
        if (__bscaler_intc_fd < 0) {
            fprintf(stderr,"open /dev/bscaler0 error.\n");
            exit(1);
        }
    }
#endif

    //fprintf(stderr,"sem get:%d\n",sem_getvalue(&sem_bsc,NULL));
    std::vector<bsc_hw_once_cfg_s> bs_cfgs;
    /**********************************************************************
     1. resize box process, pick up box of box part mode
    **********************************************************************/
    std::vector<std::pair<int, const box_resize_info_s *>> res_resize_boxes;
    for (int i = 0; i < box_num; i++) {
        const box_resize_info_s *cur_box = &(boxes[i]);
        const data_info_s *cur_dst = &(dst[i]);
        float scale_y = (float)cur_box->box.h / (float)cur_dst->height;
        int sbox_h_max = BS_RESIZE_H;
        //if (((src->format & 0x1) || (src->format == BS_DATA_NV12)) && //image
        //    (fabs(scale_y) < 1.0f)) { //amplify
        if (((src->format & 0x1) || (src->format == BS_DATA_NV12))) { //amplify
            //using affine
            sbox_h_max = BS_AFFINE_SIZE;
            //fprintf(stderr,"resize using affine...\n");
        }

        bool resize_split = (cur_dst->width > BS_RESIZE_W) || (cur_dst->height > sbox_h_max);
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
    bs_chain_stuff(bs_cfgs);
}

SYMBOL_EXPORT
int bs_resize_wait()
{
#if USING_INTERRUPT
    uint32_t value;
    ioctl(__bscaler_intc_fd, 0x100, &value);
    close(__bscaler_intc_fd);
    __bscaler_intc_fd = -1;
#else //polling
    int fail_flag = 0;
    while ((bscaler_read_reg(BSCALER_FRMC_CHAIN_CTRL, 0) & 0x1) != 0) {
        fail_flag++;
        if (fail_flag >= 0x24000000) {
            fprintf(stdout,"[Error] : time out!!\n");
            break;
        }
    }
#endif
    //bscaler_frmc_soft_reset();
    //free chain_base
    bscaler_free(bsc_chain_base);
    pthread_mutex_unlock(&bsc_lock);
}

void substring(char *ch, char* subch, int pos, int length)
{
    char *pch = ch + pos;
    int i;
    for (i = 0; i < length; i++) {
        subch[i] = *(pch++);
    }
    subch[length]='\0';
}

void get_build_time()
{
    char date[15];
    char time[10];
    sprintf(date, "%s", __DATE__);
    sprintf(time, "%s", __TIME__);

    char day[3];
    char month[3];
    char year[5];
    substring(date, month, 0, 3);
    substring(date, day, 4, 2);
    substring(date, year, 7, 4);

    //date
    int iday = atoi(day);
    int iyear = atoi(year);
    int imonth;
    if (strcmp(month, "Jan") == 0) imonth = 1;
    else if (strcmp(month, "Feb") == 0) imonth = 2;
    else if (strcmp(month, "Mar") == 0) imonth = 3;
    else if (strcmp(month, "Apr") == 0) imonth = 4;
    else if (strcmp(month, "May") == 0) imonth = 5;
    else if (strcmp(month, "Jun") == 0) imonth = 6;
    else if (strcmp(month, "Jul") == 0) imonth = 7;
    else if (strcmp(month, "Aug") == 0) imonth = 8;
    else if (strcmp(month, "Sep") == 0) imonth = 9;
    else if (strcmp(month, "Oct") == 0) imonth = 10;
    else if (strcmp(month, "Nov") == 0) imonth = 11;
    else if (strcmp(month, "Dec") == 0) imonth = 12;
    else imonth = 1;

    //time
    char hour[3];
    char min[3];
    char sec[3];
    substring(time, hour, 0, 2);
    substring(time, min, 3, 2);
    substring(time, sec, 6, 2);

    int ihour = atoi(hour);
    int imin = atoi(min);
    int isec = atoi(sec);
    printf("Build time         | %04d-%02d-%02d %02d:%02d:%02d\n", iyear, imonth, iday, ihour, imin, isec);
}

SYMBOL_EXPORT
void bs_version()
{
    printf("\n");
    printf("Configuration and build information\n");
    printf("-----------------------------------\n");
    printf("\n");
    printf("Version            | %s\n", PROJECT_VERSION);
    printf("Who compiled       | %s\n", USER_NAME);
    printf("CMake version      | %s\n", CMAKE_VERSION);
    printf("CMake generator    | %s\n", CMAKE_GENERATOR);
    printf("Configuration time | %s\n", CONFIGURATION_TIME);
    get_build_time();
    printf("C compiler         | %s\n", CMAKE_C_COMPILER);
    printf("C++ compiler       | %s\n", CMAKE_CXX_COMPILER);
    printf("Git Hash           | %s\n", GIT_HASH);

    printf("\n");
    fflush(stdout);
}
