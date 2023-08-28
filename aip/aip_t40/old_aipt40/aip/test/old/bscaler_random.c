/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : bscaler_case_func.c
 * Authors    : lzhu@abel.ic.jz.com
 * Create Time: 2020-06-04:12:32:35
 * Description:
 *
 */

#include "bscaler_random.h"

uint8_t dst_format_idx;

//#include "t_bscaler.h"
#define CLIP(val, min, max) ((val) < (min) ? (min) : (val) > (max) ? (max) : (val))

void set_run_mode(frmc_mode_sel_e mode_sel_e ,bs_hw_once_cfg_s *cfg, uint8_t is_first_frame){
    bs_hw_data_format_e chn2chn_format[4] = {BS_HW_DATA_FM_BGRA,BS_HW_DATA_FM_F32_2B,BS_HW_DATA_FM_F32_4B,BS_HW_DATA_FM_F32_8B};
    bs_hw_data_format_e nv2bgr_format[12] = {BS_HW_DATA_FM_BGRA,BS_HW_DATA_FM_GBRA,BS_HW_DATA_FM_RBGA,BS_HW_DATA_FM_BRGA,BS_HW_DATA_FM_GRBA,BS_HW_DATA_FM_RGBA,BS_HW_DATA_FM_ABGR,BS_HW_DATA_FM_AGBR,BS_HW_DATA_FM_ARBG,BS_HW_DATA_FM_ABRG,BS_HW_DATA_FM_AGRB,BS_HW_DATA_FM_ARGB};

    uint8_t src_radom_idx;
    uint8_t dst_radom_idx;
    switch(mode_sel_e){
    case bresize_chn2chn:
        printf("###bresize_chn2chn here\n");
        src_radom_idx = rand()%4;
        dst_radom_idx = src_radom_idx;
        cfg->affine = 0;
        cfg->box_mode = 1;
        cfg->src_format = chn2chn_format[src_radom_idx];
        cfg->dst_format = chn2chn_format[dst_radom_idx];
        break;
    case bresize_nv2chn:
        printf("###bresize_nv2chn here\n");
        if(is_first_frame)
            dst_format_idx = rand()%12;
        dst_radom_idx = dst_format_idx;
        cfg->affine = 0;
        cfg->box_mode = 1;
        cfg->src_format = 0;
        cfg->dst_format = nv2bgr_format[dst_radom_idx];
        break;
    case lresize_chn2chn:
        printf("###lresize_chn2chn here\n");
        src_radom_idx = rand()%4;
        dst_radom_idx = src_radom_idx;
        cfg->affine = 0;
        cfg->box_mode = 0;
        cfg->src_format = chn2chn_format[src_radom_idx];
        cfg->dst_format = chn2chn_format[dst_radom_idx];
        break;
    case lresize_nv2chn:
        printf("###lresize_nv2chn here\n");
        if(is_first_frame)
            dst_format_idx = rand()%12;
        dst_radom_idx = dst_format_idx;
        cfg->affine = 0;
        cfg->box_mode = 0;
        cfg->src_format = 0;
        cfg->dst_format = nv2bgr_format[dst_radom_idx];
        break;
    case perspective_bgr2bgr:
        printf("###perspective_bgr2bgr here\n");
        src_radom_idx = 0;
        dst_radom_idx = src_radom_idx;
        cfg->affine = 1;
        cfg->box_mode = 1;
        cfg->src_format = chn2chn_format[src_radom_idx];
        cfg->dst_format = chn2chn_format[dst_radom_idx];
        break;
    case perspective_nv2bgr:
        printf("###perspective_nv2chn here\n");
        if(is_first_frame)
            dst_format_idx = rand()%12;
        dst_radom_idx = dst_format_idx;
        cfg->affine = 1;
        cfg->box_mode = 1;
        cfg->src_format = 0;
        cfg->dst_format = nv2bgr_format[dst_radom_idx];
        break;
    case perspective_nv2nv:
        printf("###perspective_nv2nv here\n");
        cfg->affine = 1;
        cfg->box_mode = 1;
        cfg->src_format = 0;
        cfg->dst_format = 0;
        break;
    default:
        src_radom_idx = rand()%4;
        dst_radom_idx = src_radom_idx;
        cfg->affine = 0;
        cfg->box_mode = 1;
        cfg->src_format = chn2chn_format[src_radom_idx];
        cfg->dst_format = chn2chn_format[dst_radom_idx];
        break;
    }
}

void set_split_ofst(bs_hw_once_cfg_s *cfg){//should be fix
    cfg->src_box_x  = 0;
    cfg->src_box_y  = 0;
    cfg->dst_box_x  = 0;
    cfg->dst_box_y  = 0;
}
void init_base(bs_hw_once_cfg_s *cfg, frmc_random_s *radom_cfg){
    cfg->boxes_info  = NULL;
    cfg->src_base0 = NULL;
    cfg->src_base1 = NULL;
    radom_cfg->ybase_dst = NULL;
    radom_cfg->cbase_dst = NULL;
    radom_cfg->md_ybase_dst = NULL;
    radom_cfg->md_cbase_dst = NULL;
    uint32_t ii;
    for(ii=0;ii<64;ii++){
        cfg->dst_base[ii] = NULL;
    }
}
void set_frmc_src_info(bs_hw_once_cfg_s *cfg){

    uint32_t src_format = cfg->src_format & 0x1;
    uint32_t bpp_mode = (cfg->src_format >> 5) & 0x3;

    if (cfg->src_format == 0) { //nv12
        cfg->src_line_stride = cfg->src_box_w + rand()%8;//allign 1byte
        cfg->src_base0 = (uint8_t*)bscaler_malloc(1, cfg->src_line_stride*cfg->src_box_h);//allign 8byte
        cfg->src_base1 = (uint8_t*)bscaler_malloc(1, cfg->src_line_stride*cfg->src_box_h/2);
    } else { //channel
        cfg->src_line_stride =(cfg->src_box_w<<(2+bpp_mode)) + rand()%8;
        cfg->src_base0 = (uint8_t*)bscaler_malloc(1, cfg->src_line_stride*cfg->src_box_h);
    }
}
void set_frmc_dst_info(bs_hw_once_cfg_s *cfg, frmc_random_s *radom_cfg){
    uint32_t dst_format = cfg->dst_format & 0x1;
    uint32_t bpp_mode = (cfg->src_format >> 5) & 0x3;
    radom_cfg->frmc_bs_xnum = rand()%cfg->box_num + 1;
    radom_cfg->frmc_bs_ynum = (cfg->box_num/radom_cfg->frmc_bs_xnum) + ((cfg->box_num%radom_cfg->frmc_bs_xnum) ? 1 : 0);

    if (cfg->dst_format == 0) { //nv12
        cfg->dst_line_stride = cfg->dst_box_w+ rand()%8;
        radom_cfg->ybase_dst = (uint8_t*)bscaler_malloc(1, cfg->dst_line_stride*cfg->dst_box_h);//align 8bytes
        radom_cfg->cbase_dst = (uint8_t*)bscaler_malloc(1, cfg->dst_line_stride*cfg->dst_box_h/2); //align 8bytes
    } else { //channel
        if(radom_cfg-> frmc_dst_store_mode==0){
            cfg->dst_line_stride = (cfg->dst_box_w <<(2+bpp_mode))+ rand()%8;
            radom_cfg->ybase_dst = (uint8_t*)bscaler_malloc(1, cfg->dst_line_stride*cfg->dst_box_h*cfg->box_num );
        }else{
            radom_cfg->dst_line_stride1 = (cfg->dst_box_w <<(2+bpp_mode))+ rand()%8;
            cfg->dst_line_stride = radom_cfg->dst_line_stride1 * cfg->box_num;
            radom_cfg->ybase_dst = (uint8_t*)bscaler_malloc(1, radom_cfg->dst_line_stride1*cfg->dst_box_h*cfg->box_num );
        }
    }

    uint32_t bid_y,bid_x;
    if(cfg->box_mode){//box_mode
        cfg->dst_base[0] =radom_cfg->ybase_dst;
        if(!dst_format)
            cfg->dst_base[1] = radom_cfg->cbase_dst;
    }else{//line_mode
        uint32_t index=0;
        for(bid_y=0; bid_y<radom_cfg->frmc_bs_ynum; bid_y++){
            for(bid_x=0; bid_x<radom_cfg->frmc_bs_xnum; bid_x++){
                if(index>=cfg->box_num){
                    break;
                }else{
                    if(radom_cfg->frmc_dst_store_mode==0){
                        cfg->dst_base[index] = radom_cfg->ybase_dst + index*cfg->dst_box_h*cfg->dst_line_stride;
                    }else{
                        cfg->dst_base[index] = radom_cfg->ybase_dst + bid_y*cfg->dst_box_h*cfg->dst_line_stride+bid_x*radom_cfg->dst_line_stride1;
                    }
                }
                index++;
            }
        }
    }
    printf("bs base:%p,%p,%p\n",cfg->dst_base[0],radom_cfg->ybase_dst,radom_cfg->cbase_dst);
}

void set_bresize_matrix(bs_hw_once_cfg_s *cfg, frmc_random_s *radom_cfg){

    float frmc_dm0=0.0;
    float frmc_dm1=0.0;
    float frmc_dm2=0.0;
    float frmc_dm3=0.0;
    float frmc_dm4=0.0;
    float frmc_dm5=0.0;
    float frmc_dm6=0.0;
    float frmc_dm7=0.0;
    float frmc_dm8=0.0;

    uint32_t  y_scale;
    float     kk=0.0;
    float frmc_dm1_fabs;
    float   mono=0;
    uint32_t y_scale_cod = 0;

    frmc_dm0 =  (float)cfg->src_box_w /(float)cfg->dst_box_w ;
    frmc_dm1 =  0;
    frmc_dm2 =  0.5*frmc_dm0-0.5;
    frmc_dm3 =  0;
    frmc_dm4 =  (float)cfg->src_box_h /(float)cfg->dst_box_h;//0.5*scale_x -0.5
    frmc_dm5 =  0.5*frmc_dm4-0.5;
    frmc_dm6 = 0;//perspect0;
    frmc_dm7 = 0;//perspect1;
    frmc_dm8 = 0;//perspect2;

    float   coef[6] = {0.0,0.0,0.0,0.0,0.0,0.0};
    frmc_dm1_fabs = fabs(frmc_dm4);
    uint32_t  y_n=0;
    y_scale_cod = frmc_dm1_fabs < 1;//omly for resize
    if(y_scale_cod){
        if(frmc_dm1_fabs/0.5>=1){
            y_n = 1;
            kk  = frmc_dm1_fabs/0.5;
        }
        else if(frmc_dm1_fabs/0.25>=1){
            y_n = 2;
            kk  = frmc_dm1_fabs/0.25;
        }
        else if(frmc_dm1_fabs/0.125>=1){
            y_n = 3;
            kk  = frmc_dm1_fabs/0.125;
        }
        else if(frmc_dm1_fabs/0.0625>=1){
            y_n = 4;
            kk  = frmc_dm1_fabs/0.0625;
        }
        else if(frmc_dm1_fabs/0.03125>=1){
            y_n = 5;
            kk  = frmc_dm1_fabs/0.03125;
        }
        else if(frmc_dm1_fabs/0.015625>=1){
            y_n = 6;
            kk  = frmc_dm1_fabs/0.015625;
        }
        else{
            radom_cfg->frmc_cfg_fail =1;
            printf("####: y amplify cfg out range!####\n");
        }
        y_scale = 1<<y_n;
        if((cfg->src_box_h*y_scale)>=32768){//repeat signed frmc_h
            radom_cfg->frmc_cfg_fail =1;
            printf("####affine y scale: src_h=src_box_h*y_scale cfg out range(>=32768)!####\n");
        }
        cfg->y_gain_exp = y_n;
    }else{
        cfg->y_gain_exp = 0;
    }
    if(y_scale_cod){
        cfg ->matrix[0] = frmc_dm0 * 65536;
        cfg ->matrix[1] = frmc_dm1 * 65536;
        cfg ->matrix[2] = frmc_dm2 * 65536;
        if(frmc_dm4 <0){
            cfg ->matrix[4] = -(kk * 65536);
        }else{
            cfg ->matrix[4] = kk * 65536;
        }
        int32_t dm3_t = frmc_dm3 * 65536;
        int32_t dm5_t = frmc_dm5 * 65536;
        cfg ->matrix[3] = dm3_t * y_scale ;
        cfg ->matrix[5] = dm5_t * y_scale ;
    }//random y<1
    else{
        cfg ->matrix[0] = frmc_dm0 * 65536;
        cfg ->matrix[1] = frmc_dm1 * 65536;
        cfg ->matrix[2] = frmc_dm2 * 65536;
        cfg ->matrix[3] = frmc_dm3 * 65536;
        cfg ->matrix[4] = frmc_dm4 * 65536;
        cfg ->matrix[5] = frmc_dm5 * 65536;
        cfg ->matrix[6] = frmc_dm6 * 65536;
        cfg ->matrix[7] = frmc_dm7 * 65536;
        cfg ->matrix[8] = frmc_dm8 * 65536;
        cfg->y_gain_exp = 0;
    }
}

void set_lresize_box_info(bs_hw_once_cfg_s *cfg, frmc_random_s *radom_cfg){
    cfg->matrix[0] = 0;
    cfg->matrix[1] = 0;
    cfg->matrix[2] = 0;
    cfg->matrix[3] = 0;
    cfg->matrix[4] = 0;
    cfg->matrix[5] = 0;
    cfg->matrix[6] = 0;
    cfg->matrix[7] = 0;
    cfg->matrix[8] = 0;

    uint32_t line_y_n;
    uint32_t line_y_scale;
    cfg->boxes_info = (uint32_t*)bscaler_malloc(1, cfg->box_num *24);
    uint32_t *p = (uint32_t *)cfg->boxes_info;
    uint32_t box_i;
    float    bid_scale_y[64];
    for(box_i=0;box_i<64;box_i++){
        bid_scale_y[box_i] = 0.0;
    }
    float *box_base = (float *)bscaler_malloc(1, cfg->box_num*8*sizeof(float));
    float *pp = box_base;
    for (box_i = 0; box_i < cfg->box_num ;box_i++) {
        float sbx;
        float sby;
        float sw;
        float sh;
        float dm0;
        float dm2;
        float dm4;
        float dm5;
        sbx = radom_cfg->frmc_line_sbx;
        sby = radom_cfg->frmc_line_sby;
        sw  = radom_cfg->frmc_line_sw;
        sh  = radom_cfg->frmc_line_sh;

        dm0 = (float)sw/(float)cfg->dst_box_w;
        dm4 = (float)sh/(float)cfg->dst_box_h;
        dm2 = 0.5*dm0-0.5;//should be fix
        dm5 = 0.5*dm4-0.5;
        pp[box_i*8 + 0] = sbx ;
        pp[box_i*8 + 1] = sby ;
        pp[box_i*8 + 2] = sw ;
        pp[box_i*8 + 3] = sh;
        pp[box_i*8 + 4] = dm0;
        pp[box_i*8 + 5] = dm4;
        pp[box_i*8 + 6] = dm2;
        pp[box_i*8 + 7] = dm5;
        bid_scale_y[box_i] = dm4 ;
        /*printf("box[0x%0x]: %f, %f, %f, %f, %f, %f, %f, %f\n", &pp[box_i*8 + box_i],
          pp[box_i*8 + 0], pp[box_i*8 + 1], pp[box_i*8 + 2], pp[box_i*8 + 3], pp[box_i*8 + 4], pp[box_i*8 + 5],pp[box_i*8 + 6],pp[box_i*8 + 7]);*/
    }
    //cfg y_scale
    float tmp_yscale=0.0;
    uint8_t i,j;
    for(i=0;i<cfg->box_num-1;i++){
        for(j=i+1;j<cfg->box_num;j++){
            if(bid_scale_y[i]>bid_scale_y[j]){
                tmp_yscale = bid_scale_y[i];
                bid_scale_y[i] = bid_scale_y[j];
                bid_scale_y[j] = tmp_yscale;
            }
        }
    }
    if(bid_scale_y[0]<1){//amplyfy
        if(bid_scale_y[0]/0.5>=1){
            line_y_n = 1;
        }
        else if(bid_scale_y[0]/0.25>=1){
            line_y_n = 2;
        }
        else if(bid_scale_y[0]/0.125>=1){
            line_y_n = 3;
        }
        else if(bid_scale_y[0]/0.0625>=1){
            line_y_n = 4;
        }
        else if(bid_scale_y[0]/0.03125>=1){
            line_y_n = 5;
        }
        else if(bid_scale_y[0]/0.015625>=1){
            line_y_n = 6;
        }
        else{
            radom_cfg->frmc_cfg_fail =1;
            printf("####: y amplify cfg out range!####\n");
        }
        line_y_scale = 1<<line_y_n;
        printf("line_y_scale:%d\n",line_y_scale);
        if((cfg->src_box_h*line_y_scale)>=32768){//repeat signed frmc_h
            radom_cfg->frmc_cfg_fail =1;
            printf("####affine y scale: src_h=src_box_h*line_y_scale cfg out range(>=32768)!####\n");
        }

        cfg->y_gain_exp = line_y_n;
        for (box_i = 0; box_i < cfg->box_num ;box_i++) {
            uint16_t sbx = (uint16_t)pp[box_i*8 + 0];
            uint16_t sby = (uint16_t)pp[box_i*8 + 1];//*line_y_scale;
            uint16_t sw  = (uint16_t)pp[box_i*8 + 2];
            uint16_t sh  = (uint16_t)pp[box_i*8 + 3];//*line_y_scale;
            float dm0 = pp[box_i*8 + 4];
            float dm4 = pp[box_i*8 + 5]*line_y_scale;
            float dm2 = pp[box_i*8 + 6];
            float dm5 = pp[box_i*8 + 7]*line_y_scale;
            p[box_i*6 + 0] = sbx << 0 | sby << 16;
            p[box_i*6 + 1] = sw << 0 | sh << 16;
            p[box_i*6 + 2] = dm0*65536;
            p[box_i*6 + 3] = dm4*65536;
            p[box_i*6 + 4] = dm2*65536;
            p[box_i*6 + 5] = dm5*65536;
            /* printf("line resize amplify:box[0x%0x]: 0x%0x, 0x%0x, 0x%0x, 0x%0x,0x%0x,0x%0x\n", &p[box_i*6 + box_i],
               p[box_i*6 + 0], p[box_i*6 + 1], p[box_i*6 + 2], p[box_i*6 + 3], p[box_i*6 + 4], p[box_i*6 + 5]);*/
        }
    }else{
        for (box_i = 0; box_i < cfg->box_num ;box_i++) {
            uint16_t sbx = (uint16_t)pp[box_i*8 + 0];
            uint16_t sby = (uint16_t)pp[box_i*8 + 1];
            uint16_t sw  = (uint16_t)pp[box_i*8 + 2];
            uint16_t sh  = (uint16_t)pp[box_i*8 + 3];
            float dm0 = pp[box_i*8 + 4];
            float dm4 = pp[box_i*8 + 5];
            float dm2 = pp[box_i*8 + 6];
            float dm5 = pp[box_i*8 + 7];
            p[box_i*6 + 0] = sbx << 0 | sby << 16;
            p[box_i*6 + 1] = sw << 0 | sh << 16;
            p[box_i*6 + 2] = dm0*65536;
            p[box_i*6 + 3] = dm4*65536;
            p[box_i*6 + 4] = dm2*65536;
            p[box_i*6 + 5] = dm5*65536;
            /*    printf("line resize shrink:box[0x%0x]: 0x%0x, 0x%0x, 0x%0x, 0x%0x,0x%0x,0x%0x\n", &p[box_i*6 + box_i],
                  p[box_i*6 + 0], p[box_i*6 + 1], p[box_i*6 + 2], p[box_i*6 + 3], p[box_i*6 + 4], p[box_i*6 + 5]);*/
        }
    }
    if (box_base != NULL) {
        bscaler_free(box_base);
        box_base = NULL;
    }
}

void src_image_init(bs_hw_once_cfg_s *cfg){
    uint32_t x, y;
    uint32_t byte_num = 0;
    uint32_t byte_per_pix;

    uint32_t src_format = cfg->src_format & 0x1;
    uint32_t bpp_mode = (cfg->src_format >> 5) & 0x3;

    if(src_format == 0){
        byte_per_pix= 1;}
    else{
        byte_per_pix= 1<<(2+bpp_mode);
    }
    uint8_t *pc_frmc_y =(uint8_t *)cfg->src_base0;
    printf("Y:%p,C:%p,src_ps:0x%x\n",cfg->src_base0,cfg->src_base1,cfg->src_line_stride);
    for (y = 0; y < cfg->src_box_h; y++) {
        for (x = 0; x <cfg->src_box_w; x++) {
            for(byte_num=0;byte_num< byte_per_pix;byte_num++){
                    pc_frmc_y[y*cfg->src_line_stride + x*byte_per_pix+byte_num] = rand()%255;
                //if(((x==0x1f) & (y==0x0)) | ((x==0x20) & (y==0))){
                //printf("base%p: x%x, y:%x, src:%x\n",&pc_frmc_y[y*cfg->src_line_stride + x*byte_per_pix+byte_num],x,y,pc_frmc_y[y*cfg->src_line_stride + x*byte_per_pix+byte_num]);
            }
        }
    }
    if(src_format==0){
        printf("XXXXXXXXX\n");
        uint8_t *pc_frmc_c = (uint8_t *)cfg->src_base1;
        for (y = 0; y < cfg->src_box_h/2; y++) {
            for (x = 0; x < cfg->src_box_w; x++) {
                for(byte_num=0;byte_num< byte_per_pix;byte_num++){
                    pc_frmc_c[y*cfg->src_line_stride + x*byte_per_pix+byte_num] = rand()%255;
                    //printf("UVaddr:%p,y:0x%x,x:%x,ps:0x%x,byte_num:0x%x,frmc_y 0x%x(%p)\n",pc_frmc_c,y,x,cfg->src_line_stride,byte_num,pc_frmc_c[y*cfg->src_line_stride + x*byte_per_pix+byte_num],&pc_frmc_c[y*cfg->src_line_stride + x*byte_per_pix+byte_num]);
                }
            }
        }
    }
}

void printf_info(bs_hw_once_cfg_s *cfg){
    uint32_t bpp_mode = (cfg->src_format >> 5) & 0x3;
    printf("cfg src:sx_ofst:0x%x,sy_ofst:0x%x,src_w:0x%x,src_h:0x%x,src_ybase:0x%x,src_cbase:0x%x,src_ps:0x%x,src_bw:0x%x----\n",cfg->src_box_x,cfg->src_box_y,cfg->src_box_w,cfg->src_box_h,cfg->src_base0,cfg->src_base1,cfg->src_line_stride,bpp_mode);
    printf("cfg dst:dx_ofst:0x%x,dy_ofst:0x%x,dst_w:0x%x,dst_h:0x%x,dst_ps:0x%x----\n",cfg->dst_box_x,cfg->dst_box_y,cfg->dst_box_w,cfg->dst_box_h,cfg->dst_line_stride);
    if(cfg->box_mode){
        printf("cfg box:dm0:0x%x,dm1:0x%x,dm2:0x%x,dm3:0x%x,dm4:0x%x,dm5:0x%x ----\n",cfg ->matrix[0],cfg ->matrix[1],cfg ->matrix[2],cfg ->matrix[3],cfg ->matrix[4],cfg ->matrix[5]);
    }
}

uint32_t error_seed_collect(bs_hw_once_cfg_s *cfg, frmc_random_s *radom_cfg, uint32_t seed){
    uint32_t case_no_check =0;
    char log_info[100];
    uint32_t src_format = cfg->src_format & 0x1;
    uint32_t dst_format = cfg->dst_format & 0x1;
    uint32_t bpp_mode = (cfg->src_format >> 5) & 0x3;

    if(cfg->affine == 1){
        if(dst_format == 0){
            if(radom_cfg -> is_perspective){
                radom_cfg->flog = fopen("bscaler_frmc_perspect_nv12.log","a+");
            }else{
                radom_cfg->flog = fopen("bscaler_frmc_aff_nv12.log","a+");
            }
        }else{
            if(src_format == 1){//src bgr
                if(radom_cfg -> is_perspective){
                    radom_cfg->flog = fopen("bscaler_frmc_perspect_src_bgr.log","a+");
                }else{
                    radom_cfg->flog = fopen("bscaler_frmc_aff_src_bgr.log","a+");
                }
            }else{
                if(radom_cfg -> is_perspective){
                    radom_cfg->flog = fopen("bscaler_frmc_perspect_bgr.log","a+");
                }else{
                    radom_cfg->flog = fopen("bscaler_frmc_aff_bgr.log","a+");
                }
            }
        }
    }//affine
    else {//resize
        if(cfg->box_mode == 0){
            if(src_format == 0){
                radom_cfg->flog = fopen("bscaler_frmc_line_nv12.log","a+");
            }
            else{
                radom_cfg->flog = fopen("bscaler_frmc_line_chn.log","a+");
            }
        }//line
        else{//box
            if(src_format == 0){
                radom_cfg->flog = fopen("bscaler_frmc_nv12.log","a+");
            }
            else{
                radom_cfg->flog = fopen("bscaler_frmc_chn.log","a+");
            }
        }
    }
    if(radom_cfg->flog== NULL){
        fprintf(stderr, "bscaler.log open failed!\n");
    }
    if(cfg->affine == 1){
        if(radom_cfg->frmc_cfg_fail){
            case_no_check=1;
            printf("***end:affine: y amplify cfg failed,no check!***\n");
            fseek(radom_cfg->flog,0,SEEK_END);
            snprintf(log_info,100,"affine y cfg failed :seed(%d),dm1(%f)\n",seed,cfg->matrix[1]);
            fwrite(log_info,strlen(log_info),1,radom_cfg->flog);
        }
    }
    else{
        if(radom_cfg->frmc_cfg_fail){
            case_no_check=1;
            if(cfg->box_mode==1){
                printf("***end:resize box mode:box scale cfg failed,dm0|dm1 >63,no check***!\n");
                fseek(radom_cfg->flog,0,SEEK_END);
                snprintf(log_info,100,"resize box mode dm0/1 cfg failed :seed(%d)\n",seed);
                fwrite(log_info,strlen(log_info),1,radom_cfg->flog);
            }else{
                printf("resize line mode:box cfg failed,>255,no check!\n");
                fseek(radom_cfg->flog,0,SEEK_END);
                snprintf(log_info,100,"resize line mode box cfg failed :seed(%d)\n",seed);
                fwrite(log_info,strlen(log_info),1,radom_cfg->flog);
            }
        }
    }
    return case_no_check;
}

void nv2bgr_random(bs_hw_once_cfg_s *cfg){//should be fix
    uint32_t nv2bgr_coef[9] = {0x12b400, 0x19a400, 0x0, 0x12b400, 0xd1000, 0x64c00, 0x12b400, 0x0, 0x206800};
    uint8_t nv2bgr_ofst[2] = {16,128};
    uint8_t nv2bgr_alpha;

    int num1,num2;
    for(num1 =0; num1 < 9; num1++){
        cfg->coef[num1] = nv2bgr_coef[num1];//rand()% 3FFFFF;
        printf("nv2bgr_coef[%d] = %0x \n",num1,cfg->coef[num1]);
    }
    for(num2 =0; num2 < 2; num2++){
        cfg->offset[num2] = nv2bgr_ofst[num2];//rand()% 256;
        printf("nv2bgr_ofst[%d] = %0x \n",num2,cfg->offset[num2]);
    }
    nv2bgr_alpha = 0x10;//rand()% 32;
    cfg->nv2bgr_alpha = nv2bgr_alpha;
    printf("nv2bgr_alpha = %0x \n",cfg->nv2bgr_alpha);
}

void perspective_mono_extreme(bs_hw_once_cfg_s *cfg){
    float dm0;//1 scale_x
    float dm1;//1 skew_x
    float dm2;//-2 trans_x
    float dm3;//1 skew_y
    float dm4;//1 scale_y
    float dm5;//-2 trans_y
    float dm6;//1 perspect0
    float dm7;//-1 perspect1
    float dm8;//2 perspect02

    dm0 = ((float)cfg ->matrix[0])/65536;
    dm1 = ((float)cfg ->matrix[1])/65536;
    dm2 = ((float)cfg ->matrix[2])/65536;
    dm3 = ((float)cfg ->matrix[3])/65536;
    dm4 = ((float)cfg ->matrix[4])/65536;
    dm5 = ((float)cfg ->matrix[5])/65536;
    dm6 = ((float)cfg ->matrix[6])/65536;
    dm7 = ((float)cfg ->matrix[7])/65536;
    dm8 = ((float)cfg ->matrix[8])/65536;
    // printf("persp:%f,%f,%f,%f,%f,%f,%f,%f,%f\n",dm0,dm1,dm2,dm3,dm4,dm5,dm6,dm7,dm8);
    //NOT:SZ=0
    //calc x mono:->(m1*m6-m7*m0)*dx + (m1*m8-m7*m2)
    uint8_t x_mono;
    float x_value;
    //float x_value_i;
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
    // x_value_i = ((int32_t)x_value) + 1 - cfg->dst_box_x;//ceil(x_value);
    mono_x_clip = CLIP(x_value_i, 0, 64);
    cfg->mono_x = x_mono << 7 | mono_x_clip <<0;
    flag = 0;
    //calc y mono:->(m4*m6-m7*m3)*dx + m4*m8 -m7*m5
    uint8_t y_mono;
    float y_value;
    //float y_value_i;
    int32_t y_value_i;
    uint8_t mono_y_clip;

    y_mono = (dm4*dm6 - dm7*dm3)>=0 ? 0 : 1;
    y_value = (dm7*dm5 - dm4*dm8)/(dm4*dm6-dm7*dm3);
    if((y_value<0) & (y_value>-1)){
        flag = 1;
    }
    int32_t y_ceil = flag ? 0: (((int32_t)y_value) + 1);
    // y_value_i = ((int32_t)y_value) + 1 - cfg->dst_box_x;//ceil(y_value);
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
    float val;
    int32_t val_i;
    int8_t extreme_value_clip;
    for(dx_indx=0;dx_indx<cfg->dst_box_w;dx_indx++){
        val = -(dm6*dx_indx + dm8)/dm7;
        val_i = val;
        if(val_i == val){
            val_i = val;
        }else{
            val_i = val + 1;
        }
        extreme_value_clip = CLIP(val_i, 0, 64);
        cfg->extreme_point[dx_indx] = extreme_value_clip;//extreme_point_rt
        //  printf("persp:extreme:%d,val:%f\n",cfg->extreme_point[dx_indx],val);
    }

}

//for chain
void bscaler_mdl_dst_malloc(frmc_random_s *radom_cfg, bs_hw_once_cfg_s *frmc_cfg){
    uint32_t src_format = frmc_cfg->src_format & 0x1;
    uint32_t dst_format = frmc_cfg->dst_format & 0x1;
    uint32_t bpp_mode = (frmc_cfg->src_format >> 5) & 0x3;

    uint8_t bid_i;
    for(bid_i=0; bid_i<64;bid_i++){
        radom_cfg->md_base_dst[bid_i] <= NULL;
    }
    //uint8_t* md_ybase_dst=NULL ;
    //uint8_t* md_cbase_dst=NULL ;
    if (frmc_cfg->dst_format == 0) { //nv12
        uint32_t  md_ybase_len = frmc_cfg->dst_line_stride*frmc_cfg->dst_box_h;
        uint32_t  md_cbase_len = frmc_cfg->dst_line_stride*frmc_cfg->dst_box_h/2;
        radom_cfg->md_ybase_dst =(uint8_t*) bscaler_malloc(1,md_ybase_len );
        radom_cfg->md_cbase_dst =(uint8_t*) bscaler_malloc(1,md_cbase_len );
    }else{
        uint32_t  md_ybase_len=frmc_cfg->dst_line_stride*frmc_cfg->dst_box_h*frmc_cfg->box_num ;
        radom_cfg->md_ybase_dst = (uint8_t*)bscaler_malloc(1,md_ybase_len);
    }

    uint32_t bid_x,bid_y;
    if(frmc_cfg->box_mode){//box_mode
        radom_cfg->md_base_dst[0] = radom_cfg->md_ybase_dst;
        if(!dst_format)
            radom_cfg->md_base_dst[1] = radom_cfg->md_cbase_dst;
    }else{//line_mode
        uint32_t index=0;
        for(bid_y=0; bid_y<radom_cfg->frmc_bs_ynum; bid_y++){
            for(bid_x=0; bid_x<radom_cfg->frmc_bs_xnum; bid_x++){
                if (index >= frmc_cfg->box_num) {
                    break;
                }else{
                    if(radom_cfg->frmc_dst_store_mode==0){
                        radom_cfg->md_base_dst[index] = radom_cfg->md_ybase_dst + index*frmc_cfg->dst_box_h*frmc_cfg->dst_line_stride;
                    }else{
                        radom_cfg->md_base_dst[index] = radom_cfg->md_ybase_dst + bid_y*frmc_cfg->dst_box_h*frmc_cfg->dst_line_stride+bid_x*radom_cfg->dst_line_stride1;
                    }
                }
                index++;
            }
        }
    }
    printf("mdl base:%p,%p,%p\n",radom_cfg->md_base_dst[0],radom_cfg->md_ybase_dst,radom_cfg->md_cbase_dst);

}

bs_chain_ret_s  bscaler_chain_init(uint32_t frame, bs_hw_once_cfg_s *frmc_cfg, frmc_random_s *radom_cfg){
    //init chain cfg
    uint32_t dst_bnum =0;
    uint32_t cfg_bnum =0;
    uint32_t dst_wnum =0;
    uint32_t cfg_wnum =0;
    uint32_t chain_base;
    uint32_t chain_len=0;
    int n;
    for(n=0;n<frame;n++){
        uint32_t dst_format = (&frmc_cfg[n])->dst_format & 0x1;
        if(!dst_format)
            cfg_bnum = 2;
        else
            cfg_bnum = (&frmc_cfg[n]) -> box_num;
        dst_bnum = dst_bnum + cfg_bnum;
        cfg_wnum = (&frmc_cfg[n]) -> dst_box_w;
        dst_wnum = dst_wnum + cfg_wnum;
    }
    chain_base = (uint32_t)bscaler_malloc(1,8*(22+dst_bnum+dst_wnum)*frame);//align 1bytes
    if(chain_base ==NULL){
        printf("error : malloc chain_base is failed ! \n ");
    }

    //configure chain
    uint32_t chain_base1 = chain_base;
    for(n=0; n<frame; n++){//frame n
        bs_chain_ret_s  back;
        bs_hw_once_cfg_s *frmc_cfg_once = &frmc_cfg[n];
        back = bscaler_frmc_chain_cfg(frmc_cfg_once, chain_base1);//
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

void bscaler_chain_flog_open(bs_hw_once_cfg_s *cfg, frmc_random_s *radom_cfg){
    uint32_t src_format = cfg->src_format & 0x1;
    uint32_t dst_format = cfg->dst_format & 0x1;
    uint32_t bpp_mode = (cfg->src_format >> 5) & 0x3;

    if(cfg->affine == 1){
        if(dst_format == 0){
            radom_cfg->flog = fopen("./log/bscaler_frmc_chain_aff_nv12.log","a+");
        }else{
            if(src_format == 1){//src bgr
                radom_cfg->flog = fopen("./log/bscaler_frmc_chain_aff_src_bgr.log","a+");
            }else{
                radom_cfg->flog = fopen("./log/bscaler_frmc_chain_aff_bgr.log","a+");
            }
        }
    }//affine
    else {//resize
        if(cfg->box_mode == 0){
            if(src_format == 0){
                radom_cfg->flog = fopen("./log/bscaler_frmc_chain_line_nv12.log","a+");
            }
            else{
                radom_cfg->flog = fopen("./log/bscaler_frmc_chain_line_chn.log","a+");
            }
        }//line
        else{//box
            if(src_format == 0){
                radom_cfg->flog = fopen("./log/bscaler_frmc_chain_nv12.log","a+");
            }
            else{
                radom_cfg->flog = fopen("./log/bscaler_frmc_chain_chn.log","a+");
            }
        }
    }
    if(radom_cfg->flog== NULL){
        fprintf(stderr, "bscaler.log open failed!\n");
    }

}

uint32_t bscaler_result_check(bs_hw_once_cfg_s *frmc_cfg, bs_hw_once_cfg_s *mdl_cfg, frmc_random_s  *radom_cfg){
    uint8_t *bs_dst=NULL;
    uint8_t *md_dst=NULL;
    uint8_t *bs_ydst;
    uint8_t *md_ydst;
    md_ydst = radom_cfg->md_base_dst[0];
    bs_ydst = (uint8_t *)frmc_cfg->dst_base[0];
    uint8_t *bs_cdst;
    uint8_t *md_cdst;
    md_cdst = radom_cfg->md_base_dst[1];
    bs_cdst = (uint8_t *)frmc_cfg->dst_base[1];
    uint32_t bpp_mode = (frmc_cfg->src_format >> 5) & 0x3;
    uint32_t dst_format = frmc_cfg->dst_format & 0x1;
    uint32_t byte_per_pix = 1<<(2+bpp_mode);
    uint32_t wrd_num =byte_per_pix;
    uint32_t bnum_i=0;
    uint32_t bnum_j=0;
    uint32_t i,j,cnum;
    uint32_t calc_fail_flag =0;
    for(bnum_i=0;bnum_i<frmc_cfg->box_num;bnum_i++){//bnum_i
        for(j=0;j<frmc_cfg->dst_box_h;j++){
            for(i=0;i<frmc_cfg->dst_box_w;i++){
                if(dst_format==0){//dst:nv12
                    uint32_t y_bs_idx = j*frmc_cfg->dst_line_stride+i;
                    uint32_t y_mdl_idx = j*mdl_cfg->dst_line_stride+i;
                    if((md_ydst[y_mdl_idx]==bs_ydst[y_bs_idx])){
                    }
                    else{
                        printf("error:Y: j(0x%x),i(0x%x),failed:md_ydst(%p:0x%x),bs_ydst(%p:0x%x)\n",
                               j,i,&md_ydst[y_mdl_idx],md_ydst[y_mdl_idx],&bs_ydst[y_bs_idx],bs_ydst[y_bs_idx]);
                        calc_fail_flag =1;
                        break;
                    }
                    uint32_t c_bs_idx = j/2*frmc_cfg->dst_line_stride+(i/2)*2;
                    uint32_t c_mdl_idx = j/2*mdl_cfg->dst_line_stride+(i/2)*2;
                    if((md_cdst[c_mdl_idx]==bs_cdst[c_bs_idx])&(md_cdst[c_mdl_idx+1]==bs_cdst[c_bs_idx+1])){
                    }else{
                        printf("error:j(0x%x),i(0x%x),failed:->U:md_cdst(%p:0x%x),bs_cdst(%p:0x%x),->V:md_cdst(%p:0x%x),bs_cdst(%p:0x%x)\n",
                               j,i,&md_cdst[c_mdl_idx],md_cdst[c_mdl_idx],&bs_cdst[c_bs_idx],bs_cdst[c_bs_idx],
                               &md_cdst[c_mdl_idx+1],md_cdst[c_mdl_idx+1],&bs_cdst[c_bs_idx+1],bs_cdst[c_bs_idx+1]);
                        calc_fail_flag =1;
                        break;
                    }
                }//dst:nv12
                else{//dst:bgr/chn
                    for(cnum=0; cnum<wrd_num;cnum++){
                        md_dst = (uint8_t*)radom_cfg->md_base_dst[bnum_i];
                        bs_dst = (uint8_t*)frmc_cfg->dst_base[bnum_i];
                        uint32_t bs_idx = j*frmc_cfg->dst_line_stride+ i*wrd_num+cnum;
                        uint32_t mdl_idx = j*mdl_cfg->dst_line_stride+ i*wrd_num+cnum;
                        if(md_dst[mdl_idx] == bs_dst[bs_idx]){
                        } else {
                            printf("error: box_i(0x%x),j(0x%x),i(0x%x) ,num(0x%x),failed:md_addr(%p):md_data(%x),bs_addr(%p),bs_data(%x)!\n",
                                   bnum_i,j,i,cnum,&md_dst[mdl_idx],md_dst[mdl_idx],&bs_dst[bs_idx],bs_dst[bs_idx]);
                            calc_fail_flag =1;
                            break;
                        }
                    }//cnum
                }//dst:bgr/chn
            }//dst_w
        }//dst_h
    }//bnum_i
    return calc_fail_flag;
}

void bscaler_frmc_free(bs_hw_once_cfg_s *cfg, frmc_random_s *radom_cfg) {
    if (cfg->src_base0 != NULL) {
        //     printf("free ybase_src!\n");
        bscaler_free(cfg->src_base0);
        cfg->src_base0 = NULL;
        //printf("end ybase_src!\n");
    }
    if (cfg->src_base1 != NULL) {
        //printf("free cbase_src!\n");
        bscaler_free(cfg->src_base1);
        cfg->src_base1 = NULL;
        //printf("end cbase_src!\n");
    }
    if (radom_cfg->ybase_dst != NULL) {
        //printf("free ybase_dst!\n");
        bscaler_free(radom_cfg->ybase_dst);
        radom_cfg->ybase_dst = NULL;
        //printf("end ybase_dst!\n");
    }
    if (radom_cfg->cbase_dst != NULL) {
        //printf("free cbase_dst!\n");
        bscaler_free(radom_cfg->cbase_dst);
        radom_cfg->cbase_dst = NULL;
        //printf("end cbase_dst!\n");
    }

    if (cfg->boxes_info != NULL) {
        // printf("free box_base!\n");
        bscaler_free(cfg->boxes_info);
        cfg->boxes_info = NULL;
        // printf("end box_base!\n");
    }
}

