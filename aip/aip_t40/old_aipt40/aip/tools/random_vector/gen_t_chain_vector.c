/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : gen_t_chain_vector.c
 * Authors    : lzhu@abel.ic.jz.com
 * Create Time: 2020-08-20:10:01:08
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
    char *note = "Generate by gen_t_chain_vector.";

    uint32_t seed = (uint32_t)time(NULL);
    //seed = 0x5efc49f3;//0x5efad407;
    if (argc > 1)
        seed = atoi(argv[1]);
    printf("NOT:seed = 0x%08x!!!\n", seed);
    srand(seed);

    uint32_t frame = 1;
    bst_hw_once_cfg_s *frmt_cfg_inst = (bst_hw_once_cfg_s*)malloc
        (frame * (sizeof(bst_hw_once_cfg_s)));

    for (int n = 0; n < frame; n ++ ) {
        bst_hw_once_cfg_s *frmt_cfg = &(frmt_cfg_inst[n]);
        bst_random(frmt_cfg);
        frmt_cfg->dst_w = frmt_cfg->src_w;
        frmt_cfg->dst_h = frmt_cfg->src_h;
        //dump_bst_cfg_info(frmt_cfg);
    }

    bs_chain_ret_s chain_info;
    chain_info = bscaler_chain_t_init(frame, frmt_cfg_inst);

    for(int n = 0; n < frame; n ++){
        bst_hw_once_cfg_s *frmt_mdl = &frmt_cfg_inst[n];
        bst_mdl(frmt_mdl);
        frmt_mdl->isum = get_bst_isum();
        frmt_mdl->osum = get_bst_osum();
    }

    dump_t_chain_vector(frame, frmt_cfg_inst, &chain_info, seed, note);

    free(frmt_cfg_inst);
    free(chain_info.bs_ret_base);

    return 0;
}
