/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : random_api.cpp
 * Authors    : jmqi@ingenic.st.jz.com
 * Create Time: 2020-08-01:17:18:37
 * Description:
 *
 */
#include <time.h>
#include "Matrix.h"
#include "bscaler_api.h"
#include "bscaler_hal.h"
#include "random_api.h"

#define MAX_SRC_WIDTH           512
#define MIN_SRC_WIDTH           2
#define MAX_SRC_HEIGHT          512
#define MIN_SRC_HEIGHT          2
#define MAX_DST_WIDTH           512
#define MIN_DST_WIDTH           2
#define MAX_DST_HEIGHT          512
#define MIN_DST_HEIGHT          2

#define ORAM_SIZE               (896 * 1024)
#define MIN(a, b)               (a < b ? a : b)
uint32_t aligned2(uint32_t a)
{
    return (a & 0xFFFFFFFE);
}

void set_run_mode(bs_run_mode_e mode, bs_api_s *cfg)
{
    bs_run_mode_e modes[3] = {RSZ, AFFINE, PERSP};
    bs_run_mode_e random_mode = modes[rand() % 3];

    if (mode == RANDOM) {
        cfg->mode = random_mode;
    } else {
        cfg->mode = mode;
    }

    bs_data_format_e rsz_src_format[] = {BS_DATA_NV12, BS_DATA_BGRA,
                                         BS_DATA_FMU2, BS_DATA_FMU4,
                                         BS_DATA_FMU8};
    bs_data_format_e nv2bgr_format[12] = {BS_DATA_BGRA, BS_DATA_GBRA,
                                          BS_DATA_RBGA, BS_DATA_BRGA,
                                          BS_DATA_GRBA, BS_DATA_RGBA,
                                          BS_DATA_ABGR, BS_DATA_AGBR,
                                          BS_DATA_ARBG, BS_DATA_ABRG,
                                          BS_DATA_AGRB, BS_DATA_ARGB};
    bs_data_format_e affine_format[] = {BS_DATA_NV12, BS_DATA_BGRA,
                                        BS_DATA_GBRA, BS_DATA_RBGA,
                                        BS_DATA_BRGA, BS_DATA_GRBA,
                                        BS_DATA_RGBA, BS_DATA_ABGR,
                                        BS_DATA_AGBR, BS_DATA_ARBG,
                                        BS_DATA_ABRG, BS_DATA_AGRB,
                                        BS_DATA_ARGB};
    switch (cfg->mode) {
    case RSZ://check me
        cfg->src_format = rsz_src_format[rand() % 5];
        if (cfg->src_format == BS_DATA_NV12) {
            cfg->dst_format = nv2bgr_format[rand() % 12];
        } else {
            cfg->dst_format = cfg->src_format;
        }
        break;
    case AFFINE:
        cfg->src_format = affine_format[rand() % 13];
        if (cfg->src_format == BS_DATA_NV12) {
            cfg->dst_format = affine_format[rand() % 13];
        } else {
            cfg->dst_format = cfg->src_format;
        }
        break;
    case PERSP:
        cfg->src_format = affine_format[rand() % 13];
        if (cfg->src_format == BS_DATA_NV12) {
            cfg->dst_format = affine_format[rand() % 13];
        } else {
            cfg->dst_format = cfg->src_format;
        }
        break;
    default:
        break;
    }
    //delete me
    //cfg->src_format = BS_DATA_FMU8;
    //cfg->dst_format = BS_DATA_FMU8;
}

void bsc_random_stride(bs_api_s *cfg)
{
    if (cfg->src_format == BS_DATA_NV12) {
        cfg->src_line_stride = cfg->src_w;// + rand() % 8;
    } else if (cfg->src_format == BS_DATA_FMU2) {
        cfg->src_line_stride = cfg->src_w * 8;// + rand() % 8;
    } else if (cfg->src_format == BS_DATA_FMU4) {
        cfg->src_line_stride = cfg->src_w * 16;// + rand() % 8;
    } else if (cfg->src_format == BS_DATA_FMU8) {
        cfg->src_line_stride = cfg->src_w * 32;// + rand() % 8;
    } else {
        cfg->src_line_stride = cfg->src_w * 4;// + rand() % 8;
    }

    if (cfg->dst_format == BS_DATA_NV12) {
        cfg->dst_line_stride = cfg->dst_w;// + rand() % 8;
    } else if (cfg->dst_format == BS_DATA_FMU2) {
        cfg->dst_line_stride = cfg->dst_w * 8;// + rand() % 8;
    } else if (cfg->dst_format == BS_DATA_FMU4) {
        cfg->dst_line_stride = cfg->dst_w * 16;// + rand() % 8;
    } else if (cfg->dst_format == BS_DATA_FMU8) {
        cfg->dst_line_stride = cfg->dst_w * 32;// + rand() % 8;
    } else {
        cfg->dst_line_stride = cfg->dst_w * 4;// + rand() % 8;
    }
}

static void resize_random(bs_api_s *cfg)
{
    uint32_t dst_w_max = MAX_DST_WIDTH;
    uint32_t dst_h_max = MAX_DST_HEIGHT;
    uint32_t dst_w_min = MIN_DST_WIDTH;
    uint32_t dst_h_min = MIN_DST_HEIGHT;
    cfg->dst_h = aligned2(dst_h_min + rand() % (dst_h_max - dst_h_min));
    cfg->dst_w = aligned2(dst_w_min + rand() % (dst_w_max - dst_w_min));

    float scale_x = rand() % 3 ? rand() % 4 + 1 : rand() % 32 + 1;
    float scale_y = rand() % 3 ? rand() % 4 + 1 : rand() % 32 + 1;
    scale_x = rand()%2 ? scale_x : 1 / scale_x;
    scale_y = rand()%2 ? scale_y : 1 / scale_y;

    uint32_t src_bpp_mode = (cfg->src_format >> 5) & 0x3;
    uint32_t src_w_max = MAX_SRC_WIDTH;
    uint32_t src_h_max = MAX_SRC_HEIGHT;
    uint32_t src_w_min = MIN_SRC_WIDTH;
    uint32_t src_h_min = MIN_SRC_HEIGHT;
    uint32_t src_w = (uint32_t)(cfg->dst_w / scale_x);
    uint32_t src_h = (uint32_t)(cfg->dst_h / scale_y);
    src_w = (src_w < 2) ? src_w_min : src_w;
    src_h = (src_h < 2) ? src_h_min : src_h;

    if (src_bpp_mode == 0) { //RGB or NV12
        cfg->src_w = aligned2(MIN(src_w, MIN(src_w_max, 2048)));
    } else if (src_bpp_mode == 1) { //FMU2
        cfg->src_w = aligned2(MIN(src_w, MIN(src_w_max, 1024)));
    } else if (src_bpp_mode == 2) { //FMU4
        cfg->src_w = aligned2(MIN(src_w, MIN(src_w_max, 512)));
    } else if (src_bpp_mode == 3) { //FMU8
        cfg->src_w = aligned2(MIN(src_w, MIN(src_w_max, 256)));
    } else {
        cfg->src_w = aligned2(MIN(src_w, MIN(src_w_max, 256)));
    }

    if (src_bpp_mode == 0) { //RGB or NV12
        cfg->src_h = aligned2(MIN(src_h, MIN(src_h_max, 2048)));
    } else if (src_bpp_mode == 1) { //FMU2
        cfg->src_h = aligned2(MIN(src_h, MIN(src_h_max, 1024)));
    } else if (src_bpp_mode == 2) { //FMU4
        cfg->src_h = aligned2(MIN(src_h, MIN(src_h_max, 512)));
    } else if (src_bpp_mode == 3) { //FMU8
        cfg->src_h = aligned2(MIN(src_h, MIN(src_h_max, 256)));
    } else {
        cfg->src_h = aligned2(MIN(src_h, MIN(src_h_max, 256)));
    }

    assert(cfg->src_w >= 2);
    assert(cfg->src_h >= 2);
    assert(cfg->dst_w >= 2);
    assert(cfg->dst_h >= 2);
    assert(cfg->src_w % 2 == 0);
    assert(cfg->src_h % 2 == 0);
    assert(cfg->dst_w % 2 == 0);
    assert(cfg->dst_h % 2 == 0);
    assert(cfg->src_w <= src_w_max);
    assert(cfg->src_h <= src_h_max);
    assert(cfg->dst_w <= dst_w_max);
    assert(cfg->dst_h <= dst_h_max);

    float update_scale_x = ((float)(cfg->src_w) / (float)(cfg->dst_w));
    float update_scale_y = ((float)(cfg->src_h) / (float)(cfg->dst_h));
    float trans_x = (float)(rand() % 32);
    float trans_y = (float)(rand() % 32);
    cfg->matrix[0] = update_scale_x;
    cfg->matrix[1] = 0;
    cfg->matrix[2] = trans_x;
    cfg->matrix[3] = 0;
    cfg->matrix[4] = update_scale_y;
    cfg->matrix[5] = trans_y;
    cfg->matrix[6] = 0;
    cfg->matrix[7] = 0;
    cfg->matrix[8] = 0;

    // random stride
    bsc_random_stride(cfg);
}

static void affine_random(bs_api_s *cfg)
{
    uint32_t dst_w_max = MAX_DST_WIDTH;
    uint32_t dst_h_max = MAX_DST_HEIGHT;
    uint32_t dst_w_min = MIN_DST_WIDTH;
    uint32_t dst_h_min = MIN_DST_HEIGHT;
    cfg->dst_h = aligned2(dst_h_min + rand() % (dst_h_max - dst_h_min));
    cfg->dst_w = aligned2(dst_w_min + rand() % (dst_w_max - dst_w_min));

    float scale_x = rand() % 3 ? rand() % 4 + 1 : rand() % 32 + 1;
    float scale_y = rand() % 3 ? rand() % 4 + 1 : rand() % 32 + 1;
    scale_x = rand()%2 ? scale_x : 1 / scale_x;
    scale_y = rand()%2 ? scale_y : 1 / scale_y;

    uint32_t src_bpp_mode = (cfg->src_format >> 5) & 0x3;
    uint32_t src_w_max = MAX_SRC_WIDTH;
    uint32_t src_h_max = MAX_SRC_HEIGHT;
    uint32_t src_w_min = MIN_SRC_WIDTH;
    uint32_t src_h_min = MIN_SRC_HEIGHT;
    uint32_t src_w = (uint32_t)(cfg->dst_w / scale_x);
    uint32_t src_h = (uint32_t)(cfg->dst_h / scale_y);
    src_w = aligned2(MIN(src_w, src_w_max));
    src_h = aligned2(MIN(src_h, src_h_max));
    cfg->src_w = aligned2((src_w < 2) ? src_w_min : src_w);
    cfg->src_h = aligned2((src_h < 2) ? src_h_min : src_h);
    assert(cfg->src_w >= 2);
    assert(cfg->src_h >= 2);
    assert(cfg->dst_w >= 2);
    assert(cfg->dst_h >= 2);
    assert(cfg->src_w % 2 == 0);
    assert(cfg->src_h % 2 == 0);
    assert(cfg->dst_w % 2 == 0);
    assert(cfg->dst_h % 2 == 0);
    assert(cfg->src_w <= src_w_max);
    assert(cfg->src_h <= src_h_max);
    assert(cfg->dst_w <= dst_w_max);
    assert(cfg->dst_h <= dst_h_max);

    CV::Matrix trans;
    float angle = rand() % 360;
    trans.setRotate(angle, (float)(cfg->src_w - 1)/2, (float)(cfg->src_h - 1)/2);
    trans.postScale((float)(cfg->dst_w) / (float)(cfg->src_w),
                    (float)(cfg->dst_h) / (float)(cfg->src_h));
    trans.get9(cfg->matrix);

    bsc_random_stride(cfg);
}

void perspective_random(bs_api_s *cfg)
{
    uint32_t dst_w_max = MAX_DST_WIDTH;
    uint32_t dst_h_max = MAX_DST_HEIGHT;
    uint32_t dst_w_min = MIN_DST_WIDTH;
    uint32_t dst_h_min = MIN_DST_HEIGHT;
    cfg->dst_h = aligned2(dst_h_min + rand() % (dst_h_max - dst_h_min));
    cfg->dst_w = aligned2(dst_w_min + rand() % (dst_w_max - dst_w_min));

    float scale_x = rand() % 3 ? rand() % 4 + 1 : rand() % 32 + 1;
    float scale_y = rand() % 3 ? rand() % 4 + 1 : rand() % 32 + 1;
    scale_x = rand()%2 ? scale_x : 1 / scale_x;
    scale_y = rand()%2 ? scale_y : 1 / scale_y;

    uint32_t src_bpp_mode = (cfg->src_format >> 5) & 0x3;
    uint32_t src_w_max = MAX_SRC_WIDTH;
    uint32_t src_h_max = MAX_SRC_HEIGHT;
    uint32_t src_w_min = MIN_SRC_WIDTH;
    uint32_t src_h_min = MIN_SRC_HEIGHT;
    uint32_t src_w = (uint32_t)(cfg->dst_w / scale_x);
    uint32_t src_h = (uint32_t)(cfg->dst_h / scale_y);
    src_w = aligned2(MIN(src_w, src_w_max));
    src_h = aligned2(MIN(src_h, src_h_max));
    cfg->src_w = aligned2((src_w < 2) ? src_w_min : src_w);
    cfg->src_h = aligned2((src_h < 2) ? src_h_min : src_h);
    assert(cfg->src_w >= 2);
    assert(cfg->src_h >= 2);
    assert(cfg->dst_w >= 2);
    assert(cfg->dst_h >= 2);
    assert(cfg->src_w % 2 == 0);
    assert(cfg->src_h % 2 == 0);
    assert(cfg->dst_w % 2 == 0);
    assert(cfg->dst_h % 2 == 0);
    assert(cfg->src_w <= src_w_max);
    assert(cfg->src_h <= src_h_max);
    assert(cfg->dst_w <= dst_w_max);
    assert(cfg->dst_h <= dst_h_max);

    CV::Matrix trans;
    float angle = rand() % 360;
    trans.setRotate(angle, (float)(cfg->src_w - 1)/2, (float)(cfg->src_h - 1)/2);
    trans.postScale((float)(cfg->dst_w) / (float)(cfg->src_w),
                    (float)(cfg->dst_h) / (float)(cfg->src_h));
    trans.get9(cfg->matrix);
    float persp0 = 1.0f / ((rand() % 1000) + 1);
    float persp1 = 1.0f / ((rand() % 1000) + 1);
    cfg->matrix[6] = rand() % 2 ? persp0 : -persp0;
    cfg->matrix[7] = rand() % 2 ? persp1 : -persp1;
    cfg->matrix[8] = 1.0f;
    //delete me
    //cfg->src_w = cfg->dst_w = 256;
    //cfg->src_h = cfg->dst_h = 256;
    //cfg->matrix[0] = cfg->matrix[4] = cfg->matrix[8] = 1.0f;
    //cfg->matrix[1] = cfg->matrix[2] = cfg->matrix[3] = cfg->matrix[5] = cfg->matrix[6] = cfg->matrix[7] = 0.0f;

    bsc_random_stride(cfg);
}

int get_src_buffer_size(bs_api_s *cfg)
{
    if (cfg->src_format == BS_DATA_NV12) {
        return (cfg->src_line_stride * cfg->src_h * 3 / 2);
    } else {
        return (cfg->src_line_stride * cfg->src_h);
    }
}

int get_dst_buffer_size(bs_api_s *cfg)
{
    if (cfg->dst_format == BS_DATA_NV12) {
        return (cfg->dst_line_stride * cfg->dst_h * 3 / 2);
    } else {
        return (cfg->dst_line_stride * cfg->dst_h);
    }
}

void matrix_random(bs_api_s *cfg)
{
    assert(MAX_SRC_WIDTH > MIN_SRC_WIDTH);
    assert(MAX_SRC_HEIGHT > MIN_SRC_HEIGHT);
    assert(MAX_DST_WIDTH > MIN_DST_WIDTH);
    assert(MAX_DST_HEIGHT > MIN_DST_HEIGHT);

    assert(MIN_SRC_WIDTH >= 2);
    assert(MIN_SRC_HEIGHT >= 2);
    assert(MIN_DST_WIDTH >= 2);
    assert(MIN_DST_HEIGHT >= 2);

    if (cfg->mode == RSZ) {
        resize_random(cfg);
    } else if (cfg->mode == AFFINE) {
        affine_random(cfg);
    } else if (cfg->mode == PERSP) {
        perspective_random(cfg);
    } else {
        //TODO
        //printf("not support yet\n");
        cfg->mode = PERSP;
        perspective_random(cfg);
    }

    int src_buf_size = get_src_buffer_size(cfg);
    int dst_buf_size = get_dst_buffer_size(cfg);

    if (src_buf_size > ORAM_SIZE) {
        cfg->src_locate = BS_DATA_NMEM;//ddr
    }

    if (dst_buf_size > ORAM_SIZE) {
        cfg->dst_locate = BS_DATA_NMEM;//ddr
    }

    if ((src_buf_size < ORAM_SIZE) && (dst_buf_size < ORAM_SIZE)) {
        if ((src_buf_size + dst_buf_size + 512) < ORAM_SIZE) {
            cfg->src_locate = BS_DATA_ORAM;//oram
            cfg->dst_locate = BS_DATA_ORAM;//oram
        } else {
            cfg->src_locate = rand() % 2 ? BS_DATA_ORAM : BS_DATA_NMEM;
            cfg->dst_locate = cfg->src_locate == BS_DATA_ORAM ? BS_DATA_NMEM : BS_DATA_ORAM;
        }
    } else if ((src_buf_size < ORAM_SIZE) && (dst_buf_size > ORAM_SIZE)) {
        cfg->src_locate = BS_DATA_ORAM;//oram
        cfg->dst_locate = BS_DATA_NMEM;//ddr
    } else if ((src_buf_size > ORAM_SIZE) && (dst_buf_size < ORAM_SIZE)) {
        cfg->src_locate = BS_DATA_NMEM;//ddr
        cfg->dst_locate = BS_DATA_ORAM;//oram
    }
}

void random_init_space(void *ptr, int size)
{
    uint32_t *word_ptr = (uint32_t *)ptr;
    for (int i = 0; i < size / 4; i++) {
        word_ptr[i] = rand();
        //delete me
        //word_ptr[i] = 0xFFFFFFFF;
    }
}

#define format_to_string(format)                    \
    (format == BS_DATA_NV12 ? "NV12" :              \
     format == BS_DATA_BGRA ? "BGRA" :              \
     format == BS_DATA_GBRA ? "GBRA" :              \
     format == BS_DATA_RBGA ? "RBGA" :              \
     format == BS_DATA_BRGA ? "BRGA" :              \
     format == BS_DATA_GRBA ? "GRBA" :              \
     format == BS_DATA_RGBA ? "RGBA" :              \
     format == BS_DATA_ABGR ? "ABGR" :              \
     format == BS_DATA_AGBR ? "AGBR" :              \
     format == BS_DATA_ARBG ? "ARBG" :              \
     format == BS_DATA_ABRG ? "ABRG" :              \
     format == BS_DATA_AGRB ? "AGRB" :              \
     format == BS_DATA_ARGB ? "ARGB" :              \
     format == BS_DATA_FMU2 ? "FMU2" :              \
     format == BS_DATA_FMU4 ? "FMU4" :              \
     format == BS_DATA_FMU8 ? "FMU8" :              \
     format == BS_DATA_VBGR ? "VBGR" :              \
     format == BS_DATA_VRGB ? "VRGB" : "unknown")

void show_bs_api(bs_api_s *cfg)
{
    printf("mode : %s\n",
           cfg->mode == RSZ ? "RSZ":
           cfg->mode == AFFINE ? "AFFINE":
           cfg->mode == PERSP ? "PERSP": "unknown");
    printf("src_format : %s\n", format_to_string(cfg->src_format));
    printf("src_w      : %d\n", cfg->src_w);
    printf("src_h      : %d\n", cfg->src_h);
    printf("src_base   : %p\n", cfg->src_base);
    printf("src_line_stride : %d\n", cfg->src_line_stride);
    printf("src_locate   : %s\n", cfg->src_locate == BS_DATA_ORAM ? "ORAM" :
           cfg->src_locate == BS_DATA_NMEM ? "NMEM":"RMEM");
    printf("dst_format : %s\n", format_to_string(cfg->dst_format));
    printf("dst_w      : %d\n", cfg->dst_w);
    printf("dst_h      : %d\n", cfg->dst_h);
    printf("dst_base   : %p\n", cfg->dst_base);
    printf("dst_line_stride : %d\n", cfg->dst_line_stride);
    printf("dst_locate   : %s\n", cfg->dst_locate == BS_DATA_ORAM ? "ORAM" :
           cfg->dst_locate == BS_DATA_NMEM ? "NMEM":"RMEM");
    printf(" Matrix:\n");
    for (int i = 0; i < 9; i++) {
        if ((cfg->mode == RSZ) && ((i == 2) || (i == 5))) {
            printf("x.x, ");
        } else {
            printf("%f, ", cfg->matrix[i]);
        }
    }
    printf("\n");
}

void bs_api_recoder(bs_api_s *cfg)
{
#if 0
    time_t tt = time(NULL);
    tm* t = localtime(&tt);
    printf("%d-%02d-%02d %02d:%02d:%02d\n",
           t->tm_year + 1900,
           t->tm_mon + 1,
           t->tm_mday,
           t->tm_hour,
           t->tm_min,
           t->tm_sec);
#endif

    FILE *fp = fopen(".recoder", "a+");
    if (fp == NULL) {
        fprintf(stderr, "recoder random parameter failed!\n");
    }

    fprintf(fp, "{\"mode\":\"%s\", ",
            cfg->mode == RSZ ? "RSZ":
            cfg->mode == AFFINE ? "AFFINE":
            cfg->mode == PERSP ? "PERSP": "unknown");
    fprintf(fp, "\"src_format\":\"%s\", ", format_to_string(cfg->src_format));
    fprintf(fp, "\"src_w\":%d, ", cfg->src_w);
    fprintf(fp, "\"src_h\":%d, ", cfg->src_h);
    fprintf(fp, "\"src_line_stride\":%d, ", cfg->src_line_stride);
    fprintf(fp, "\"dst_format\":\"%s\", ", format_to_string(cfg->dst_format));
    fprintf(fp, "\"dst_w\":%d, ", cfg->dst_w);
    fprintf(fp, "\"dst_h\":%d, ", cfg->dst_h);
    fprintf(fp, "\"dst_line_stride\":%d, ", cfg->dst_line_stride);

    fprintf(fp, "\"matirx\":[");
    for (int i = 0; i < 9; i++) {
        if (i < 8)
            fprintf(fp, "%f, ", cfg->matrix[i]);
        else
            fprintf(fp, "%f", cfg->matrix[i]);
    }
    fprintf(fp, "]");
    fprintf(fp, "}\n");
    fclose(fp);
}

int data_check(data_info_s *gld, data_info_s *dut)
{
    int errnum = 0;
    int vague_diff = 1;
    uint8_t *gld_ptr = (uint8_t *)gld->base;
    uint8_t *dut_ptr = (uint8_t *)dut->base;
    int dst_bpp_mode = (gld->format >> 5) & 0x3;
    int bpp;
    if (gld->format == 0) {
        bpp = 1;
    } else {
        bpp = 1 << (2 + dst_bpp_mode);
    }

    for (int i = 0; i < gld->height; i++) {
        for (int j = 0; j < gld->width; j++) {
            for (int k = 0; k < bpp; k++) {
                uint8_t g_val = gld_ptr[i * gld->line_stride + j * bpp + k];
                uint8_t d_val = dut_ptr[i * dut->line_stride + j * bpp + k];
                int diff = g_val - d_val;
                if (abs(diff) > vague_diff) {
                    if (errnum < 3) {
                        printf("[Error] (%d,%d,%d) : (G)0x%x -- (E)0x%x\n",
                               j, i, k, g_val, d_val);
                    }
                    errnum++;
                }
                if (gld->format == 0) {
                    int gld_idx = gld->line_stride * gld->height +
                        i/2 * gld->line_stride + j;
                    int dut_idx = dut->line_stride * gld->height +
                        i/2 * dut->line_stride + j;
                    uint8_t g_val = gld_ptr[gld_idx];
                    uint8_t d_val = dut_ptr[dut_idx];
                    int diff = g_val - d_val;
                    if (abs(diff) > vague_diff) {
                        if (errnum < 3) {
                            printf("[Error:UV] (%d,%d,%d) : (G)0x%x -- (E)0x%x\n",
                                   j, i, k, g_val, d_val);
                        }
                        errnum++;
                    }
                }
            }
        }
    }
    return errnum;
}

uint32_t bscaler_random(uint32_t min, uint32_t max, uint32_t align)
{
    if ((min%align) != 0 || (max%align) != 0)
        fprintf(stderr, "error: random error! min = %0d, max = %0d, align = %0d\n", min, max, align);
    int offset = align - 1;
    uint32_t random_val = ((rand()%(max - min + 1) + min) + offset) & (~(align - 1));
    if (random_val < min)
        random_val = min;
    else if (random_val > max)
        random_val = max;
    return random_val;
}

void bst_random_api(data_info_s *src, data_info_s *dst, task_info_s *info)
{
    //bs_data_format_e src_format[] = {BS_DATA_NV12, BS_DATA_BGRA,
    //                                 BS_DATA_FMU2, BS_DATA_FMU4,
    //                                 BS_DATA_FMU8};

    bs_data_format_e src_format[] = {BS_DATA_NV12, BS_DATA_BGRA};//fixme
    bs_data_format_e dst_format[] = {BS_DATA_VBGR, BS_DATA_VRGB,
                                     BS_DATA_BGRA,
                                     BS_DATA_FMU2, BS_DATA_FMU4,
                                     BS_DATA_FMU8, };

    //src->format = src_format[rand() % 5];
    src->format = src_format[rand() % 2];
    if (src->format == BS_DATA_NV12) {
        dst->format = dst_format[rand() % 3];
    } else if (src->format == BS_DATA_BGRA) {
        dst->format = dst_format[rand() % 3];
    } else if ((src->format == BS_DATA_FMU2) ||
               (src->format == BS_DATA_FMU4) ||
               (src->format == BS_DATA_FMU8)) {
        dst->format = src->format;
    } else {
        assert(0);
    }

    //delete me
    //src->format = BS_DATA_BGRA;
    //dst->format = BS_DATA_BGRA;

    src->locate = BS_DATA_NMEM;
    int ksizes[4] = {1, 3, 5, 7};
    int ksize = ksizes[rand()%4];
    if ((dst->format == BS_DATA_VBGR) || (dst->format == BS_DATA_VRGB)) {
        if (ksize == 1) {
            src->width = bscaler_random(8, 4096/16, 2);
            src->height = bscaler_random(8, 1024/16, 2);
        } else if (ksize == 3) {
            src->width = bscaler_random(8, 2048/16, 2);
            src->height = bscaler_random(8, 1024/16, 2);
        } else if (ksize == 5) {
            src->width = bscaler_random(8, 1024/16, 2);
            src->height = bscaler_random(8, 1024/16, 2);
        } else if (ksize == 7) {
            src->width = bscaler_random(8, 512/16, 2);
            src->height = bscaler_random(8, 1024/16, 2);
        } else {
            assert(0);
        }
    } else {
        src->width = bscaler_random(2, 1024/16, 2);
        src->height = bscaler_random(2, 1024/16, 2);
    }

    src->line_stride = (src->format == BS_DATA_NV12) ? src->width : src->width * 4;
    src->chn = (src->format == BS_DATA_NV12) ? 1 : 4;

    info->task_len = rand() % 5 + 1;
    info->zero_point = rand();

    dst->locate = BS_DATA_NMEM;
    dst->width = src->width;
    dst->height = src->height;

    dst->line_stride = (dst->format == BS_DATA_NV12) ? src->line_stride :
        src->line_stride * 4;
    dst->chn = (dst->format == BS_DATA_NV12) ? 1 : 4;//fixme
    uint32_t dst_plane_stride = info->task_len * dst->line_stride;
    info->plane_stride = dst_plane_stride;//not same
#if 1
    printf("src_format : %s\n", format_to_string(src->format));
    printf("src_w : %d\n", src->width);
    printf("src_h : %d\n", src->height);
    printf("src_line_stride : %d\n", src->line_stride);

    printf("dst_format : %s\n", format_to_string(dst->format));
    printf("dst_w : %d\n", dst->width);
    printf("dst_h : %d\n", dst->height);
    printf("dst_line_stride : %d\n", dst->line_stride);

    printf("zero_point : 0x%08x\n", info->zero_point);
    printf("task_len : 0x%08x\n", info->task_len);
#else
    printf("%s -> %s\n", format_to_string(src->format), format_to_string(dst->format));
#endif
}
