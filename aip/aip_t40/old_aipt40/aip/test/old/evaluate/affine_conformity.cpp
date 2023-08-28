/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : perspective_conformity.cpp
 * Authors    : jmqi@taurus
 * Create Time: 2020-05-20:20:21:23
 * Description: test perspective golden and dut (model)
 *
 */

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include "affine_whole_float.h"
#include "affine_whole_int.h"

#include "image_process.h"
#include "bscaler_hw_api.h"

#define DUMP_IMAGE              0

#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a > b ? b : a)

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
#ifdef EYER_SIM_ENV
    eyer_system_ini(0);// eyer environment initialize.
    eyer_reg_segv();   // simulation error used
    eyer_reg_ctrlc();  // simulation error used
#elif CSE_SIM_ENV
    bscaler_mem_init();
    int ret = 0;
    if ((ret = __aie_mmap(0x4000000)) == 0) { //64MB
        printf ("nna box_base virtual generate failed !!\n");
        return 0;
    }
#endif

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
        fprintf(stderr, "%s ../image/affine_matrix_64x64.data 0 1 ../image/chess.bmp\n", argv[0]);
        exit(1);
    }

    //2. read input image
    int img_w, img_h, img_chn;
#ifdef CSE_SIM_ENV
    const char *input_image = "random.png";
#else
    const char *input_image = "../image/random.png";
#endif
    if (argc > 4) {
        input_image = argv[4];
    }
    uint8_t *img = stbi_load(input_image, &img_w, &img_h, &img_chn, 4);
    if (img == NULL) {
        printf("load input image failed!\n");
        exit(1);
    }

    // 3. load affine param file
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

    // 5. statistics difference of output images
    double *diffs = (double *)malloc(sizeof(double) * num);
    if (diffs == NULL) {
        fprintf(stderr, "malloc space for param error!\n");
        exit(1);
    }
    memset(diffs, 0, sizeof(double) * num);

    // 6. Run a specified number of floating-point and integer models
    const uint8_t zero_point = 0;
    const int box_num = 1;

    for (int idx = idx_st; idx < idx_end; idx++) {
        printf("####### idx = %d ##############\n", idx);
#if DUMP_IMAGE
        std::string src_base = "src";
        std::string src_name = src_base + std::to_string((double)idx) + ".png";
        std::string dut_base = "dut";
        std::string dut_name = dut_base + std::to_string((double)idx) + ".png";
        std::string gld_base = "gld";
        std::string gld_name = gld_base + std::to_string((double)idx) + ".png";
#endif
        float *ptr = (float *)(param_ptr + idx * 11);
        int src_w = ((float*)(ptr))[0];
        int src_h = ((float*)(ptr))[1];
        float *matrix = (float *)(&(ptr[2]));
        //float matrix[9] = {2.458836, 7.6811775, 4.024922, 5.791087, 12.582414, 3.260357, 0, 0, 1};
        //int dst_w = src_w;
        //int dst_h = src_h;
        int dst_w = 64;
        int dst_h = 64;
        printf("src_w=%d, src_h=%d\n", src_w, src_h);
        int src_line_stride = src_w * 4;
        int dst_line_stride = dst_w * 4;
        data_info_s src = {NULL, BS_DATA_ARGB, 4, src_w, src_h, src_line_stride};
        data_info_s gld = {NULL, BS_DATA_ARGB, 4, dst_w, dst_h, dst_line_stride};
        data_info_s dut = {NULL, BS_DATA_ARGB, 4, dst_w, dst_h, dst_line_stride};
        box_affine_info_s info = { {0, 0, src_w, src_h},
                                   {matrix[0], matrix[1], matrix[2],
                                    matrix[3], matrix[4], matrix[5],
                                    0, 0, 1}, 0, zero_point};
#if (defined CSE_SIM_ENV) || (defined CHIP_SIM_ENV) || (defined EYER_SIM_ENV)
        src.base = (uint8_t*)bscaler_malloc(1, src_w * 4 * src_h);
#else
        src.base = (uint8_t*)malloc(src_w * 4 * src_h);
#endif
        if (src.base == NULL) {
            fprintf(stderr, "malloc space for src error!\n");
        }

        gld.base = (uint8_t*)malloc(dst_w * 4 * dst_h);
        if (gld.base == NULL) {
            fprintf(stderr, "malloc space for gld error!\n");
            exit(1);
        }

#if (defined CSE_SIM_ENV) || (defined CHIP_SIM_ENV) || (defined EYER_SIM_ENV)
        dut.base = (uint8_t*)bscaler_malloc(1, dst_w * 4 * dst_h);
#else
        dut.base = (uint8_t*)malloc(dst_w * 4 * dst_h);
#endif
        if (dut.base == NULL) {
            fprintf(stderr, "malloc space for dut error!\n");
            exit(1);
        }

        // resize image for affine input
        bilinear(img, img_w * 4, 4, img_w, img_h,
                 (uint8_t*)src.base, src_w * 4, 4, src_w, src_h);

        // golden int affine
        affine_whole_int(&src, box_num, &gld, &info, coef, offset);

        // dut integer affine
#if (defined CSE_SIM_ENV) || (defined CHIP_SIM_ENV) || (defined EYER_SIM_ENV)
        bs_affine_start(&src, box_num, &dut, &info, coef, offset);
        bs_affine_wait();
#else
        bs_affine_mdl(&src, box_num, &dut, &info, coef, offset);
#endif
#if DUMP_IMAGE
        stbi_write_png(src_name.c_str(), src_w, src_h, 4, src.base, src_line_stride);
        stbi_write_png(gld_name.c_str(), dst_w, dst_h, 4, gld.base, dst_line_stride);
        stbi_write_png(dut_name.c_str(), dst_w, dst_h, 4, dut.base, dst_line_stride);
#endif
        //check
        uint8_t *gld_ptr = (uint8_t *)gld.base;
        uint8_t *dut_ptr = (uint8_t *)dut.base;
        double diff = 0;
        int errnum = 0;
        for (int i = 0; i < dst_h; i++) {
            for (int j = 0; j < dst_w; j++) {
                for (int k = 0; k < 3; k++) {
                    uint8_t g_val = gld_ptr[i * dst_w * 4 + j * 4 + k];
                    uint8_t d_val = dut_ptr[i * dst_w * 4 + j * 4 + k];
                    diff += ((g_val - d_val) * (g_val - d_val));
                    if (g_val != d_val) {
                        if (errnum < 3) {
                            printf("[Error] idx(%d) (%d,%d,%d) : (G) %d -- (E) %d\n", idx, j, i, k, g_val, d_val);
                        }
                        errnum++;
                    }
                }
            }
        }
        diff = (sqrt(diff) / (dst_w * dst_h * 3)) * 100;
        diffs[idx] = diff;
        if (diff > 10.0) {
            printf("####### idx = %d ##############\n", idx);
            printf("diff = %f\n", diff);
        }
#if (defined CSE_SIM_ENV) || (defined CHIP_SIM_ENV) || (defined EYER_SIM_ENV)
        bscaler_free(dut.base);
        bscaler_free(src.base);
#else
        free(dut.base);
        free(src.base);
#endif
        free(gld.base);
    }

    double diff_sum = 0;
    double diff_max = 0;
    for (int idx = idx_st; idx < idx_end; idx++) {
        diff_sum += diffs[idx];
        if (diffs[idx] > diff_max) {
            diff_max = diffs[idx];
        }
    }
    double avg_diff = diff_sum / (idx_end - idx_st);
    printf("avg_diff=%lf, diff_max=%lf\n", avg_diff, diff_max);

    free(diffs);
    free(param_ptr);
#ifdef EYER_SIM_ENV
    eyer_stop();
#endif
    return 0;
}
