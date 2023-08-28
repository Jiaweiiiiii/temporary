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
#define AIP_V20_BO              0x22
#define NNA_V20_OP              0x23

// NNA
typedef enum {
	NNA_OP_MUTEX_LOCK,
	NNA_OP_MUTEX_UNLOCK,
	NNA_OP_ORAM_ALLOC,
	NNA_OP_ORAM_FREE,
	NNA_OP_ORAM_STATUS,
} aip_v20_ioctl_nna_op_t;

typedef struct {
	size_t                          size;
	unsigned long                   handle;
	void *                          paddr;
} aip_v20_ioctl_nna_oram_mem_t;

typedef struct {
	size_t                          size;
	uint32_t                        base;
} aip_v20_ioctl_nna_oram_status_t;

typedef union {
	aip_v20_ioctl_nna_oram_mem_t    mem;
	aip_v20_ioctl_nna_oram_status_t status;
} aip_v20_ioctl_nna_oprds_t;

typedef struct {
	aip_v20_ioctl_nna_op_t          op;
	aip_v20_ioctl_nna_oprds_t       oprds;
} aip_v20_ioctl_nna_t;

// AIP
typedef enum {
	AIP_CH_OP_P = 0,
	AIP_CH_OP_T,
	AIP_CH_OP_F,
} aip_v20_ioctl_ch_op_t;

typedef enum {
        AIP_DAT_POS_DDR = 0,
        AIP_DAT_POS_ORAM = 1,
} aip_v20_ioctl_dat_pos_t;

typedef enum {
	AIP_F_FORMAT_Y = 0,
	AIP_F_FORMAT_C = 1,
	AIP_F_FORMAT_BGRA = 2,
	AIP_F_FORMAT_FEATURE2 = 3,
	AIP_F_FORMAT_FEATURE4 = 4,
	AIP_F_FORMAT_FEATURE8 = 5,
	AIP_F_FORMAT_FEATURE8_H = 6,
	AIP_F_FORMAT_NV12 = 7,
} aip_v20_ioctl_f_format_t;

// AIP submit

typedef struct {
	uint32_t                      timeout[2];
	uint32_t                      scale_x[2];
	uint32_t                      scale_y[2];
	int32_t                       trans_x[2];
	int32_t                       trans_y[2];
	uint32_t                      src_base_y[2];
	uint32_t                      src_base_uv[2];
	uint32_t                      dst_base_y[2];
	uint32_t                      dst_base_uv[2];
	uint32_t                      src_size[2];
	uint32_t                      dst_size[2];
	uint32_t                      stride[2];
	uint32_t                      f_ctrl[2];
} aip_v20_f_node_t;

typedef struct {
	int                           box_num;
} aip_v20_ioctl_submit_p_t;

typedef struct {
	int                           box_num;
} aip_v20_ioctl_submit_t_t;

typedef struct {
	aip_v20_ioctl_f_format_t      format;
	aip_v20_ioctl_dat_pos_t       pos_in;
	aip_v20_ioctl_dat_pos_t       pos_out;

	int                           node_handle;
	size_t                        node_num;
	uint64_t                      seqno;
} aip_v20_ioctl_submit_f_t;

typedef union {
	aip_v20_ioctl_submit_p_t      p;
	aip_v20_ioctl_submit_t_t      t;
	aip_v20_ioctl_submit_f_t      f;
} aip_v20_ioctl_submit_oprds_t;

typedef struct {
	aip_v20_ioctl_ch_op_t         op;
	aip_v20_ioctl_submit_oprds_t  oprds;
} aip_v20_ioctl_submit_t;


// AIP wait
typedef struct {
	aip_v20_ioctl_ch_op_t         op;
	uint64_t                      seqno;
	uint64_t                      timeout_ns;
} aip_v20_ioctl_wait_t;


// AIP bo
typedef enum {
	AIP_BO_OP_APPEND = 0,
	AIP_BO_OP_DESTROY,
} aip_v20_ioctl_bo_op_t;

typedef struct {
	aip_v20_ioctl_bo_op_t         op;
	size_t                        size;
	int                           handle;
	void                         *paddr;
} aip_v20_ioctl_bo_t;


#endif
