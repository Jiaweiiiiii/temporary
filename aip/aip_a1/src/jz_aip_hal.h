#ifndef __JZ_AIP_H__
#define __JZ_AIP_H__

#include <stdint.h>


#define JZ_AIP_IOBASE		0x13090000
#define JZ_AIP_IOSIZE		0x1000

#define JZ_AIP_T_IOBASE 	0x13090000
#define JZ_AIP_F_IOBASE		0x13090200
#define JZ_AIP_P_IOBASE		0x13090300

#define JZ_AIP_T_OFFSET    0x000
#define JZ_AIP_F_OFFSET    0x200
#define JZ_AIP_P_OFFSET    0x300

#define IOCTL_AIP_IRQ_WAIT_CMP		_IOWR('P', 0, int)
#define IOCTL_AIP_MALLOC		_IOWR('P', 1, int)
#define IOCTL_AIP_FREE			_IOWR('P', 2, int)

#define AIP_F_DEV	"jzaip_f"
#define AIP_T_DEV	"jzaip_t"
#define AIP_P_DEV	"jzaip_p"

#define AIP_P_CTRL			    (JZ_AIP_P_OFFSET + 0x00)
#define AIP_P_IQR			    (JZ_AIP_P_OFFSET + 0x04)
#define AIP_P_CFG			    (JZ_AIP_P_OFFSET + 0x08)
#define AIP_P_TIMEOUT			(JZ_AIP_P_OFFSET + 0x0c)
#define AIP_P_MODE			    (JZ_AIP_P_OFFSET + 0x10)
#define AIP_P_SRC_Y_BASE		(JZ_AIP_P_OFFSET + 0x14)
#define AIP_P_SRC_UV_BASE		(JZ_AIP_P_OFFSET + 0x18)
#define AIP_P_SRC_STRIDE		(JZ_AIP_P_OFFSET + 0x1c)
#define AIP_P_DST_BASE			(JZ_AIP_P_OFFSET + 0x20)
#define AIP_P_DST_STRIDE		(JZ_AIP_P_OFFSET + 0x24)
#define AIP_P_DST_SIZE			(JZ_AIP_P_OFFSET + 0x28)
#define AIP_P_SRC_SIZE			(JZ_AIP_P_OFFSET + 0x2c)
#define AIP_P_DUMMY_VAL			(JZ_AIP_P_OFFSET + 0x30)
#define AIP_P_COEF0			    (JZ_AIP_P_OFFSET + 0x34)
#define AIP_P_COEF1			    (JZ_AIP_P_OFFSET + 0x38)
#define AIP_P_COEF2			    (JZ_AIP_P_OFFSET + 0x3c)
#define AIP_P_COEF3			    (JZ_AIP_P_OFFSET + 0x40)
#define AIP_P_COEF4			    (JZ_AIP_P_OFFSET + 0x44)
#define AIP_P_COEF5			    (JZ_AIP_P_OFFSET + 0x48)
#define AIP_P_COEF6			    (JZ_AIP_P_OFFSET + 0x4c)
#define AIP_P_COEF7			    (JZ_AIP_P_OFFSET + 0x50)
#define AIP_P_COEF8			    (JZ_AIP_P_OFFSET + 0x54)
#define AIP_P_COEF9			    (JZ_AIP_P_OFFSET + 0x58)
#define AIP_P_COEF10			(JZ_AIP_P_OFFSET + 0x5c)
#define AIP_P_COEF11			(JZ_AIP_P_OFFSET + 0x60)
#define AIP_P_COEF12			(JZ_AIP_P_OFFSET + 0x64)
#define AIP_P_COEF13			(JZ_AIP_P_OFFSET + 0x68)
#define AIP_P_COEF14			(JZ_AIP_P_OFFSET + 0x6c)
#define AIP_P_PARAM0			(JZ_AIP_P_OFFSET + 0x70)
#define AIP_P_PARAM1			(JZ_AIP_P_OFFSET + 0x74)
#define AIP_P_PARAM2			(JZ_AIP_P_OFFSET + 0x78)
#define AIP_P_PARAM3			(JZ_AIP_P_OFFSET + 0x7c)
#define AIP_P_PARAM4			(JZ_AIP_P_OFFSET + 0x80)
#define AIP_P_PARAM5			(JZ_AIP_P_OFFSET + 0x84)
#define AIP_P_PARAM6			(JZ_AIP_P_OFFSET + 0x88)
#define AIP_P_PARAM7			(JZ_AIP_P_OFFSET + 0x8c)
#define AIP_P_PARAM8			(JZ_AIP_P_OFFSET + 0x90)
#define AIP_P_OFFSET			(JZ_AIP_P_OFFSET + 0x94)
#define AIP_P_CHAIN_BASE		(JZ_AIP_P_OFFSET + 0x98)
#define AIP_P_CHAIN_SIZE		(JZ_AIP_P_OFFSET + 0x9c)
#define AIP_P_ISUM			    (JZ_AIP_P_OFFSET + 0xa0)
#define AIP_P_OSUM			    (JZ_AIP_P_OFFSET + 0xa4)

#define AIP_T_CTRL			    (JZ_AIP_T_OFFSET + 0x00)
#define AIP_T_IQR			    (JZ_AIP_T_OFFSET + 0x04)
#define AIP_T_CFG			    (JZ_AIP_T_OFFSET + 0x08)
#define AIP_T_TIMEOUT			(JZ_AIP_T_OFFSET + 0x0c)
#define AIP_T_TASK_LEN			(JZ_AIP_T_OFFSET + 0x10)
#define AIP_T_SRC_Y_BASE		(JZ_AIP_T_OFFSET + 0x14)
#define AIP_T_SRC_UV_BASE		(JZ_AIP_T_OFFSET + 0x18)
#define AIP_T_DST_BASE			(JZ_AIP_T_OFFSET + 0x1c)
#define AIP_T_STRIDE			(JZ_AIP_T_OFFSET + 0x20)
#define AIP_T_SRC_SIZE			(JZ_AIP_T_OFFSET + 0x24)
#define AIP_T_PARAM0			(JZ_AIP_T_OFFSET + 0x28)
#define AIP_T_PARAM1			(JZ_AIP_T_OFFSET + 0x2c)
#define AIP_T_PARAM2			(JZ_AIP_T_OFFSET + 0x30)
#define AIP_T_PARAM3			(JZ_AIP_T_OFFSET + 0x34)
#define AIP_T_PARAM4			(JZ_AIP_T_OFFSET + 0x38)
#define AIP_T_PARAM5			(JZ_AIP_T_OFFSET + 0x3c)
#define AIP_T_PARAM6			(JZ_AIP_T_OFFSET + 0x40)
#define AIP_T_PARAM7			(JZ_AIP_T_OFFSET + 0x44)
#define AIP_T_PARAM8			(JZ_AIP_T_OFFSET + 0x48)
#define AIP_T_OFFSET			(JZ_AIP_T_OFFSET + 0x4c)
#define AIP_T_CHAIN_BASE		(JZ_AIP_T_OFFSET + 0x50)
#define AIP_T_CHAIN_SIZE		(JZ_AIP_T_OFFSET + 0x54)
#define AIP_T_TASL_SKIP_BASE		(JZ_AIP_T_OFFSET + 0x58)
#define AIP_T_ISUM			(JZ_AIP_T_OFFSET + 0x5c)
#define AIP_T_OSUM			(JZ_AIP_T_OFFSET + 0x60)

#define AIP_F_CTRL			(JZ_AIP_F_OFFSET + 0x00)
#define AIP_F_IQR			(JZ_AIP_F_OFFSET + 0x04)
#define AIP_F_CFG			(JZ_AIP_F_OFFSET + 0x08)
#define AIP_F_TIMEOUT			(JZ_AIP_F_OFFSET + 0x0c)
#define AIP_F_SCALE_X			(JZ_AIP_F_OFFSET + 0x10)
#define AIP_F_SCALE_Y			(JZ_AIP_F_OFFSET + 0x14)
#define AIP_F_TRANS_X			(JZ_AIP_F_OFFSET + 0x18)
#define AIP_F_TRANS_Y			(JZ_AIP_F_OFFSET + 0x1c)
#define AIP_F_SRC_Y_BASE		(JZ_AIP_F_OFFSET + 0x20)
#define AIP_F_SRC_UV_BASE		(JZ_AIP_F_OFFSET + 0x24)
#define AIP_F_DST_Y_BASE		(JZ_AIP_F_OFFSET + 0x28)
#define AIP_F_DST_UV_BASE		(JZ_AIP_F_OFFSET + 0x2c)
#define AIP_F_SRC_SIZE			(JZ_AIP_F_OFFSET + 0x30)
#define AIP_F_DST_SIZE			(JZ_AIP_F_OFFSET + 0x34)
#define AIP_F_STRIDE			(JZ_AIP_F_OFFSET + 0x38)
#define AIP_F_CHAIN_BASE		(JZ_AIP_F_OFFSET + 0x3c)
#define AIP_F_CHAIN_SIZE		(JZ_AIP_F_OFFSET + 0x40)
#define AIP_F_ISUM		    	(JZ_AIP_F_OFFSET + 0x44)
#define AIP_F_OSUM			(JZ_AIP_F_OFFSET + 0x48)

#define AIP_SOFT_RESET_BIT 0x2

typedef enum {
	INDEX_AIP_T = 0,
	INDEX_AIP_F = 1,
	INDEX_AIP_P = 2,
} AIP_TYPE;

typedef enum {
	AIP_F_TYPE_Y = 0,
	AIP_F_TYPE_UV,
	AIP_F_TYPE_BGRA,
	AIP_F_TYPE_FEATURE2,
	AIP_F_TYPE_FEATURE4,
	AIP_F_TYPE_FEATURE8,
	AIP_F_TYPE_FEATURE8_H,
	AIP_F_TYPE_NV12,
} AIP_F_DATA_FORMAT;

typedef enum {
	AIP_NV2BGRA = 0,
	AIP_NV2GBRA,
	AIP_NV2RBGA,
	AIP_NV2BRGA,
	AIP_NV2GRBA,
	AIP_NV2RGBA,
	AIP_NV2ABGR = 8,
	AIP_NV2AGBR,
	AIP_NV2ARBG,
	AIP_NV2ABRG,
	AIP_NV2AGRB,
	AIP_NV2ARGB,
} AIP_RGB_FORMAT;

typedef enum {
	AIP_DATA_DDR = 0,
	AIP_DATA_ORAM,
} AIP_DDRORORAM;

// aip_p
struct aip_p_chainnode {
	uint32_t		mode;
	uint32_t		src_base_y;
	uint32_t		src_base_uv;
	uint32_t		src_stride;
	uint32_t		dst_base;
	uint32_t		dst_stride;
	uint16_t		dst_width;
	uint16_t		dst_height;
	uint16_t		src_width;
	uint16_t		src_height;
	uint32_t		dummy_val;
	int32_t			coef_parm0;
	int32_t			coef_parm1;
	int32_t			coef_parm2;
	int32_t			coef_parm3;
	int32_t			coef_parm4;
	int32_t			coef_parm5;
	int32_t			coef_parm6;
	int32_t			coef_parm7;
	int32_t			coef_parm8;
	int32_t			coef_parm9;
	int32_t			coef_parm10;
	int32_t			coef_parm11;
	int32_t			coef_parm12;
	int32_t			coef_parm13;
	int32_t			coef_parm14;
	uint32_t		offset;
	uint32_t		p_ctrl;
};

//aip_t
struct aip_t_chainnode {
	uint32_t		src_base_y;
	uint32_t		src_base_uv;
	uint32_t		dst_base;
	uint16_t		src_width;
	uint16_t		src_height;
	uint16_t		src_stride;
	uint16_t		dst_stride;
	uint32_t		task_len;
	uint32_t		task_skip_base;
	uint32_t		t_ctrl;
};

//aip_f
struct aip_f_chainnode {
	uint32_t		scale_x;
	uint32_t		scale_y;
	int32_t			trans_x;
	int32_t			trans_y;
	uint32_t		src_base_y;
	uint32_t		src_base_uv;
	uint32_t		dst_base_y;
	uint32_t		dst_base_uv;
	uint16_t		src_width;
	uint16_t		src_height;
	uint16_t		dst_width;
	uint16_t		dst_height;
	uint16_t		dst_stride;
	uint16_t		src_stride;
	uint32_t		f_ctrl;
};

struct jz_aip_chainbuf {
	void *vaddr;
	uint32_t paddr;
	uint32_t size;
};

struct  aip_p_chaindesc {
	uint32_t	timeout[2];
	uint32_t	mode[2];
	uint32_t	src_base_y[2];
	uint32_t	src_base_uv[2];
	uint32_t	src_stride[2];
	uint32_t	dst_base[2];
	uint32_t	dst_stride[2];
	uint32_t	dst_size[2];
	uint32_t	src_size[2];
	uint32_t	dummy_val[2];
	uint32_t	coef0[2];
	int32_t		coef1[2];
	int32_t		coef2[2];
	int32_t		coef3[2];
	int32_t		coef4[2];
	int32_t		coef5[2];
	int32_t		coef6[2];
	int32_t		coef7[2];
	int32_t		coef8[2];
	int32_t		coef9[2];
	int32_t		coef10[2];
	int32_t		coef11[2];
	int32_t		coef12[2];
	int32_t		coef13[2];
	int32_t		coef14[2];
	uint32_t	offset[2];
	uint32_t	p_ctrl[2];
};

struct aip_t_chaindesc {
	uint32_t		timeout[2];
	uint32_t		src_base_y[2];
	uint32_t		src_base_uv[2];
	uint32_t		dst_base[2];
	uint32_t		src_size[2];
	uint32_t		stride[2];
	uint32_t		task_len[2];
	uint32_t		task_skip_base[2];
	uint32_t		t_ctrl[2];
};

struct aip_f_chaindesc {
	uint32_t        cfg[2];
	uint32_t		timeout[2];
	uint32_t		scale_x[2];
	uint32_t		scale_y[2];
	int32_t			trans_x[2];
	int32_t			trans_y[2];
	uint32_t		src_base_y[2];
	uint32_t		src_base_uv[2];
	uint32_t		dst_base_y[2];
	uint32_t		dst_base_uv[2];
	uint32_t		src_size[2];
	uint32_t		dst_size[2];
	uint32_t		stride[2];
	uint32_t		f_ctrl[2];
};

struct aip_manage {
	char index;
	char name[16];
	int fd_open_cnt;
	int fd;
	int mem_fd;
	uint32_t aip_io_size;
	uint32_t aip_io_vaddr;

	struct jz_aip_chainbuf chainbuf;
	uint32_t chain_num;
};

int aip_mem_init(void);
int aip_mem_free(void);
int aip_p_init(struct aip_p_chainnode *chains, int chain_num,
		const uint32_t *nv2rgb_parm, uint32_t cfg);

int aip_t_init(struct aip_t_chainnode *chains, int chain_num,
		const uint32_t *nv2rgb_parm,  uint32_t offset, uint32_t cfg);
int aip_t_exit();

int aip_f_init(struct aip_f_chainnode *chains, int chain_num, uint32_t cfg);

int aip_p_start(int chain_num);
int aip_t_start(uint32_t task_len, uint32_t task_skip_base);
int aip_f_start(int chain_num);

int aip_p_wait();
int aip_t_wait();
int aip_f_wait();

int get_box_info(int type);

#endif

