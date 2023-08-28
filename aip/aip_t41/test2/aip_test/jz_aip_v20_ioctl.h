/*
 * Ingenic AIP driver ver2.0
 *
 * Copyright (c) 2023 LiuTianyang
 *
 * This file is released under the GPLv2
 */

#ifndef _JZ_AIP_V20_IOCTL_H_
#define _JZ_AIP_V20_IOCTL_H_

// Reference Documentation/ioctl/ioctl-number.txt
#define AIP_V20_SUBMIT          0x20
#define AIP_V20_WAIT            0x21

typedef enum {
	AIP_OP_P = 0,
	AIP_OP_T,
	AIP_OP_F,
	AIP_OP_PT,
	AIP_OP_PF,
	AIP_OP_TF,
	AIP_OP_PTF,
} aip_v20_ioctl_op_t;


// AIP submit

typedef struct {
        uint32_t                      scale_x;
        uint32_t                      scale_y;
        int32_t                       trans_x;
        int32_t                       trans_y;
        uint32_t                      src_base_y;
        uint32_t                      src_base_uv;
        uint32_t                      dst_base_y;
        uint32_t                      dst_base_uv;
        uint16_t                      src_width;
        uint16_t                      src_height;
        uint16_t                      dst_width;
        uint16_t                      dst_height;
        uint16_t                      dst_stride;
        uint16_t                      src_stride;
        uint32_t                      f_ctrl;	
} aip_v20_f_node_t;

typedef struct {
	int                           box_num;
} aip_v20_ioctl_submit_p_t;

typedef struct {
	int                           box_num;
} aip_v20_ioctl_submit_t_t;

typedef struct {
	aip_v20_f_node_t             *f_node;
    	uint32_t                      f_cfg;
	int                           box_num;
} aip_v20_ioctl_submit_f_t;

typedef union {
	aip_v20_ioctl_submit_p_t     p;
	aip_v20_ioctl_submit_t_t     t;
	aip_v20_ioctl_submit_f_t     f;
} aip_v20_ioctl_submit_oprds_t;

typedef struct {
	aip_v20_ioctl_op_t           op;
	aip_v20_ioctl_submit_oprds_t oprds;
} aip_v20_ioctl_submit_t;


// AIP wait
typedef struct {
	aip_v20_ioctl_op_t            op;
} aip_v20_ioctl_wait_t;

#endif
