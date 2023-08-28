/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : rand_api.cpp
 * Authors    : lzhu@joshua
 * Create Time: 2020-07-14:15:59:54
 * Description:
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include "Matrix.h"

#include "bscaler_api.h"
#include "rand_api.h"

int resize_random(uint32_t src_bpp_mode, float *matrix, int *src_w, int *src_h, int *dst_w, int *dst_h)
{
    bool split = rand()%2; // 0 - not split, 1 - split
    if (split) {
        if (rand()%10 > 8) { // 1 / 10
            *dst_h = 256 + 1 + rand()%(2048 - 256);
            *dst_w = 64 + 1 + rand()%(2048 - 64);
        } else { // 9/10
            *dst_h = 256 + 1 + rand()%(512);
            *dst_w = 64 + 1 + rand()%(128);
        }
    } else {
        *dst_h = rand()%256 + 1;
        *dst_w = rand()%64 + 1;
    }

    *dst_w = (*dst_w) % 2 ? *dst_w - 1 : *dst_w;
    *dst_h = (*dst_h) % 2 ? *dst_h - 1 : *dst_h;
    if(*dst_w == 0){
        *dst_w = 2;
    }
    if(*dst_h == 0){
        *dst_h = 2;
    }

    float scale_x = rand() % 3 ? rand() % 4 + 1 : rand() % 32 + 1;
    float scale_y = rand() % 3 ? rand() % 4 + 1 : rand() % 32 + 1;

    scale_x = rand()%2 ? scale_x : 1 / scale_x;
    scale_y = rand()%2 ? scale_y : 1 / scale_y;

    *src_w = (int)(*dst_w / scale_x);
    *src_h = (int)(*dst_h / scale_y);
    uint32_t src_w_max = 0;
    if(src_bpp_mode == 0){
        src_w_max = 2048;
    }
    else if(src_bpp_mode == 1){
        src_w_max = 1024;
    }
    else if(src_bpp_mode == 2){
        src_w_max = 512;
    }
    else if(src_bpp_mode == 3){
        src_w_max = 256;
    }else{
        src_w_max = 256;
    }

    if (*src_w > src_w_max) {
        float scale_w = *src_w / src_w_max;
        *dst_w = (int )(*dst_w / scale_w);
        *src_w = src_w_max;
    }

    if (*src_h > 2048) {
        float scale_h = *src_h / 2048;
        *dst_h = (int )(*dst_h / scale_h);
        *src_h = 2048;
    }

    if (*src_w < 2) {
        *src_w = 2;
        scale_x = ((float)(*src_w) / (float)(*dst_w));
    }

    if (*src_h < 2) {
        *src_h = 2;
        scale_y = ((float)(*src_h) / (float)(*dst_h));
    }

    *src_w = (*src_w) % 2 ? *src_w - 1 : *src_w;
    *src_h = (*src_h) % 2 ? *src_h - 1 : *src_h;

    if(*src_w == 0){
        *src_w = 2;
    }
    if(*src_h == 0){
        *src_h = 2;
    }

    scale_x = (float)(*dst_w) / (float)(*src_w);
    scale_y = (float)(*dst_h) / (float)(*src_h);

    float trans_x = (float)(rand() % 32);
    float trans_y = (float)(rand() % 32);
    //printf("%d x %d -- %d x %d -- %f, %f\n", *dst_w, *dst_h, *src_w, *src_h, scale_x, scale_y);
    matrix[0] = scale_x;
    matrix[1] = 0;
    matrix[2] = trans_x;
    matrix[3] = 0;
    matrix[4] = scale_y;
    matrix[5] = trans_y;
    matrix[6] = 0;
    matrix[7] = 0;
    matrix[8] = 0;
}

int affine_random(float *matrix, int *src_w, int *src_h, int *dst_w, int *dst_h)
{
    bool split = rand()%2; // 0 - not split, 1 - split
    if (split) {
        if (rand()%10 > 8) { // 1 / 10
            *dst_h = 64 + 1 + rand()%(2048 - 64);
            *dst_w = 64 + 1 + rand()%(2048 - 64);
        } else { // 9/10
            *dst_h = 64 + 1 + rand()%(128);
            *dst_w = 64 + 1 + rand()%(128);
        }
    } else {
        *dst_h = rand()%64 + 1;
        *dst_w = rand()%64 + 1;
    }

    *src_h = 64 + 1 + rand()%(2048 - 64);
    *src_w = 64 + 1 + rand()%(2048 - 64);
    *src_h = (*src_h) % 2 ? *src_h - 1 : *src_h;
    *src_w = (*src_w) % 2 ? *src_w - 1 : *src_w;
    *dst_w = (*dst_w) % 2 ? *dst_w - 1 : *dst_w;
    *dst_h = (*dst_h) % 2 ? *dst_h - 1 : *dst_h;
    if(*src_w == 0){
        *src_w = 2;
    }
    if(*src_h == 0){
        *src_h = 2;
    }
    if(*dst_w == 0){
        *dst_w = 2;
    }
    if(*dst_h == 0){
        *dst_h = 2;
    }

    CV::Matrix trans;
    float angle = rand() % 360;
    trans.setRotate(angle, (float)(*src_w - 1)/2, (float)(*src_h - 1)/2);
    trans.postScale((float)(*dst_w) / (float)(*src_w),
                    (float)(*dst_h) / (float)(*src_h));
    trans.get9(matrix);
}

int perspective_random(float *matrix, int *src_w, int *src_h, int *dst_w, int *dst_h)
{
    bool split = rand()%2; // 0 - not split, 1 - split
    if (split) {
        if (rand()%10 > 8) { // 1 / 10
            *dst_h = 64 + 1 + rand()%(2048 - 64);
            *dst_w = 64 + 1 + rand()%(2048 - 64);
        } else { // 9/10
            *dst_h = 64 + 1 + rand()%(128);
            *dst_w = 64 + 1 + rand()%(128);
        }
    } else {
        *dst_h = rand()%64 + 1;
        *dst_w = rand()%64 + 1;
    }

    *src_h = 64 + 1 + rand()%(2048 - 64);
    *src_w = 64 + 1 + rand()%(2048 - 64);
    *src_h = (*src_h) % 2 ? *src_h - 1 : *src_h;
    *src_w = (*src_w) % 2 ? *src_w - 1 : *src_w;

    *dst_w = (*dst_w) % 2 ? *dst_w - 1 : *dst_w;
    *dst_h = (*dst_h) % 2 ? *dst_h - 1 : *dst_h;

    if(*src_w == 0){
        *src_w = 2;
    }
    if(*src_h == 0){
        *src_h = 2;
    }
    if(*dst_w == 0){
        *dst_w = 2;
    }
    if(*dst_h == 0){
        *dst_h = 2;
    }

    CV::Matrix trans;
    float angle = rand() % 360;
    trans.setRotate(angle, (float)(*src_w - 1)/2, (float)(*src_h - 1)/2);
    trans.postScale((float)(*dst_w) / (float)(*src_w),
                    (float)(*dst_h) / (float)(*src_h));
    trans.get9(matrix);
    matrix[6] = 1.0f / (rand() % 1000);
    matrix[7] = 1.0f / (rand() % 1000);
    matrix[8] = 1;
}

int matrix_random(uint32_t src_bpp_mode, int mode, float *matrix, int *src_w, int *src_h,
                  int *dst_w, int *dst_h)
{
    // int mode = rand() % 3; // 0 - resize, 1 - affine, 2 - perspective
    if (mode == 0) {
        resize_random(src_bpp_mode, matrix, src_w, src_h, dst_w, dst_h);
    } else if (mode == 1) {
        affine_random(matrix, src_w, src_h, dst_w, dst_h);
    } else if (mode == 2) {
        perspective_random(matrix, src_w, src_h, dst_w, dst_h);
    }
}

void set_run_mode(api_mode_sel_e mode_sel_e, api_info_s *src_cfg, api_info_s *dst_cfg){
    bs_data_format_e chn2chn_format[4] = {BS_DATA_BGRA,BS_DATA_FMU2,BS_DATA_FMU4,BS_DATA_FMU8};
    bs_data_format_e nv2bgr_format[12] = {BS_DATA_BGRA,BS_DATA_GBRA,BS_DATA_RBGA,BS_DATA_BRGA,BS_DATA_GRBA,BS_DATA_RGBA,BS_DATA_ABGR,BS_DATA_AGBR,BS_DATA_ARBG,BS_DATA_ABRG,BS_DATA_AGRB,BS_DATA_ARGB};

    uint8_t src_radom_idx;
    uint8_t dst_radom_idx;
    switch(mode_sel_e){
    case bresize_chn2chn:
        //printf("###bresize_chn2chn here\n");
        src_radom_idx = rand()%4;
        dst_radom_idx = src_radom_idx;
        src_cfg->mode = 0;
        src_cfg->format = chn2chn_format[src_radom_idx];
        dst_cfg->format = chn2chn_format[dst_radom_idx];
        break;
    case bresize_nv2chn:
        //printf("###bresize_nv2chn here\n");
        dst_radom_idx = rand()%12;
        src_cfg->mode = 0;
        src_cfg->format = BS_DATA_NV12;
        dst_cfg->format = nv2bgr_format[dst_radom_idx];
        break;
    case affine_bgr2bgr:
        //printf("###affine_bgr2bgr here\n");
        src_radom_idx = 0;
        dst_radom_idx = src_radom_idx;
        src_cfg->mode = 1;
        src_cfg->format = chn2chn_format[src_radom_idx];
        dst_cfg->format = chn2chn_format[dst_radom_idx];
        break;
    case affine_nv2bgr:
        //printf("###affine_nv2chn here\n");
        dst_radom_idx = rand()%12;
        src_cfg->mode = 1;
        src_cfg->format = BS_DATA_NV12;
        dst_cfg->format = nv2bgr_format[dst_radom_idx];
        break;
    case affine_nv2nv:
        //printf("###affine_nv2nv here\n");
        src_cfg->mode = 1;
        src_cfg->format = BS_DATA_NV12;
        dst_cfg->format = BS_DATA_NV12;
        break;
    case perspective_bgr2bgr:
        //printf("###perspective_bgr2bgr here\n");
        src_radom_idx = 0;
        dst_radom_idx = src_radom_idx;
        src_cfg->mode = 2;
        src_cfg->format = chn2chn_format[src_radom_idx];
        dst_cfg->format = chn2chn_format[dst_radom_idx];
        break;
    case perspective_nv2bgr:
        //printf("###perspective_nv2chn here\n");
        dst_radom_idx = rand()%12;
        src_cfg->mode = 2;
        src_cfg->format = BS_DATA_NV12;
        dst_cfg->format = nv2bgr_format[dst_radom_idx];
        break;
    case perspective_nv2nv:
        //printf("###perspective_nv2nv here\n");
        src_cfg->mode = 2;
        src_cfg->format = BS_DATA_NV12;
        dst_cfg->format = BS_DATA_NV12;
        break;
    default:
        src_radom_idx = rand()%4;
        dst_radom_idx = src_radom_idx;
        src_cfg->mode = 0;
        src_cfg->format = chn2chn_format[src_radom_idx];
        dst_cfg->format = chn2chn_format[dst_radom_idx];
        break;
    }
    dst_cfg->mode = src_cfg->mode;
}

void src_image_init(uint32_t src_format, uint32_t bpp_mode,
                    int src_w,int src_ps,int src_h,
                    uint8_t *src_base0){
    uint32_t x, y;
    uint32_t byte_num = 0;
    uint32_t byte_per_pix;

    if(src_format == 0){
        byte_per_pix= 1;}
    else{
        byte_per_pix= 1<<(2+bpp_mode);
    }
    uint8_t *pc_frmc_y =(uint8_t *)src_base0;
    for (y = 0; y < src_h; y++) {
        for (x = 0; x <src_w; x++) {
            for(byte_num=0;byte_num< byte_per_pix;byte_num++){
                    pc_frmc_y[y*src_ps + x*byte_per_pix+byte_num] = rand()%255;
            }
        }
    }
    if(src_format==0){
        uint8_t *pc_frmc_c = &src_base0[src_ps * src_h];
        for (y = 0; y < src_h/2; y++) {
            for (x = 0; x < src_w; x++) {
                for(byte_num=0;byte_num< byte_per_pix;byte_num++){
                    pc_frmc_c[y*src_ps + x*byte_per_pix+byte_num] = rand()%255;
                }
            }
        }
    }
}

