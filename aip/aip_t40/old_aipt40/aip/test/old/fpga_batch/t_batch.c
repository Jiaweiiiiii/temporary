/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : t_batch.c
 * Authors    : jmqi@ingenic.st.jz.com
 * Create Time: 2020-07-29:11:32:04
 * Description:
 *
 */

#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include "affine_whole_float.h"
#include "affine_whole_int.h"

#include "image_process.h"
#include "bscaler_hw_api.h"
#include "bscaler_api.h"
#include "rand_api.h"

int main(int argc, char** argv)
{
#ifdef EYER_SIM_ENV
    eyer_system_ini(0);// eyer environment initialize.
    eyer_reg_segv();   // simulation error used
    eyer_reg_ctrlc();  // simulation error used
#elif CSE_SIM_ENV
    bscaler_mem_init();
    int ret = 0;
    if ((ret = __aie_mmap(0x4000000)) == 0) { //64MB
        printf ("nna box_base virtual generate failed !!\n");
        return 0;
    }
#endif

    // 1. read parameter bin file

    // 2. load src data, from bin or read png ??
    int src_size = 0;
    if (src_format == 0) { //nv12
        src_size = src_line_stride * src_h * 3 / 2;
    } else { //channel
        src_size = src_line_stride * src_h;
    }
    src_base0 = (uint8_t*)bscaler_malloc(1, src_size);
    if (src_base0 == NULL) {
        fprintf(stderr, "[Error] : alloc space for src error!\n");
        exit(1);
    }

    int dst_size = 0;
    if (dst_format == 0) { //nv12
        dst_size = dst_line_stride * dst_h * 3 / 2;
    } else { //channel
        dst_size = dst_line_stride * dst_h;
    }

    dst_base0 = (uint8_t*)bscaler_malloc(1, dst_size);
    if (dst_base0 == NULL) {
        fprintf(stderr, "[Error] : alloc space for dst error!\n");
        exit(1);
    }

    box_resize_info_s *infos = (box_resize_info_s *)malloc(sizeof(box_resize_info_s) * box_num);
    infos[i].box.x = resize_src_x;
    infos[i].box.y = resize_src_y;
    infos[i].box.w = resize_src_w;
    infos[i].box.h = resize_src_h;
    infos[i].wrap = 0;
    infos[i].zero_point = 0;//fix me and fix the alpha

    src.base = src_base0;
    gld.base = gld_base0;
    dut.base = dst_base0;

    switch (src_cfg->mode) {
    case 0:
        printf("api resize here!\n");
        bs_resize_start(&src, box_num, &dut, infos, coef, offset);
        bs_resize_wait();
        break;
    case 1:
        printf("api affine here!\n");
        bs_affine_start(&src, box_num, &dut, &info, coef, offset);
        bs_affine_wait();
        break;
    case 2:
        printf("api perspective here!\n");
        bs_perspective_start(&src, box_num, &dut, &info, coef, offset);
        bs_perspective_wait();
        break;
    default:
        printf("api resize here!\n");
        bs_resize_start(&src, box_num, &dut, infos, coef, offset);
        bs_resize_wait();
        break;
    }

    //check sum
    uint32_t dchk_isum = bscaler_read_reg(BSCALER_FRMC_ISUM, 0);
    uint32_t dchk_osum = bscaler_read_reg(BSCALER_FRMC_OSUM, 0);
    uint32_t errnum = 0;
    if (mchk_isum != dchk_isum) {
        errnum = errnum | 0x1;
    }
    if (mchk_osum != dchk_osum) {
        errnum = errnum | (0x1 << 1);
    }
    if (errnum) {
        fprintf(stderr, "error..more information\n");
    }

    bscaler_free(src.base);
    bscaler_free(gld.base);

#ifdef EYER_SIM_ENV
    eyer_stop();
#endif
    return 0;
}
