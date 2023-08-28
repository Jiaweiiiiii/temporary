/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : t_bist.c
 * Authors    : lzhu@abel.ic.jz.com
 * Create Time: 2020-07-31:15:21:58
 * Description:
 *
 */

#ifndef BS_BIST_CPU__C
#define BS_BIST_CPU__C
#ifdef SRC_CPU
#include <instructions.h>
#include <test.h>
#include <exp_supp.h>
#include <debug.h>
#include <stdio.h>
#include <stdlib.h>
#else
#define CPU_AHB
#include "ingenic_cpu.h"
#endif

int bs_bist_cpu() {
    unsigned int test_finish, bist_fail, bist_end;
    svWaitTime(100);

    do {
        svWaitTime(100);
        bist_fail   = svHdlRead("soc_top.bscaler_soc_dut.bist_test.bist_fail");
        bist_end    = svHdlRead("soc_top.bscaler_soc_dut.bist_test.bist_end");
        test_finish = svHdlRead("soc_top.bscaler_soc_dut.bist_test.test_finish");
    }while(!test_finish && !bist_fail);

    if(bist_fail == 1){
        return 1;
    }
    else{
        return 0;
    }
}
TEST_MAIN(bs_bist_cpu)
#endif
