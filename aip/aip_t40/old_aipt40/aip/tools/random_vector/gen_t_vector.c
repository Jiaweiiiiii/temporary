/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : gen_t_vector.c
 * Authors    : jmqi@joshua
 * Create Time: 2020-06-26:16:15:14
 * Description:
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bscaler_hal.h"
#include "random_vector.h"
#include "bscaler_mdl.h"
#include "dump_t_vector.h"
#include "image_process.h"

int main(int argc, char** argv)
{
    char *note = "Generate by gen_t_vector.";

    uint32_t seed = (uint32_t)time(NULL);
    if (argc > 1)
        seed = atoi(argv[1]);
    //seed = 0x5ef84e72;
    printf("seed = 0x%08x\n", seed);
    srand(seed);

    bst_hw_once_cfg_s cfg;
    bst_random(&cfg);
    //update the dst wxh
    cfg.dst_w = cfg.src_w;
    cfg.dst_h = cfg.src_h;

    bst_mdl(&cfg);
    cfg.isum = get_bst_isum();
    cfg.osum = get_bst_osum();

    dump_bst_cfg_info(&cfg);

    //dump vector
    dump_t_vector(&cfg, seed, note);

    if (cfg.dst_format == 1)
        stbi_write_png("out.png", cfg.dst_w, cfg.dst_h, 4, cfg.dst_base0, cfg.dst_w * 4);

    return 0;
}
