/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : bscaler_random.c
 * Authors    : jmqi@joshua
 * Create Time: 2020-06-23:17:23:04
 * Description:
 *
 */

#include "random_vector.h"

#define CLIP(val, min, max) ((val) < (min) ? (min) : (val) > (max) ? (max) : (val))

typedef enum {
    BOX_RSZ_CHN2CHN     = 0,
    BOX_RSZ_NV2BGR      = 1,
    LINE_RSZ_CHN2CHN    = 2,
    LINE_RSZ_NV2BGR     = 3,
    AFFINE_BGR2BGR      = 4,
    AFFINE_NV2BGR       = 5,
    AFFINE_NV2NV        = 6,
    PERSP_BGR2BGR       = 7,
    PERSP_NV2BGR        = 8,
    PERSP_NV2NV         = 9,
} frmc_mode_sel_e;

void print_data_format(const char *fmt, bsc_hw_data_format_e format)
{
    fprintf(stderr, "%s = %s\n", fmt,
            format == BSC_HW_DATA_FM_NV12 ? "NV12" :
            format == BSC_HW_DATA_FM_BGRA ? "BGRA" :
            format == BSC_HW_DATA_FM_GBRA ? "GBRA" :
            format == BSC_HW_DATA_FM_RBGA ? "RBGA" :
            format == BSC_HW_DATA_FM_BRGA ? "BRGA" :
            format == BSC_HW_DATA_FM_GRBA ? "GRBA" :
            format == BSC_HW_DATA_FM_RGBA ? "RGBA" :
            format == BSC_HW_DATA_FM_ABGR ? "ABGR" :
            format == BSC_HW_DATA_FM_AGBR ? "AGBR" :
            format == BSC_HW_DATA_FM_ARBG ? "ARBG" :
            format == BSC_HW_DATA_FM_ABRG ? "ABRG" :
            format == BSC_HW_DATA_FM_AGRB ? "AGRB" :
            format == BSC_HW_DATA_FM_ARGB  ? "ARGB" :
            format == BSC_HW_DATA_FM_F32_2B ? "F32_2B" :
            format == BSC_HW_DATA_FM_F32_4B ? "F32_4B" :
            format == BSC_HW_DATA_FM_F32_8B ? "F32_8B" : "unknown");
}

void dump_bsc_cfg_info(bsc_hw_once_cfg_s *cfg)
{
    fprintf(stderr, "src_base0 = %p\n", cfg->src_base0);
    fprintf(stderr, "src_base1 = %p\n", cfg->src_base1);
    print_data_format("src_format", cfg->src_format);
    fprintf(stderr, "src_line_stride = %d\n", cfg->src_line_stride);
    fprintf(stderr, "src_box_x = %d\n", cfg->src_box_x);
    fprintf(stderr, "src_box_y = %d\n", cfg->src_box_y);
    fprintf(stderr, "src_box_w = %d\n", cfg->src_box_w);
    fprintf(stderr, "src_box_h = %d\n", cfg->src_box_h);

    print_data_format("dst_format", cfg->dst_format);
    fprintf(stderr, "dst_line_stride = %d\n", cfg->dst_line_stride);
    fprintf(stderr, "dst_box_x = %d\n", cfg->dst_box_x);
    fprintf(stderr, "dst_box_y = %d\n", cfg->dst_box_y);
    fprintf(stderr, "dst_box_w = %d\n", cfg->dst_box_w);
    fprintf(stderr, "dst_box_h = %d\n", cfg->dst_box_h);

    fprintf(stderr, "affine = %d\n", cfg->affine);
    fprintf(stderr, "box_mode = %d\n", cfg->box_mode);
    fprintf(stderr, "y_gain_exp = %d\n", cfg->y_gain_exp);
    fprintf(stderr, "zero_point = %d\n", cfg->zero_point);
    fprintf(stderr, "nv2bgr_alpha = %d\n", cfg->nv2bgr_alpha);
    fprintf(stderr, "coef = {");
    for (int i = 0; i < 9; i++) {
        fprintf(stderr, "%d, ", cfg->coef[i]);
    }
    fprintf(stderr, "}\n");

    fprintf(stderr, "offset = {");
    for (int i = 0; i < 2; i++) {
        fprintf(stderr, "%d, ", cfg->offset[i]);
    }
    fprintf(stderr, "}\n");

    fprintf(stderr, "matrix = {");
    for (int i = 0; i < 9; i++) {
        fprintf(stderr, "%d, ", cfg->matrix[i]);
    }
    fprintf(stderr, "}\n");

    fprintf(stderr, "box_num = %d\n", cfg->box_num);

    fprintf(stderr, "dst_base :\n");
    for (int i = 0; i < 64; i++) {
        fprintf(stderr, "%p, ", cfg->dst_base[i]);
    }
    fprintf(stderr, "\n");

    if (cfg->box_mode == false) {
        for (int i = 0; i < cfg->box_num; i++) {
            fprintf(stderr, "%08x\n", cfg->boxes_info[i*6 + 0]);
            fprintf(stderr, "%08x\n", cfg->boxes_info[i*6 + 1]);
            fprintf(stderr, "%08x\n", cfg->boxes_info[i*6 + 2]);
            fprintf(stderr, "%08x\n", cfg->boxes_info[i*6 + 3]);
            fprintf(stderr, "%08x\n", cfg->boxes_info[i*6 + 4]);
            fprintf(stderr, "%08x\n", cfg->boxes_info[i*6 + 5]);
        }
    }
    fprintf(stderr, "mono_x = %08x\n", cfg->mono_x);
    fprintf(stderr, "mono_y = %08x\n", cfg->mono_y);
    fprintf(stderr, "extreme_point = {");
    for (int i = 0; i < 64; i++) {
        fprintf(stderr, "%d,", cfg->extreme_point[i]);
    }
    fprintf(stderr, "}\n");
    fprintf(stderr, "isum = %08x\n", cfg->isum);
    fprintf(stderr, "osum = %08x\n", cfg->osum);
    fprintf(stderr, "box_bus = %08x\n", cfg->box_bus);
    fprintf(stderr, "ibufc_bus = %08x\n", cfg->ibufc_bus);
    fprintf(stderr, "obufc_bus = %08x\n", cfg->obufc_bus);
    fprintf(stderr, "irq_mask = %08x\n", cfg->irq_mask);//1 - mask
}

void dump_bst_cfg_info(bst_hw_once_cfg_s *cfg)
{
    print_data_format("src_format", cfg->src_format);
    print_data_format("dst_format", cfg->dst_format);

    fprintf(stderr, "kernel_size = %d\n", cfg->kernel_size);
    fprintf(stderr, "kernel_xstride = %d\n", cfg->kernel_xstride);
    fprintf(stderr, "kernel_ystride = %d\n", cfg->kernel_ystride);
    fprintf(stderr, "zero_point = %d\n", cfg->zero_point);
    fprintf(stderr, "pad_left = %d\n", cfg->pad_left);
    fprintf(stderr, "pad_right = %d\n", cfg->pad_right);
    fprintf(stderr, "pad_top = %d\n", cfg->pad_top);
    fprintf(stderr, "pad_bottom = %d\n", cfg->pad_bottom);
    fprintf(stderr, "task_len = %d\n", cfg->task_len);
    fprintf(stderr, "chain_bus = 0x%x\n", cfg->chain_bus);
    fprintf(stderr, "ibuft_bus = 0x%x\n", cfg->ibuft_bus);
    fprintf(stderr, "obuft_bus = 0x%x\n", cfg->obuft_bus);

    fprintf(stderr, "src_base0 = %p\n", cfg->src_base0);
    fprintf(stderr, "src_base1 = %p\n", cfg->src_base1);
    fprintf(stderr, "src_w = %d\n", cfg->src_w);
    fprintf(stderr, "src_h = %d\n", cfg->src_h);
    fprintf(stderr, "src_line_stride = %d\n", cfg->src_line_stride);
    fprintf(stderr, "dst_base0 = %p\n", cfg->dst_base0);
    fprintf(stderr, "dst_base1 = %p\n", cfg->dst_base1);
    fprintf(stderr, "dst_line_stride = %d\n", cfg->dst_line_stride);
    fprintf(stderr, "dst_plane_stride = %d\n", cfg->dst_plane_stride);
    fprintf(stderr, "isum = 0x%08x\n", cfg->isum);
    fprintf(stderr, "osum = 0x%08x\n", cfg->osum);
    fprintf(stderr, "irq_mask = 0x%08x\n", cfg->irq_mask);
}

static void initial_cfg(bsc_hw_once_cfg_s *cfg)
{
    memset(cfg, 0, sizeof(bsc_hw_once_cfg_s));
    cfg->box_bus = 0;
    cfg->ibufc_bus = 0;
    cfg->obufc_bus = 0;

    cfg->irq_mask = 1;
}

static frmc_mode_sel_e random_mode(bsc_hw_once_cfg_s *cfg, int md_sel)
{
    frmc_mode_sel_e mode_array[10] = {BOX_RSZ_CHN2CHN, BOX_RSZ_NV2BGR,
                                      LINE_RSZ_CHN2CHN, LINE_RSZ_NV2BGR,
                                      AFFINE_BGR2BGR, AFFINE_NV2BGR, AFFINE_NV2NV,
                                      PERSP_BGR2BGR, PERSP_NV2BGR, PERSP_NV2NV};
    //int mode_idx = rand() % 10;
    //int mode_idx = 4;//fix me
    int mode_idx = md_sel;
    frmc_mode_sel_e mode_sel = mode_array[mode_idx];
    bsc_hw_data_format_e chn2chn_format[4] = {BSC_HW_DATA_FM_BGRA, BSC_HW_DATA_FM_F32_2B,
                                             BSC_HW_DATA_FM_F32_4B, BSC_HW_DATA_FM_F32_8B};
    bsc_hw_data_format_e nv2bgr_format[12] = {BSC_HW_DATA_FM_BGRA, BSC_HW_DATA_FM_GBRA,
                                             BSC_HW_DATA_FM_RBGA, BSC_HW_DATA_FM_BRGA,
                                             BSC_HW_DATA_FM_GRBA, BSC_HW_DATA_FM_RGBA,
                                             BSC_HW_DATA_FM_ABGR, BSC_HW_DATA_FM_AGBR,
                                             BSC_HW_DATA_FM_ARBG, BSC_HW_DATA_FM_ABRG,
                                             BSC_HW_DATA_FM_AGRB, BSC_HW_DATA_FM_ARGB};
    uint8_t src_radom_idx;
    uint8_t dst_radom_idx;
    switch (mode_sel) {
    case BOX_RSZ_CHN2CHN:
        cfg->affine = false;
        cfg->box_mode = true;
        src_radom_idx = 0;//fix me
        cfg->src_format = chn2chn_format[src_radom_idx];
        cfg->dst_format = cfg->src_format;
        break;
    case BOX_RSZ_NV2BGR:
        cfg->affine = false;
        cfg->box_mode = true;
        cfg->src_format = BSC_HW_DATA_FM_NV12;
        dst_radom_idx = rand() % 12;
        cfg->dst_format = nv2bgr_format[dst_radom_idx];
        break;
    case LINE_RSZ_CHN2CHN:
        cfg->affine = false;
        cfg->box_mode = false;
        src_radom_idx = rand() % 4;
        cfg->src_format = chn2chn_format[src_radom_idx];
        cfg->dst_format = cfg->src_format;
        break;
    case LINE_RSZ_NV2BGR:
        cfg->affine = false;
        cfg->box_mode = false;
        cfg->src_format = BSC_HW_DATA_FM_NV12;
        dst_radom_idx = rand() % 12;
        cfg->dst_format = nv2bgr_format[dst_radom_idx];
        break;
    case AFFINE_BGR2BGR:
        cfg->affine = true;
        cfg->box_mode = true;
        cfg->src_format = BSC_HW_DATA_FM_BGRA;
        cfg->dst_format = BSC_HW_DATA_FM_BGRA;
        break;
    case AFFINE_NV2BGR:
        cfg->affine = true;
        cfg->box_mode = true;
        cfg->src_format = BSC_HW_DATA_FM_NV12;
        dst_radom_idx = rand() % 12;
        cfg->dst_format = nv2bgr_format[dst_radom_idx];
        break;
    case AFFINE_NV2NV:
        cfg->affine = true;
        cfg->box_mode = true;
        cfg->src_format = BSC_HW_DATA_FM_NV12;
        cfg->dst_format = BSC_HW_DATA_FM_NV12;
        break;
    case PERSP_BGR2BGR:
        cfg->affine = true;
        cfg->box_mode = true;
        cfg->src_format = BSC_HW_DATA_FM_BGRA;
        cfg->dst_format = BSC_HW_DATA_FM_BGRA;
        break;
    case PERSP_NV2BGR:
        cfg->affine = true;
        cfg->box_mode = true;
        cfg->src_format = BSC_HW_DATA_FM_NV12;
        dst_radom_idx = rand() % 12;
        cfg->dst_format = nv2bgr_format[dst_radom_idx];
        break;
    case PERSP_NV2NV:
        cfg->affine = true;
        cfg->box_mode = true;
        cfg->src_format = BSC_HW_DATA_FM_NV12;
        cfg->dst_format = BSC_HW_DATA_FM_NV12;
        break;
    default:
        fprintf(stderr, "[Error][%s]: unknown mode\n", __func__);
        break;
    }
    return mode_sel;
}

bool set_bresize_matrix(bsc_hw_once_cfg_s *cfg)
{
    int frmc_cfg_fail=0;

    uint32_t y_scale;
    float kk = 0.0;
    float mono = 0;
    float frmc_dm0 = (float)cfg->src_box_w / (float)cfg->dst_box_w ;
    float frmc_dm1 = 0;
    float frmc_dm2 = 0.5 * frmc_dm0 - 0.5;
    float frmc_dm3 = 0;
    float frmc_dm4 = (float)cfg->src_box_h / (float)cfg->dst_box_h;//0.5*scale_x -0.5
    float frmc_dm5 = 0.5*frmc_dm4 - 0.5;
    float frmc_dm6 = 0;//perspect0;
    float frmc_dm7 = 0;//perspect1;
    float frmc_dm8 = 0;//perspect2;

    float coef[6] = {0.0,0.0,0.0,0.0,0.0,0.0};
    float frmc_dm1_fabs = fabs(frmc_dm4);
    uint32_t  y_n=0;
    uint32_t y_scale_cod = frmc_dm1_fabs < 1; // only for resize
    if (frmc_dm1_fabs < 1) {
        if ((frmc_dm1_fabs / 0.5) >= 1) {
            y_n = 1;
            kk = frmc_dm1_fabs / 0.5;
        } else if ((frmc_dm1_fabs / 0.25) >= 1) {
            y_n = 2;
            kk = frmc_dm1_fabs / 0.25;
        } else if ((frmc_dm1_fabs / 0.125) >= 1) {
            y_n = 3;
            kk = frmc_dm1_fabs / 0.125;
        } else if ((frmc_dm1_fabs / 0.0625) >= 1) {
            y_n = 4;
            kk = frmc_dm1_fabs / 0.0625;
        } else if ((frmc_dm1_fabs / 0.03125) >= 1) {
            y_n = 5;
            kk = frmc_dm1_fabs/0.03125;
        } else if ((frmc_dm1_fabs / 0.015625) >= 1) {
            y_n = 6;
            kk = frmc_dm1_fabs/0.015625;
        } else {
            frmc_cfg_fail = true;
            printf("####: y amplify cfg out range!####\n");
        }
        y_scale = 1 << y_n;
        if ((cfg->src_box_h * y_scale) > 32768) {
            frmc_cfg_fail = true;
            printf("####affine y scale: src_h=src_box_h*y_scale cfg out range(>65536)!####\n");
        }
        cfg->y_gain_exp = y_n;
    } else {
        cfg->y_gain_exp = 0;
    }

    if (frmc_dm1_fabs < 1) {
        cfg->matrix[0] = frmc_dm0 * 65536;
        cfg->matrix[1] = frmc_dm1 * 65536;
        cfg->matrix[2] = frmc_dm2 * 65536;
        if (frmc_dm4 < 0) {
            cfg->matrix[4] = -(kk * 65536);
        } else {
            cfg->matrix[4] = kk * 65536;
        }
        int32_t dm3_t = frmc_dm3 * 65536;
        int32_t dm5_t = frmc_dm5 * 65536;
        cfg->matrix[3] = dm3_t * y_scale;
        cfg->matrix[5] = dm5_t * y_scale;
    } else {
        cfg->matrix[0] = frmc_dm0 * 65536;
        cfg->matrix[1] = frmc_dm1 * 65536;
        cfg->matrix[2] = frmc_dm2 * 65536;
        cfg->matrix[3] = frmc_dm3 * 65536;
        cfg->matrix[4] = frmc_dm4 * 65536;
        cfg->matrix[5] = frmc_dm5 * 65536;
        cfg->matrix[6] = frmc_dm6 * 65536;
        cfg->matrix[7] = frmc_dm7 * 65536;
        cfg->matrix[8] = frmc_dm8 * 65536;
        cfg->y_gain_exp = 0;
    }
    return frmc_cfg_fail;
}

bool set_lresize_box_info(bsc_hw_once_cfg_s *cfg)
{
    bool frmc_cfg_fail = false;
    cfg->boxes_info = (uint32_t*)malloc(cfg->box_num * 24);
    uint32_t *p = (uint32_t *)cfg->boxes_info;
    uint32_t line_y_n;
    uint32_t line_y_scale;
    float bid_scale_y[64];
    for (int i = 0; i < 64; i++) {
        bid_scale_y[i] = 0.0;
    }

    float *box_base = (float *)malloc(cfg->box_num * 8 * sizeof(float));
    float *pp = box_base;
    for (int i = 0; i < cfg->box_num; i++) {
        float sbx = rand()%cfg->src_box_w + 1;
        float sby = rand()%cfg->src_box_h + 1;
        float sw  = rand()%255 +1;
        float sh  = rand()%255 +1;
        float dm0 = (float)sw / (float)cfg->dst_box_w;
        float dm4 = (float)sh / (float)cfg->dst_box_h;
        float dm2 = 0.5 * dm0 - 0.5;//should be fix
        float dm5 = 0.5 * dm4 - 0.5;
        pp[i*8 + 0] = sbx ;
        pp[i*8 + 1] = sby ;
        pp[i*8 + 2] = sw ;
        pp[i*8 + 3] = sh;
        pp[i*8 + 4] = dm0;
        pp[i*8 + 5] = dm4;
        pp[i*8 + 6] = dm2;
        pp[i*8 + 7] = dm5;
        bid_scale_y[i] = dm4 ;
        /*printf("box[0x%0x]:bid:%d, %f, %f, %f, %f, %f, %f, %f, %f\n", &pp[i*8 + i], i,
          pp[i*8 + 0], pp[i*8 + 1], pp[i*8 + 2], pp[i*8 + 3], pp[i*8 + 4], pp[i*8 + 5],pp[i*8 + 6],pp[i*8 + 7]);*/

    }
    //cfg y_scale
    float tmp_yscale=0.0;
    for (int i = 0; i < cfg->box_num - 1; i++) {
        for(int j = i + 1; j < cfg->box_num; j++) {
            if (bid_scale_y[i] > bid_scale_y[j]) {
                tmp_yscale = bid_scale_y[i];
                bid_scale_y[i] = bid_scale_y[j];
                bid_scale_y[j] = tmp_yscale;
            }
        }
    }

    if (bid_scale_y[0] < 1) {//amplyfy
        if ((bid_scale_y[0] / 0.5) >= 1) {
            line_y_n = 1;
        } else if ((bid_scale_y[0] / 0.25) >= 1) {
            line_y_n = 2;
        } else if (bid_scale_y[0] / 0.125 >= 1) {
            line_y_n = 3;
        } else if (bid_scale_y[0]/0.0625 >= 1) {
            line_y_n = 4;
        } else if (bid_scale_y[0]/0.03125>=1){
            line_y_n = 5;
        } else if (bid_scale_y[0]/0.015625>=1){
            line_y_n = 6;
        } else {
            frmc_cfg_fail = true;
            printf("####: y amplify cfg out range!####\n");
        }
        line_y_scale = 1<<line_y_n;
        printf("line_y_scale:%d\n",line_y_scale);
        if((cfg->src_box_h*line_y_scale)>=32768){//repeat signed frmc_h
            frmc_cfg_fail = true;
            printf("####affine y scale: src_h=src_box_h*line_y_scale cfg out range(>=32768)!####\n");
        }

        cfg->y_gain_exp = line_y_n;

        for (int i = 0; i < cfg->box_num; i++) {
            uint16_t sbx = (uint16_t)pp[i*8 + 0];
            uint16_t sby = (uint16_t)pp[i*8 + 1];// * line_y_scale;
            uint16_t sw     = (uint16_t)pp[i*8 + 2];
            uint16_t sh     = (uint16_t)pp[i*8 + 3];// * line_y_scale;
            float dm0 = pp[i*8 + 4];
            float dm4 = pp[i*8 + 5] * line_y_scale;
            float dm2 = pp[i*8 + 6];
            float dm5 = pp[i*8 + 7] * line_y_scale;
            p[i*6 + 0] = sbx << 0 | sby << 16;
            p[i*6 + 1] = sw << 0 | sh << 16;
            p[i*6 + 2] = dm0 * 65536;
            p[i*6 + 3] = dm4 * 65536;
            p[i*6 + 4] = dm2 * 65536;
            p[i*6 + 5] = dm5 * 65536;
            /*printf("line resize amplify:box[0x%0x]: 0x%0x, 0x%0x, 0x%0x, 0x%0x,0x%0x,0x%0x\n", &p[i*6 + i],
              p[i*6 + 0], p[i*6 + 1], p[i*6 + 2], p[i*6 + 3], p[i*6 + 4], p[i*6 + 5]);*/

        }
    } else {
        for (int i = 0; i < cfg->box_num; i++) {
            uint16_t sbx = (uint16_t)pp[i*8 + 0];
            uint16_t sby = (uint16_t)pp[i*8 + 1];
            uint16_t sw     = (uint16_t)pp[i*8 + 2];
            uint16_t sh     = (uint16_t)pp[i*8 + 3];
            float dm0 = pp[i*8 + 4];
            float dm4 = pp[i*8 + 5];
            float dm2 = pp[i*8 + 6];
            float dm5 = pp[i*8 + 7];
            p[i*6 + 0] = sbx << 0 | sby << 16;
            p[i*6 + 1] = sw << 0 | sh << 16;
            p[i*6 + 2] = dm0*65536;
            p[i*6 + 3] = dm4*65536;
            p[i*6 + 4] = dm2*65536;
            p[i*6 + 5] = dm5*65536;
        }
    }
    free(box_base);
    return frmc_cfg_fail;
}

void perspective_mono_extreme(bsc_hw_once_cfg_s *cfg)
{
    float dm0 = ((float)cfg->matrix[0]) / 65536;
    float dm1 = ((float)cfg->matrix[1]) / 65536;
    float dm2 = ((float)cfg->matrix[2]) / 65536;
    float dm3 = ((float)cfg->matrix[3]) / 65536;
    float dm4 = ((float)cfg->matrix[4]) / 65536;
    float dm5 = ((float)cfg->matrix[5]) / 65536;
    float dm6 = ((float)cfg->matrix[6]) / 65536;
    float dm7 = ((float)cfg->matrix[7]) / 65536;
    float dm8 = ((float)cfg->matrix[8]) / 65536;
    //printf("persp:%f,%f,%f,%f,%f,%f,%f,%f,%f\n",dm0,dm1,dm2,dm3,dm4,dm5,dm6,dm7,dm8);
    //NOT:SZ=0
    //calc x mono:->(m1*m6-m7*m0)*dx + (m1*m8-m7*m2)
    uint8_t x_mono;
    float x_value;
    int32_t x_value_i;
    uint8_t mono_x_clip;
    x_mono = (dm1*dm6 - dm7*dm0)>=0 ? 0 : 1;
    x_value = (dm7*dm2 - dm1*dm8)/(dm1*dm6-dm7*dm0);
    uint8_t flag=0;
    if((x_value<0) & (x_value>-1)){
        flag = 1;
    }
    int32_t x_ceil = flag ? 0: (((int32_t)x_value) + 1);
    x_value_i = x_ceil - cfg->dst_box_x;//ceil(x_value);
    mono_x_clip = CLIP(x_value_i, 0, 64);
    cfg->mono_x = x_mono << 7 | mono_x_clip << 0;
    flag = 0;
    //calc y mono:->(m4*m6-m7*m3)*dx + m4*m8 -m7*m5
    uint8_t y_mono;
    float y_value;
    int32_t y_value_i;
    uint8_t mono_y_clip;
    y_mono = (dm4*dm6 - dm7*dm3)>=0 ? 0 : 1;
    y_value = (dm7*dm5 - dm4*dm8)/(dm4*dm6-dm7*dm3);
    if((y_value<0) & (y_value>-1)){
        flag = 1;
    }
    int32_t y_ceil = flag ? 0: (((int32_t)y_value) + 1);
    y_value_i =  y_ceil - cfg->dst_box_x;//ceil(y_value);
    mono_y_clip = CLIP(y_value_i, 0, 64);
    cfg->mono_y = y_mono << 7 | mono_y_clip <<0;
    //printf("persp:mono_x:0x%x,x_value:%f,x_value_i:0x%x,mono_y:%d,y_value:%f,y_value_i:%d\n",cfg->mono_x,x_value,x_value_i,cfg->mono_y,y_value,y_value_i);
    //calc extreme -(m6*dx + m8)/m7
    uint8_t dx_indx;
    int8_t extreme_value[64] = {0};
    for(dx_indx=0;dx_indx<64; dx_indx++){
        cfg->extreme_point[dx_indx] = 0;
    }

    for (int dx_indx = 0; dx_indx < cfg->dst_box_w; dx_indx++) {
        float val = -(dm6*dx_indx + dm8)/dm7;
        int32_t val_i = val;
        if (val_i == val) {
            val_i = val;
        } else {
            val_i = val + 1;
        }
        int8_t extreme_value_clip = CLIP(val_i, 0, 64);
        cfg->extreme_point[dx_indx] = extreme_value_clip;//extreme_point_rt
        // printf("persp:extreme:%d,val:%f\n",cfg->extreme_point[dx_indx],val);
    }
}

void random_size(bsc_hw_once_cfg_s *cfg)
{
    uint32_t src_format = cfg->src_format & 0x1;
    uint32_t dst_format = cfg->dst_format & 0x1;
    uint32_t bpp_mode = (cfg->src_format >> 5) & 0x3;

    //src size
    //cfg->src_box_h = rand() % 1023 + 1;
    cfg->src_box_h = rand() % 255 + 1;
    if (bpp_mode == 0) {//bgr
        //cfg->src_box_w = rand() % 2047 + 1;//1->2048
        cfg->src_box_w = rand() % 255 + 1;//1->2048
    } else if (bpp_mode == 1) {//2bit
        //cfg->src_box_w=rand() % 1023 + 1;//1->>1024
        cfg->src_box_w=rand() % 127 + 1;//1->>1024
    } else if (bpp_mode == 2) {//4bit
        //cfg->src_box_w = rand() % 511 + 1;//1->>512
        cfg->src_box_w = rand() % 63 + 1;//1->>512
    } else {//8bit channel
        //cfg->src_box_w = rand() % 255 + 1;//1->>256
        cfg->src_box_w = rand() % 63 + 1;//1->>512
    }

    if ((cfg->src_format == BSC_HW_DATA_FM_NV12) || (bpp_mode == 0)) {
        cfg->src_box_w = ((cfg->src_box_w >> 1) + 1) << 1; //align 2pixel
        cfg->src_box_h = ((cfg->src_box_h >> 1) + 1) << 1; //align 2pixel
    } else {//feature map:2/4/8-->align 64bit
        cfg->src_box_w = cfg->src_box_w;
        cfg->src_box_h = cfg->src_box_h;
    }
    //fix me
    cfg->src_box_h = 20;//0x15c;//fix me
    cfg->src_box_w = 20;//0x15c;//fix me

    if (cfg->src_format == BSC_HW_DATA_FM_NV12) { //nv12
        cfg->src_line_stride = cfg->src_box_w + rand()%8;//align 1byte
    } else { //channel / bgr
        cfg->src_line_stride = (cfg->src_box_w << (2 + bpp_mode)) + rand()%8;
    }

    //dst info
    if (cfg->affine == false) {
        if (cfg->box_mode == true) {
            cfg->dst_box_w = rand() % 64 + 1;//bresize:0-64
            //cfg->dst_box_h = rand() % 255 + 1;
            cfg->dst_box_h = rand() % 127 + 1;
        } else {
            cfg->dst_box_w = rand() % 100 + 1;//###NOTING the random range
            cfg->dst_box_h = rand() % 100 + 1;//###NOTING the random range
        }
    } else {
        cfg->dst_box_w = rand() % 63 + 1;
        cfg->dst_box_h = rand() % 63 + 1;
        if (dst_format == 0) {//nv12 should be even
            cfg->dst_box_w = ((cfg->dst_box_w >> 1) + 1) << 1;
            cfg->dst_box_h = ((cfg->dst_box_h >> 1) + 1) << 1;
        } else {
            cfg->dst_box_w = cfg->dst_box_w ;
            cfg->dst_box_h = cfg->dst_box_h ;
        }
    }
    //fix me
    cfg->dst_box_w = 20;//0x40;
    cfg->dst_box_h = 20;//0x40;
    if (cfg->dst_format == 0) { //nv12
        cfg->dst_line_stride = cfg->dst_box_w + rand()%8;
    } else { //channel
        cfg->dst_line_stride = (cfg->dst_box_w <<(2+bpp_mode))+ rand()%8;
    }

    //box_num info
    if (cfg->box_mode == true) {//box_mode
        cfg->box_num = 1;
    } else {
        cfg->box_num = rand()%63 +1;
    }

    if (cfg->box_mode == true) {
        cfg->src_box_x = 0;//rand()%20+1;//should be fix
        cfg->src_box_y = 0;//rand()%20+1;
        cfg->dst_box_x = 0;//rand()%20+1;
        cfg->dst_box_y = 0;//rand()%20+1;
    } else {
        cfg->src_box_x = 0;//rand()%20+1;//should be fix
        cfg->src_box_y = 0;
        cfg->dst_box_x = 0;
        cfg->dst_box_y = 0;
    }
}

bool random_param(bsc_hw_once_cfg_s *cfg, frmc_mode_sel_e mode)
{
    uint32_t nv2bgr_coef[9];//={0x4ad, 0x669, 0x0, 0x4ad, 0x344, 0x193, 0x4ad, 0x0, 0x81a};
    uint8_t nv2bgr_ofst[2];//={16,128};
    bool frmc_cfg_fail = false;
    int num1,num2;
    /*for (int i = 0; i < 9; i++) {
      cfg->coef[i] = rand() % 4096;
      }

      cfg->offset[0] = rand()% 256;
      cfg->offset[1] = rand()% 256;*/

    cfg->coef[0] = 1220542;
    cfg->coef[1] = 2116026;
    cfg->coef[2] = 0;
    cfg->coef[3] = 1220542;
    cfg->coef[4] = 409993;
    cfg->coef[5] = 852492;
    cfg->coef[6] = 1220542;
    cfg->coef[7] = 0;
    cfg->coef[8] = 1673527;

    cfg->offset[0] = 16;
    cfg->offset[1] = 128;

    cfg->nv2bgr_alpha = 10;

    cfg->zero_point = 20;//should be fix
    if ((mode == AFFINE_BGR2BGR) || (mode == AFFINE_NV2BGR) ||
        (mode == AFFINE_NV2NV))    {
        cfg->matrix[0] = rand();
        cfg->matrix[1] = rand();
        cfg->matrix[2] = rand();
        cfg->matrix[3] = rand();
        cfg->matrix[4] = rand();
        cfg->matrix[5] = rand();
        cfg->matrix[6] = 0;
        cfg->matrix[7] = 0;
        cfg->matrix[8] = 65536;
    } else if ((mode == PERSP_BGR2BGR) || (mode == PERSP_NV2BGR) ||
               (mode == PERSP_NV2NV)) {
        cfg->matrix[0] = rand();
        cfg->matrix[1] = rand();
        cfg->matrix[2] = rand();
        cfg->matrix[3] = rand();
        cfg->matrix[4] = rand();
        cfg->matrix[5] = rand();
        cfg->matrix[6] = rand();
        cfg->matrix[7] = rand();
        cfg->matrix[8] = rand();
        perspective_mono_extreme(cfg);
    } else if ((mode == BOX_RSZ_CHN2CHN) || (mode == BOX_RSZ_NV2BGR)) {
        frmc_cfg_fail = set_bresize_matrix(cfg);
    } else if ((mode == LINE_RSZ_CHN2CHN) || (mode == LINE_RSZ_NV2BGR)) {
        frmc_cfg_fail = set_lresize_box_info(cfg);
    }
    //delete me
    //cfg->matrix[0] = 0x4d869;
    //cfg->matrix[1] = 0xfffeacaa;
    //cfg->matrix[2] = 0x6e3981;
    //cfg->matrix[3] = 0x1c982;
    //cfg->matrix[4] = 0x6b064;
    //cfg->matrix[5] = 0xffa13da8;
    //cfg->matrix[6] = 0;//0xc0
    //cfg->matrix[7] = 0;//0xa9
    //cfg->matrix[8] = 65536;//0x100bb

    return frmc_cfg_fail;
}

void alloc_space(bsc_hw_once_cfg_s *cfg)
{
    if (cfg->src_format == BSC_HW_DATA_FM_NV12) {
        int size = cfg->src_line_stride * cfg->src_box_h;
        cfg->src_base0 = malloc(size);
        cfg->src_base1 = malloc(size / 2);
        for (int i = 0; i < size; i++) {
            cfg->src_base0[i] = rand()%256;
        }
        for (int i = 0; i < size / 2; i++) {
            cfg->src_base1[i] = rand()%256;
        }
    } else {
        uint32_t bpp_mode = (cfg->src_format >> 5) & 0x3;
        int size = cfg->src_line_stride * cfg->src_box_h;
        cfg->src_base0 = malloc(size);
        /* for (int i = 0; i < size; i++) {
           cfg->src_base0[i] = rand()%256;
           }*/
        int i,j,k;
        uint32_t idx;
        uint8_t bpp = 1<<(2+bpp_mode);
        for(j=0; j<cfg->src_box_h; j++){
            for(i=0; i<cfg->src_box_w; i++){
                for(k=0;k<bpp;k++){
                    idx = cfg->src_line_stride * j + i*bpp + k;
                    if((cfg->affine == 1) & (cfg->src_format != BSC_HW_DATA_FM_NV12) & (k == 3)){
                        cfg->src_base0[idx] = cfg->zero_point;  //NOT 1(zero_point),2(obufc_order)
                    }else{
                        cfg->src_base0[idx] = rand()%256;
                    }
                }
            }
        }
    }

    if (cfg->dst_format == BSC_HW_DATA_FM_NV12) {
        int size = cfg->dst_line_stride * cfg->dst_box_h;
        cfg->dst_base[0] = malloc(size);
        cfg->dst_base[1] = malloc(size / 2);
    } else {
        uint32_t bpp_mode = (cfg->dst_format >> 5) & 0x3;
        int size = cfg->dst_line_stride * cfg->dst_box_h;
        /*if (bpp_mode == 0) {
          size *= 4;
          } else if (bpp_mode == 1) {
          size *= 8;
          } else if (bpp_mode == 2) {
          size *= 16;
          } else if (bpp_mode == 3) {
          size *= 32;
          }*/

        if (cfg->box_mode) {
            cfg->dst_base[0] = malloc(size);
        } else {
            void *dst_all = malloc(size * cfg->box_num);
            for (int i = 0; i < cfg->box_num; i++) {
                cfg->dst_base[i] = (uint8_t *)dst_all + size * i;
            }
        }
    }
}

bool bsc_random(bsc_hw_once_cfg_s *cfg, int mode_sel)
{
    bool frmc_cfg_fail = false;

    // 1. initial cfg
    initial_cfg(cfg);

    // 2. random mode and format
    frmc_mode_sel_e mode = random_mode(cfg, mode_sel);

    // 3. random size
    random_size(cfg);

    // 4. random param
    frmc_cfg_fail = random_param(cfg, mode);

    // 5. alloc space
    alloc_space(cfg);

    //6. check the cfg
    return frmc_cfg_fail;

}

bs_chain_ret_s bscaler_c_chain_cfg(bsc_hw_once_cfg_s *cfg, uint32_t *addr)
{
    uint32_t len = 0;
    uint32_t i;
    uint32_t box_num;
    uint32_t src_format = cfg->src_format & 0x1;
    uint32_t dst_format = cfg->dst_format & 0x1;
    uint32_t bpp_mode = (cfg->src_format >> 5) & 0x3;
    bs_chain_ret_s ret;
    if (!dst_format)
        box_num = 2;
    else
        box_num = cfg->box_num;
    len += write_chain(BSCALER_FRMC_YBASE_SRC,//1
                       cfg->src_base0, &addr, 0);
    len += write_chain(BSCALER_FRMC_CBASE_SRC,//2
                       cfg->src_base1, &addr, 0);

    len += write_chain(BSCALER_FRMC_BOX_BASE,//3
                       cfg->boxes_info, &addr, 0);

    for (i = 0; i < box_num; i++) {
        len += write_chain(BSCALER_FRMC_BASE_DST,
                           cfg->dst_base[i], &addr, 0);
    }

    len += write_chain(BSCALER_FRMC_MODE, ((src_format & 0x1) << 0 |//4
                                           (cfg->affine & 0x1) << 1 |
                                           (cfg->box_mode & 0x1) << 2 |
                                           (dst_format & 0x1) << 3 |
                                           (bpp_mode & 0x3) << 4 |
                                           (cfg->zero_point & 0xff) << 8 |
                                           (cfg->y_gain_exp & 0x7) << 16), &addr, 0);
    len += write_chain(BSCALER_FRMC_WH_SRC, (cfg->src_box_h << 16 |//5
                                             cfg->src_box_w), &addr, 0);
    len += write_chain(BSCALER_FRMC_PS_SRC, cfg->src_line_stride, &addr, 0);//6
    len += write_chain(BSCALER_FRMC_WH_DST, (cfg->dst_box_h << 16 |//7
                                             cfg->dst_box_w), &addr, 0);
    len += write_chain(BSCALER_FRMC_PS_DST,//8
                       ((cfg->dst_line_stride & 0xffff) << 0), &addr, 0);
    //fill box info
    len += write_chain(BSCALER_FRMC_BOX0, (cfg->src_box_y << 16 |//9
                                           cfg->src_box_x) , &addr, 0);
    len += write_chain(BSCALER_FRMC_BOX1, (cfg->dst_box_y << 16 |//10
                                           cfg->dst_box_x) , &addr, 0);
    len += write_chain(BSCALER_FRMC_BOX2, cfg->matrix[0], &addr, 0);//11
    len += write_chain(BSCALER_FRMC_BOX3, cfg->matrix[1], &addr, 0);//12
    len += write_chain(BSCALER_FRMC_BOX4, cfg->matrix[2], &addr, 0);//13
    len += write_chain(BSCALER_FRMC_BOX5, cfg->matrix[3], &addr, 0);//14
    len += write_chain(BSCALER_FRMC_BOX6, cfg->matrix[4], &addr, 0);//15
    len += write_chain(BSCALER_FRMC_BOX7, cfg->matrix[5], &addr, 0);//16
    len += write_chain(BSCALER_FRMC_BOX8, cfg->matrix[6], &addr, 0);//17
    len += write_chain(BSCALER_FRMC_BOX9, cfg->matrix[7], &addr, 0);//18
    len += write_chain(BSCALER_FRMC_BOX10, cfg->matrix[8], &addr, 0);//19
    len += write_chain(BSCALER_FRMC_MONO, (cfg->mono_x << 0 |
                                           cfg->mono_y << 16), &addr, 0);//20
    for (i = 0; i < cfg->dst_box_w; i += 4) {
        len += write_chain(BSCALER_FRMC_EXTREME_Y,
                           (cfg->extreme_point[i] << 0 |
                            cfg->extreme_point[i+1] << 8 |
                            cfg->extreme_point[i+2] << 16 |
                            cfg->extreme_point[i+3] << 24), &addr, 0);
    }

    len += write_chain(BSCALER_FRMC_CTRL,
                       (cfg->obufc_bus << 12 |
                        cfg->ibufc_bus << 11 |
                        cfg->box_bus << 10 |
                        cfg->irq_mask << 9 | 1 << 0), &addr, 1);//21

    {
        ret.bs_ret_base = addr;
        ret.bs_ret_len = len;
        return (ret);
    }
}

bs_chain_ret_s  bscaler_chain_init(uint32_t frame, bsc_hw_once_cfg_s *frmc_cfg){
    //init chain cfg
    uint32_t dst_bnum = 0;
    uint32_t cfg_bnum = 0;
    uint32_t dst_wnum = 0;
    uint32_t cfg_wnum = 0;
    uint32_t chain_base;
    uint32_t chain_len = 0;
    int n;
    for(n = 0; n < frame; n++){
        uint32_t dst_format = (&frmc_cfg[n])->dst_format & 0x1;
        if(!dst_format)
            cfg_bnum = 2;
        else
            cfg_bnum = (&frmc_cfg[n]) -> box_num;
        dst_bnum = dst_bnum + cfg_bnum;
        cfg_wnum = (&frmc_cfg[n]) -> dst_box_w;
        dst_wnum = dst_wnum + cfg_wnum;
    }
    chain_base = (uint32_t)malloc(8*(22 + dst_bnum + dst_wnum) * frame);//align 1bytes
    if(chain_base == NULL){
        printf("error : malloc chain_base is failed ! \n ");
    }

    //configure chain
    uint32_t chain_base1 = chain_base;
    for(n=0; n<frame; n++){//frame n
        bs_chain_ret_s  back;
        bsc_hw_once_cfg_s *frmc_cfg_once = &frmc_cfg[n];
        back = bscaler_c_chain_cfg(frmc_cfg_once, chain_base1);//
        chain_base1 = back.bs_ret_base;
        chain_len = chain_len + back.bs_ret_len;
    }//frame n
    {
        bs_chain_ret_s chain_info;
        chain_info.bs_ret_base = chain_base;
        chain_info.bs_ret_len  = chain_len;
        //printf("chain info:0x%x,0x%x\n",chain_info[0],chain_info[1]);
        return (chain_info);
    }
}

bs_chain_ret_s  bscaler_t_chain_cfg (bst_hw_once_cfg_s *cfg, uint32_t *addr)
{
    uint32_t len = 0;
    bs_chain_ret_s ret;

    len += write_chain(BSCALER_FRMT_YBASE_SRC,
                       cfg->src_base0, &addr, 0);
    len += write_chain(BSCALER_FRMT_CBASE_SRC,
                       cfg->src_base1, &addr, 0);
    len += write_chain(BSCALER_FRMT_YBASE_DST,
                       cfg->dst_base0, &addr, 0);
    len += write_chain(BSCALER_FRMT_CBASE_DST,
                       cfg->dst_base1, &addr, 0);

    len += write_chain(BSCALER_FRMT_WH_SRC, (cfg->src_h << 16 |
                                             cfg->src_w), &addr, 0);
    len += write_chain(BSCALER_FRMT_PS_SRC,
                       (cfg->src_line_stride & 0xFFFF), &addr, 0);

    len += write_chain(BSCALER_FRMT_PS_DST, cfg->dst_line_stride, &addr, 0);
    len += write_chain(BSCALER_FRMT_FS_DST, cfg->dst_plane_stride, &addr, 0);
    len += write_chain(BSCALER_FRMT_DUMMY, cfg->zero_point, &addr, 0);
    len += write_chain(BSCALER_FRMT_FORMAT, ((cfg->src_format & 0x1) << 0 | //[1:0]
                                             (cfg->dst_format & 0x3) << 2 | //[3:2]
                                             (cfg->kernel_xstride & 0x3) << 4 | //[5:4]
                                             (cfg->kernel_ystride & 0x3) << 6 | //[7:6]
                                             (cfg->kernel_size & 0x3) << 8 | //[9:8]
                                             (cfg->pad_left & 0x7) << 16 | //[18:16]
                                             (cfg->pad_right & 0x7) << 20 | //[22:20]
                                             (cfg->pad_top & 0x7) << 24 | //[26:24]
                                             (cfg->pad_bottom & 0x7) << 28), &addr, 0);//[30:28]
    len += write_chain(BSCALER_FRMT_CTRL, ((cfg->obuft_bus & 0x1) << 11 |
                                           (cfg->ibuft_bus & 0x1) << 10 |
                                           (cfg->irq_mask & 0x1) << 9 | 1), &addr, 1);
    {
        ret.bs_ret_base = addr;
        ret.bs_ret_len = len;
        return (ret);
    }

}

//for frmt chain init
bs_chain_ret_s  bscaler_chain_t_init(uint32_t frame, bst_hw_once_cfg_s *frmt_cfg){
    //init chain cfg
    uint32_t chain_base;
    uint32_t chain_len = 0;
    int n;
    chain_base = (uint32_t)malloc(8* 11 * frame);//align 1bytes
    if(chain_base == NULL){
        printf("error : malloc chain_base is failed ! \n ");
    }

    //configure chain
    uint32_t chain_base1 = chain_base;
    for(n=0; n<frame; n++){//frame n
        bs_chain_ret_s  back;
        bst_hw_once_cfg_s *frmt_cfg_once = &frmt_cfg[n];
        back = bscaler_t_chain_cfg(frmt_cfg_once, chain_base1);//
        chain_base1 = back.bs_ret_base;
        chain_len = chain_len + back.bs_ret_len;
    }//frame n
    {
        bs_chain_ret_s chain_info;
        chain_info.bs_ret_base = chain_base;
        chain_info.bs_ret_len  = chain_len;
        return (chain_info);
    }
}

uint32_t bacaler_random(uint32_t min, uint32_t max, uint32_t align)
{
    if ((min % align) != 0 || (max%align) != 0)
        fprintf(stderr, "error: random error! min = %0d, max = %0d, align = %0d\n", min, max, align);
    int offset = align - 1;
    uint32_t random_val = ((rand()%(max - min + 1) + min) + offset) & (~(align - 1));
    if (random_val < min)
        random_val = min;
    else if (random_val > max)
        random_val = max;
    return random_val;
}

void bst_format(bst_hw_once_cfg_s *cfg)
{
    // source format
    cfg->src_format = rand()%2; //0: nv12; 1: bgr;
    if (cfg->src_format == 0) {
        uint8_t tmp = rand()%3; //0: nv12; 1: bgr; 3 virchn
        cfg->dst_format = (tmp == 2)? 3 : tmp;
    } else {
        uint8_t tmp = rand()%2; //0: nv12; 1: bgr; 3 virchn
        cfg->dst_format = (tmp == 0) ? 1 : 3;
    }
    cfg->src_format = 0;//fix me
    cfg->dst_format = 1;
    cfg->kernel_size = (cfg->dst_format == 3) ? bacaler_random(0, 3, 1) : 0;  //0: 1x1; 1: 3x3; 2: 5x5; 3: 7x7
    cfg->kernel_xstride = (cfg->dst_format == 3) ? bacaler_random(1, 3, 1) : 1; //1~3;
    cfg->kernel_ystride = (cfg->dst_format == 3) ? bacaler_random(1, 3, 1) : 1;//1~3;
    cfg->zero_point = (cfg->dst_format == 3) ? bacaler_random(0, 1<<32 - 1, 1) : 0;
    cfg->pad_left = (cfg->dst_format == 3) ? bacaler_random(0, 7, 1) : 0; //left panding edge.
    cfg->pad_right = (cfg->dst_format == 3) ? bacaler_random(0, 7, 1) : 0; //right panding edge.
    cfg->pad_top = (cfg->dst_format == 3) ? bacaler_random(0, 7, 1) : 0; //top panding edge.
    cfg->pad_bottom = (cfg->dst_format == 3) ? bacaler_random(0, 7, 1) : 0; //botom panding edge.
    uint32_t nv2bgr_order_rand[12] ={0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd};
    uint32_t order_index  = rand()%12;
    // cfg->nv2bgr_order = (cfg->dst_format == 3) ? 0 : nv2bgr_order_rand[order_index];//0:bgra//fix me
    //cfg->nv2bgr_order = 0;//fixme
}

void bst_size(bst_hw_once_cfg_s *cfg)
{
    cfg->task_len = 2; //cfg->frmt_h_src;
    if (cfg->dst_format == 3) { //kernel
        switch (cfg->kernel_size) {
        case (0):
            cfg->src_w = bacaler_random(2, 2048, 2);
            cfg->src_h = bacaler_random(2, 1024, 2);
            break;
        case (1):
            cfg->src_w = bacaler_random(4, 1024, 2);
            cfg->src_h = bacaler_random(4, 1024, 2);
            break;
        case (2):
            cfg->src_w = bacaler_random(6, 512, 2);
            cfg->src_h = bacaler_random(6, 1024, 2);
            break;
        case (3):
            cfg->src_w = bacaler_random(8, 512, 2);
            cfg->src_h = bacaler_random(8, 1024, 2);;
            break;
        }
    } else { //not kernel
        cfg->src_w = bacaler_random(2, 2048, 2);
        cfg->src_h = bacaler_random(2, 1024, 2);
    }
    cfg->src_w = 8;//fix me
    cfg->src_h = 8;
    //printf("format: src_format(%0d),dst_format(%0d),kernel_size(%0d),kernel_xstride(%0d),kernel_ystride(%0d),frmt_w(0x%x),frmt_h(0x%x)\n",
    //        cfg->src_format,cfg->dst_format,cfg->kernel_size,cfg->kernel_xstride,cfg->kernel_ystride,cfg->src_w,cfg->src_h);
    if (cfg->src_format) { //bgr
        cfg->src_line_stride = (cfg->src_w + bacaler_random(0, 7, 1))*4;
    } else { //nv12
        cfg->src_line_stride = cfg->src_w + bacaler_random(0, 7, 1);
    }
    uint8_t pixel_kernel_w;
    int32_t kernel_frame_w;
    switch (cfg->dst_format) {
    case (0): //nv12
        cfg->dst_line_stride = cfg->src_w + bacaler_random(0, 70, 1);
        break;
    case (1): //bgr
        cfg->dst_line_stride = cfg->src_w*4 + bacaler_random(0, 70, 1);
        break;
    case (2): fprintf(stderr, "error: nv22 isnot support"); break;
    case (3): //kerenel
        pixel_kernel_w = ((cfg->kernel_size == 3) ? 7 :
                          (cfg->kernel_size == 2) ? 5 :
                          (cfg->kernel_size == 1) ? 3 : 1);
        kernel_frame_w = (cfg->src_w + cfg->pad_left + cfg->pad_right - pixel_kernel_w)/cfg->kernel_xstride + 1;
        cfg->dst_line_stride = kernel_frame_w*32;
        cfg->dst_plane_stride = cfg->dst_line_stride*cfg->task_len;
        break;
    }
    printf("src_line_stride = 0x%0x, dst_line_stride = 0x%0x\n", cfg->src_line_stride, cfg->dst_line_stride);
}

void bst_alloc_space(bst_hw_once_cfg_s *cfg)
{
    // src
    cfg->src_base0 = (uint8_t *)malloc(cfg->src_line_stride * cfg->src_h);
    if (cfg->src_format == 0) {//nv12
        cfg->src_base1 = (uint8_t *)malloc(cfg->src_line_stride * cfg->src_h/2);
    } else {
        cfg->src_base1 = NULL;
    }

    // dst
    if (cfg->dst_format == 3) {
        //0 - 1x1
        //1 - 3x3
        //2 - 5x5
        //3 - 7x7
        uint8_t frame_num = ((cfg->kernel_size == 3) ? 5 :
                             (cfg->kernel_size == 2) ? 3 : 1);
        printf("frame_num=%d, cfg->dst_plane_stride=%d\n", frame_num, cfg->dst_plane_stride);
        //cfg->dst_base0 = (uint32_t)malloc(frame_num * cfg->dst_plane_stride);

        int pixel_kernel_h = ((cfg->kernel_size == 3) ? 7 :
                              (cfg->kernel_size == 2) ? 5 :
                              (cfg->kernel_size == 1) ? 3 : 1);
        int oh = (cfg->src_h + cfg->pad_top + cfg->pad_bottom - pixel_kernel_h) / cfg->kernel_ystride + 1;
        printf("cfg->dst_line_stride=%d, oh=%d\n", cfg->dst_line_stride, oh);
        cfg->dst_base0 = (uint32_t)malloc(frame_num * cfg->dst_line_stride * oh);
    } else {
        if (cfg->task_len%2 != 0) {
            fprintf(stderr, "error: malloc dst failed! task_len = %0d\n", cfg->task_len);
        } else {
            cfg->dst_base0 = (uint32_t)malloc(cfg->dst_line_stride * cfg->src_h);
            if (cfg->dst_format == 0) //nv12
                cfg->dst_base1 = (uint32_t)malloc(cfg->dst_line_stride * cfg->src_h/2);
        }
    }

    // initial src data
    uint8_t *p = cfg->src_base0;
    for (int y = 0; y < cfg->src_h; y++) {
        for (int x = 0; x < cfg->src_line_stride; x++) {
            //p[y*cfg->src_line_stride + x] = /*y*cfg->src_line_stride + x; */rand()%255;
            p[y * cfg->src_line_stride + x] = 200;
        }
    }
    if (cfg->src_format == 0) {
        p = (uint8_t *)cfg->src_base1;
        for (int y = 0; y < cfg->src_h/2; y++) {
            for (int x = 0; x < cfg->src_line_stride; x++) {
                //p[y * cfg->src_line_stride + x] = /*y*cfg->src_line_stride + x; */rand()%255;
                p[y * cfg->src_line_stride + x] = 255;
            }
        }
    }
}

void bst_init(bst_hw_once_cfg_s *cfg)
{
    cfg->src_base0 = NULL;
    cfg->src_base1 = NULL;
    cfg->dst_base0 = NULL;
    cfg->dst_base1 = NULL;
    cfg->isum = 0;
    cfg->osum = 0;
    cfg->chain_bus = 0;
    cfg->ibuft_bus = 0;
    cfg->obuft_bus = 0;

    cfg->irq_mask = 1;
    cfg->nv2bgr_coef[0] = 1220542;
    cfg->nv2bgr_coef[1] = 2116026;
    cfg->nv2bgr_coef[2] = 0;
    cfg->nv2bgr_coef[3] = 1220542;
    cfg->nv2bgr_coef[4] = 409993;
    cfg->nv2bgr_coef[5] = 852492;
    cfg->nv2bgr_coef[6] = 1220542;
    cfg->nv2bgr_coef[7] = 0;
    cfg->nv2bgr_coef[8] = 1673527;

    cfg->nv2bgr_ofst[0] = 16;
    cfg->nv2bgr_ofst[1] = 128;

    cfg->nv2bgr_alpha = 10;
}

void bst_random(bst_hw_once_cfg_s *cfg)
{
    bst_init(cfg);

    bst_format(cfg);

    bst_size(cfg);

    bst_alloc_space(cfg);
}
