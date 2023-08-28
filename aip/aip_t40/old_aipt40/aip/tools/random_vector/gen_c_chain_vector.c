/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : gen_c_chain_vector.c
 * Authors    : lzhu@abel.ic.jz.com
 * Create Time: 2020-08-10:12:21:48
 * Description:
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MDL_ONLY
#include "bscaler_hal.h"
#include "random_vector.h"
#include "bscaler_mdl.h"
#include "dump_c_vector.h"

int main(int argc, char** argv)
{
    int ret = 0;
    char *note = "Generate by gen_c_chain_vector.";

    uint32_t seed = (uint32_t)time(NULL);
    //seed = 0x5efc49f3;//0x5efad407;
    if (argc > 1)
        seed = atoi(argv[1]);
    printf("NOT:seed = 0x%08x!!!\n", seed);
    srand(seed);

    uint32_t frame = 2;
    bsc_hw_once_cfg_s *frmc_cfg_inst = (bsc_hw_once_cfg_s*)malloc
        (frame * (sizeof(bsc_hw_once_cfg_s)));
    int mode_sel = 0;//rand() % 10;
    for (int n = 0; n < frame; n ++ ) {
        bsc_hw_once_cfg_s *frmc_cfg = &(frmc_cfg_inst[n]);
        ret = (int)bsc_random(frmc_cfg, mode_sel);
        if (ret == 1) {
            n=n-1;
            printf("###frmc cfg failed,recfg again###\n");
        }else{
            //dump_bsc_cfg_info(frmc_cfg);
        }
    }

    bs_chain_ret_s chain_info;
    chain_info = bscaler_chain_init(frame, frmc_cfg_inst);
    printf("base:0x%x,len:0x%x\n",chain_info.bs_ret_base,chain_info.bs_ret_len);
    //start model
    for(int n = 0; n < frame; n ++){
        bsc_hw_once_cfg_s *frmc_mdl = &frmc_cfg_inst[n];
        bsc_mdl(frmc_mdl);
        frmc_mdl->isum = get_bsc_isum();
        frmc_mdl->osum = get_bsc_osum();
    }

    //dump vector
    dump_c_chain_vector(frame, frmc_cfg_inst, &chain_info, seed, note);

    free(frmc_cfg_inst);
    free(chain_info.bs_ret_base);

    return 0;
}
