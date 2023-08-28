/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : t_bscaler.c
 * Authors    : cbzhang@abdon.ic.jz.com
 * Create Time: 2019-08-18:12:02:29
 * Description:
 *
 */

#include <stdio.h>
#include <time.h>
#include "bscaler_hw_api.h"
#include "bscaler_mdl.h"

int bscaler_dump(FILE *fp, char *s, char *p, char size, int h, int w, int stride){
    int x, y;
    fprintf(fp, "{//%0dx%0d; %s\n", h, w, s);
    if (size== 1) { //char
        unsigned char *pp= (unsigned char *)p;
        for (y= 0; y< h; y++) {
            for (x= 0; x< w; x++) {
                fprintf(fp, "0x%0x,", pp[y*stride+ x]);
            }
            fprintf(fp, "//y= 0x%0x\n", y);
        }
    } else if (size== 2) { //short
        unsigned short *pp= (unsigned short *)p;
        for (y= 0; y< h; y++) {
            for (x= 0; x< w; x++) {
                fprintf(fp, "0x%0x,", pp[y*stride+ x]);
            }
            fprintf(fp, "//y= 0x%0x\n", y);
        }
    } else if (size== 4) { //int
        unsigned int *pp= (unsigned int *)p;
        for (y= 0; y< h; y++) {
            for (x= 0; x< w; x++) {
                fprintf(fp, "0x%0x,", pp[y*stride+ x]);
            }
            fprintf(fp, "//y= 0x%0x\n", y);
        }
    }
    fprintf(fp, "}\n");
    return 0;
}

uint32_t bacaler_random(uint32_t min, uint32_t max, uint32_t align) {
    //printf("bacaler_random = 0x%0x, 0x%0x, 0x%0x\n", min, max, align);
    if ((min%align) != 0 || (max%align) != 0)
        fprintf(stderr, "error: random error! min = %0d, max = %0d, align = %0d\n", min, max, align);
    int offset = align - 1;
    uint32_t random_val = ((rand()%(max - min + 1) + min) + offset) & (~(align - 1));
    if (random_val < min)
        random_val = min;
    else if (random_val > max)
        random_val = max;
    //printf("random_val = 0x%0x\n", random_val);
    return (random_val);
}

void bscaler_frmt_init(bst_hw_once_cfg_s *cfg)
{
    printf("bscaler_frmt_init begin\n");
    int x, y;
    { //initial for format
        cfg->src_format = rand()%2; //0: nv12; 1: bgr;
        if (cfg->src_format == 0) {
            uint8_t tmp = rand()%3;
            switch (tmp) {
            case (0) : cfg->dst_format = 0; break; //nv12 -> nv12
            case (1) : cfg->dst_format = 1; break; //nv12 -> bgr
            case (2) : cfg->dst_format = 3; break; //nv12 -> kernel
            }
        } else {
            uint8_t tmp = rand()%2;
            switch (tmp) {
            case (0) : cfg->dst_format = 1; break; //bgr -> bgr
            case (1) : cfg->dst_format = 3; break; //bgr -> kernel
            }
        }
        //cfg->src_format = 1;
        //cfg->dst_format = 1;
        cfg->kernel_size = (cfg->dst_format == 3) ? bacaler_random(0, 3, 1) : 0;  //0: 1x1; 1: 3x3; 2: 5x5; 3: 7x7
        cfg->kernel_xstride = (cfg->dst_format == 3) ? bacaler_random(1, 3, 1) : 1; //1~3;
        cfg->kernel_ystride = (cfg->dst_format == 3) ? bacaler_random(1, 3, 1) : 1;//1~3;
        cfg->kernel_dummy_val = (cfg->dst_format == 3) ? bacaler_random(0, 1<<32 - 1, 1) : 0;
        cfg->panding_lf = (cfg->dst_format == 3) ? bacaler_random(0, 7, 1) : 0; //left panding edge.
        cfg->panding_rt = (cfg->dst_format == 3) ? bacaler_random(0, 7, 1) : 0; //right panding edge.
        cfg->panding_tp = (cfg->dst_format == 3) ? bacaler_random(0, 7, 1) : 0; //top panding edge.
        cfg->panding_bt = (cfg->dst_format == 3) ? bacaler_random(0, 7, 1) : 0; //botom panding edge.
    }
    cfg->frmt_chain_bus = 0;
    cfg->frmt_ibuft_bus = 0;
    cfg->frmt_obuft_bus = 0;
    cfg->frmt_task_len = 2; //cfg->frmt_h_src;
    { //initial for w, h, ps_src, ps_dst
        if (cfg->dst_format == 3) { //kernel
            switch(cfg->kernel_size) {
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
        //cfg->src_w = 2;
        //cfg->src_h = 2;
        printf("format: src_format(%0d),dst_format(%0d),kernel_size(%0d),kernel_xstride(%0d),kernel_ystride(%0d),frmt_w(0x%x),frmt_h(0x%x)\n",
               cfg->src_format,cfg->dst_format,cfg->kernel_size,cfg->kernel_xstride,cfg->kernel_ystride,cfg->src_w,cfg->src_h);
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
            kernel_frame_w = (cfg->src_w + cfg->panding_lf + cfg->panding_rt - pixel_kernel_w)/cfg->kernel_xstride + 1;
            cfg->dst_line_stride = kernel_frame_w*32;
            cfg->frmt_fs_dst = cfg->dst_line_stride*cfg->frmt_task_len;
            break;
        }
        printf("src_line_stride = 0x%0x, dst_line_stride = 0x%0x\n", cfg->src_line_stride, cfg->dst_line_stride);
    }
    { //malloc src, dst
        if (cfg->frmt_ibuft_bus) {
            cfg->src_base0 = (uint32_t)bscaler_malloc_oram(1, cfg->src_line_stride*cfg->src_h);
            if (cfg->src_format == 0) //nv12
                cfg->src_base1 = (uint32_t)bscaler_malloc_oram(1, cfg->src_line_stride*cfg->src_h/2);
        } else {
            cfg->src_base0 = (uint32_t)bscaler_malloc(1, cfg->src_line_stride*cfg->src_h);
            if (cfg->src_format == 0) //nv12
                cfg->src_base1 = (uint32_t)bscaler_malloc(1, cfg->src_line_stride*cfg->src_h/2);
        }
        if (cfg->dst_format == 3) {
            uint8_t frame_num = ((cfg->kernel_size == 3) ? 5 :
                                 (cfg->kernel_size == 2) ? 3 : 1);
            if (cfg->frmt_obuft_bus) {
                cfg->dst_base0 = (uint32_t)bscaler_malloc_oram(1, frame_num*cfg->frmt_fs_dst);
            } else {
                cfg->dst_base0 = (uint32_t)bscaler_malloc(1, frame_num*cfg->frmt_fs_dst);
            }
        } else {
            if (cfg->frmt_task_len%2 != 0) {
                fprintf(stderr, "error: malloc dst failed! task_len = %0d\n", cfg->frmt_task_len);
            } else {
                if (cfg->frmt_obuft_bus) {
                    cfg->dst_base0 = (uint32_t)bscaler_malloc_oram(1, cfg->dst_line_stride*cfg->frmt_task_len);
                    if (cfg->dst_format == 0) //nv12
                        cfg->dst_base1 = (uint32_t)bscaler_malloc_oram(1, cfg->dst_line_stride*cfg->frmt_task_len/2);
                } else {
                    cfg->dst_base0 = (uint32_t)bscaler_malloc(1, cfg->dst_line_stride*cfg->frmt_task_len);
                    if (cfg->dst_format == 0) //nv12
                        cfg->dst_base1 = (uint32_t)bscaler_malloc(1, cfg->dst_line_stride*cfg->frmt_task_len/2);
                }
            }
        }
    }
    { //initial T input data
        uint8_t *p;
        p = (uint8_t *)cfg->src_base0;
        for (y = 0; y < cfg->src_h; y++) {
            for (x = 0; x < cfg->src_line_stride; x++) {
                p[y*cfg->src_line_stride + x] = /*y*cfg->src_line_stride + x; */rand()%255;
            }
        }
        /*
          bscaler_dump(stderr, "src_base0:",
          (char *)cfg->src_base0,
          sizeof(char),
          cfg->src_h,
          cfg->src_line_stride,
          cfg->src_line_stride);
        */
        if (cfg->src_format == 0) {
            p = (uint8_t *)cfg->src_base1;
            for (y = 0; y < cfg->src_h/2; y++) {
                for (x = 0; x < cfg->src_line_stride; x++) {
                    p[y*cfg->src_line_stride + x] = /*y*cfg->src_line_stride + x; */rand()%255;
                }
            }
            /*
              bscaler_dump(stderr, "src_base1:",
              (char *)cfg->src_base1,
              sizeof(char),
              cfg->src_h/2,
              cfg->src_w,
              cfg->src_line_stride);
            */
        }
    }
    printf("bscaler_frmt_init end\n");
}

void bscaler_frmt_free(bst_hw_once_cfg_s *cfg) {
    printf("bscaler_frmt_free begin\n");
    if (cfg->frmt_ibuft_bus) {
        bscaler_free_oram(cfg->src_base0);
        if (cfg->src_format == 0)
            bscaler_free_oram(cfg->src_base1);
    } else {
        bscaler_free(cfg->src_base0);
        if (cfg->src_format == 0)
            bscaler_free(cfg->src_base1);
    }
    if (cfg->frmt_obuft_bus) {
        bscaler_free_oram(cfg->dst_base0);
        if (cfg->dst_format == 0)
            bscaler_free_oram(cfg->dst_base1);
    } else {
        bscaler_free(cfg->dst_base0);
        if (cfg->dst_format == 0)
            bscaler_free(cfg->dst_base1);
    }
    printf("bscaler_frmt_free end\n");
}

int main()
{
#ifdef EYER_SIM_ENV
    eyer_system_ini(0);// eyer environment initialize.
    eyer_reg_segv();   // simulation error used
    eyer_reg_ctrlc();  // simulation error used
#elif CSE_SIM_ENV
    bscaler_mem_init();
    int ret = NULL;
    if ((ret = __aie_mmap(0x4000000)) == NULL) { //64MB
        printf ("nna box_base virtual generate failed !!\n");
        return 0;
    }
#endif
    uint32_t seed = (uint32_t)time(NULL);
    uint32_t seed_idx = 0;
    uint32_t seed_num = 1000;
    uint32_t nv2bgr_coef[9] = {1220542, 1673527, 0,
                               1220542, 409993, 852492,
                               1220542, 2116026, 0};
    uint32_t nv2bgr_ofst[2] = {16, 128};

    do {
        srand(seed + seed_idx);
        printf("current SEED is %ld\n",seed + seed_idx);
        uint32_t calc_fail_flag=0;
        uint32_t nv2bgr_fail_flag=0;
        bst_hw_once_cfg_s dut_cfg;
        bscaler_frmt_init(dut_cfg);
        dut_cfg.nv2bgr_ofst[0] = nv2bgr_ofst[0];
        dut_cfg.nv2bgr_ofst[1] = nv2bgr_ofst[1];
        dut_cfg.nv2bgr_alpha = rand() % 32;
        uint32_t nv2bgr_order_rand[12] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5,
                                          0x8, 0x9, 0xa, 0xb, 0xc, 0xd};
        uint32_t order_index  = rand() % 12;
        dut_cfg.nv2bgr_order = (dut_cfg.dst_format == 3) ? 0 : nv2bgr_order_rand[order_index];//0:bgra
        for (int i = 0; i< 9; i++) {
            dut_cfg.nv2bgr_coef[i] = nv2bgr_coef[i];
        }

        //reset
        bscaler_write_reg(BSCALER_CFG, 1);
        while (bscaler_read_reg(BSCALER_CFG, 1) != 0);

        //configure
        //bscaler_param_cfg(nv2bgr_coef, nv2bgr_ofst, nv2bgr_order);
        bscaler_frmt_cfg(frmt_cfg);

        uint8_t pixel_kernel_w;
        uint8_t pixel_kernel_h;
        uint32_t kernel_frame_w;
        uint32_t kernel_frame_h;
        uint8_t *ref_dst_y = NULL;
        uint8_t *ref_dst_c = NULL;
        uint32_t imp_task_ynum;
        uint32_t ref_frame_num;
        uint32_t ref_frame_stride;
        uint32_t ref_total_size;
        bst_hw_once_cfg_s gld_cfg;
        memcpy(&dut_cfg, &gld_cfg, sizeof(bst_hw_once_cfg_s));

        switch (dut_cfg.dst_format) {
        case (0): //nv12
            ref_frame_num = 1;
            imp_task_ynum = dut_cfg.src_h;
            ref_total_size = dut_cfg.dst_line_stride*dut_cfg.src_h;
            if ((gld_cfg.base0 = (uint8_t*)malloc(ref_total_size)) == NULL)
                fprintf(stderr, "error: malloc ref_dst_y failed!\n");
            if ((gld_cfg.base1 = (uint8_t*)malloc(ref_total_size/2)) == NULL) {
                fprintf(stderr, "error: malloc ref_dst_c failed!\n");
            }
            break;
        case (1): //bgr
            ref_frame_num = 1;
            imp_task_ynum = dut_cfg.src_h;
            ref_total_size = dut_cfg.dst_line_stride*dut_cfg.src_h;
            if ((ref_dst_y = (uint8_t*)malloc(ref_total_size)) == NULL)
                fprintf(stderr, "error: malloc ref_dst_y failed!\n");
            else
                printf("malloc ref_dst_y = 0x%0x\n", ref_dst_y);
            break;
        case (2): break;
        case (3):
            {
                pixel_kernel_w = ((dut_cfg.kernel_size == 3) ? 7 :
                                  (dut_cfg.kernel_size == 2) ? 5 :
                                  (dut_cfg.kernel_size == 1) ? 3 : 1);
                pixel_kernel_h = pixel_kernel_w;
                ref_frame_num = ((dut_cfg.kernel_size == 3) ? 5 :
                                 (dut_cfg.kernel_size == 2) ? 3 : 1);
                kernel_frame_w = (dut_cfg.src_w + dut_cfg.panding_lf + dut_cfg.panding_rt - pixel_kernel_w)/dut_cfg.kernel_xstride + 1;
                kernel_frame_h = (dut_cfg.src_h + dut_cfg.panding_tp + dut_cfg.panding_bt - pixel_kernel_h)/dut_cfg.kernel_ystride + 1;
                ref_frame_stride = kernel_frame_w*kernel_frame_h*32;
                imp_task_ynum = kernel_frame_h;
                ref_total_size = ref_frame_stride*ref_frame_num;
                if ((ref_dst_y = (uint8_t*)malloc(ref_total_size)) == NULL)
                    fprintf(stderr, "error: malloc ref_dst_y(ref_total_size) failed!\n", ref_total_size);
            }
            break;
        }

        bst_mdl(&gld_cfg);

        uint8_t *imp_task_dst_y = (uint8_t*)dut_cfg.dst_base0;
        uint8_t *imp_task_dst_c = (uint8_t*)dut_cfg.dst_base1;
        int imp_task_y = 0;
        bscaler_write_reg(BSCALER_FRMT_TASK, (dut_cfg.frmt_task_len << 16) | 1 | 1 << 1);
        while(1) {
            if ((bscaler_read_reg(BSCALER_FRMT_TASK, 0) & 0x1) == 0) {
                uint32_t frmt_task_len_real = (((imp_task_y + dut_cfg.frmt_task_len) > imp_task_ynum) ?
                                               (imp_task_ynum - imp_task_y) : dut_cfg.frmt_task_len);
                uint8_t frame_i;
                uint32_t x, y;
                switch (dut_cfg.dst_format) {
                case(0): //nv12
                    for (y = 0; y < frmt_task_len_real; y++) {
                        for (x = 0; x < dut_cfg.src_w; x++) {
                            if (ref_dst_y[(imp_task_y+y)*dut_cfg.dst_line_stride + x] != imp_task_dst_y[y*dut_cfg.dst_line_stride + x] ) {
                                fprintf(stderr, "error: nv12, dst_y[%0d][%0x][%0x] = 0x%0x -> 0x%0x\n", imp_task_y, y, x,
                                        ref_dst_y[(imp_task_y+y)*dut_cfg.dst_line_stride + x],
                                        imp_task_dst_y[y*dut_cfg.dst_line_stride + x]);
                                nv2bgr_fail_flag =1;
                                break;
                            }
                        }
                    }
                    for (y = 0; y < frmt_task_len_real/2; y++) {
                        for (x = 0; x < dut_cfg.src_w; x++) {
                            if (ref_dst_c[(imp_task_y/2+y)*dut_cfg.dst_line_stride + x] != imp_task_dst_c[y*dut_cfg.dst_line_stride + x] ) {
                                fprintf(stderr, "error: nv12, dst_c[%0d][%0x][%0x] = 0x%0x -> 0x%0x\n", imp_task_y, y, x,
                                        ref_dst_c[(imp_task_y+y)*dut_cfg.dst_line_stride + x],
                                        imp_task_dst_c[y*dut_cfg.dst_line_stride + x]);
                                nv2bgr_fail_flag =1;
                                break;
                            }
                        }
                    }
                    break;
                case(1): //bgr
                    for (y = 0; y < frmt_task_len_real; y++) {
                        for (x = 0; x < dut_cfg.src_w*4; x++) {
                            if (ref_dst_y[(imp_task_y+y)*dut_cfg.dst_line_stride + x] != imp_task_dst_y[y*dut_cfg.dst_line_stride + x] ) {
                                fprintf(stderr, "error: bgr, dst_y[%0d][%0x][%0x] = 0x%0x -> 0x%0x\n", imp_task_y, y, x,
                                        ref_dst_y[(imp_task_y+y)*dut_cfg.dst_line_stride + x],
                                        imp_task_dst_y[y*dut_cfg.dst_line_stride + x]);
                                nv2bgr_fail_flag =1;
                                break;
                            } else {
                                //fprintf(stderr, "info: bgr, dst_y[%0d][%0x][%0x] = 0x%0x -> 0x%0x\n", imp_task_y, y, x,
                                //        ref_dst_y[(imp_task_y+y)*dut_cfg.dst_line_stride + x],
                                //        imp_task_dst_y[y*dut_cfg.dst_line_stride + x]);
                            }
                        }
                    }
                    break;
                case(2): break;
                case(3): //kernel
                    for (frame_i = 0; frame_i < ref_frame_num; frame_i++) {
                        for (y = 0; y < frmt_task_len_real; y++) {
                            for (x = 0; x < dut_cfg.dst_line_stride; x++) {
                                if (ref_dst_y[frame_i*ref_frame_stride + (imp_task_y+y)*dut_cfg.dst_line_stride + x] !=
                                    imp_task_dst_y[frame_i*dut_cfg.frmt_fs_dst + y*dut_cfg.dst_line_stride + x] ) {
                                    fprintf(stderr, "error: kernel, dst_y[%0d][%0d][%0x][%0x] = 0x%0x -> 0x%0x\n", imp_task_y, frame_i, y, x,
                                            ref_dst_y[frame_i*ref_frame_stride + (imp_task_y+y)*dut_cfg.dst_line_stride + x],
                                            imp_task_dst_y[frame_i*dut_cfg.frmt_fs_dst + y*dut_cfg.dst_line_stride + x]);
                                    nv2bgr_fail_flag =1;
                                    break;
                                } else {
                                    /*
                                      fprintf(stderr, "pass: kernel, dst_y[%0d][%0d][%0x][%0x] = 0x%0x -> 0x%0x\n", imp_task_y, frame_i, y, x,
                                      ref_dst_y[frame_i*ref_frame_stride + (imp_task_y+y)*dut_cfg.dst_line_stride + x],
                                      imp_task_dst_y[frame_i*dut_cfg.frmt_fs_dst + y*dut_cfg.dst_line_stride + x]);
                                    */
                                }
                            }
                        }
                    }
                    break;
                }
                imp_task_y += dut_cfg.frmt_task_len;
                if (imp_task_y >= imp_task_ynum) {
                    break;
                } else {
                    bscaler_write_reg(BSCALER_FRMT_TASK, (dut_cfg.frmt_task_len << 16) | 1 | 1 << 1);
                }
            }
        }
        if (ref_dst_y != NULL) {
            free(ref_dst_y);
            ref_dst_y = NULL;
        }
        if (ref_dst_c != NULL) {
            free(ref_dst_c);
            ref_dst_c = NULL;
        }
        //polling
        while ((bscaler_read_reg(BSCALER_FRMT_CTRL, 0) & 0x1) != 0);
        //check summary
        uint32_t mchk_isum = dut_cfg.isum;
        uint32_t mchk_osum = dut_cfg.osum;
        uint32_t dchk_isum = bscaler_read_reg(BSCALER_FRMT_ISUM, 0);
        uint32_t dchk_osum = bscaler_read_reg(BSCALER_FRMT_OSUM, 0);
        if (mchk_osum != dchk_osum) {
            fprintf(stderr, "error: frmt OSUM is failed: %0x -> %0x \n", mchk_osum ,dchk_osum);
            nv2bgr_fail_flag =1;
        }
        //report result
        if(nv2bgr_fail_flag!=0) {
            if(nv2bgr_fail_flag){
                printf("frmt simulation failed!\n");
            }
            char log_info[100];
            FILE *flog=NULL;
            flog = fopen("bscaler_frmt.log","a+");
            if(flog== NULL){
                fprintf(stderr, "bscaler_frmt.log open filed!\n");
            }
            fseek(flog,0,SEEK_END);
            snprintf(log_info,100,"bscaler error:seed is %d\n",seed + seed_idx);
            fwrite(log_info,strlen(log_info),1,flog);
            fclose(flog);
            break;
        } else {
            printf("simulation is passed!\n");
        }
        bscaler_frmt_free(frmt_cfg);

        seed_idx ++;
    } while (seed_idx < seed_num);

#ifdef EYER_SIM_ENV
    eyer_stop();
#endif
    return 0;
}
