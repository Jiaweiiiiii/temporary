#ifndef __IAIC_CORE_H__
#define __IAIC_CORE_H__

#include <sys/ioctl.h>

#include "iaic_ioctl.h"

#if 0
#define M_CURR_CTX ________iaic_curr_context________
#define M_CURR_CTX_FD M_CURR_CTX.fd

typedef struct {
	int fd;
} iaic_context_t;

extern iaic_context_t M_CURR_CTX;
#endif

#define IAIC_BO_EXT_MAP 1

// BO order size 64byte
#define BO_ORDER_SHIFT  8
#define BO_ORDER_SIZE   (1 << BO_ORDER_SHIFT)


// PAGE_SIZE_4KB
#define PAGE_SHIFT      12
#define PAGE_SIZE       (1 << PAGE_SHIFT)

#define __round_mask(x, y) ((__typeof__(x))((y)-1))
#define round_up(x, y) ((((x)-1) | __round_mask(x, y))+1)
#define round_down(x, y) ((x) & ~__round_mask(x, y))
#define iaic_round(x) (((x) >= 0) ? (int)((x) + 0.5) : (int)((x) - 0.5))
#define iaic_fabs(x) (((x) < 0) ? -(x) : (x))

#define IAIC_IOCTL iaic_ioctl

#define JZ_AIP_T_BASE 0x12b00000

typedef enum {
	IAIC_CODE_OK,
	IAIC_CODE_NO_AIP_DEVICE,
	IAIC_CODE_NO_MEM_DEVICE,
	IAIC_CODE_CTX_DEINIT,
	IAIC_CODE_CTX_DESTROY_FAIL,
	IAIC_CODE_BO_CREATE_FAIL,
	IAIC_CODE_BO_CREATE_MMAP_FAIL,
	IAIC_CODE_BO_CREATE_EXT_FAIL,
	IAIC_CODE_BO_DESTROY_FAIL,
	IAIC_CODE_NNA_STATUS_FAIL,
	IAIC_CODE_NNA_ORAM_INIT_FAIL,
	IAIC_CODE_NNA_DMA_INIT_FAIL,
	IAIC_CODE_NNA_DMA_DESRAM_INIT_FAIL,
	IAIC_CODE_NNA_ORAM_ALLOC_FAIL,
	IAIC_CODE_NNA_ORAM_FREE_FAIL,
	IAIC_CODE_NNA_MUTEX_LOCK_FAIL,
	IAIC_CODE_NNA_MUTEX_UNLOCK_FAIL,
	IAIC_CODE_AIP_F_SUBMIT_FAIL,
	IAIC_CODE_AIP_P_SUBMIT_FAIL,
	IAIC_CODE_AIP_T_SUBMIT_FAIL,
	IAIC_CODE_AIP_WAIT_FAIL,
	IAIC_CODE_AIP_T_CLOSE_FAIL,
	IAIC_CODE_DRV_VERSION_NOMATCH,
	IAIC_CODE_LIB_VERSION_NOMATCH,
	IAIC_CODE_AIP_CACHE_FLUSH_FAIL,
	IAIC_CODE_CPU_FAST_IOB_INIT_FAIL,
	IAIC_CODE_INIT_FAIL,
	IAIC_CODE_SPIN_LOCK_FAIL,
	IAIC_CODE_SPIN_UNLOCK_FAIL
} iaic_ret_code_t;


struct aip_list {
	struct aip_list *prev;
	struct aip_list *next;
};

typedef struct block{
	struct aip_list node;
	size_t size; // 使用大小
	int idle; // 状态，1表示空闲，0表示占用
	int is_bo; // 记录bo的节点，1表示是，2表示否
	int brother;
	int handle;
	void *paddr;
	void *vaddr;
} block_t;

typedef struct {
	size_t size;
	size_t esize;
	int handle;
	void *paddr;
	void *vaddr;
	void *evaddr;
} iaic_bo_t;

typedef struct {
	uint32_t offset;
	size_t size;
	int handle;
	void *vaddr;
	void *paddr;
} iaic_oram_t;


extern
int iaic_ioctl(int fd, unsigned long request, void *arg);

extern
int iaic_set_proc_nice(int aip_fd, int32_t value);

#endif
