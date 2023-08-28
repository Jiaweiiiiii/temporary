/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : gen_vector.c
 * Authors    : jmqi@joshua
 * Create Time: 2020-06-23:11:25:08
 * Description: only run @x86
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
    char *note = "Generate by gen_c_vector.";

    uint32_t seed = (uint32_t)time(NULL);
    //seed = 0x5efc49f3;//0x5efad407;
    if (argc > 1)
        seed = atoi(argv[1]);
    printf("NOT:seed = 0x%08x!!!\n", seed);
    srand(seed);

    bsc_hw_once_cfg_s cfg0;
    int mode_sel = 4;//rand() % 10;
    ret = (int)bsc_random(&cfg0, mode_sel);
    if (ret) {
        printf("error:cfg failed!\n");
        return ret;
    }else{
    }

    bsc_mdl(&cfg0);
    cfg0.isum = get_bsc_isum();
    cfg0.osum = get_bsc_osum();

    dump_bsc_cfg_info(&cfg0);

    //dump vector
    dump_c_vector(&cfg0, seed, note);
    return 0;
}
