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

#define HW_OPEN 1
#define BS_RANDOM 1
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "t_bscaler.h"
#include "bscaler_mdl.h"
#include "bscaler_hw_api.h"
#include "bscaler_random.h"

void bscaler_frmc_init(bs_hw_once_cfg_s *cfg, frmc_random_s *radom_cfg)
{
    radom_cfg->frmc_cfg_fail = 0;
    radom_cfg -> is_perspective = 0;
    cfg->box_bus = 0;
    cfg->ibufc_bus = 0;
    cfg->obufc_bus = 0;
    radom_cfg->frmc_dst_store_mode =0;//0-->bulk 1->joint
    cfg->irq_mask=1;
    init_base(cfg,radom_cfg);
    uint8_t mode_sel =0;
    frmc_mode_sel_e mode_sel_e[7] = {bresize_chn2chn,bresize_nv2chn,lresize_chn2chn,lresize_nv2chn,perspective_bgr2bgr,perspective_nv2bgr,perspective_nv2nv};

    float dm0 = 1;//1 scale_x
    float dm1 = 0;//1 skew_x
    float dm2 = 0;//-2 trans_x
    float dm3 = 0;//1 skew_y
    float dm4 = 1;//1 scale_y
    float dm5 = 0;//-2 trans_y
    float dm6 = 0;//1 perspect0
    float dm7 = 0;//-1 perspect1
    float dm8 = 1;//2 perspect02

#if BS_RANDOM
    //bresize_chn2chn;bresize_nv2chn;lresize_chn2chn;lresize_nv2chn;
    //perspective_chn2chn;perspective_nv2chn;perspective_nv2nv
    //cfg info
    //mode_sel = rand()%3 + 4; //affine
    mode_sel = rand()%7;
    set_run_mode(mode_sel_e[mode_sel], cfg, radom_cfg->is_first_frame);
    //frmc_bw
    uint32_t src_format = cfg->src_format & 0x1;
    uint32_t dst_format = cfg->dst_format & 0x1;
    uint32_t bpp_mode = (cfg->src_format >> 5) & 0x3;

    //src info
    cfg->src_box_h = rand()%1023+1;
    if(bpp_mode == 0){
        cfg->src_box_w = rand()%2047+1;//1->2048
    }else if(bpp_mode == 1){
        cfg->src_box_w=rand()%1023+1;//1->>1024
    }else if(bpp_mode == 2){
        cfg->src_box_w=rand()%511+1;//1->>512
    }else{
        cfg->src_box_w=rand()%255+1;//1->>256
    }
    if((src_format == 0) | (bpp_mode == 0)){
        cfg->src_box_w = ((cfg->src_box_w >> 1) + 1) << 1; //align 2pixel
        cfg->src_box_h = ((cfg->src_box_h >> 1) + 1) << 1; //align 2pixel
    } else {
        cfg->src_box_w = cfg->src_box_w; //feature map:2/4/8-->align 64bit
        cfg->src_box_h = cfg->src_box_h;
    }
    //dst info
    if(cfg->affine==0 ){
        if(cfg->box_mode==1){
            cfg->dst_box_w = rand()%64+1;//bresize:0-64
            cfg->dst_box_h = rand()%255+1;
        }else{
            cfg->dst_box_w = rand()%100+1;//###NOTING the random range
            cfg->dst_box_h = rand()%100+1;//###NOTING the random range
        }
    }else{
        cfg->dst_box_w = rand()%63+1;
        cfg->dst_box_h = rand()%63+1;
        if(dst_format ==0){//nv12 should be even
            cfg->dst_box_w = ((cfg->dst_box_w >> 1) + 1) << 1;
            cfg->dst_box_h = ((cfg->dst_box_h >> 1) + 1) << 1;
        }else{
            cfg->dst_box_w = cfg->dst_box_w ;
            cfg->dst_box_h = cfg->dst_box_h ;
        }
    }
    //box_num info
    if (cfg->box_mode == 1) {//box_mode
        cfg->box_num = 1;
    }else{
        cfg->box_num = rand()%63 +1;
    }
    uint8_t is_affine=0;
    if(cfg->affine){
        printf("AFFINE HERE\n");
        cfg ->matrix[0] = rand()%0xFFFFFFFF + 1;
        cfg ->matrix[1] = rand()%0xFFFFFFFF + 1;
        cfg ->matrix[2] = rand()%0xFFFFFFFF + 1;
        cfg ->matrix[3] = rand()%0xFFFFFFFF + 1;
        cfg ->matrix[4] = rand()%0xFFFFFFFF + 1;
        cfg ->matrix[5] = rand()%0xFFFFFFFF + 1;
        // cfg ->matrix[6] = 0;
        //cfg ->matrix[7] = 0;
        //cfg ->matrix[8] = 65536;
        cfg ->matrix[6] = rand()%0xFFFFFFFF + 1;
        cfg ->matrix[7] = rand()%0xFFFFFFFF + 1;
        cfg ->matrix[8] = rand()%0xFFFFFFFF + 1;
        is_affine = (cfg ->matrix[6]==0) & (cfg ->matrix[7]==0) & (cfg ->matrix[8]==65536);
        radom_cfg -> is_perspective = !is_affine;
        printf("persp:0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",cfg ->matrix[0],cfg ->matrix[1],cfg ->matrix[2],cfg ->matrix[3],cfg ->matrix[4],cfg ->matrix[5],cfg ->matrix[6],cfg ->matrix[7],cfg ->matrix[8]);
    }
    cfg->y_gain_exp = 0;
    //box_sbx;box_sby;box_sw;box_sh

    if (cfg->box_mode == 1) {
        cfg->src_box_x = rand()%20+1;//should be fix
        cfg->src_box_y = rand()%20+1;
        cfg->dst_box_x = rand()%20+1;
        cfg->dst_box_y = rand()%20+1;
    }else{
        cfg->src_box_x = 0;//rand()%20+1;//should be fix
        cfg->src_box_y = 0;
        cfg->dst_box_x = 0;
        cfg->dst_box_y = 0;
    }
    //zero_point info
    cfg->zero_point = 0x20;//should be fix
    //line box info
    radom_cfg->frmc_line_sbx = rand()%cfg->src_box_w + 1;
    radom_cfg->frmc_line_sby = rand()%cfg->src_box_h + 1;
    radom_cfg->frmc_line_sw = rand()%255 +1;
    radom_cfg->frmc_line_sh = rand()%255 +1;
#else //BS_RANDOM
    mode_sel = 6;
    set_run_mode(mode_sel_e[mode_sel], cfg, radom_cfg->is_first_frame);
    //frmc_bw
    uint32_t src_format = cfg->src_format & 0x1;
    uint32_t dst_format = cfg->dst_format & 0x1;
    uint32_t bpp_mode = (cfg->src_format >> 5) & 0x3;

    cfg->src_box_w = 4;
    cfg->src_box_h = 4;
    cfg->dst_box_w = 4;
    cfg->dst_box_h = 4;
    cfg->box_num = 1;

    cfg->src_box_x = 0;
    cfg->src_box_y = 0;
    cfg->dst_box_x = 0;
    cfg->dst_box_y = 0;

    radom_cfg->frmc_line_sbx = 0;
    radom_cfg->frmc_line_sby = 0;
    radom_cfg->frmc_line_sw = 4;
    radom_cfg->frmc_line_sh = 4;

    cfg->zero_point = 0x0;
    if(cfg->affine){
        dm0 = 1;//1 scale_x
        dm1 = 0;//1 skew_x
        dm2 = 0;//-2 trans_x
        dm3 = 0;//1 skew_y
        dm4 = 1;//1 scale_y
        dm5 = 0;//-2 trans_y
        dm6 = 0;//1 perspect0
        dm7 = 0;//-1 perspect1
        dm8 = 1;//2 perspect02

        cfg ->matrix[0] = dm0 * 65536;
        cfg ->matrix[1] = dm1 * 65536;
        cfg ->matrix[2] = dm2 * 65536;
        cfg ->matrix[3] = dm3 * 65536;
        cfg ->matrix[4] = dm4 * 65536;
        cfg ->matrix[5] = dm5 * 65536;
        cfg ->matrix[6] = dm6 * 65536;
        cfg ->matrix[7] = dm7 * 65536;
        cfg ->matrix[8] = dm8 * 65536;

        //cfg ->matrix[0] = 193381;//317545;//324035;
        //cfg ->matrix[1] = -29531;//-86870;//56009;
        //cfg ->matrix[2] = 2215284;//7223682;//-2741141
        //cfg ->matrix[3] = 3284;//117122;//-77409
        //cfg ->matrix[4] = 169816;//438372;//327681
        //cfg ->matrix[5] = 240749;//-6210136;//2014766
        //cfg ->matrix[6] = -15;//192;//40
        //cfg ->matrix[7] = -289;//169;//-19
        //cfg ->matrix[8] = 65022;//65723;//65094
        printf("persp:0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",cfg ->matrix[0],cfg ->matrix[1],cfg ->matrix[2],cfg ->matrix[3],cfg ->matrix[4],cfg ->matrix[5],cfg ->matrix[6],cfg ->matrix[7],cfg ->matrix[8]);

    }

    //
    cfg->y_gain_exp = 0;
#endif
    if(cfg->affine==1)
        perspective_mono_extreme(cfg);

    set_frmc_src_info(cfg);

    set_frmc_dst_info(cfg, radom_cfg);

    if(mode_sel_e[mode_sel] == bresize_chn2chn | mode_sel_e[mode_sel] == bresize_nv2chn){
        set_bresize_matrix(cfg, radom_cfg);
    }
    else if(mode_sel_e[mode_sel] == lresize_chn2chn | mode_sel_e[mode_sel] == lresize_nv2chn){
        set_lresize_box_info(cfg, radom_cfg);
    }

    src_image_init(cfg);

    printf_info(cfg);
}

#ifdef CHIP_SIM_ENV
#define TEST_NAME  t_bscaler
#else
#define TEST_NAME  main
#endif
int TEST_NAME()
{
#if HW_OPEN
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
#endif

    uint32_t case_no_check=0;
    uint32_t case_time_out=0;
    uint32_t seed_idx = 0;
    uint32_t seed_num = 500;//000;

    uint32_t seed_time;
    uint32_t seed;
    seed_time = (unsigned)time(NULL);
    //seed_time = 1592788786;
    do {
        uint32_t case_no_check=0;
        uint32_t case_time_out=0;
        seed = seed_time + seed_idx;
        //seed = seed_time;
        srand(seed);
        printf("current SEED is %ld\n",seed);
        uint32_t calc_fail_flag=0;

        bs_hw_once_cfg_s  frmc_cfg_inst;
        bs_hw_once_cfg_s  *frmc_cfg = &frmc_cfg_inst;

        bs_hw_once_cfg_s  frmc_mdl_inst;
        bs_hw_once_cfg_s  *frmc_mdl = &frmc_mdl_inst;

        frmc_random_s  radom_cfg_inst;
        frmc_random_s  *radom_cfg = &radom_cfg_inst;
        radom_cfg->flog = NULL;

        //SOFT RESET
#if HW_OPEN
        bscaler_softreset_set();
#endif
        radom_cfg->is_first_frame = 1;//for chain
        bscaler_frmc_init(frmc_cfg, radom_cfg);

        uint32_t  frmt_ckg_set =0;
        uint32_t  frmc_ckg_set =0;
#if HW_OPEN
        bscaler_clkgate_mask_set(frmt_ckg_set, frmc_ckg_set);
#endif
        case_no_check = error_seed_collect(frmc_cfg, radom_cfg, seed);

        if(case_no_check==0){//case should be check

            nv2bgr_random(frmc_cfg);
            uint32_t nv2bgr_order = (frmc_cfg->dst_format/*src_format*/ >> 1) & 0xF;

            char log_info[100];
#if HW_OPEN
            //configure
            // bscaler_param_cfg(frmc_cfg->coef,frmc_cfg->offset,nv2bgr_order);
            bscaler_frmc_cfg(frmc_cfg);

            int fail_flag=0;
            int time_out=0;

            //polling
            while ((bscaler_read_reg(BSCALER_FRMC_CTRL, 0)&0x3F) != 0){
                fail_flag++;
                if(fail_flag >= 0x24000000){
                    time_out=1;
                    break;
                }
            };

            printf("DEBUG:FRMC_CTRL:0x%x\n",bscaler_read_reg(BSCALER_FRMC_CTRL, 0));
            if(time_out){
                case_time_out =1;
                printf("Noting:#####TIMEOUT####!\n");
                fseek(radom_cfg->flog,0,SEEK_END);
                snprintf(log_info,100,"timeout :seed is %d\n",seed);
                fwrite(log_info,strlen(log_info),1,radom_cfg->flog);
                fclose(radom_cfg->flog);
#ifdef EYER_SIM_ENV
                eyer_stop();
#endif

                return 0;
            }
#endif//hw_open
      //c model malloc
      //for mdl
            memcpy(frmc_mdl,frmc_cfg,sizeof(bs_hw_once_cfg_s));
            uint32_t src_format = frmc_cfg->src_format & 0x1;
            uint32_t dst_format = frmc_cfg->dst_format & 0x1;
            uint32_t bpp_mode = (frmc_cfg->src_format >> 5) & 0x3;

            uint8_t *md_base_dst[64] ;
            uint8_t bid_i;
            for(bid_i=0; bid_i<64;bid_i++){
                md_base_dst[bid_i] <= NULL;
            }
            uint8_t* md_ybase_dst=NULL ;
            uint8_t* md_cbase_dst=NULL ;
            if (frmc_cfg->dst_format == 0) { //nv12
                uint32_t  md_ybase_len = frmc_cfg->dst_line_stride*frmc_cfg->dst_box_h;
                uint32_t  md_cbase_len = frmc_cfg->dst_line_stride*frmc_cfg->dst_box_h/2;
                md_ybase_dst =(uint8_t*) bscaler_malloc(1,md_ybase_len );
                md_cbase_dst =(uint8_t*) bscaler_malloc(1,md_cbase_len );
            }else{
                uint32_t  md_ybase_len=frmc_cfg->dst_line_stride*frmc_cfg->dst_box_h*frmc_cfg->box_num ;
                md_ybase_dst = (uint8_t*)bscaler_malloc(1,md_ybase_len);
            }

            uint32_t bid_x,bid_y;
            if(frmc_cfg->box_mode){//box_mode
                md_base_dst[0] = md_ybase_dst;
                if(!dst_format)
                    md_base_dst[1] = md_cbase_dst;
            }else{//line_mode
                uint32_t index=0;
                for(bid_y=0; bid_y<radom_cfg->frmc_bs_ynum; bid_y++){
                    for(bid_x=0; bid_x<radom_cfg->frmc_bs_xnum; bid_x++){
                        if(index>frmc_cfg->box_num){
                            break;
                        }else{
                            if(radom_cfg->frmc_dst_store_mode==0){
                                md_base_dst[index] = md_ybase_dst + index*frmc_cfg->dst_box_h*frmc_cfg->dst_line_stride;
                            }else{
                                md_base_dst[index] = md_ybase_dst + bid_y*frmc_cfg->dst_box_h*frmc_cfg->dst_line_stride+bid_x*radom_cfg->dst_line_stride1;
                            }
                        }
                        index++;
                    }
                }
            }
#if 1
            frmc_mdl->isum = 0;
            frmc_mdl->osum = 0;
            uint8_t index;
            uint8_t box_num;

            if(!dst_format){
                box_num = 2;
            }else{
                box_num = frmc_mdl->box_num;
            }
            for(index=0; index < box_num; index++){
                frmc_mdl->dst_base[index] = md_base_dst[index];
            }
            bscaler_mdl(frmc_mdl);
#endif
            //check

            uint8_t *bs_dst=NULL;
            uint8_t *md_dst=NULL;
            /*      for(bid_i=0;bid<frmc_cfg-> box_num;bid_i++){
                    md_dst[bid_i] = (uint8_t *)md_base_dst[bid_i];
                    bs_dst[bid_i] = (uint8_t *)frmc_cfg->dst_base[bid_i];
                    }*/
            uint8_t *bs_ydst;
            uint8_t *md_ydst;
            md_ydst = md_base_dst[0];
            bs_ydst = (uint8_t *)frmc_cfg->dst_base[0];
            uint8_t *bs_cdst;
            uint8_t *md_cdst;
            md_cdst = md_base_dst[1];
            bs_cdst = (uint8_t *)frmc_cfg->dst_base[1];

            printf("check data here!\n");

            uint32_t byte_per_pix = 1<<(2+bpp_mode);
            // uint32_t wrd_num =byte_per_pix/4;
            uint32_t wrd_num =byte_per_pix;
            uint32_t bnum_i=0;
            uint32_t bnum_j=0;
            uint32_t i,j,cnum;

#if HW_OPEN
#if 1
            //checksum
            uint32_t dchk_isum = bscaler_read_reg(BSCALER_FRMC_ISUM, 0);
            uint32_t dchk_osum = bscaler_read_reg(BSCALER_FRMC_OSUM, 0);
            uint32_t mchk_isum = frmc_mdl->isum;
            uint32_t mchk_osum = frmc_mdl->osum;
            /*if (mchk_isum != dchk_isum) {
                fprintf(stderr, "error: frmc ISUM failed: %0x -> %0x \n", mchk_isum ,dchk_isum);
                calc_fail_flag =1;
            }else{
                printf("frmc ISUM sucess: %0x -> %0x \n", mchk_isum ,dchk_isum);
                }*/
            if (mchk_osum != dchk_osum) {
                fprintf(stderr, "error: frmc OSUM failed: %0x -> %0x \n", mchk_osum ,dchk_osum);
                calc_fail_flag =1;
            }else{
                printf("frmc OSUM sucess: %0x -> %0x \n", mchk_osum ,dchk_osum);
            }
#endif
#endif

#if 1
            for(bnum_i=0;bnum_i<frmc_cfg->box_num;bnum_i++){//bnum_i
                for(j=0;j<frmc_cfg->dst_box_h;j++){
                    for(i=0;i<frmc_cfg->dst_box_w;i++){
                        if(dst_format==0){//dst:nv12
                            uint32_t y_idx = j*frmc_cfg->dst_line_stride+i;
                            if((md_ydst[y_idx]==bs_ydst[y_idx])){
                                //printf("Y: j(0x%x),i(0x%x),sucess:md_ydst(%p:0x%x),bs_ydst(%p:0x%x)\n",j,i,&md_ydst[y_idx],md_ydst[y_idx],&bs_ydst[y_idx],bs_ydst[y_idx]);
                            }
                            else{
                                printf("error:Y: j(0x%x),i(0x%x),failed:md_ydst(%p:0x%x),bs_ydst(%p:0x%x)\n",j,i,&md_ydst[y_idx],md_ydst[y_idx],&bs_ydst[y_idx],bs_ydst[y_idx]);
                                calc_fail_flag =1;
                                break;
                            }
                            //check uv
                            uint32_t c_idx = j/2*frmc_cfg->dst_line_stride+(i/2)*2;
                            if((md_cdst[c_idx]==bs_cdst[c_idx])&(md_cdst[c_idx+1]==bs_cdst[c_idx+1])){
                                //printf("j(0x%x),i(0x%x),sucess:->U:md_cdst(%p:0x%x),bs_cdst(%p:0x%x),->V:md_cdst(%p:0x%x),bs_cdst(%p:0x%x)\n",j,i,&md_cdst[c_idx],md_cdst[c_idx],&bs_cdst[c_idx],bs_cdst[c_idx],&md_cdst[c_idx+1],md_cdst[c_idx+1],&bs_cdst[c_idx+1],bs_cdst[c_idx+1]);
                            }else{
                                printf("error:j(0x%x),i(0x%x),failed:->U:md_cdst(%p:0x%x),bs_cdst(%p:0x%x),->V:md_cdst(%p:0x%x),bs_cdst(%p:0x%x)\n",j,i,&md_cdst[c_idx],md_cdst[c_idx],&bs_cdst[c_idx],bs_cdst[c_idx],&md_cdst[c_idx+1],md_cdst[c_idx+1],&bs_cdst[c_idx+1],bs_cdst[c_idx+1]);
                                calc_fail_flag =1;
                                break;
                            }
                        }//dst:nv12
                        else{//dst:bgr/chn
                            for(cnum=0; cnum<wrd_num;cnum++){
                                md_dst = (uint8_t*)md_base_dst[bnum_i];
                                bs_dst = (uint8_t*)frmc_cfg->dst_base[bnum_i];
                                uint32_t idx = j*frmc_cfg->dst_line_stride+ i*wrd_num+cnum;
                                if(md_dst[idx] == bs_dst[idx]){
                                    // printf("box_i(0x%x),j(0x%x),i(0x%x) ,num(0x%x),sucess:md_addr(%p):md_data(%x),bs_addr(%p),bs_data(%x)!\n",bnum_i,j,i,cnum,&md_dst[idx],md_dst[idx],&bs_dst[idx],bs_dst[idx]);
                                } else {
                                    printf("error: box_i(0x%x),j(0x%x),i(0x%x) ,num(0x%x),failed:md_addr(%p):md_data(%x),bs_addr(%p),bs_data(%x)!\n",bnum_i,j,i,cnum,&md_dst[idx],md_dst[idx],&bs_dst[idx],bs_dst[idx]);
                                    calc_fail_flag =1;
                                    break;
                                }
                            }//cnum
                        }//dst:bgr/chn
                    }//dst_w
                }//dst_h
            }//bnum_i
#endif

            if((calc_fail_flag!=0)){
                printf("frmc simulation failed!\n");
                fseek(radom_cfg->flog,0,SEEK_END);
                snprintf(log_info,100,"bscaler error:seed is %d\n",seed);
                printf("%p,%d,%p\n",log_info,strlen(log_info),radom_cfg->flog);
                fwrite(log_info,strlen(log_info),1,radom_cfg->flog);
            }else{
                printf("simulation is passed!\n");
            }
            if (md_ybase_dst != NULL) {
                bscaler_free(md_ybase_dst);
                md_ybase_dst = NULL;
            }
            if (md_cbase_dst != NULL) {
                bscaler_free(md_cbase_dst);
                md_cbase_dst = NULL;
            }
        }//case should be check
        printf("bscaler_frmc_free............................\n");
        bscaler_frmc_free(frmc_cfg, radom_cfg);
        if (radom_cfg->flog != NULL) {
            fclose(radom_cfg->flog);
            radom_cfg->flog = NULL;
        }
        seed_idx ++;
        printf("***end:seed_idx(case_num)(%d)***\n", seed_idx);
    } while(seed_idx < seed_num);
#ifdef EYER_SIM_ENV
#if HW_OPEN
    eyer_stop();
#endif
#endif
    return 0;
}
#ifdef CHIP_SIM_ENV
TEST_MAIN(TEST_NAME);
#endif
