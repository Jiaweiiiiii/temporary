/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : gen_random_bin.cpp
 * Authors    : jmqi@taurus
 * Create Time: 2020-05-20:20:21:23
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
void parse_cmd_line(int argc, char **argv)
{

}

void dump_random_cfg(bs_api_s *cfg, int seed, char *file)
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

int main(int argc, char** argv)
{
    // 1. set random seed
    //int seed = (int)time(NULL);
    struct timeb timer;
    ftime(&timer);
    int seed = ((timer.time * 1000 + timer.millitm) *
                (timer.time * 1000 + timer.millitm));
    printf("seed = 0x%08x\n", seed);
    srand(seed);

    // 2. parse cmd line
    const uint32_t coef[9] = {1220542, 1673527, 0,
                              1220542, 409993, 852492,
                              1220542, 2116026, 0};
    const uint32_t offset[2] = {16, 128};

    // 3. random parameter
    bs_api_s cfg;
    // RANDOM, RSZ, AFFINE, PERSP
    bs_run_mode_e mode = RANDOM;

    set_run_mode(mode, &cfg);
    matrix_random(&cfg);
    if (0) {
        if (cfg.src_w > 4)
            cfg.src_w = 4;
        if (cfg.src_h > 4)
            cfg.src_h = 4;
        if (cfg.dst_w > 4)
            cfg.dst_w = 4;
        if (cfg.dst_h > 4)
            cfg.dst_h = 4;
    }

    // 4. alloc space and random init
    int src_buf_size = get_src_buffer_size(&cfg);
    int dst_buf_size = get_dst_buffer_size(&cfg);
    cfg.src_base = (uint8_t*)malloc(src_buf_size);
    random_init_space(cfg.src_base, src_buf_size);
    //memset(cfg.src_base, 1, src_buf_size);
    cfg.dst_base = (uint8_t*)malloc(dst_buf_size);
    show_bs_api(&cfg);
    bs_api_recoder(&cfg);
    // 5. Run a specified number of floating-point and integer models
    const uint8_t zero_point = 0;
    const int box_num = 1;
    int src_bpp_mode = (cfg.src_format >> 5) & 0x3;
    int dst_bpp_mode = (cfg.dst_format >> 5) & 0x3;
    int src_bpp = 1 << (2 + src_bpp_mode);
    int dst_bpp = 1 << (2 + dst_bpp_mode);

    data_info_s src = {cfg.src_base, NULL, cfg.src_format, src_bpp,
                       cfg.src_w, cfg.src_h, cfg.src_line_stride};
    data_info_s dut = {cfg.dst_base, NULL, cfg.dst_format, dst_bpp,
                       cfg.dst_w, cfg.dst_h, cfg.dst_line_stride};
    box_resize_info_s *resize_infos = NULL;
    box_affine_info_s *affine_infos = NULL;
    //debug_point(121, 1);
    if (cfg.mode == RSZ) {
        resize_infos = (box_resize_info_s *)malloc(sizeof(box_resize_info_s) * box_num);
        for (int i = 0; i < box_num; i++) {
            resize_infos[i].box.x = 0;
            resize_infos[i].box.y = 0;
            resize_infos[i].box.w = cfg.src_w;
            resize_infos[i].box.h = cfg.src_h;
            resize_infos[i].wrap = 0;
            resize_infos[i].zero_point = zero_point;//fix me and fix the alpha
        }
    } else if ((cfg.mode == AFFINE) || (cfg.mode == PERSP)) {
        affine_infos = (box_affine_info_s *)malloc(sizeof(box_affine_info_s) * box_num);
        for (int i = 0; i < box_num; i++) {
            affine_infos[i].box.x = 0;
            affine_infos[i].box.y = 0;
            affine_infos[i].box.w = cfg.src_w;
            affine_infos[i].box.h = cfg.src_h;
            affine_infos[i].wrap = 0;
            affine_infos[i].zero_point = zero_point;//fix me and fix the alpha
            for (int j = 0; j < 9; j++) {
                affine_infos[i].matrix[j] = cfg.matrix[j];
            }
        }
    }

    // dut integer affine
    char name[20];
    sprintf(name, "0x%x.bin", seed);
    if (cfg.mode == RSZ) {
        bs_resize_mdl(&src, box_num, &dut, resize_infos, coef, offset);
    } else if (cfg.mode == AFFINE) {
        bs_affine_mdl(&src, box_num, &dut, affine_infos, coef, offset);
    } else if (cfg.mode == PERSP) {
        bs_perspective_mdl(&src, box_num, &dut, affine_infos, coef, offset);
    } else {
        printf("not support yet!");
    }

    dump_random_cfg(&cfg, seed, name);
    free(cfg.dst_base);
    free(cfg.src_base);
    free(resize_infos);
    free(affine_infos);
    return 0;
}
