/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : rand_mix_api.cpp
 * Authors    : jmqi@taurus
 * Create Time: 2020-05-20:20:21:23
 * Description:
 *             random test case, all mode random,
 *             only run in eyer and FPGA
 *
 */

#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include "affine_whole_float.h"
#include "affine_whole_int.h"

#include "image_process.h"
#include "bscaler_hw_api.h"
#include "bscaler_api.h"
#include "rand_api.h"

static void bscaler_driver_init()
{
#if (defined EYER_SIM_ENV)
    eyer_system_ini(0);// eyer environment initialize.
    eyer_reg_segv();   // simulation error used
    eyer_reg_ctrlc();  // simulation error used
#elif (defined CSE_SIM_ENV)
    bscaler_mem_init();
    int ret = 0;
    if ((ret = __aie_mmap(0x4000000)) == 0) { //64MB
        printf ("nna box_base virtual generate failed !!\n");
        return 0;
    }
#endif
}

int main(int argc, char** argv)
{
    const uint32_t coef[9] = {1220542, 1673527, 0,
                              1220542, 409993, 852492,
                              1220542, 2116026, 0};
    const uint32_t offset[2] = {16, 128};

    bscaler_driver_init();

    FILE *flog = NULL;
    char log_info[100];
    //0.start position and run number
    int num = 1;
    int idx_st = 0;
    int idx_end = num;
    if (argc > 1) {
        idx_st = atoi(argv[1]);
    }
    if (argc > 2) {
        idx_end = idx_st + atoi(argv[2]);
    }

    for (int idx = idx_st; idx < idx_end; idx++) {
        printf("####### idx = %d ##############\n", idx);
        //0. set random seed
        int seed = (int)time(NULL);
        srand(seed);
        printf("current SEED is %ld\n",seed);
        //1. set mode,random matrix, set input cfg
        uint8_t mode_sel = 2;
        api_mode_sel_e mode_sel_e[8] = {bresize_chn2chn, bresize_nv2chn,
                                        affine_bgr2bgr, affine_nv2bgr,
                                        affine_nv2nv, perspective_bgr2bgr,
                                        perspective_nv2bgr, perspective_nv2nv};
        mode_sel = rand()%9;
        //mode_sel = 4;
        //except nv12-nv12
        if ((mode_sel == 4) || (mode_sel == 7)) {//fix me
            mode_sel = 2;
        }

        api_info_s src_cfg_inst;
        api_info_s *src_cfg = &src_cfg_inst;

        api_info_s dst_cfg_inst;
        api_info_s *dst_cfg = &dst_cfg_inst;
        set_run_mode(mode_sel_e[mode_sel], src_cfg, dst_cfg);
        float matrix_rad[9];
        int src_w_rad, src_h_rad, dst_w_rad, dst_h_rad;

        uint32_t src_format = src_cfg->format & 0x1;
        uint32_t src_bpp_mode = (src_cfg->format >> 5) & 0x3;

        matrix_random(src_bpp_mode,src_cfg->mode, matrix_rad, &src_w_rad, &src_h_rad, &dst_w_rad, &dst_h_rad);
        printf("Matrix:\n");
        printf("{ %.6f, %.6f, %.6f }\n", matrix_rad[0], matrix_rad[1], matrix_rad[2]);
        printf("{ %.6f, %.6f, %.6f }\n", matrix_rad[3], matrix_rad[4], matrix_rad[5]);
        printf("{ %.6f, %.6f, %.6f }\n", matrix_rad[6], matrix_rad[7], matrix_rad[8]);
        printf("dst: %d x %d, src: %d x %d\n", dst_w_rad, dst_h_rad, src_w_rad, src_h_rad);

        //only for debug
        /*matrix_rad[0] = 1;
          matrix_rad[1] = 0;
          matrix_rad[2] = 0;
          matrix_rad[3] = 0;
          matrix_rad[4] = 1;
          matrix_rad[5] = 0;
          matrix_rad[6] = 0;
          matrix_rad[7] = 0;
          matrix_rad[8] = 1;*/

        //2. read input random image
        //src
        int src_line_stride_rad = 0;
        uint8_t *src_base0 = NULL;
        uint8_t *src_base1 = NULL;
        if (src_format == 0) { //nv12
            src_line_stride_rad = src_w_rad + rand()%8;//allign 1byte
#if (defined CSE_SIM_ENV) || (defined CHIP_SIM_ENV) || (defined EYER_SIM_ENV)
            src_base0 = (uint8_t*)bscaler_malloc(1,src_line_stride_rad*src_h_rad*3/2);
#else
            src_base0 = (uint8_t*)malloc(src_line_stride_rad*src_h_rad*3/2);
#endif
            //src_base1 = (uint8_t*)malloc(src_line_stride_rad*src_h_rad/2);
        } else { //channel
            src_line_stride_rad =(src_w_rad<<(2+src_bpp_mode)) + rand()%8;
#if (defined CSE_SIM_ENV) || (defined CHIP_SIM_ENV) || (defined EYER_SIM_ENV)
            src_base0 = (uint8_t*)bscaler_malloc(1,src_line_stride_rad*src_h_rad);
#else
            src_base0 = (uint8_t*)malloc(src_line_stride_rad*src_h_rad);
#endif
        }
        if (src_base0 == NULL) {
            fprintf(stderr, "malloc space for src error!\n");
        }

        src_image_init(src_format,src_bpp_mode,
                       src_w_rad,src_line_stride_rad,src_h_rad,
                       src_base0);
        //dst
        uint32_t dst_format = dst_cfg->format & 0x1;
        uint32_t dst_bpp_mode = (dst_cfg->format >> 5) & 0x3;
        int dst_line_stride_rad = 0;
        uint8_t *dst_base0 = NULL;

        if (dst_format == 0) { //nv12
            dst_line_stride_rad = dst_w_rad + rand()%8;
#if (defined CSE_SIM_ENV) || (defined CHIP_SIM_ENV) || (defined EYER_SIM_ENV)
            dst_base0 = (uint8_t*)bscaler_malloc(1,dst_line_stride_rad*dst_h_rad*3/2);
#else
            dst_base0 = (uint8_t*)malloc(dst_line_stride_rad*dst_h_rad*3/2);
#endif
        } else { //channel
            dst_line_stride_rad = (dst_w_rad <<(2+dst_bpp_mode))+ rand()%8;
#if (defined CSE_SIM_ENV) || (defined CHIP_SIM_ENV) || (defined EYER_SIM_ENV)
            dst_base0 = (uint8_t*)bscaler_malloc(1,dst_line_stride_rad*dst_h_rad);
#else
            dst_base0 = (uint8_t*)malloc(dst_line_stride_rad*dst_h_rad);
#endif
        }
        if (dst_base0 == NULL) {
            fprintf(stderr, "malloc space for dst error!\n");
            exit(1);
        }
        //gld
        uint8_t *gld_base0 = NULL;
        if (dst_format == 0){
            gld_base0 = (uint8_t*)malloc(dst_line_stride_rad*dst_h_rad*3/2);
        }else{
            gld_base0 = (uint8_t*)malloc(dst_line_stride_rad*dst_h_rad);
        }
        if (gld_base0 == NULL) {
            fprintf(stderr, "malloc space for gld error!\n");
            exit(1);
        }

        // 5. Run a specified number of floating-point and integer models
        const uint8_t zero_point = 0;
        const int box_num = 1;

        int src_w = src_w_rad;
        int src_h = src_h_rad;
        float *matrix = matrix_rad;
        int dst_w = dst_w_rad;//confirm the .data
        int dst_h = dst_h_rad;
        int src_line_stride = src_line_stride_rad;
        int dst_line_stride = dst_line_stride_rad;
        data_info_s src = {NULL, src_cfg->format, src_bpp_mode, src_w, src_h, src_line_stride};
        data_info_s gld = {NULL, dst_cfg->format, dst_bpp_mode, dst_w, dst_h, dst_line_stride};
        data_info_s dut = {NULL, dst_cfg->format, dst_bpp_mode, dst_w, dst_h, dst_line_stride};
        box_affine_info_s info = { {0, 0, src_w, src_h},
                                   {matrix[0], matrix[1], matrix[2],
                                    matrix[3], matrix[4], matrix[5],
                                    matrix[6], matrix[7], matrix[8]}, 0, zero_point};
        box_resize_info_s *infos = (box_resize_info_s *)malloc(sizeof(box_resize_info_s) * box_num);
        for (int i = 0; i < box_num; i++) {
            int resize_src_x = 0;
            int resize_src_y = 0;
            int resize_src_w = src_w;
            int resize_src_h = src_h;

            infos[i].box.x = resize_src_x;
            infos[i].box.y = resize_src_y;
            infos[i].box.w = resize_src_w;
            infos[i].box.h = resize_src_h;
            infos[i].wrap = 0;
            infos[i].zero_point = 0;//fix me and fix the alpha
        }
        src.base = src_base0;
        gld.base = gld_base0;
        dut.base = dst_base0;

        // dut integer affine
#if (defined CSE_SIM_ENV) || (defined CHIP_SIM_ENV) || (defined EYER_SIM_ENV)
        switch(src_cfg->mode){
        case 0:
            printf("api resize here!\n");
            bs_resize_start(&src, box_num, &dut, infos, coef, offset);
            bs_resize_wait();
            bs_resize_mdl(&src, box_num, &gld, infos, coef, offset);
            break;
        case 1:
            printf("api affine here!\n");
            bs_affine_mdl(&src, box_num, &gld, &info, coef, offset);
            bs_affine_start(&src, box_num, &dut, &info, coef, offset);
            bs_affine_wait();
            break;
        case 2:
            printf("api perspective here!\n");
            bs_perspective_start(&src, box_num, &dut, &info, coef, offset);
            bs_perspective_wait();
            bs_perspective_mdl(&src, box_num, &gld, &info, coef, offset);
            break;
        default:
            printf("api resize here!\n");
            bs_resize_start(&src, box_num, &dut, infos, coef, offset);
            bs_resize_wait();
            bs_resize_mdl(&src, box_num, &gld, infos, coef, offset);
            break;
        }
        // golden int affine
        //affine_whole_int(&src, box_num, &gld, &info, coef, offset);
#else
        bs_affine_mdl(&src, box_num, &dut, &info, coef, offset);
#endif
        //check
        uint8_t *gld_ptr = (uint8_t *)gld.base;
        uint8_t *dut_ptr = (uint8_t *)dut.base;
        uint32_t osum_check =0;
        int errnum = 0;
        int check_dst_h = 0;
        uint32_t byte_per_pix;
        if(dst_format == 0){
            byte_per_pix = 1;
        }else{
            byte_per_pix = 1<<(2+dst_bpp_mode);
        }
        for (int i = 0; i < dst_h; i++) {
            for (int j = 0; j < dst_w; j++) {
                for (int k = 0; k < byte_per_pix; k++) {
                    uint8_t g_val = gld_ptr[i * dst_line_stride + j * byte_per_pix + k];
                    uint8_t d_val = dut_ptr[i * dst_line_stride + j * byte_per_pix + k];
                    osum_check += d_val;
                    if (g_val != d_val) {
                        if (errnum < 3) {
                            printf("[Error] idx(0x%x) (0x%x,0x%x,0x%x) : (G) 0x%x -- (E) 0x%x\n", idx, j, i, k, g_val, d_val);
                        }
                        errnum++;
                    }
                    if(dst_format == 0){
                        uint8_t g_val = gld_ptr[dst_line_stride * dst_h + i/2 * dst_line_stride + j];
                        uint8_t d_val = dut_ptr[dst_line_stride * dst_h + i/2 * dst_line_stride + j];
                        if (g_val != d_val) {
                            if (errnum < 3) {
                                printf("[Error:UV] idx(0x%x) (0x%x,0x%x,0x%x) : (G) 0x%x -- (E) 0x%x\n", idx, j, i, k, g_val, d_val);
                            }
                            errnum++;
                        }
                    }
                }
            }
        }

        if(osum_check ==0){
            printf("FAILED:OSUM_CHECK == NULL!\n");
        }
        if((errnum !=0) | (osum_check ==0)){
            printf("Simulation FAILED!\n");
            flog = fopen("./rand_api.log","a+");
            fseek(flog,0,SEEK_END);
            snprintf(log_info,100,"bscaler error:seed is %d\n",seed);
            printf("%p,%d,%p\n",log_info,strlen(log_info),flog);
            fwrite(log_info,strlen(log_info),1,flog);
            printf("frmc simulation failed!\n");
        }else{
            printf("Simulation SUCCESSED!\n");
        }

#if (defined CSE_SIM_ENV) || (defined CHIP_SIM_ENV) || (defined EYER_SIM_ENV)
        bscaler_free(dut.base);
        bscaler_free(src.base);
#else
        free(dut.base);
        free(src.base);
#endif
        if(gld.base!=NULL){
            free(gld.base);
            gld.base = NULL;
        }
        if(infos!=NULL){
            free(infos);
            infos = NULL;
        }
        if (flog != NULL) {
            fclose(flog);
            flog = NULL;
        }
    }
#ifdef EYER_SIM_ENV
    eyer_stop();
#endif
    return 0;
}
