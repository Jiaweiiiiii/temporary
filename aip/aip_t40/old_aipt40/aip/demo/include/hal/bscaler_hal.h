#ifndef __BSCALER_HAL_H__
#define __BSCALER_HAL_H__
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

//#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BSCALER_FRMC_CTRL       (0*4)
#define BSCALER_FRMC_MODE       (1*4)
#define BSCALER_FRMC_YBASE_SRC  (2*4)
#define BSCALER_FRMC_CBASE_SRC  (3*4)
#define BSCALER_FRMC_WH_SRC     (4*4)
#define BSCALER_FRMC_PS_SRC     (5*4)
#define BSCALER_FRMC_BASE_DST   (6*4)
#define BSCALER_FRMC_WH_DST     (7*4)
#define BSCALER_FRMC_PS_DST     (8*4)
#define BSCALER_FRMC_BOX_BASE   (9*4)
#define BSCALER_FRMC_MONO       (10*4)
#define BSCALER_FRMC_EXTREME_Y  (11*4)

#define BSCALER_FRMC_CHAIN_CTRL (12*4)
#define BSCALER_FRMC_CHAIN_BASE (13*4)
#define BSCALER_FRMC_CHAIN_LEN  (14*4)
#define BSCALER_FRMC_TIMEOUT    (15*4)

#define BSCALER_FRMC_BOX0       (16*4)
#define BSCALER_FRMC_BOX1       (17*4)
#define BSCALER_FRMC_BOX2       (18*4)
#define BSCALER_FRMC_BOX3       (19*4)
#define BSCALER_FRMC_BOX4       (20*4)
#define BSCALER_FRMC_BOX5       (21*4)
#define BSCALER_FRMC_BOX6       (22*4)
#define BSCALER_FRMC_BOX7       (23*4)
#define BSCALER_FRMC_BOX8       (24*4)
#define BSCALER_FRMC_BOX9       (25*4)
#define BSCALER_FRMC_BOX10      (26*4)

#define BSCALER_FRMT_CTRL       (32*4)
#define BSCALER_FRMT_TASK       (33*4)
#define BSCALER_FRMT_YBASE_SRC  (34*4)
#define BSCALER_FRMT_CBASE_SRC  (35*4)
#define BSCALER_FRMT_WH_SRC     (36*4)
#define BSCALER_FRMT_PS_SRC     (37*4)
#define BSCALER_FRMT_YBASE_DST  (38*4)
#define BSCALER_FRMT_CBASE_DST  (39*4)
#define BSCALER_FRMT_FS_DST     (40*4)
#define BSCALER_FRMT_PS_DST     (41*4)
#define BSCALER_FRMT_DUMMY      (42*4)
#define BSCALER_FRMT_FORMAT     (43*4)

#define BSCALER_FRMT_CHAIN_CTRL (44*4)
#define BSCALER_FRMT_CHAIN_BASE (45*4)
#define BSCALER_FRMT_CHAIN_LEN  (46*4)
#define BSCALER_FRMT_TIMEOUT    (47*4)

#define BSCALER_PARAM0          (48*4)
#define BSCALER_PARAM1          (49*4)
#define BSCALER_PARAM2          (50*4)
#define BSCALER_PARAM3          (51*4)
#define BSCALER_PARAM4          (52*4)
#define BSCALER_PARAM5          (53*4)
#define BSCALER_PARAM6          (54*4)
#define BSCALER_PARAM7          (55*4)
#define BSCALER_PARAM8          (56*4)
#define BSCALER_PARAM9          (57*4)

#define BSCALER_FRMT_ISUM       (60*4)
#define BSCALER_FRMT_OSUM       (61*4)
#define BSCALER_FRMC_ISUM       (62*4)
#define BSCALER_FRMC_OSUM       (63*4)

#define BSCALER_DB_D_POS        (64*4)
#define BSCALER_DB_BOX          (65*4)
#define BSCALER_DB_S_POS        (66*4)
#define BSCALER_DB_SW           (67*4)
#define BSCALER_DB_SD0          (68*4)
#define BSCALER_DB_SD1          (69*4)
#define BSCALER_DB_SD2          (70*4)
#define BSCALER_FRMT_CSUM       (72*4)
#define BSCALER_FRMC_CSUM       (73*4)

typedef enum {
    BS_BGRA                     = 0,
    BS_GBRA                     = 1,
    BS_RBGA                     = 2,
    BS_BRGA                     = 3,
    BS_GRBA                     = 4,
    BS_RGBA                     = 5,
    BS_ABGR                     = 8,
    BS_AGBR                     = 9,
    BS_ARBG                     = 10,
    BS_ABRG                     = 11,
    BS_AGRB                     = 12,
    BS_ARGB                     = 13,
} rgb_order_e;

typedef enum { //[]
    BST_HW_DATA_FM_NV12         = 0x00, //0000,0000
    BST_HW_DATA_FM_BGRA         = 0x01, //0000,0001
    BST_HW_DATA_FM_GBRA         = 0x11, //0001,0001
    BST_HW_DATA_FM_RBGA         = 0x21, //0010,0001
    BST_HW_DATA_FM_BRGA         = 0x31, //0011,0001
    BST_HW_DATA_FM_GRBA         = 0x41, //0100,0001
    BST_HW_DATA_FM_RGBA         = 0x51, //0101,0001
    BST_HW_DATA_FM_ABGR         = 0x81, //1000,0001
    BST_HW_DATA_FM_AGBR         = 0x91, //1001,0001
    BST_HW_DATA_FM_ARBG         = 0xA1, //1010,0001
    BST_HW_DATA_FM_ABRG         = 0xB1, //1011,0001
    BST_HW_DATA_FM_AGRB         = 0xC1, //1100,0001
    BST_HW_DATA_FM_ARGB         = 0xD1, //1101,0001
    BST_HW_DATA_FM_VBGR         = 0x03, //0000,0011
    BST_HW_DATA_FM_VRGB         = 0x53, //0101,0011
} bst_hw_data_format_e;

typedef struct {
    bst_hw_data_format_e        src_format;
    uint16_t                    src_w;
    uint16_t                    src_h;
    uint8_t                     *src_base0;
    uint8_t                     *src_base1;
    uint32_t                    src_line_stride;

    bst_hw_data_format_e        dst_format;
    uint16_t                    dst_w;
    uint16_t                    dst_h;
    uint8_t                     *dst_base0;
    uint8_t                     *dst_base1;
    uint32_t                    dst_line_stride;
    uint32_t                    dst_plane_stride;

    uint8_t                     kernel_size; //0: 1x1; 1: 3x3; 2: 5x5; 3: 7x7
    uint8_t                     kernel_xstride; //1~3;
    uint8_t                     kernel_ystride; //1~3;
    uint32_t                    zero_point; // padding for virtual channel mode
    uint8_t                     pad_left;   // left pad
    uint8_t                     pad_right;  // right pad
    uint8_t                     pad_top;    // top pad
    uint8_t                     pad_bottom; // botom pad

    uint32_t                    task_len;
    uint8_t                     chain_bus; //0-ddr, 1-oram
    uint8_t                     ibuft_bus; //0-ddr, 1-oram
    uint8_t                     obuft_bus; //0-ddr, 1-oram

    uint8_t                     nv2bgr_alpha;
    uint32_t                    nv2bgr_coef[9];
    uint8_t                     nv2bgr_ofst[2];
    uint32_t                    isum;
    uint32_t                    osum;
    uint32_t                    irq_mask;
    uint8_t                     t_chain_irq_mask;
    uint8_t                     t_timeout_irq_mask;
    uint32_t                    t_timeout_val;
} bst_hw_once_cfg_s;

/*----------------------------------------------------*/
/*-------------- bscaler c ---------------------------*/
/*----------------------------------------------------*/
typedef enum {
    BSC_HW_DATA_FM_NV12         = 0,  //000,0000
    BSC_HW_DATA_FM_BGRA         = 1,  //000,0001
    BSC_HW_DATA_FM_GBRA         = 3,  //000,0011
    BSC_HW_DATA_FM_RBGA         = 5,  //000,0101
    BSC_HW_DATA_FM_BRGA         = 7,  //000,0111
    BSC_HW_DATA_FM_GRBA         = 9,  //000,1001
    BSC_HW_DATA_FM_RGBA         = 11, //000,1011
    BSC_HW_DATA_FM_ABGR         = 17, //001,0001
    BSC_HW_DATA_FM_AGBR         = 19, //001,0011
    BSC_HW_DATA_FM_ARBG         = 21, //001,0101
    BSC_HW_DATA_FM_ABRG         = 23, //001,0111
    BSC_HW_DATA_FM_AGRB         = 25, //001,1001
    BSC_HW_DATA_FM_ARGB         = 27, //001,1011
    BSC_HW_DATA_FM_F32_2B       = 33, //010,0001
    BSC_HW_DATA_FM_F32_4B       = 65, //100,0001
    BSC_HW_DATA_FM_F32_8B       = 97, //110,0001
} bsc_hw_data_format_e;

/**
 * for bscaler hardware API, doing once
 */
typedef struct {
    //single source box info
    uint8_t                     *src_base0;
    uint8_t                     *src_base1;
    bsc_hw_data_format_e        src_format;
    uint32_t                    src_line_stride;
    uint32_t                    src_box_x;
    uint32_t                    src_box_y;
    uint32_t                    src_box_w;
    uint32_t                    src_box_h;

    //single destination box info(size must same)
    bsc_hw_data_format_e        dst_format;
    uint32_t                    dst_line_stride;
    uint32_t                    dst_box_x;
    uint32_t                    dst_box_y;
    uint32_t                    dst_box_w;
    uint32_t                    dst_box_h;

    bool                        affine;
    bool                        box_mode;
    uint32_t                    y_gain_exp;
    uint8_t                     zero_point;
    uint8_t                     nv2bgr_alpha;
    uint32_t                    coef[9];
    uint8_t                     offset[2];
    int32_t                     matrix[9];

    //line mode, multi-box info
    uint32_t                    box_num;//only for hw_api
    uint8_t                     *dst_base[64];
    uint32_t                    *boxes_info;
    /*
      boxes_info[n * 6 + 0] = src_box_x << 16 | src_box_y
      boxes_info[n * 6 + 1] = src_box_w << 16 | src_box_h
      boxes_info[n * 6 + 2] = scale_x
      boxes_info[n * 6 + 3] = scale_y
      boxes_info[n * 6 + 4] = trans_x
      boxes_info[n * 6 + 5] = trans_y
    */

    uint8_t                     mono_x;
    uint8_t                     mono_y;
    int8_t                      extreme_point[64];
    uint32_t                    isum;
    uint32_t                    osum;
    uint8_t                     box_bus;
    uint8_t                     ibufc_bus;
    uint8_t                     obufc_bus;
    uint8_t                     irq_mask;//1 - mask
} bsc_hw_once_cfg_s;

//fixme, recode TODO

typedef struct {
    //for chain
    uint32_t                    *bs_chain_base;
    uint32_t                    bs_chain_len;
    uint8_t                     bs_chain_irq_mask;
    uint8_t                     c_timeout_irq_mask;
    uint8_t                     chain_bus;
    uint32_t                    c_timeout_val;
} bs_chain_cfg_s;

typedef struct {
    uint32_t                    *bs_ret_base;
    uint32_t                    bs_ret_len;
} bs_chain_ret_s;

void bscaler_common_param_cfg(uint32_t *coef, uint8_t *offset, uint8_t alpha);
void bst_order_cfg(uint8_t order);
void bsc_order_cfg(uint8_t order);

#ifdef CSE_SIM_ENV
void bscaler_mem_init();
#endif
void *bscaler_malloc(size_t align, size_t size);
void bscaler_free(void *p2);
void *bscaler_malloc_oram(size_t align, size_t size);
void bscaler_free_oram(void *p2);

void bscaler_write_reg(uint32_t reg, uint32_t val);
uint32_t bscaler_read_reg(uint32_t reg, uint32_t val);

void bscaler_frmt_soft_reset();
void bscaler_frmc_soft_reset();
void bscaler_frmt_clkgate_mask_set(uint8_t t);
void bscaler_frmc_clkgate_mask_set(uint8_t c);

void bscaler_frmt_cfg(bst_hw_once_cfg_s *cfg);
void bscaler_frmc_cfg(bsc_hw_once_cfg_s *cfg);

uint32_t bscaler_frmt_cfg_chain(bst_hw_once_cfg_s *cfg, uint32_t *addr);
bs_chain_ret_s bscaler_frmc_chain_cfg(bsc_hw_once_cfg_s *cfg, uint32_t *addr);
void bsc_chain_hw_cfg(bs_chain_cfg_s *cfg);
int write_chain(uint32_t reg, uint32_t val, uint32_t **addr, uint32_t tm);
#ifdef __cplusplus
}
#endif
#endif //__BSCALER_HAL_H__
