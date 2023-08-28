/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : bsc_reset.c
 * Authors    : jmqi@ingenic.st.jz.com
 * Create Time: 2020-08-25:18:05:13
 * Description:
 *
 */

#include "bscaler_hal.h"
#include "bscaler_api.h"
#include "bscaler_mdl.h"
#include "bscaler_wrap.h"
#include "random_api.h"
#ifdef CSE_SIM_ENV
#include "aie_mmap.h"
#else
#include "platform.h"
#endif

int main()
{
    printf("BSC reset start\n");
#ifdef EYER_SIM_ENV
    eyer_system_ini(0);// eyer environment initialize.
    eyer_reg_segv();   // simulation error used
    eyer_reg_ctrlc();  // simulation error used
#elif CSE_SIM_ENV
    bscaler_mem_init();
#endif

    printf("wait for reset finish!\n");
    bscaler_frmc_soft_reset();
    printf("BSC reset finish!\n");

#ifdef EYER_SIM_ENV
    eyer_stop();
#endif
}
