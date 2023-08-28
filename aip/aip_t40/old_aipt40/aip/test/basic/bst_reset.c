/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : bst_reset.c
 * Authors    : jmqi@ingenic.st.jz.com
 * Create Time: 2020-08-25:18:05:13
 * Description:
 *
 */

#include "bscaler_hal.h"
#include "bscaler_api.h"
#ifdef CSE_SIM_ENV
#include "aie_mmap.h"
#else
#include "platform.h"
#endif

int main()
{
    pf_printf("BST reset start\n");
#ifdef EYER_SIM_ENV
    eyer_system_ini(0);// eyer environment initialize.
    eyer_reg_segv();   // simulation error used
    eyer_reg_ctrlc();  // simulation error used
#elif CSE_SIM_ENV
    int ret = 0;
    bs_version();
    nna_cache_attr_t desram_cache_attr = NNA_UNCACHED_ACCELERATED;
    nna_cache_attr_t oram_cache_attr = NNA_UNCACHED_ACCELERATED;
    nna_cache_attr_t ddr_cache_attr = NNA_CACHED;
    if ((ret = __aie_mmap(0x4000000, 1, desram_cache_attr, oram_cache_attr, ddr_cache_attr)) == 0) { //64MB
        printf ("nna box_base virtual generate failed !!\n");
        return 0;
    }
#endif

    pf_printf("wait for reset finish!\n");
    bscaler_frmt_soft_reset();
    pf_printf("BST reset finish!\n");
#ifdef EYER_SIM_ENV
    eyer_stop();
#endif

}
