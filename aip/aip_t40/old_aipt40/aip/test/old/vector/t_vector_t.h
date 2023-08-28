/* Generate by gen_t_vector. */
/* seed = 0x5ef86944 */

#ifndef __T_VECTOR_T_H__
#define __T_VECTOR_T_H__

#include "bscaler_hw_api.h"

bst_hw_once_cfg_s cfg = {
    0,//src_format
    1,//dst_format
    0,//kernel_size
    1,//kernel_xstride
    1,//kernel_ystride
    0,//kernel_dummy_val
    0,//panding_lf
    0,//panding_rt
    0,//panding_tp
    0,//panding_bt
    2,//frmt_task_len
    0,//frmt_chain_bus
    0,//frmt_ibuft_bus
    0,//frmt_obuft_bus
    NULL,//src_base0
    NULL,//src_base1
    2,//src_w
    2,//src_h
    5,//src_line_stride
    NULL,//dst_base0
    NULL,//dst_base1
    55,//dst_line_stride
    1,//frmt_fs_dst
    0,//nv2bgr_order
    {1220542, 2116026, 0, 1220542, 409993, 852492, 1220542, 0, 1673527, }, //nv2bgr_coef
    {16, 128, }, //nv2bgr_ofst
    1310,//isum
    2284,//osum
    1,//irq_mask
};

__place_k0_data__ uint8_t src0[] = {
/*  0*/ 0xc8,0xc8,0xc8,0xc8,0xc8,
/*  1*/ 0xc8,0xc8,0xc8,0xc8,0xc8,
};

__place_k0_data__ uint8_t src1[] = {
0xff,0xff,0xff,0xff,0xff,};

uint8_t gld_dst[] = {
/**** dst_base 0 ****/
/*  0*/ 0xff,0x3d,0xff,0x00,0xff,0x3d,0xff,0x00,
/*  1*/ 0xff,0x3d,0xff,0x00,0xff,0x3d,0xff,0x00,
};

#endif /* __T_VECTOR_T_H__ */
