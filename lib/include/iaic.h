
#ifndef __IAIC_H__
#define __IAIC_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "iaic_core.h"

// R&D Version 2.0.0     LiuTianyang
// R&D Version 2.1.0     LiuTianyang    2023-06-14
// Beta Version 2.0.1    ZhangJiawei    2023-07-27
#define AIP_V20_LIB_VERSION ((0x2 << 16) | (0x0 << 8) | (0x1))

#define iaic_ctx_init(ctx, num) \
	({ \
		iaic_ret_code_t __r; \
		__r = iaic_ctx_init_internal(ctx, num); \
		if (__r == IAIC_CODE_OK) { \
			if ((ctx)->status.lib_version != AIP_V20_LIB_VERSION) { \
				printf("[IAIC] lib version not match 0x%08x 0x%08x\n", \
					AIP_V20_LIB_VERSION, \
					(ctx)->status.lib_version); \
				__r = IAIC_CODE_LIB_VERSION_NOMATCH; \
			} \
		} \
		__r; \
	})

#define iaic_process_lock(ctx)\
	pthread_mutex_lock(&(ctx)->shm->mutex_lock);

#define iaic_process_unlock(ctx)\
	pthread_mutex_unlock(&(ctx)->shm->mutex_lock);

#define iaic_fast_iob(ctx)\
    ({\
        __asm__ __volatile__("sync\n lw\t$0, 0(%0)" \
			::"r"((ctx)->cpu_fast_iob));\
    })

#define i_rdhwr(src)\
    ({\
        __asm__ __volatile__("rdhwr\t$0, $%0" ::"i"(src));\
    })

#define iaic_nndma_rch0_wait i_rdhwr(8)
#define iaic_nndma_rch1_wait i_rdhwr(9)
#define iaic_nndma_rch2_wait i_rdhwr(10)
#define iaic_nndma_wch0_wait i_rdhwr(11)
#define iaic_nndma_wch1_wait i_rdhwr(12)

#define iaic_nndma_rch0_start(_ctx,\
		_bo_paddr, _oram_oft, _size)\
	({\
	 	iaic_nndma_start(_ctx, _bo_paddr,\
				_oram_oft, _size, 0x0);\
	})

#define iaic_nndma_rch1_start(_ctx,\
		_bo_paddr, _oram_oft, _size)\
	({\
	 	iaic_nndma_start(_ctx, _bo_paddr,\
				_oram_oft, _size, 0x4);\
	})

#define iaic_nndma_rch2_start(_ctx,\
		_bo_paddr, _oram_oft, _size)\
	({\
	 	iaic_nndma_start(_ctx, _bo_paddr,\
				_oram_oft, _size, 0x8);\
	})

#define iaic_nndma_wch0_start(_ctx,\
		_bo_paddr, _oram_oft, _size)\
	({\
	 	iaic_nndma_start(_ctx, _bo_paddr,\
				_oram_oft, _size, 0x10);\
	})

#define iaic_nndma_wch1_start(_ctx,\
		_bo_paddr, _oram_oft, _size)\
	({\
	 	iaic_nndma_start(_ctx, _bo_paddr,\
				_oram_oft, _size, 0x14);\
	})

#define iaic_nna_mutex_lock(ctx)\
	({\
		iaic_ret_code_t __r;\
		__r = iaic_nna_mutex_lock_internal\
		(ctx, 0, NULL);\
		__r;\
	})

#define iaic_nna_mutex_lock_name(ctx, name)\
	({\
		iaic_ret_code_t __r;\
		__r = iaic_nna_mutex_lock_internal\
		(ctx, 0, name);\
		__r;\
	})

#define iaic_nna_mutex_trylock(ctx) \
	({\
		iaic_ret_code_t __r;\
		__r = iaic_nna_mutex_lock_internal\
		(ctx, 1, NULL);\
		__r;\
	})

#define iaic_nna_mutex_trylock_name(ctx, name) \
	({\
		iaic_ret_code_t __r;\
		__r = iaic_nna_mutex_lock_internal\
		(ctx, 1, name);\
		__r;\
	})

#define iaic_nna_mutex_unlock(ctx)\
	({\
		iaic_ret_code_t __r;\
		__r = iaic_nna_mutex_unlock_internal\
		(ctx, 0, 0);\
		__r;\
	})

#define iaic_nna_mutex_unlock_set(ctx, cpu)\
	({\
		iaic_ret_code_t __r;\
		__r = iaic_nna_mutex_unlock_internal\
		(ctx, cpu, 0);\
		__r;\
	})

#define iaic_nna_mutex_unlock_force(ctx)\
	({\
		iaic_ret_code_t __r;\
		__r = iaic_nna_mutex_unlock_internal\
		(ctx, 0, 1);\
		__r;\
	})

#define iaic_aip_wait(ctx,\
		_seqno,\
		_timeout_ns,\
		_wait_time,\
		_op)\
	({\
	 	iaic_wait __wait;\
		iaic_ret_code_t __r;\
		int __e;\
		int64_t *__wait_time = _wait_time;\
	 	__wait.op = _op;\
	 	__wait.seqno = _seqno;\
	 	__wait.timeout_ns = _timeout_ns;\
		__e = IAIC_IOCTL(ctx->aip_fd, IOCTL_AIP_V20_WAIT, &__wait);\
	 	if (__wait_time != NULL)\
			*__wait_time = __wait.wait_time;\
		__r = (__e < 0) ? IAIC_CODE_AIP_WAIT_FAIL : \
					IAIC_CODE_OK;\
	 	__r;\
	 })


// AIP F
#define iaic_aip_f_chain(ctx,\
		_format,\
		_pos_in,\
		_pos_out,\
		_bo,\
		_node_num,\
		_name,\
		_seqno)\
	({\
		iaic_f_submit_t __submit;\
		iaic_ret_code_t __r;\
		__submit.op = AIP_CH_OP_F;\
		__submit.format = _format;\
		__submit.pos_in = _pos_in;\
		__submit.pos_out = _pos_out;\
		__submit.node_handle = (_bo)->handle;\
		__submit.node_num = _node_num;\
		__submit.name = _name;\
		__submit.paddr = (uint32_t)((_bo)->paddr);\
		__r = iaic_aip_f_chain_submit(ctx, &__submit);\
		*_seqno = __submit.seqno;\
		__r;\
	 })

// AIP p
#define iaic_aip_p_chain(ctx,\
		_mode,\
		_pos_in,\
		_pos_out,\
		_offset_order,\
		_offset_alpha,\
		_offset_offset0,\
		_offset_offset1,\
		_param,\
		_bo,\
		_node_num,\
		_name,\
		_seqno)\
	({\
		iaic_p_submit_t __submit;\
		iaic_ret_code_t __r;\
		__submit.op = AIP_CH_OP_P;\
		__submit.mode = _mode;\
		__submit.offset.order = _offset_order;\
		__submit.offset.alpha = _offset_alpha;\
		__submit.offset.offset[0] = _offset_offset0;\
		__submit.offset.offset[1] = _offset_offset1;\
		__submit.pos_in = _pos_in;\
		__submit.pos_out = _pos_out;\
		__submit.param = _param;\
		__submit.node_handle = (_bo)->handle;\
		__submit.node_num = _node_num;\
		__submit.paddr = (uint32_t)(_bo)->paddr;\
		__submit.name = _name;\
		__r = iaic_aip_p_chain_submit(ctx, &__submit);\
		*_seqno = __submit.seqno;\
		__r;\
	 })

// AIP t
#define iaic_aip_t(ctx,\
		_pos_in,\
		_pos_out,\
		_offset_order,\
		_offset_alpha,\
		_offset_offset0,\
		_offset_offset1,\
		_param,\
		_node,\
		_name,\
		_seqno)\
	({\
		iaic_t_submit_t __submit;\
		iaic_ret_code_t __r;\
		__submit.op = AIP_CH_OP_T;\
		__submit.pos_in = _pos_in;\
		__submit.pos_out = _pos_out;\
		__submit.offset.order = _offset_order;\
		__submit.offset.alpha = _offset_alpha;\
		__submit.offset.offset[0] = _offset_offset0;\
		__submit.offset.offset[1] = _offset_offset1;\
		__submit.param = _param;\
		__submit.node = _node;\
		__submit.name = _name;\
		__r = iaic_aip_t_submit(ctx, &__submit);\
		*_seqno = __submit.seqno;\
		__r;\
	 })

#define iaic_set_nice(ctx, nice) \
	(void)iaic_set_proc_nice((ctx)->aip_fd, nice)

#define iaic_get_mem_total(ctx) iaic_get_meminfo(ctx,0)
#define iaic_get_mem_free(ctx)  iaic_get_meminfo(ctx,1)
#define iaic_get_cma_total(ctx) iaic_get_meminfo(ctx,2)
#define iaic_get_cma_free(ctx)  iaic_get_meminfo(ctx,3)

typedef enum {
        IAIC_SCHED_HIGH = -19,
        IAIC_SCHED_MEDIUM = -9,
        IAIC_SCHED_LOW = -1,
} iaic_priority_t;

typedef struct {
	uint32_t drv_version;
	uint32_t lib_version;
	uint32_t chip_version;
} iaic_status_t;

typedef struct {
	pthread_mutex_t mutex_lock;
	int value;
} iaic_shm_t;

typedef struct {
	size_t total_l2c_size;
	size_t curr_l2c_size;
	size_t oram_size;
	void *oram_vbase;
	void *oram_pbase;

	size_t dma_size;
	void *dma_vbase;
	void *dma_pbase;

	size_t dma_desram_size;
	void *dma_desram_vbase;
	void *dma_desram_pbase;
} iaic_nna_status_t;

// From linux kernel include/linux/dma-direction.h
typedef enum {
        DMA_BIDIRECTIONAL = 0,
        DMA_TO_DEVICE = 1,
        DMA_FROM_DEVICE = 2,
        DMA_NONE = 3,
} dma_data_direction_t;


typedef struct {
	int aip_fd;
	int mem_fd;
	int shm_id;
	size_t max_mem_size;
	iaic_status_t status;
	iaic_nna_status_t nna_status;
	iaic_bo_t shared_bo;

	void *cpu_fast_iob;
	void *aip_iob;
	iaic_shm_t *shm;

	struct aip_list bo_list;
	pthread_t thread;
	pthread_mutex_t mutex_lock;
	pthread_spinlock_t spin_lock;
} iaic_ctx_t;

typedef aip_v20_ioctl_wait_t iaic_wait;

// AIP F
typedef aip_v20_ioctl_submit_f_t iaic_f_submit_t;
typedef aip_v20_f_node_t iaic_f_node_t;

// AIP P
typedef aip_v20_ioctl_submit_p_t iaic_p_submit_t;
typedef aip_v20_p_node_t iaic_p_node_t;

// AIP T
typedef aip_v20_ioctl_submit_t_t iaic_t_submit_t;
typedef aip_v20_t_node_t iaic_t_node_t;

/*************************************************
 * name: iaic_img_mode_t
 *
 * description:
 * 	Merge all image formats. Bits 0-4 represent
 * 	the processing content of the AIP_T, bits 4-7
 * 	represent the processing content of he AIP_F,
 * 	and bits 8-11 represent the processing content
 * 	of the AIP_P.
 ************************************************/
typedef enum {
	IAIC_NV12 = 0x2 << 8 | 0x7 << 4 | 0xf,
	IAIC_Y    = 0x0 << 8 | 0x0 << 4 | 0xf,
	IAIC_UV   = 0x1 << 8 | 0x1 << 4 | 0xf,
	IAIC_BGRA = 0x3 << 8 | 0x2 << 4 | 0x0,
	IAIC_GBRA = 0x3 << 8 | 0x2 << 4 | 0x1,
	IAIC_RBGA = 0x3 << 8 | 0x2 << 4 | 0x2,
	IAIC_BRGA = 0x3 << 8 | 0x2 << 4 | 0x3,
	IAIC_GRBA = 0x3 << 8 | 0x2 << 4 | 0x4,
	IAIC_RGBA = 0x3 << 8 | 0x2 << 4 | 0x5,
	IAIC_ABGR = 0x3 << 8 | 0x2 << 4 | 0x8,
	IAIC_AGBR = 0x3 << 8 | 0x2 << 4 | 0x9,
	IAIC_ARBG = 0x3 << 8 | 0x2 << 4 | 0xa,
	IAIC_ABRG = 0x3 << 8 | 0x2 << 4 | 0xb,
	IAIC_AGRB = 0x3 << 8 | 0x2 << 4 | 0xc,
	IAIC_ARGB = 0x3 << 8 | 0x2 << 4 | 0xd,
	// Exclusive to AIP_T mode
	IAIC_FEATURE2   = 0xf << 8 | 0x3 << 4 | 0xf,
	IAIC_FEATURE4   = 0xf << 8 | 0x4 << 4 | 0xf,
	IAIC_FEATURE8   = 0xf << 8 | 0x5 << 4 | 0xf,
	IAIC_FEATURE8_H = 0xf << 8 | 0x6 << 4 | 0xf,
	IAIC_RGBA2BGRA  = 0xf << 8 | 0xf << 4 | 0xe,
	IAIC_BGRA2RGBA  = 0xf << 8 | 0xf << 4 | 0xe
} iaic_img_format_t;

static inline uint32_t iaic_readl(iaic_ctx_t *ctx, uint32_t offset)
{
	return *(volatile uint32_t *)(ctx->aip_iob + offset);
}

static inline void iaic_writel(iaic_ctx_t *ctx,
		uint32_t offset, uint32_t value)
{
	*(volatile uint32_t *)(ctx->aip_iob + offset) = value;
}

static inline uint32_t iaic_aip_t_ctrl_value(iaic_ctx_t *ctx)
{
	return *(volatile uint32_t *)ctx->aip_iob;
}

static inline void iaic_aip_task_start(iaic_ctx_t *ctx)
{
	*(volatile uint32_t *)ctx->aip_iob = 0x1<<4;
	while (0x10 != (iaic_aip_t_ctrl_value(ctx) & 0x10));
}


// context
extern
iaic_ret_code_t iaic_ctx_init_internal(iaic_ctx_t *ctx, size_t mem_size);

extern
iaic_ret_code_t iaic_ctx_destroy(iaic_ctx_t *ctx);

// Action
extern
iaic_ret_code_t iaic_get_status(iaic_ctx_t *ctx, iaic_status_t *status);

extern
iaic_ret_code_t iaic_cacheflush(iaic_ctx_t *ctx, void *vaddr, size_t size, dma_data_direction_t dir);

extern
iaic_ret_code_t iaic_mutex_lock(iaic_ctx_t *ctx);

extern
iaic_ret_code_t iaic_mutex_unlock(iaic_ctx_t *ctx);


// BO
iaic_ret_code_t create_bo_node(iaic_ctx_t *ctx, size_t size);

extern
iaic_ret_code_t iaic_fast_alloc_bo(iaic_ctx_t *ctx, size_t size, iaic_bo_t *bo);
extern
iaic_ret_code_t iaic_fast_free_bo(iaic_ctx_t *ctx, iaic_bo_t *bo);

extern
iaic_ret_code_t iaic_alloc_bo(iaic_ctx_t *ctx, size_t size, iaic_bo_t *bo);
extern
iaic_ret_code_t iaic_free_bo(iaic_ctx_t *ctx, iaic_bo_t *bo);

extern
iaic_ret_code_t iaic_create_bo(iaic_ctx_t *ctx, size_t size, iaic_bo_t *bo);
extern
iaic_ret_code_t iaic_destroy_bo(iaic_ctx_t *ctx, iaic_bo_t *bo);

// NNA
extern
iaic_ret_code_t iaic_nna_status(iaic_ctx_t *ctx, iaic_nna_status_t *status);

extern
iaic_ret_code_t iaic_nna_oram_alloc(iaic_ctx_t *ctx, size_t size, iaic_oram_t *oram);

extern
iaic_ret_code_t iaic_nna_oram_free(iaic_ctx_t *ctx, iaic_oram_t *oram);

extern
iaic_ret_code_t iaic_nna_mutex_lock_internal(iaic_ctx_t *ctx, int is_try, char *name);

extern
iaic_ret_code_t iaic_nna_mutex_unlock_internal(iaic_ctx_t *ctx, int is_force, int restore_cpumask);

// AIP F
extern
iaic_ret_code_t iaic_aip_f_chain_submit(iaic_ctx_t *ctx, iaic_f_submit_t *submit);

// AIP P
extern
iaic_ret_code_t iaic_aip_p_chain_submit(iaic_ctx_t *ctx, iaic_p_submit_t *submit);

// AIP T
extern
iaic_ret_code_t iaic_aip_t_submit(iaic_ctx_t *ctx, iaic_t_submit_t *submit);

// AIP wait
extern
iaic_ret_code_t iaic_aip_f_wait(iaic_ctx_t *ctx,
		uint64_t seqno,
		uint64_t timeout_ns,
		int64_t *wait_time);

extern
iaic_ret_code_t iaic_aip_p_wait(iaic_ctx_t *ctx,
		uint64_t seqno,
		uint64_t timeout_ns,
		int64_t *wait_time);

extern
iaic_ret_code_t iaic_aip_t_wait(iaic_ctx_t *ctx,
		uint64_t seqno,
		uint64_t timeout_ns,
		int64_t *wait_time);

extern
iaic_ret_code_t iaic_convert_config(iaic_ctx_t *ctx,
		uint32_t pos_in, uint32_t pos_out,
		uint32_t order, uint32_t alpha,
		uint32_t offset0, uint32_t offset1,
		uint32_t *param, void *node,
		char *name, uint64_t *seqno);
extern
void iaic_convert_start(iaic_ctx_t *ctx,
		uint32_t dst_base, uint32_t task_len);
extern
void iaic_convert_wait(iaic_ctx_t *ctx);
extern
iaic_ret_code_t iaic_convert_exit(iaic_ctx_t *ctx);
extern
void iaic_convert_reg_read(iaic_ctx_t *ctx);
extern
void iaic_perspective_reg_read(iaic_ctx_t *ctx);

extern
iaic_ret_code_t iaic_nndma_start(iaic_ctx_t *ctx,
		uint32_t bo_paddr, uint32_t oram_oft,
		size_t size, uint32_t dma_oft);

extern
iaic_ret_code_t iaic_create_fnode(iaic_ctx_t *ctx, int num, iaic_bo_t *bo);

extern
iaic_ret_code_t iaic_create_pnode(iaic_ctx_t *ctx, int num, iaic_bo_t *bo);

// Obtaining Space Status
extern
int iaic_get_meminfo(iaic_ctx_t *ctx, int mode);
extern
int iaic_get_bo_free(iaic_ctx_t *ctx);

// Convenient for users to perform simple operations on AIP

void get_img_info(iaic_img_format_t format,
		int wdt, int hgt, int *bpp, int *chn,
		int *stride, int *size);

iaic_bo_t iaic_img_read(
		iaic_ctx_t *ctx, char *img_path,
		size_t wdt, size_t hgt,
		iaic_img_format_t format);

iaic_ret_code_t iaic_img_show(iaic_bo_t img_bo, char *out_path);


iaic_bo_t iaic_cvt_format(iaic_ctx_t *ctx,
		iaic_bo_t src_bo,
		size_t wdt, size_t hgt,
		iaic_img_format_t format);

iaic_bo_t iaic_resize(iaic_ctx_t *ctx,
		iaic_bo_t src_bo,
		size_t src_w, size_t src_h,
		size_t dst_w, size_t dst_h,
		iaic_img_format_t format);

iaic_bo_t iaic_perspective(iaic_ctx_t *ctx,
		iaic_bo_t src_bo,
		size_t src_w, size_t src_h,
		size_t dst_w, size_t dst_h,
		iaic_img_format_t format,
		float matrix[9]);

#ifdef __cplusplus
}
#endif

#endif
