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

#include "bscaler_mdl.h"
#include "bscaler_hal.h"
#include "bscaler_random.h"

void bscaler_frmc_init(bsc_hw_once_cfg_s *cfg, frmc_random_s *radom_cfg,
                       uint8_t mode_sel)
{
    radom_cfg->frmc_cfg_fail = 0;
    radom_cfg->is_perspective = 0;
    cfg->box_bus = 0;
    cfg->ibufc_bus = 0;
    cfg->obufc_bus = 0;
    radom_cfg->frmc_dst_store_mode = 0;//0-->bulk 1->joint
    cfg->irq_mask = 1;
    init_base(cfg, radom_cfg);
    //uint8_t mode_sel =0;
    frmc_mode_sel_e mode_sel_e[7] = {bresize_chn2chn, bresize_nv2chn,
                                     lresize_chn2chn, lresize_nv2chn,
                                     perspective_bgr2bgr, perspective_nv2bgr,
                                     perspective_nv2nv};
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
    set_run_mode(mode_sel_e[mode_sel], cfg, radom_cfg->is_first_frame);
    //frmc_bw
    uint32_t src_format = cfg->src_format & 0x1;
    uint32_t dst_format = cfg->dst_format & 0x1;
    uint32_t bpp_mode = (cfg->src_format >> 5) & 0x3;

    //src info
    cfg->src_box_h = rand()%1024+1;//rand()%1023+1;
    if(bpp_mode == 0){
        cfg->src_box_w = random()%2047 + 1;//rand()%2047+1;//1->2048
    }else if(bpp_mode == 1){
        cfg->src_box_w = random()%1024 + 1;//rand()%1023+1;//1->>1024
    }else if(bpp_mode == 2){
        cfg->src_box_w = random()%512 + 1;//rand()%511+1;//1->>512
    }else{
        cfg->src_box_w = random()%256 + 1;//rand()%255+1;//1->>256
    }
    if((src_format == 0) | (bpp_mode == 0)){
        cfg->src_box_w = ((cfg->src_box_w >> 1) + 1) << 1; //align 2pixel
        cfg->src_box_h = ((cfg->src_box_h >> 1) + 1) << 1; //align 2pixel
    } else {
        cfg->src_box_w = cfg->src_box_w;
        cfg->src_box_h = cfg->src_box_h;
    }
    //dst info
    if(cfg->affine==0 ){
        if(cfg->box_mode==1){
            cfg->dst_box_w = rand()%64+1;//bresize:0-64
            cfg->dst_box_h = rand()%256+1;
        }else{
            cfg->dst_box_w = rand()%256+1;
            cfg->dst_box_h = rand()%256+1;
        }
    }else{
        if(dst_format ==0){//nv12 should be even
            cfg->dst_box_w = rand()%63+1;
            cfg->dst_box_h = rand()%63+1;
            cfg->dst_box_w = ((cfg->dst_box_w >> 1) + 1) << 1;
            cfg->dst_box_h = ((cfg->dst_box_h >> 1) + 1) << 1;
        }else{
            cfg->dst_box_w = rand()%64+1;
            cfg->dst_box_h = rand()%64+1;
            cfg->dst_box_w = cfg->dst_box_w ;
            cfg->dst_box_h = cfg->dst_box_h ;
        }
    }
    //box_num info
    if (cfg->box_mode == 1) {//box_mode
        cfg->box_num = 1;
    }else{
        cfg->box_num = rand()%64 +1;
    }
    printf("box_num:0x%x\n",cfg->box_num);
    uint8_t is_affine = 0;
    uint8_t is_affine_mux = rand()%2;
    if(cfg->affine){
        printf("AFFINE HERE\n");
        cfg ->matrix[0] = rand()%0xFFFFFFFF ;
        cfg ->matrix[1] = rand()%0xFFFFFFFF ;
        cfg ->matrix[2] = rand()%0xFFFFFFFF ;
        cfg ->matrix[3] = rand()%0xFFFFFFFF ;
        cfg ->matrix[4] = rand()%0xFFFFFFFF ;
        cfg ->matrix[5] = rand()%0xFFFFFFFF ;
        if(is_affine_mux){
            printf("matrix is affine here!\n");
            cfg ->matrix[6] = 0;
            cfg ->matrix[7] = 0;
            cfg ->matrix[8] = 65536;
        }else{
            cfg ->matrix[6] = rand()%0xFFFFFFFF ;
            cfg ->matrix[7] = rand()%0xFFFFFFFF ;
            cfg ->matrix[8] = rand()%0xFFFFFFFF ;
        }
        is_affine = (cfg ->matrix[6]==0) & (cfg ->matrix[7]==0) & (cfg ->matrix[8]==65536);
        radom_cfg -> is_perspective = !is_affine;
        printf("persp:0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",cfg ->matrix[0],cfg ->matrix[1],cfg ->matrix[2],cfg ->matrix[3],cfg ->matrix[4],cfg ->matrix[5],cfg ->matrix[6],cfg ->matrix[7],cfg ->matrix[8]);
    }
    cfg->y_gain_exp = 0;
    //box_sbx;box_sby;box_sw;box_sh

    if (cfg->box_mode == 1) {
        cfg->src_box_x = rand()%20+1;//fix me
        cfg->src_box_y = rand()%20+1;
        cfg->dst_box_x = rand()%20+1;
        cfg->dst_box_y = rand()%20+1;
    }else{
        cfg->src_box_x = 0;//fix me
        cfg->src_box_y = 0;
        cfg->dst_box_x = 0;
        cfg->dst_box_y = 0;
    }
    //zero_point info
    cfg->zero_point = 0x20;//fix me
    //line box info
    radom_cfg->frmc_line_sbx = rand()%cfg->src_box_w + 1;
    radom_cfg->frmc_line_sby = rand()%cfg->src_box_h + 1;
    radom_cfg->frmc_line_sw = rand()%256 +1;
    radom_cfg->frmc_line_sh = rand()%256 +1;
#else //BS_RANDOM
    //mode_sel = 4;
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
    uint32_t case_no_check=0;
    uint32_t case_time_out=0;
    uint32_t seed_idx = 0;
    uint32_t seed_num = 100;//15000;

    uint32_t seed_time;
    uint32_t seed;
    seed_time = (unsigned)time(NULL);
    //seed_time = 1595321364;//1595321361;
    do{
        uint32_t case_no_check=0;
        uint32_t case_time_out=0;
        seed = seed_time + seed_idx;
        srand(seed);
        printf("current SEED is %ld,seed_time(%ld),seed_idx(%d)\n",seed,seed_time,seed_idx);
        char log_info[100];
        FILE *flog = NULL;

        uint32_t  frmt_ckg_set =0;
        uint32_t  frmc_ckg_set =0;
        bscaler_clkgate_mask_set(frmt_ckg_set,frmc_ckg_set);
        uint32_t frame = rand()%6 + 1;
        printf("----chain start: frame = %d----\n",frame);
        /*bsc_hw_once_cfg_s  frmc_cfg_inst[frame];
          bsc_hw_once_cfg_s  frmc_mdl_inst[frame];

          frmc_random_s  radom_cfg_inst[frame];*/
        bsc_hw_once_cfg_s *frmc_cfg_inst = (bsc_hw_once_cfg_s*)malloc(frame * (sizeof(bsc_hw_once_cfg_s)));
        bsc_hw_once_cfg_s *frmc_mdl_inst = (bsc_hw_once_cfg_s*)malloc(frame * (sizeof(bsc_hw_once_cfg_s)));
        frmc_random_s *radom_cfg_inst = (frmc_random_s*)malloc(frame * (sizeof(frmc_random_s)));

        int n;
        uint8_t mode_sel =0;
        mode_sel= rand()%7;//one chain have the cofirm mode
        //mode_sel= rand()%3 + 4;
        for(n=0; n<frame; n++){
            frmc_random_s *radom_cfg = &(radom_cfg_inst[n]);
            bsc_hw_once_cfg_s *frmc_cfg = &(frmc_cfg_inst[n]);
            radom_cfg->is_first_frame =0;//only for chain
            if(n==0){
                radom_cfg -> is_first_frame =1;
            }
            bscaler_frmc_init(frmc_cfg, radom_cfg, mode_sel);
            if(radom_cfg -> frmc_cfg_fail==1){ //bypass the cfg failed
                n=n-1;
                printf("###frmc cfg failed,recfg again###\n");
            }
        }

        bs_chain_ret_s  chain_info;
        chain_info = bscaler_chain_init(frame, frmc_cfg_inst, radom_cfg_inst);

        uint32_t chain_base = chain_info.bs_ret_base;
        uint32_t chain_len = chain_info.bs_ret_len;

        uint32_t nv2bgr_order = ((&frmc_cfg_inst[0])->dst_format/*src_format*/ >> 1) & 0xF;
        nv2bgr_random(&frmc_cfg_inst[0]);
        //union for param
        uint8_t ii;
        for(n=1; n<frame; n++){
            (&frmc_cfg_inst[n])->nv2bgr_alpha = (&frmc_cfg_inst[0])->nv2bgr_alpha;
            for(ii=0;ii<9;ii++){
                (&frmc_cfg_inst[n])->coef[ii] = (&frmc_cfg_inst[0])->coef[ii];
                if(ii<2){
                    (&frmc_cfg_inst[n])->offset[ii] = (&frmc_cfg_inst[0])->offset[ii];
                }
            }
        }
        printf("----chain init end!----\n");

        //start for hw
        bscaler_param_cfg((&frmc_cfg_inst[0])->coef, (&frmc_cfg_inst[0])->offset, (&frmc_cfg_inst[0])->nv2bgr_alpha, nv2bgr_order);
        printf("nv2bgr_order:0x%x\n",nv2bgr_order);
        //cfg chain bus

        bscaler_write_reg(BSCALER_FRMC_ISUM, 0); //clear summary
        bscaler_write_reg(BSCALER_FRMC_OSUM, 0); //clear summary

        bscaler_write_reg(BSCALER_FRMC_CHAIN_LEN, chain_len);
        bscaler_write_reg(BSCALER_FRMC_CHAIN_BASE, chain_base);
        printf("debug:%p,0x%x\n",chain_base,chain_len);
        uint8_t  frmc_chain_irq_mask =1;
        uint8_t  frmc_c_timeout_mask =1;
        //start chain
        uint32_t chain_bus = 0;
        bscaler_write_reg(BSCALER_FRMC_CHAIN_CTRL,(frmc_chain_irq_mask & 0x1)<<3 |
                          (frmc_c_timeout_mask & 0x1)<<5 |
                          chain_bus<<1 |
                          1<<0);//start hw
        printf("----hw start----\n");

        //polling 1
        printf("----polling----\n");
        while ((bscaler_read_reg(BSCALER_FRMC_CHAIN_CTRL, 0) & 0x1) != 0){
        }
        //for mdl init
        for(n=0; n<frame; n++){
            bsc_hw_once_cfg_s *frmc_cfg = &frmc_cfg_inst[n];
            bsc_hw_once_cfg_s *frmc_mdl = &frmc_mdl_inst[n];
            frmc_random_s  *radom_cfg = &radom_cfg_inst[n];
            memcpy(frmc_mdl,frmc_cfg,sizeof(bsc_hw_once_cfg_s));
            bscaler_mdl_dst_malloc(radom_cfg, frmc_cfg);

            uint32_t dst_format = frmc_mdl->dst_format & 0x1;
            uint8_t index;
            uint8_t box_num;
            if(!dst_format){
                box_num = 2;
            }else{
                box_num = frmc_mdl->box_num;
            }

            for(index=0; index < box_num; index++){
                frmc_mdl->dst_base[index] = radom_cfg->md_base_dst[index];
            }

        }

        //start for model
        for(n=0; n<frame; n++){
            bsc_hw_once_cfg_s *frmc_mdl = &frmc_mdl_inst[n];
            frmc_random_s  *radom_cfg = &radom_cfg_inst[n];
            bscaler_mdl(frmc_mdl);
        }

        //for check
        printf("----check result----\n");
        uint32_t calc_fail_flag = 0;
        uint32_t errnum=0;;
        for(n=0; n<frame; n++){
            frmc_random_s  *radom_cfg = &radom_cfg_inst[n];
            bsc_hw_once_cfg_s *frmc_cfg = &frmc_cfg_inst[n];

            calc_fail_flag = bscaler_result_check(&frmc_cfg_inst[n],&frmc_mdl_inst[n],&radom_cfg_inst[n]);

            if(calc_fail_flag == 1){
                errnum++;
                printf("----frame:0x%x check FAILED---\n",n);
                calc_fail_flag =0;
            }
        }

        if((errnum!=0)){
            flog = fopen("./log/bscaler_frmc_chain.log","a+");
            fseek(flog,0,SEEK_END);
            snprintf(log_info,100,"bscaler error:seed is %d\n",seed);
            printf("%p,%d,%p\n",log_info,strlen(log_info),flog);
            fwrite(log_info,strlen(log_info),1,flog);
            printf("frmc simulation failed!\n");
        }else{
            printf("simulation is passed!\n");
        }

        //for free
        if (flog != NULL) {
            printf("malloc test2\n");
            fclose(flog);
            flog = NULL;
        }
        printf("bscaler_frmc_free............................\n");
        for(n=0; n<frame; n++){
            bsc_hw_once_cfg_s *frmc_cfg = &frmc_cfg_inst[n];
            bsc_hw_once_cfg_s *frmc_mdl = &frmc_mdl_inst[n];
            frmc_random_s  *radom_cfg = &radom_cfg_inst[n];
            if (radom_cfg->md_ybase_dst != NULL) {
                printf("malloc test0:%p\n",radom_cfg->md_ybase_dst);
                bscaler_free(radom_cfg->md_ybase_dst);
                radom_cfg->md_ybase_dst = NULL;
                printf("test0 end\n");
            }
            if (radom_cfg->md_cbase_dst != NULL) {
                printf("malloc test1:%p\n",radom_cfg->md_cbase_dst);
                bscaler_free(radom_cfg->md_cbase_dst);
                radom_cfg->md_cbase_dst = NULL;
                printf("test1 end\n");
            }
            bscaler_frmc_free(frmc_cfg,radom_cfg);
        }
        bscaler_free(chain_base);
        if(frmc_cfg_inst != NULL){
            free(frmc_cfg_inst);
            frmc_cfg_inst = NULL;
        }
        if(frmc_mdl_inst != NULL){
            free(frmc_mdl_inst);
            frmc_mdl_inst = NULL;
        }
        if(radom_cfg_inst != NULL){
            free(radom_cfg_inst);
            radom_cfg_inst = NULL;
        }
        seed_idx ++;
        printf("***end:seed_idx(case_num)(%d)***\n", seed_idx);
    }while(seed_idx < seed_num);
#ifdef EYER_SIM_ENV
    eyer_stop();
#endif
    return 0;
}
#ifdef CHIP_SIM_ENV
TEST_MAIN(TEST_NAME);
#endif
