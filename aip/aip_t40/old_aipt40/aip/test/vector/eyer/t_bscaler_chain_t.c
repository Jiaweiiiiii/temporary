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
#include "bscaler_hal.h"
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
        cfg->zero_point = (cfg->dst_format == 3) ? bacaler_random(0, 1<<32 - 1, 1) : 0;
        cfg->pad_left = (cfg->dst_format == 3) ? bacaler_random(0, 7, 1) : 0; //left panding edge.
        cfg->pad_right = (cfg->dst_format == 3) ? bacaler_random(0, 7, 1) : 0; //right panding edge.
        cfg->pad_top = (cfg->dst_format == 3) ? bacaler_random(0, 7, 1) : 0; //top panding edge.
        cfg->pad_bottom = (cfg->dst_format == 3) ? bacaler_random(0, 7, 1) : 0; //botom panding edge.
    }
    cfg->chain_bus = 0;
    cfg->ibuft_bus = 0;
    cfg->obuft_bus = 0;
    cfg->task_len = 2; //cfg->frmt_h_src;
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
            kernel_frame_w = (cfg->src_w + cfg->pad_left + cfg->pad_right - pixel_kernel_w)/cfg->kernel_xstride + 1;
            cfg->dst_line_stride = kernel_frame_w*32;
            cfg->dst_plane_stride = cfg->dst_line_stride*cfg->task_len;
            break;
        }
        printf("src_line_stride = 0x%0x, dst_line_stride = 0x%0x\n", cfg->src_line_stride, cfg->dst_line_stride);
    }
    { //malloc src, dst
        if (cfg->ibuft_bus) {
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
            if (cfg->obuft_bus) {
                cfg->dst_base0 = (uint32_t)bscaler_malloc_oram(1, frame_num*cfg->dst_plane_stride);
            } else {
                cfg->dst_base0 = (uint32_t)bscaler_malloc(1, frame_num*cfg->dst_plane_stride);
            }
        } else {
            if (cfg->task_len%2 != 0) {
                fprintf(stderr, "error: malloc dst failed! task_len = %0d\n", cfg->task_len);
            } else {
                if (cfg->obuft_bus) {
                    cfg->dst_base0 = (uint32_t)bscaler_malloc_oram(1, cfg->dst_line_stride*cfg->task_len);
                    if (cfg->dst_format == 0) //nv12
                        cfg->dst_base1 = (uint32_t)bscaler_malloc_oram(1, cfg->dst_line_stride*cfg->task_len/2);
                } else {
                    cfg->dst_base0 = (uint32_t)bscaler_malloc(1, cfg->dst_line_stride*cfg->task_len);
                    if (cfg->dst_format == 0) //nv12
                        cfg->dst_base1 = (uint32_t)bscaler_malloc(1, cfg->dst_line_stride*cfg->task_len/2);
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
    if (cfg->ibuft_bus) {
        bscaler_free_oram(cfg->src_base0);
        if (cfg->src_format == 0)
            bscaler_free_oram(cfg->src_base1);
    } else {
        bscaler_free(cfg->src_base0);
        if (cfg->src_format == 0)
            bscaler_free(cfg->src_base1);
    }
    if (cfg->obuft_bus) {
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
/*
  void bscaler_frmt_init_chain(bst_hw_once_cfg_s *pre, bst_hw_once_cfg_s *cur, uint32_t *chain_base) {
  bscaler_frmt_init(cur);
  bscaler_frmt_cfg_chain(pre, cur, chain_base);
  }
*/
#ifdef CHIP_SIM_ENV
#define TEST_NAME  t_bscaler_chain_t
#else
#define TEST_NAME  main
#endif
int TEST_NAME()
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
    unsigned int seed;
    seed = (unsigned)time(NULL);
    //seed =1595071384;//obuf bug
    uint32_t seed_idx = 0;
    uint32_t seed_num = 100;
    do {
        srand(seed + seed_idx);
        //srand(1587629386);
        printf("current SEED is %ld\n",seed + seed_idx);
        printf("--------seed_idx is %d--------\n",seed_idx);

        uint32_t calc_fail_flag=0;
        uint32_t nv2bgr_fail_flag=0;
        bst_hw_once_cfg_s frmt_cfg_inst;
        bst_hw_once_cfg_s *frmt_cfg = &frmt_cfg_inst;
        bscaler_frmt_init(frmt_cfg);
        uint32_t nv2bgr_coef[9]={0x4ad, 0x669, 0x0, 0x4ad, 0x344, 0x193, 0x4ad, 0x0, 0x81a, 16, 128};//fix me
        uint32_t nv2bgr_ofst[2]={16,128};
        frmt_cfg->nv2bgr_ofst[0] = nv2bgr_ofst[0];
        frmt_cfg->nv2bgr_ofst[1] = nv2bgr_ofst[1];
        frmt_cfg->nv2bgr_alpha  = rand()%32;
        uint32_t nv2bgr_order_rand[12] ={0x0,0x1,0x2,0x3,0x4,0x5,0x8,0x9,0xa,0xb,0xc,0xd};
        uint32_t order_index  = rand()%12;
        //frmt_cfg->nv2bgr_order = (frmt_cfg->dst_format == 3) ? 0 : nv2bgr_order_rand[order_index];//0:bgra
        //for (int i = 0; i< 9; i++) {
        //    frmt_cfg->nv2bgr_coef[i] = nv2bgr_coef[i];
        //}

        //reset
        bscaler_write_reg(BSCALER_CFG, 1);
        while(bscaler_read_reg(BSCALER_CFG, 1) != 0);
        bscaler_write_reg(BSCALER_FRMT_ISUM, 0); //clear summary
        bscaler_write_reg(BSCALER_FRMT_OSUM, 0); //clear summary
        //configure
        uint32_t chain_len_pre = 200;
        uint32_t *chain_base = (uint32_t *)bscaler_malloc(4, chain_len_pre);
        uint32_t chain_len = bscaler_frmt_chain_cfg(frmt_cfg, chain_base);
        if (chain_len > chain_len_pre)
            fprintf(stderr, "error: chain space is too small to store. 0x%0x < 0x%0x\n", chain_len_pre, chain_len);
        else
            printf("info: malloc chain succues base = 0x%0x, len = 0x%0x\n", (uint32_t)chain_base, chain_len);
        bscaler_write_reg(BSCALER_FRMT_CHAIN_BASE, (uint32_t)chain_base);
        bscaler_write_reg(BSCALER_FRMT_CHAIN_LEN, chain_len);
        bscaler_write_reg(BSCALER_FRMT_CHAIN_CTRL, (1<<3 | //irq_mask
                                                    0<<1 | //chain_bus
                                                    1<<0));//start hw

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
        switch (frmt_cfg->dst_format) {
        case (0): //nv12
            ref_frame_num = 1;
            imp_task_ynum = frmt_cfg->src_h;
            ref_total_size = frmt_cfg->dst_line_stride*frmt_cfg->src_h;
            if ((ref_dst_y = (uint8_t*)malloc(ref_total_size)) == NULL)
                fprintf(stderr, "error: malloc ref_dst_y failed!\n");
            if ((ref_dst_c = (uint8_t*)malloc(ref_total_size/2)) == NULL) {
                fprintf(stderr, "error: malloc ref_dst_c failed!\n");
            }
            break;
        case (1): //bgr
            ref_frame_num = 1;
            imp_task_ynum = frmt_cfg->src_h;
            ref_total_size = frmt_cfg->dst_line_stride*frmt_cfg->src_h;
            if ((ref_dst_y = (uint8_t*)malloc(ref_total_size)) == NULL)
                fprintf(stderr, "error: malloc ref_dst_y failed!\n");
            else
                printf("malloc ref_dst_y = 0x%0x\n", ref_dst_y);
            break;
        case (2): break;
        case (3):
        {
            pixel_kernel_w = ((frmt_cfg->kernel_size == 3) ? 7 :
                              (frmt_cfg->kernel_size == 2) ? 5 :
                              (frmt_cfg->kernel_size == 1) ? 3 : 1);
            pixel_kernel_h = pixel_kernel_w;
            ref_frame_num = ((frmt_cfg->kernel_size == 3) ? 5 :
                             (frmt_cfg->kernel_size == 2) ? 3 : 1);
            kernel_frame_w = (frmt_cfg->src_w + frmt_cfg->pad_left + frmt_cfg->pad_right - pixel_kernel_w)/frmt_cfg->kernel_xstride + 1;
            kernel_frame_h = (frmt_cfg->src_h + frmt_cfg->pad_top + frmt_cfg->pad_bottom - pixel_kernel_h)/frmt_cfg->kernel_ystride + 1;
            ref_frame_stride = kernel_frame_w*kernel_frame_h*32;
            imp_task_ynum = kernel_frame_h;
            ref_total_size = ref_frame_stride*ref_frame_num;
            if ((ref_dst_y = (uint8_t*)malloc(ref_total_size)) == NULL)
                fprintf(stderr, "error: malloc ref_dst_y(ref_total_size) failed!\n", ref_total_size);
        }
        break;
        }
        frmt_cfg->isum = 0;
        frmt_cfg->osum = 0;
        bscaler_frmt_nv2bgr_kernel(frmt_cfg->src_format, //0: nv12; 1: bgr
                                   frmt_cfg->dst_format, //0: normal mode; 1: kernel mode;
                                   frmt_cfg->kernel_size, //0: 1x1; 1: 3x3; 2: 5x5; 3: 7x7
                                   frmt_cfg->kernel_xstride, //1~3;
                                   frmt_cfg->kernel_ystride, //1~3;
                                   frmt_cfg->zero_point,
                                   frmt_cfg->pad_left,  //left panding edge.
                                   frmt_cfg->pad_right,  //right panding edge.
                                   frmt_cfg->pad_top,  //top panding edge.
                                   frmt_cfg->pad_bottom, //botom panding edge.
                                   frmt_cfg->src_base0,
                                   frmt_cfg->src_base1,
                                   frmt_cfg->src_line_stride,
                                   ref_dst_y,
                                   ref_dst_c,
                                   frmt_cfg->dst_line_stride,
                                   ref_frame_stride,
                                   frmt_cfg->src_h,
                                   frmt_cfg->src_w,
                                   frmt_cfg->nv2bgr_order,
                                   frmt_cfg->nv2bgr_coef,
                                   frmt_cfg->nv2bgr_ofst,
                                   frmt_cfg->nv2bgr_alpha,
                                   &frmt_cfg->isum,
                                   &frmt_cfg->osum);
        uint8_t *imp_task_dst_y = (uint8_t*)frmt_cfg->dst_base0;
        uint8_t *imp_task_dst_c = (uint8_t*)frmt_cfg->dst_base1;
        int imp_task_y = 0;
        while ((bscaler_read_reg(BSCALER_FRMT_CTRL, 0) & 0x1) == 0);
        bscaler_write_reg(BSCALER_FRMT_TASK, (frmt_cfg->task_len << 16) | 1 | 1 << 1);
        while(1) {
            if ((bscaler_read_reg(BSCALER_FRMT_TASK, 0) & 0x1) == 0) {
                uint32_t task_len_real = (((imp_task_y + frmt_cfg->task_len) > imp_task_ynum) ?
                                          (imp_task_ynum - imp_task_y) : frmt_cfg->task_len);
                uint8_t frame_i;
                uint32_t x, y;
                switch (frmt_cfg->dst_format) {
                case(0): //nv12
                    for (y = 0; y < task_len_real; y++) {
                        for (x = 0; x < frmt_cfg->src_w; x++) {
                            if (ref_dst_y[(imp_task_y+y)*frmt_cfg->dst_line_stride + x] != imp_task_dst_y[y*frmt_cfg->dst_line_stride + x] ) {
                                fprintf(stderr, "error: nv12, dst_y[%0d][%0x][%0x] = 0x%0x -> 0x%0x\n", imp_task_y, y, x,
                                        ref_dst_y[(imp_task_y+y)*frmt_cfg->dst_line_stride + x],
                                        imp_task_dst_y[y*frmt_cfg->dst_line_stride + x]);
                                nv2bgr_fail_flag =1;
                                break;
                            }
                        }
                    }
                    for (y = 0; y < task_len_real/2; y++) {
                        for (x = 0; x < frmt_cfg->src_w; x++) {
                            if (ref_dst_c[(imp_task_y/2+y)*frmt_cfg->dst_line_stride + x] != imp_task_dst_c[y*frmt_cfg->dst_line_stride + x] ) {
                                fprintf(stderr, "error: nv12, dst_c[%0d][%0x][%0x] = 0x%0x -> 0x%0x\n", imp_task_y, y, x,
                                        ref_dst_c[(imp_task_y+y)*frmt_cfg->dst_line_stride + x],
                                        imp_task_dst_c[y*frmt_cfg->dst_line_stride + x]);
                                nv2bgr_fail_flag =1;
                                break;
                            }
                        }
                    }
                    break;
                case(1): //bgr
                    for (y = 0; y < task_len_real; y++) {
                        for (x = 0; x < frmt_cfg->src_w*4; x++) {
                            if (ref_dst_y[(imp_task_y+y)*frmt_cfg->dst_line_stride + x] != imp_task_dst_y[y*frmt_cfg->dst_line_stride + x] ) {
                                fprintf(stderr, "error: bgr, dst_y[%0d][%0x][%0x] = 0x%0x -> 0x%0x\n", imp_task_y, y, x,
                                        ref_dst_y[(imp_task_y+y)*frmt_cfg->dst_line_stride + x],
                                        imp_task_dst_y[y*frmt_cfg->dst_line_stride + x]);
                                nv2bgr_fail_flag =1;
                                break;
                            } else {
                                //fprintf(stderr, "info: bgr, dst_y[%0d][%0x][%0x] = 0x%0x -> 0x%0x\n", imp_task_y, y, x,
                                //      ref_dst_y[(imp_task_y+y)*frmt_cfg->dst_line_stride + x],
                                //      imp_task_dst_y[y*frmt_cfg->dst_line_stride + x]);
                            }
                        }
                    }
                    break;
                case(2): break;
                case(3): //kernel
                    for (frame_i = 0; frame_i < ref_frame_num; frame_i++) {
                        for (y = 0; y < task_len_real; y++) {
                            for (x = 0; x < frmt_cfg->dst_line_stride; x++) {
                                if (ref_dst_y[frame_i*ref_frame_stride + (imp_task_y+y)*frmt_cfg->dst_line_stride + x] !=
                                    imp_task_dst_y[frame_i*frmt_cfg->dst_plane_stride + y*frmt_cfg->dst_line_stride + x] ) {
                                    fprintf(stderr, "error: kernel, dst_y[%0d][%0d][%0x][%0x] = 0x%0x -> 0x%0x\n", imp_task_y, frame_i, y, x,
                                            ref_dst_y[frame_i*ref_frame_stride + (imp_task_y+y)*frmt_cfg->dst_line_stride + x],
                                            imp_task_dst_y[frame_i*frmt_cfg->dst_plane_stride + y*frmt_cfg->dst_line_stride + x]);
                                    nv2bgr_fail_flag =1;
                                    break;
                                } else {
                                    /*
                                      fprintf(stderr, "pass: kernel, dst_y[%0d][%0d][%0x][%0x] = 0x%0x -> 0x%0x\n", imp_task_y, frame_i, y, x,
                                      ref_dst_y[frame_i*ref_frame_stride + (imp_task_y+y)*frmt_cfg->dst_line_stride + x],
                                      imp_task_dst_y[frame_i*frmt_cfg->dst_plane_stride + y*frmt_cfg->dst_line_stride + x]);
                                    */
                                }
                            }
                        }
                    }
                    break;
                }
                imp_task_y += frmt_cfg->task_len;
                if (imp_task_y >= imp_task_ynum) {
                    break;
                } else {
                    bscaler_write_reg(BSCALER_FRMT_TASK, (frmt_cfg->task_len << 16) | 1 | 1 << 1);
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
        while (bscaler_read_reg(BSCALER_FRMT_CHAIN_CTRL, 0) & 0x1 != 0);
        //check summary
        uint32_t mchk_isum = frmt_cfg->isum;
        uint32_t mchk_osum = frmt_cfg->osum;
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
#ifdef CHIP_SIM_ENV
TEST_MAIN(TEST_NAME);
#endif
