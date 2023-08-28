/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : gen_real_param.cpp
 * Authors    : jmqi@ingenic.st.jz.com
 * Create Time: 2020-08-28:20:45:04
 * Description:
 *
 */

#include <string>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#include "image_process.h"
#include "bscaler_hal.h"
#include "bscaler_api.h"
#include "bscaler_mdl.h"
#include "random_api.h"

void dump_random_cfg(bs_api_s *cfg, int seed, const char *file)
{
    FILE *fpo;
    fpo = fopen(file, "w+");
    if (fpo == NULL) {
        fprintf(stderr, "Open %s failed!\n", file);
        exit(1);
    }
    fwrite(&cfg->mode, 4, 1, fpo);
    fwrite(&cfg->src_format, 4, 1, fpo);
    fwrite(&cfg->src_w, 4, 1, fpo);
    fwrite(&cfg->src_h, 4, 1, fpo);
    fwrite(&cfg->src_line_stride, 4, 1, fpo);
    fwrite(&cfg->src_locate, 4, 1, fpo);
    int src_size = get_src_buffer_size(cfg);
    fwrite(&src_size, 4, 1, fpo);
    fwrite(cfg->src_base, 1, src_size, fpo);
    fwrite(&cfg->dst_format, 4, 1, fpo);
    fwrite(&cfg->dst_w, 4, 1, fpo);
    fwrite(&cfg->dst_h, 4, 1, fpo);
    fwrite(&cfg->dst_line_stride, 4, 1, fpo);
    fwrite(&cfg->dst_locate, 4, 1, fpo);
    fwrite(&cfg->matrix, 4, 9, fpo);
    uint32_t bsc_isum = get_bsc_isum();
    uint32_t bsc_osum = get_bsc_osum();
    fwrite(&bsc_isum, 4, 1, fpo);
    fwrite(&bsc_osum, 4, 1, fpo);
#if 1 //dump dst data
    int dst_size = get_dst_buffer_size(cfg);
    fwrite(&dst_size, 4, 1, fpo);
    fwrite(cfg->dst_base, 1, dst_size, fpo);
#endif
    fwrite(&seed, 4, 1, fpo);
    time_t a = time(NULL);
    char *date = ctime(&a);
    fwrite(date, 1, strlen(date), fpo);
}
#define DUMP_IMAGE              0
#define THREADS_NUM             (16)
static inline float fclip(float v, float minV, float maxV)
{
    return v > maxV ? maxV : v < minV ? minV : v;
}

size_t get_file_size(const char* filename)
{
    struct stat statbuf;
    stat(filename, &statbuf);
    size_t size = statbuf.st_size;
    return size;
}

size_t load_file(void *buffer, const char *filename)
{
    FILE *fp;
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "[Error][%s]: Open %s failed!\n", __func__, filename);
        exit(1);
    }
    size_t size = get_file_size(filename);
    size_t ret_size = fread(buffer, 1, size, fp);

    if (ret_size != size) {
        fprintf(stderr, "[Error][%s]: fread failed!\n", __func__);
        exit(1);
    }

    fclose(fp);
    return ret_size;
}

static void bilinear(const uint8_t *src, int istride, int ibpp, int iw, int ih,
                     uint8_t *dst, int ostride, int obpp, int ow, int oh)
{
    float invert_scale_x = (float)iw / (float)ow;
    float invert_scale_y = (float)ih / (float)oh;
    for (int oh_idx = 0; oh_idx < oh; oh_idx++) {
        float y = fclip(oh_idx * invert_scale_y, 0, (float)ih);
        int y0 = (int)y;
        int y1 = (int)ceilf(y);
        float y_w = y - (float)y0;
        if (y1 > ih - 1) {
            y1 = y0;
        }

        for (int ow_idx = 0; ow_idx < ow; ow_idx++) {
            float x = fclip(ow_idx * invert_scale_x, 0, (float)iw);
            int x0 = (int)x;
            int x1 = (int)ceilf(x);
            if (x1 > iw - 1) {
                x1 = x0;
            }

            float x_w = x - (float)x0;
            for (int bpp_idx = 0; bpp_idx < obpp; bpp_idx++) {
                uint8_t c00 = src[y0 * istride + ibpp * x0 + bpp_idx];
                uint8_t c01 = src[y0 * istride + ibpp * x1 + bpp_idx];
                uint8_t c10 = src[y1 * istride + ibpp * x0 + bpp_idx];
                uint8_t c11 = src[y1 * istride + ibpp * x0 + bpp_idx];
                float val = ((1.0f - x_w) * (1.0f - y_w) * c00 +
                             x_w * (1.0f - y_w) * c01 +
                             y_w * (1.0 - x_w) * c10 +
                             x_w * y_w * (c11));
                val = MIN(MAX(val, 0.0f), 255.0f);
                dst[oh_idx * ostride + ow_idx * obpp + bpp_idx] = (uint8_t)val;
            }
        }
    }
}

int main(int argc, char** argv)
{
    const uint32_t coef[9] = {1220542, 1673527, 0,
                              1220542, 409993, 852492,
                              1220542, 2116026, 0};
    const uint32_t offset[2] = {16, 128};

    //1. check command line
    if (argc < 2) {
        fprintf(stderr, "[Error] : command line error!\n");
        fprintf(stderr, "such as:\n");
        fprintf(stderr, "%s [param_file] [start_pos]\n", argv[0]);
        fprintf(stderr, "%s [param_file] [start_pos] [num]\n", argv[0]);
        fprintf(stderr, "%s [param_file] [start_pos] [num] [input.jpg]\n", argv[0]);
        fprintf(stderr, "%s ../image/perspective_matrix_64x64.data 0 1 ../image/chess.bmp\n", argv[0]);
        exit(1);
    }

    //2. read input image
    int img_w, img_h, img_chn;
    const char *input_image = "../image/random.png";
    if (argc > 4) {
        input_image = argv[4];
    }
    uint8_t *img = stbi_load(input_image, &img_w, &img_h, &img_chn, 4);
    if (img == NULL) {
        printf("load input image failed!\n");
        exit(1);
    }

    // 3. load perspective param file
    int param_file_size = get_file_size(argv[1]);
    uint32_t *param_ptr = (uint32_t *)malloc(param_file_size);
    if (param_ptr == NULL) {
        fprintf(stderr, "malloc space for param error!\n");
        exit(1);
    }
    load_file(param_ptr, argv[1]);

    // 4. parsing parameters for start position and run number
    int num = param_file_size / 44;
    int idx_st = 0;
    int idx_end = num;
    if (argc > 2) {
        idx_st = atoi(argv[2]);
    }
    if (argc > 3) {
        idx_end = idx_st + atoi(argv[3]);
    }

    // 6. Run a specified number of floating-point and integer models
    const uint8_t zero_point = 0x0;
    const int box_num = 1;

    for (int idx = idx_st; idx < idx_end; idx++) {
        float *ptr = (float *)(param_ptr + idx * 11);
        int src_w = ((float*)(ptr))[0];
        int src_h = ((float*)(ptr))[1];
        float *matrix = (float *)(&(ptr[2]));
#if 0
        int dst_w = src_w;
        int dst_h = src_h;
#else
        int dst_w = 64;
        int dst_h = 64;
#endif
        std::string bin_name = "128x128_" + std::to_string((int)idx) + ".bin";
        int src_line_stride = src_w * 4;
        int dst_line_stride = dst_w * 4;
        data_info_s src = {NULL, NULL, BS_DATA_BGRA, 4, src_w, src_h, src_line_stride, 0};
        data_info_s dst = {NULL, NULL, BS_DATA_BGRA, 4, dst_w, dst_h, dst_line_stride, 0};
        box_affine_info_s info = { {0, 0, src_w, src_h},
                                   {matrix[0], matrix[1], matrix[2],
                                    matrix[3], matrix[4], matrix[5],
                                    matrix[6], matrix[7], matrix[8]}, 0, zero_point};
        src.base = (uint8_t*)malloc(src_w * 4 * src_h);
        if (src.base == NULL) {
            fprintf(stderr, "malloc space for src error!\n");
        }

        dst.base = (uint8_t*)malloc(dst_w * 4 * dst_h);
        if (dst.base == NULL) {
            fprintf(stderr, "malloc space for dst error!\n");
            exit(1);
        }

        // resize image for perspective input
        bilinear(img, img_w * 4, 4, img_w, img_h,
                 (uint8_t*)src.base, src_w * 4, 4, src_w, src_h);
        // dst integer perspective
        bs_perspective_mdl(&src, box_num, &dst, &info, coef, offset);
        bs_api_s cfg;
        cfg.mode = PERSP;
        cfg.src_format = src.format;
        cfg.src_w = src.width;
        cfg.src_h = src.height;
        cfg.src_line_stride = src.line_stride;
        cfg.src_locate = src.locate;
        cfg.src_base = (uint8_t *)src.base;
        cfg.dst_format = dst.format;
        cfg.dst_w = dst.width;
        cfg.dst_h = dst.height;
        cfg.dst_line_stride = dst.line_stride;
        cfg.dst_locate = dst.locate;
        cfg.dst_base = (uint8_t *)dst.base;
        memcpy(cfg.matrix, matrix, sizeof(float) * 9);

        dump_random_cfg(&cfg, 0, bin_name.c_str());

        free(dst.base);
        free(src.base);
    }

    free(param_ptr);
    return 0;
}
