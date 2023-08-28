/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : read_all_reg.c
 * Authors    : jmqi@ingenic.st.jz.com
 * Create Time: 2020-08-25:18:05:13
 * Description:
 *
 */

#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>

#include "affine_whole_float.h"
#include "affine_whole_int.h"
#include "image_process.h"
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

#define USING_INTERRUPT 1

int main()
{

#ifdef EYER_SIM_ENV
    eyer_system_ini(0);// eyer environment initialize.
    eyer_reg_segv();   // simulation error used
    eyer_reg_ctrlc();  // simulation error used
#elif CSE_SIM_ENV
    bscaler_mem_init();
    int ret = 0;
    if ((ret = __aie_mmap(0x4000000, 1, NNA_UNCACHED, NNA_UNCACHED_ACCELERATED, NNA_CACHED)) == 0) { //64MB
        printf ("nna box_base virtual generate failed !!\n");
        return 0;
    }
#endif
#if USING_INTERRUPT
    int bscaler_intc_fd = open("/dev/bscaler0", O_RDWR);
    if (bscaler_intc_fd < 0) {
        fprintf(stderr,"open /dev/bscaler0 error.\n");
        exit(1);
    }
#endif

    uint32_t bscaler_base = __aie_get_bscaler_io_vbase();
    uint32_t reg_val;
#if 0
    //BSCALER_FRMC_CTRL
    uint32_t reg_0 = *((uint32_t *)(bscaler_base + 0x00));
    printf("is_lzma    : %s\n", ((reg_0 >> 31) & 0x1) ? "ture" : "false");
    printf("obufc_bus  : %s\n", ((reg_0 >> 12) & 0x1) ? "ORAM" : "DDR");
    printf("ibufc_bus  : %s\n", ((reg_0 >> 11) & 0x1) ? "ORAM" : "DDR");
    printf("box_bus    : %s\n", ((reg_0 >> 10) & 0x1) ? "ORAM" : "DDR");
    printf("irq_mask   : %d\n", ((reg_0 >> 9) & 0x1));
    printf("irq        : %d\n", ((reg_0 >> 8) & 0x1));
    printf("obufc_busy : %d\n", ((reg_0 >> 5) & 0x1));
    printf("calc_busy  : %d\n", ((reg_0 >> 4) & 0x1));
    printf("ibufc_busy : %d\n", ((reg_0 >> 3) & 0x1));
    printf("box_busy   : %d\n", ((reg_0 >> 2) & 0x1));
    printf("~axi_empty : %d\n", ((reg_0 >> 1) & 0x1));
    printf("~axi_empty | frmc_busy: %d\n", ((reg_0 >> 0) & 0x1));

    //BSCALER_FRMC_MODE
    uint32_t reg_1 = *((uint32_t *)(bscaler_base + 0x04));
    printf("yscale_src    : %d\n", (reg_1 >> 16) & 0x7);
    printf("zero_padding  : %d\n", (reg_1 >> 8) & 0xFF);
    printf("is_y_skip     : %d\n", (reg_1 >> 6) & 0x1);
    printf("bw            : %d\n", (reg_1 >> 4) & 0x3);
    printf("format_dst    : %d\n", (reg_1 >> 3) & 0x1);
    printf("box_mode      : %d\n", (reg_1 >> 2) & 0x1);
    printf("run_mode      : %d\n", (reg_1 >> 1) & 0x1);
    printf("format_src    : %d\n", (reg_1 >> 0) & 0x1);

    //BSCALER_FRMC_YBASE_SRC 
    uint32_t reg_1 = *((uint32_t *)(bscaler_base + 0x04));
    printf("ybase_src    : 0x%08x\n", reg_1);

    //BSCALER_FRMC_CBASE_SRC
    uint32_t reg_1 = *((uint32_t *)(bscaler_base + 0x04));
    printf("cbase_src    : 0x%08x\n", reg_1);

    //BSCALER_FRMC_WH_SRC
    uint32_t reg_1 = *((uint32_t *)(bscaler_base + 0x04));
    printf("frmc_h_src    : 0x%04x\n", (reg_1 >> 16) & 0xFFFF);
    printf("frmc_w_src    : 0x%04x\n", (reg_1 >> 0) & 0xFFFF);

    //BSCALER_FRMC_PS_SRC
    uint32_t reg_1 = *((uint32_t *)(bscaler_base + 0x04));
    printf("ps_src    : 0x%08x\n", (reg_1 >> 0) & 0xFFFF);

    //BSCALER_FRMC_WH_DST
    uint32_t reg_1 = *((uint32_t *)(bscaler_base + 0x04));
    printf("frmc_h_dst    : 0x%04x\n", (reg_1 >> 16) & 0x1FF);
    printf("frmc_w_dst    : 0x%04x\n", (reg_1 >> 0) & 0x1FF);

    //BSCALER_FRMC_PS_DST
    uint32_t reg_1 = *((uint32_t *)(bscaler_base + 0x04));
    printf("ps_dst    : 0x%08x\n", (reg_1 >> 0) & 0xFFFF);

    //BSCALER_FRMC_BASE_DST
    uint32_t reg_1 = *((uint32_t *)(bscaler_base + 0x04));
    printf("base_dst    : 0x%08x\n", reg_1);

    //BSCALER_FRMC_BOX_BASE
    uint32_t reg_1 = *((uint32_t *)(bscaler_base + 0x04));
    printf("box_base    : 0x%08x\n", reg_1);
#endif
    //.....

    //BSCALER_PARAM0
    reg_val = *((uint32_t *)(bscaler_base + 0xC0));
    printf("param[0]            : 0x%08X\n", reg_val);

    //BSCALER_PARAM1
    reg_val = *((uint32_t *)(bscaler_base + 0xC4));
    printf("param[1]            : 0x%08X\n", reg_val);

    //BSCALER_PARAM2
    reg_val = *((uint32_t *)(bscaler_base + 0xC8));
    printf("param[2]            : 0x%08X\n", reg_val);

    //BSCALER_PARAM3
    reg_val = *((uint32_t *)(bscaler_base + 0xCC));
    printf("param[3]            : 0x%08X\n", reg_val);

    //BSCALER_PARAM4
    reg_val = *((uint32_t *)(bscaler_base + 0xD0));
    printf("param[4]            : 0x%08X\n", reg_val);

    //BSCALER_PARAM5
    reg_val = *((uint32_t *)(bscaler_base + 0xD4));
    printf("param[5]            : 0x%08X\n", reg_val);

    //BSCALER_PARAM6
    reg_val = *((uint32_t *)(bscaler_base + 0xD8));
    printf("param[6]            : 0x%08X\n", reg_val);

    //BSCALER_PARAM7
    reg_val = *((uint32_t *)(bscaler_base + 0xDC));
    printf("param[7]            : 0x%08X\n", reg_val);

    //BSCALER_PARAM8
    reg_val = *((uint32_t *)(bscaler_base + 0xE0));
    printf("param[8]            : 0x%08X\n", reg_val);

    //BSCALER_PARAM9
    reg_val = *((uint32_t *)(bscaler_base + 0xE4));
    printf("nv2bgr_ofst[1]      : 0x%02X\n", (reg_val >> 24) & 0xFF);
    printf("nv2bgr_ofst[0]      : 0x%02X\n", (reg_val >> 16) & 0xFF);
    printf("nv2bgr_alpha        : 0x%02X\n", (reg_val >> 8) & 0xFF);
    printf("nv2bgr_order_t:     : 0x%01X\n", (reg_val >> 4) & 0xF);
    printf("nv2bgr_order_t      : 0x%01X\n", (reg_val >> 0) & 0xF);

    printf("==============================================\n");
    printf("======= Bscaler T registers parse ============\n");
    printf("==============================================\n");
    //BSCALER_FRMT_CTRL
    reg_val = *((uint32_t *)(bscaler_base + 0x80));
    printf("obufc_bus           : %s\n", ((reg_val >> 11) & 0x1) ? "ORAM" : "DDR");
    printf("ibufc_bus           : %s\n", ((reg_val >> 10) & 0x1) ? "ORAM" : "DDR");
    printf("frmt_irq_mask       : %d\n", (reg_val >> 9) & 0x1);
    printf("frmt_irq            : %d\n", (reg_val >> 8) & 0x1);
    printf("busy_obuft          : %d\n", (reg_val >> 4) & 0x1);
    printf("busy_nv2bgr         : %d\n", (reg_val >> 3) & 0x1);
    printf("busy_ibuft          : %d\n", (reg_val >> 2) & 0x1);
    printf("~axi_empty_t        : %d\n", (reg_val >> 1) & 0x1);
    printf("frmt_busy           : %d\n", (reg_val >> 0) & 0x1);

    //BSCALER_FRMT_TASK
    reg_val = *((uint32_t *)(bscaler_base + 0x84));
    printf("task_len            : %d\n", (reg_val >> 16) & 0xFFFF);
    printf("task_irq            : %d\n", (reg_val >> 2) & 0x1);
    printf("task_irq_mask       : %d\n", (reg_val >> 1) & 0x1);
    printf("task_busy           : %d\n", (reg_val >> 0) & 0x1);

    //BSCALER_FRMT_YBASE_SRC
    reg_val = *((uint32_t *)(bscaler_base + 0x88));
    printf("ybase_src           : 0x%08X\n", reg_val);

    //BSCALER_FRMT_CBASE_SRC
    reg_val = *((uint32_t *)(bscaler_base + 0x8C));
    printf("cbase_src           : 0x%08X\n", reg_val);

    //BSCALER_FRMT_WH_SRC
    reg_val = *((uint32_t *)(bscaler_base + 0x90));
    printf("h_src               : %d\n", (reg_val >> 16) & 0xFFFF);
    printf("w_src               : %d\n", (reg_val >> 0) & 0xFFFF);

    //BSCALER_FRMT_PS_SRC
    reg_val = *((uint32_t *)(bscaler_base + 0x94));
    printf("ps_src              : %d\n", (reg_val >> 0) & 0xFFFF);

    //BSCALER_FRMT_FS_DST
    reg_val = *((uint32_t *)(bscaler_base + 0x98));
    printf("fs_dst              : %d\n", reg_val);

    //BSCALER_FRMT_YBASE_DST
    reg_val = *((uint32_t *)(bscaler_base + 0x9C));
    printf("ybase_dst           : 0x%08X\n", reg_val);

    //BSCALER_FRMT_CBASE_DST
    reg_val = *((uint32_t *)(bscaler_base + 0xA0));
    printf("cbase_dst           : 0x%08X\n", reg_val);

    //BSCALER_FRMT_PS_DST
    reg_val = *((uint32_t *)(bscaler_base + 0xA4));
    printf("ps_dst              : %d\n", reg_val);

    //BSCALER_FRMT_DUMMY
    reg_val = *((uint32_t *)(bscaler_base + 0xA8));
    printf("dummy_val           : 0x%08X\n", reg_val);

    //BSCALER_FRMT_FORMAT
    reg_val = *((uint32_t *)(bscaler_base + 0xAC));
    printf("pad_bt              : %d\n", (reg_val >> 28) & 0x3);
    printf("pad_tp              : %d\n", (reg_val >> 24) & 0x3);
    printf("pad_rt              : %d\n", (reg_val >> 20) & 0x3);
    printf("pad_lf              : %d\n", (reg_val >> 16) & 0x3);
    printf("kernel_size         : %d\n", (reg_val >> 8) & 0x3);
    printf("y_stride            : %d\n", (reg_val >> 6) & 0x3);
    printf("x_stride            : %d\n", (reg_val >> 4) & 0x3);
    printf("dst_format          : %s\n", ((reg_val >> 2) & 0x3) == 0 ? "nv12" :
           ((reg_val >> 2) & 0x3) == 1 ? "bgr" :
           ((reg_val >> 2) & 0x3) == 2 ? "nv22" :
           ((reg_val >> 2) & 0x3) == 3 ? "kernel" : "unknown"
           );
    printf("src_format          : %s\n", (reg_val & 0x1) ? "bgr" : "nv12");

    //BSCALER_FRMT_CHAIN_CTRL
    reg_val = *((uint32_t *)(bscaler_base + 0xB0));
    printf("clk_gate_mask       : %d\n", (reg_val >> 18) & 0x1);
    printf("soft_reset_mask     : %d\n", (reg_val >> 17) & 0x1);
    printf("soft_reset          : %d\n", (reg_val >> 16) & 0x1);

    printf("timeout_irq         : %d\n", (reg_val >> 6) & 0x1);
    printf("timeout_irq_mask    : %d\n", (reg_val >> 5) & 0x1);
    printf("chain_irq           : %d\n", (reg_val >> 4) & 0x1);
    printf("chain_irq_mask      : %d\n", (reg_val >> 3) & 0x1);
    printf("chain_bus           : %d\n", (reg_val >> 1) & 0x1);
    printf("chain_busy          : %d\n", (reg_val >> 0) & 0x1);

    //BSCALER_FRMT_CHAIN_BASE
    reg_val = *((uint32_t *)(bscaler_base + 0xB4));
    printf("chain_base          : 0x%08X\n", reg_val);

    //BSCALER_FRMT_CHAIN_LEN
    reg_val = *((uint32_t *)(bscaler_base + 0xB8));
    printf("chain_len           : %d\n", reg_val);

    //BSCALER_FRMT_TIMEOUT
    reg_val = *((uint32_t *)(bscaler_base + 0xBC));
    printf("timecnt             : 0x%08X\n", reg_val);

    //BSCALER_FRMT_ISUM
    reg_val = *((uint32_t *)(bscaler_base + 0xF0));
    printf("isum                : 0x%08X\n", reg_val);

    //BSCALER_FRMT_OSUM
    reg_val = *((uint32_t *)(bscaler_base + 0xF4));
    printf("osum                : 0x%08X\n", reg_val);

    close(bscaler_intc_fd);

#ifdef EYER_SIM_ENV
    eyer_stop();
#endif

}
