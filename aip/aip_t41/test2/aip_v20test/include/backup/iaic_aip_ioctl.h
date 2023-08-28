
#ifndef __IAIC_AIP_IOCTL_H__
#define __IAIC_AIP_IOCTL_H__

#define IAIC_AIP_IOCTL_SUBMIT 0x00
#define IAIC_AIP_IOCTL_WAIT   0x01

typedef enum {
	AIP_OP_P = 0,
	AIP_OP_T,
	AIP_OP_F,
	AIP_OP_PT,
	AIP_OP_PF,
	AIP_OP_TF,
	AIP_OP_PTF,
} iaic_aip_ioctl_op_t;


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
} iaic_aip_f_node_t;

typedef struct {
	int                           box_num;
} iaic_aip_ioctl_submit_p_t;

typedef struct {
	int                           box_num;
} iaic_aip_ioctl_submit_t_t;

typedef struct {
	iaic_aip_f_node_t             *f_node;
    	uint32_t                      f_cfg;
	int                           box_num;
} iaic_aip_ioctl_submit_f_t;

typedef union {
	iaic_aip_ioctl_submit_p_t     p;
	iaic_aip_ioctl_submit_t_t     t;
	iaic_aip_ioctl_submit_f_t     f;
} iaic_aip_ioctl_submit_oprds_t;

typedef struct {
	iaic_aip_ioctl_op_t           op;
	iaic_aip_ioctl_submit_oprds_t oprds;
} iaic_aip_ioctl_submit_t;


// AIP wait
typedef struct {
	iaic_aip_ioctl_op_t            op;
} iaic_aip_ioctl_wait_t;

#endif
