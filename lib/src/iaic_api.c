
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sched.h>
#include <ctype.h>
#include <sys/shm.h>

#include "iaic.h"
#include "iaic_list.h"

iaic_ret_code_t iaic_ctx_init_internal(iaic_ctx_t *ctx, size_t mem_size)
{
	size_t max_size;
	uint32_t aip_iobase;
	iaic_ret_code_t ret_code;
	key_t key;
	int is_existing = 0;

#if 0
	if (ctx->aip_fd > 0) {
		printf("[IAIC] Repeat initialization\n");
		return IAIC_CODE_INIT_FAIL;
	}
#endif
	ctx->aip_fd = open("/dev/aip2.0", O_RDWR);
	if (ctx->aip_fd < 0) {
		perror("[IAIC] can not open AIP device");
		return IAIC_CODE_NO_AIP_DEVICE;
	}

	// Create shared memory
	if (iaic_mutex_lock(ctx) != 0) {
		perror("[IAIC] iaic_mutex_lock failed");
		ret_code = IAIC_CODE_INIT_FAIL;
		goto err;
	}
	key = ftok("/dev/aip2.0", 99);
	if (key < 0) {
		perror("[IAIC] ftok failed\n");
		iaic_mutex_unlock(ctx);
		ret_code = IAIC_CODE_INIT_FAIL;
		goto err;
	}
	ctx->shm_id = shmget(key, sizeof(iaic_shm_t), IPC_EXCL | IPC_CREAT | 0666);
	if (ctx->shm_id < 0) {
		ctx->shm_id = shmget(key, sizeof(iaic_shm_t), IPC_CREAT | 0666);
		if (ctx->shm_id < 0) {
			perror("[IAIC] shmget failed\n");
			iaic_mutex_unlock(ctx);
			ret_code = IAIC_CODE_INIT_FAIL;
			goto err;
		}
		is_existing = 1;
	}
	ctx->shm = (iaic_shm_t *)shmat(ctx->shm_id, NULL, 0);
	if (ctx->shm == (void*)-1) {
		perror("[IAIC] shmat failed\n");
		iaic_mutex_unlock(ctx);
		ret_code = IAIC_CODE_INIT_FAIL;
		goto err;
	}
	if (!is_existing) {
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
		if (pthread_mutex_init(&ctx->shm->mutex_lock, &attr) != 0) {
			perror("[IAIC] pthread_mutex_init failed\n");
			iaic_mutex_unlock(ctx);
			ret_code = IAIC_CODE_INIT_FAIL;
			goto err;
		}
	}
	iaic_mutex_unlock(ctx);

	ret_code = iaic_get_status(ctx, &ctx->status);
	if (ret_code)
		goto err;
	printf("\033[1;32mIAIC lib-version:0x%05x drv-version:0x%05x\033[0m\n",
			ctx->status.lib_version, ctx->status.drv_version);

	if (ctx->status.chip_version == 0xa1) {
		max_size = 0x10000000;
		aip_iobase = 0x13090000;
	} else {
		max_size = 0x8000000;
		aip_iobase = 0x12b00000;
	}
	if (mem_size > max_size) {
		printf("[IAIC] Max_mem_size is set too large!\n");
		goto err;
	}

	ctx->mem_fd = open("/dev/mem", O_RDWR);
	if (ctx->mem_fd < 0) {
		perror("[IAIC] can not open MEM device");
		ret_code = IAIC_CODE_NO_MEM_DEVICE;
		goto err;
	}

	ret_code = iaic_nna_status(ctx, &ctx->nna_status);
	if (ret_code != IAIC_CODE_OK) {
		goto fail;
	}

	// NNA ORAM memory map
	ctx->nna_status.oram_vbase = mmap(NULL,
			ctx->nna_status.oram_size,
			PROT_READ | PROT_WRITE, MAP_SHARED, ctx->aip_fd,
			(off_t)ctx->nna_status.oram_pbase);
	if (ctx->nna_status.oram_vbase == MAP_FAILED) {
		perror("[IAIC] NNA ORAM init failed");
		ret_code = IAIC_CODE_NNA_ORAM_INIT_FAIL;
		goto fail;
	}

	// NNA DMA memory map
	ctx->nna_status.dma_vbase = mmap(NULL, ctx->nna_status.dma_size,
			PROT_READ | PROT_WRITE, MAP_SHARED,
			ctx->mem_fd,
			(off_t)ctx->nna_status.dma_pbase);
	if (ctx->nna_status.dma_vbase == MAP_FAILED) {
		perror("[IAIC] NNA DMA init failed");
		ret_code = IAIC_CODE_NNA_DMA_INIT_FAIL;
		goto nna_dma_fail;
	}

	// NNA DMA-desram memory map
	ctx->nna_status.dma_desram_vbase = mmap(NULL, ctx->nna_status.dma_desram_size,
			PROT_READ | PROT_WRITE, MAP_SHARED,
			ctx->mem_fd,
			(off_t)ctx->nna_status.dma_desram_pbase);
	if (ctx->nna_status.dma_desram_vbase == MAP_FAILED) {
		perror("[IAIC] NNA DMA-desram init failed");
		ret_code = IAIC_CODE_NNA_DMA_DESRAM_INIT_FAIL;
		goto nna_dma_desram_fail;
	}

	// CPU fast-iob memory map
	ctx->cpu_fast_iob = mmap(NULL, PAGE_SIZE,
			PROT_READ, MAP_SHARED,
			ctx->aip_fd,
			(off_t)0);
	if (ctx->cpu_fast_iob == MAP_FAILED) {
		perror("[IAIC] CPU fsat-iob init failed");
		ret_code = IAIC_CODE_CPU_FAST_IOB_INIT_FAIL;
		goto cpu_fast_iob_fail;
	}

	ctx->aip_iob = mmap(NULL, PAGE_SIZE,
			PROT_READ | PROT_WRITE, MAP_SHARED,
			ctx->mem_fd, (off_t)aip_iobase);
	if (ctx->aip_iob == MAP_FAILED) {
		perror("[IAIC] AIP iobase init failed");
		ret_code = IAIC_CODE_CPU_FAST_IOB_INIT_FAIL;
		goto aip_iobase_fail;
	}

	(void)iaic_set_proc_nice(ctx->aip_fd, -19);

	ctx->max_mem_size = mem_size;
	AIP_LIST_INIT(&ctx->bo_list);
	pthread_mutex_init(&ctx->mutex_lock, NULL);

	if (mem_size > 1) {
		if(create_bo_node(ctx, mem_size)) {
			pthread_mutex_destroy(&ctx->mutex_lock);
			goto aip_iobase_fail;
		}
	}

	return iaic_create_bo(ctx, 0x10, &ctx->shared_bo);

aip_iobase_fail:
	munmap(ctx->cpu_fast_iob, PAGE_SIZE);
	ctx->cpu_fast_iob = MAP_FAILED;

cpu_fast_iob_fail:
	munmap(ctx->nna_status.dma_desram_vbase, ctx->nna_status.dma_desram_size);
	ctx->nna_status.dma_desram_vbase = MAP_FAILED;

nna_dma_desram_fail:
	munmap(ctx->nna_status.dma_vbase, ctx->nna_status.dma_size);
	ctx->nna_status.dma_vbase = MAP_FAILED;

nna_dma_fail:
	munmap(ctx->nna_status.oram_vbase -
	       ctx->nna_status.curr_l2c_size,
			ctx->nna_status.oram_size);
	ctx->nna_status.oram_vbase = MAP_FAILED;

fail:
	close(ctx->mem_fd);
	ctx->mem_fd = 0;

err:
	close(ctx->aip_fd);
	ctx->aip_fd = 0;

	return ret_code;
}

iaic_ret_code_t iaic_ctx_destroy(iaic_ctx_t *ctx)
{
	iaic_destroy_bo(ctx, &ctx->shared_bo);
	pthread_mutex_destroy(&ctx->mutex_lock);

	if (ctx->aip_iob != MAP_FAILED) {
		munmap(ctx->aip_iob, PAGE_SIZE);
		ctx->aip_iob = MAP_FAILED;
	}

	if (ctx->cpu_fast_iob != MAP_FAILED) {
		munmap(ctx->cpu_fast_iob, PAGE_SIZE);
		ctx->cpu_fast_iob = MAP_FAILED;
	}

	if (ctx->nna_status.oram_vbase != MAP_FAILED) {
		munmap(ctx->nna_status.oram_vbase,
				ctx->nna_status.oram_size);
		ctx->nna_status.oram_vbase = MAP_FAILED;
	}

	if (ctx->nna_status.dma_vbase != MAP_FAILED) {
		munmap(ctx->nna_status.dma_vbase,
				ctx->nna_status.dma_size);
		ctx->nna_status.dma_vbase = MAP_FAILED;
	}

	if (ctx->nna_status.dma_desram_vbase != MAP_FAILED) {
		munmap(ctx->nna_status.dma_desram_vbase,
				ctx->nna_status.dma_desram_size);
		ctx->nna_status.dma_desram_vbase = MAP_FAILED;
	}


	if (ctx->mem_fd > 0) {
		close(ctx->mem_fd);
		ctx->mem_fd = -1;
	}

	if (ctx->aip_fd > 0) {
		shmdt(ctx->shm);
		shmctl(ctx->shm_id, IPC_RMID, NULL);
		close(ctx->aip_fd);
		ctx->aip_fd = -1;
	}

	return 0;
}

iaic_ret_code_t iaic_get_status(iaic_ctx_t *ctx, iaic_status_t *status)
{
	int __attribute__((__unused__)) ret;
	int fd = ctx->aip_fd;
	aip_v20_ioctl_action_status_t ioctl_action_status;
	ioctl_action_status.drv_version = 0;
	ioctl_action_status.chip_version = 0;

	ioctl_action_status.op = AIP_ACTION_STATUS;
	status->lib_version = AIP_V20_LIB_VERSION;
	status->drv_version = AIP_V20_DRV_VERSION;

	ret = IAIC_IOCTL(fd, IOCTL_AIP_V20_ACTION, &ioctl_action_status);
	status->drv_version = ioctl_action_status.drv_version;
	status->chip_version = ioctl_action_status.chip_version;

	if (status->drv_version < AIP_V20_DRV_VERSION) {
		printf("[IAIC] drv version not match 0x%08x 0x%08x\n",
				AIP_V20_DRV_VERSION,
				status->drv_version);
		return IAIC_CODE_DRV_VERSION_NOMATCH;
	}

	return IAIC_CODE_OK;
}

iaic_ret_code_t iaic_mutex_lock(iaic_ctx_t *ctx)
{
	aip_v20_ioctl_action_status_t ioctl_action;
	ioctl_action.op = AIP_ACTION_CTX_LOCK;
	if (IAIC_IOCTL(ctx->aip_fd, IOCTL_AIP_V20_ACTION, &ioctl_action) != 0) {
		return IAIC_CODE_SPIN_LOCK_FAIL;
	}
	return IAIC_CODE_OK;
}

iaic_ret_code_t iaic_mutex_unlock(iaic_ctx_t *ctx)
{
	aip_v20_ioctl_action_status_t ioctl_action;
	ioctl_action.op = AIP_ACTION_CTX_UNLOCK;
	if (IAIC_IOCTL(ctx->aip_fd, IOCTL_AIP_V20_ACTION, &ioctl_action) != 0) {
		return IAIC_CODE_SPIN_UNLOCK_FAIL;
	}
	return IAIC_CODE_OK;
}

iaic_ret_code_t iaic_cacheflush(iaic_ctx_t *ctx, void *vaddr, size_t size, dma_data_direction_t dir)
{
	int ret;
	int fd = ctx->aip_fd;
	aip_v20_ioctl_action_cacheflush_t ioctl_action_cacheflush;

	ioctl_action_cacheflush.op = AIP_ACTION_CACHE_FLUSH;
	ioctl_action_cacheflush.vaddr = vaddr;
	ioctl_action_cacheflush.size = size;
	ioctl_action_cacheflush.dir = (int)(dir);

	ret = IAIC_IOCTL(fd, IOCTL_AIP_V20_ACTION, &ioctl_action_cacheflush);
	if (ret < 0) {
		perror("[IAIC] AIP cache flush failed");
		return IAIC_CODE_AIP_CACHE_FLUSH_FAIL;
	}

	return IAIC_CODE_OK;
}

iaic_ret_code_t iaic_create_bo(iaic_ctx_t *ctx, size_t size, iaic_bo_t *bo)
{
	if (ctx->max_mem_size > 1) {
		return iaic_fast_alloc_bo(ctx, size, bo);
	}
	return iaic_alloc_bo(ctx, size, bo);
}

iaic_ret_code_t iaic_destroy_bo(iaic_ctx_t *ctx, iaic_bo_t *bo)
{
	if (ctx->max_mem_size > 1) {
		return iaic_fast_free_bo(ctx, bo);
	}
	return iaic_free_bo(ctx, bo);
}


iaic_ret_code_t iaic_nna_status(iaic_ctx_t *ctx, iaic_nna_status_t *status)
{
	int ret;
	int fd = ctx->aip_fd;

	aip_v20_ioctl_nna_status_t nna;
	nna.op = NNA_OP_STATUS;
	ret = IAIC_IOCTL(fd, IOCTL_NNA_V20_OP, &nna);
	if (ret < 0) {
		perror("[IAIC] failed to get the ORAM status");
		return IAIC_CODE_NNA_STATUS_FAIL;
	}

	status->total_l2c_size = nna.total_l2c_size;
	status->curr_l2c_size = nna.curr_l2c_size;

	status->oram_size = nna.oram_size;
	status->oram_vbase = (void *)(0);
	status->oram_pbase = (void *)(nna.oram_pbase) +
		status->curr_l2c_size;

	status->dma_size = nna.dma_size;
	status->dma_vbase = (void *)(0);
	status->dma_pbase = (void *)(nna.dma_pbase);

	status->dma_desram_size = nna.dma_desram_size;
	status->dma_desram_vbase = (void *)(0);
	status->dma_desram_pbase = (void *)(nna.dma_desram_pbase);

	return IAIC_CODE_OK;
}


iaic_ret_code_t iaic_nna_oram_alloc(iaic_ctx_t *ctx, size_t size, iaic_oram_t *oram)
{
	int ret;
	int fd = ctx->aip_fd;

	aip_v20_ioctl_nna_oram_t nna;
	nna.op = NNA_OP_ORAM_ALLOC;
	nna.size = size;

	ret = IAIC_IOCTL(fd, IOCTL_NNA_V20_OP, &nna);
	if (ret < 0) {
		perror("[IAIC] NNA ORAM alloc failed");
		return IAIC_CODE_NNA_ORAM_ALLOC_FAIL;
	}

	oram->size = size;
	oram->handle = nna.handle;
	oram->paddr = (void *)nna.paddr;
	oram->offset = oram->paddr -
		ctx->nna_status.oram_pbase +
		ctx->nna_status.curr_l2c_size;
	oram->vaddr = ctx->nna_status.oram_vbase -
		ctx->nna_status.curr_l2c_size +
		oram->offset;

	return IAIC_CODE_OK;
}

iaic_ret_code_t iaic_nna_oram_free(iaic_ctx_t *ctx, iaic_oram_t *oram)
{
	int ret;
	int fd = ctx->aip_fd;

	aip_v20_ioctl_nna_oram_t nna;
	nna.op = NNA_OP_ORAM_FREE;
	nna.handle = oram->handle;
	nna.size = oram->size;
	nna.paddr = oram->paddr;

	ret = IAIC_IOCTL(fd, IOCTL_NNA_V20_OP, &nna);
	if (ret < 0) {
		perror("[IAIC] NNA ORAM free failed");
		return IAIC_CODE_NNA_ORAM_FREE_FAIL;
	}

	oram->handle = 0;

	return IAIC_CODE_OK;
}

iaic_ret_code_t iaic_nna_mutex_lock_internal(iaic_ctx_t *ctx, int is_try, char *name)
{
	int ret;
	int fd = ctx->aip_fd;

	aip_v20_ioctl_nna_mutex_lock_t nna;
	nna.op = is_try ? NNA_OP_MUTEX_TRYLOCK : NNA_OP_MUTEX_LOCK;
	nna.name = name;

	ret = IAIC_IOCTL(fd, IOCTL_NNA_V20_OP, &nna);
	if (ret < 0) {
		perror("[IAIC] NNA mutex lock failed");
		return IAIC_CODE_NNA_MUTEX_LOCK_FAIL;
	}
	//ret = sched_yield();

	return IAIC_CODE_OK;
}

iaic_ret_code_t iaic_nna_mutex_unlock_internal(iaic_ctx_t *ctx, int cpumask, int is_force)
{
	int ret;
	int fd = ctx->aip_fd;

	aip_v20_ioctl_nna_mutex_unlock_t nna;
	nna.op = NNA_OP_MUTEX_UNLOCK;
	nna.force_unlock = is_force;
	nna.cpumask = cpumask;

	ret = IAIC_IOCTL(fd, IOCTL_NNA_V20_OP, &nna);
	if (ret < 0) {
		perror("[IAIC] NNA mutex unlock failed");
		return IAIC_CODE_NNA_MUTEX_UNLOCK_FAIL;
	}

	return IAIC_CODE_OK;
}

// AIP F
iaic_ret_code_t iaic_aip_f_chain_submit(iaic_ctx_t *ctx, iaic_f_submit_t *submit)
{
	int ret;
	int fd = ctx->aip_fd;

	if(ctx->status.chip_version == 0xa1) {
		submit->node_num++;
	}

	ret = IAIC_IOCTL(fd, IOCTL_AIP_V20_SUBMIT, submit);

	return (ret < 0) ? IAIC_CODE_AIP_F_SUBMIT_FAIL :
				IAIC_CODE_OK;
}

// AIP P
iaic_ret_code_t iaic_aip_p_chain_submit(iaic_ctx_t *ctx, iaic_p_submit_t *submit)
{
	int ret;
	int fd = ctx->aip_fd;

	if(ctx->status.chip_version == 0xa1) {
		submit->node_num++;
	}

	ret = IAIC_IOCTL(fd, IOCTL_AIP_V20_SUBMIT, submit);

	return (ret < 0) ? IAIC_CODE_AIP_P_SUBMIT_FAIL :
				IAIC_CODE_OK;
}

// AIP T
iaic_ret_code_t iaic_aip_t_submit(iaic_ctx_t *ctx, iaic_t_submit_t *submit)
{
	int ret;
	int fd = ctx->aip_fd;

	iaic_t_node_t *node = (iaic_t_node_t *)submit->node;
	if (node->task_len < 2 || node->src_h < 2) {
		printf("IAIC error:[task_len] & [src_h] must be greater than to 1.\n");
		return IAIC_CODE_AIP_T_SUBMIT_FAIL;
	}

	ret = IAIC_IOCTL(fd, IOCTL_AIP_V20_SUBMIT, submit);

	return (ret < 0) ? IAIC_CODE_AIP_T_SUBMIT_FAIL :
				IAIC_CODE_OK;
}

// AIP wait
iaic_ret_code_t iaic_aip_f_wait(iaic_ctx_t *ctx,
		uint64_t seqno,
		uint64_t timeout_ns,
		int64_t *wait_time)
{
	return iaic_aip_wait(ctx,
			seqno, timeout_ns, wait_time,
			AIP_CH_OP_F);
}

iaic_ret_code_t iaic_aip_p_wait(iaic_ctx_t *ctx,
		uint64_t seqno,
		uint64_t timeout_ns,
		int64_t *wait_time)
{
	return iaic_aip_wait(ctx,
			seqno, timeout_ns, wait_time,
			AIP_CH_OP_P);
}

iaic_ret_code_t iaic_aip_t_wait(iaic_ctx_t *ctx,
		uint64_t seqno,
		uint64_t timeout_ns,
		int64_t *wait_time)
{
	return iaic_aip_wait(ctx,
			seqno, timeout_ns, wait_time,
			AIP_CH_OP_T);
}

iaic_ret_code_t iaic_convert_config(iaic_ctx_t *ctx,
		uint32_t pos_in, uint32_t pos_out,
		uint32_t order, uint32_t alpha,
		uint32_t offset0, uint32_t offset1,
		uint32_t *param, void *node,
		char *name, uint64_t *seqno)
{
	int fd = ctx->aip_fd;
	iaic_t_submit_t submit;
	iaic_ret_code_t ret;

	iaic_t_node_t *user_node = (iaic_t_node_t *)node;
	if (user_node->task_len < 2 || user_node->src_h < 2) {
		printf("IAIC error:[task_len] & [src_h] must be greater than to 1.\n");
		return IAIC_CODE_AIP_T_SUBMIT_FAIL;
	}

	submit.op = AIP_CH_OP_T_TASK;
	submit.pos_in = pos_in;
	submit.pos_out = pos_out;
	submit.offset.order = order;
	submit.offset.alpha = alpha;
	submit.offset.offset[0] = offset0;
	submit.offset.offset[1] = offset1;
	submit.param = param;
	submit.node = node;
	submit.name = name;

	ret = IAIC_IOCTL(fd, IOCTL_AIP_V20_SUBMIT, &submit);

	*seqno = submit.seqno;
	return (ret < 0) ? IAIC_CODE_AIP_T_SUBMIT_FAIL :
				IAIC_CODE_OK;
}

void iaic_convert_start(iaic_ctx_t *ctx,
		uint32_t dst_base, uint32_t task_len)
{
	iaic_writel(ctx, 0x1c, dst_base);
	iaic_writel(ctx, 0x10, task_len);
	iaic_aip_task_start(ctx);
}

void iaic_convert_wait(iaic_ctx_t *ctx)
{
	while (0x10 == (iaic_aip_t_ctrl_value(ctx) & 0x10));
	while (0x08 != (iaic_aip_t_ctrl_value(ctx) & 0x08));
}

iaic_ret_code_t iaic_convert_exit(iaic_ctx_t *ctx)
{
	iaic_wait wait;
	iaic_ret_code_t ret;
	wait.op = AIP_CH_OP_T_TASK;
	wait.seqno = 0;
	wait.timeout_ns = -1ll;

	ret = IAIC_IOCTL(ctx->aip_fd,
			IOCTL_AIP_V20_WAIT, &wait);
	ret = (ret < 0) ? IAIC_CODE_AIP_WAIT_FAIL :
		IAIC_CODE_OK;

	return ret;
}

void iaic_convert_reg_read(iaic_ctx_t *ctx)
{
	printf("AIP_T_CTRL[0x%08x]\n", iaic_readl(ctx, 0x00));
	printf("AIP_T_IRQ [0x%08x]\n", iaic_readl(ctx, 0x04));
	printf("AIP_T_CFG [0x%08x]\n", iaic_readl(ctx, 0x08));
	printf("AIP_T_TIME[0x%08x]\n", iaic_readl(ctx, 0x0c));
	printf("AIP_TASK_L[0x%08x]\n", iaic_readl(ctx, 0x10));
	printf("AIP_SRC_YB[0x%08x]\n", iaic_readl(ctx, 0x14));
	printf("AIP_SRC_CB[0x%08x]\n", iaic_readl(ctx, 0x18));
	printf("AIP_DST_B [0x%08x]\n", iaic_readl(ctx, 0x1c));
	printf("AIP_SRTIDE[0x%08x]\n", iaic_readl(ctx, 0x20));
	printf("AIP_T_W&H [0x%08x]\n", iaic_readl(ctx, 0x24));
	printf("AIP_PARAM0[0x%08x]\n", iaic_readl(ctx, 0x28));
	printf("AIP_PARAM1[0x%08x]\n", iaic_readl(ctx, 0x2c));
	printf("AIP_PARAM2[0x%08x]\n", iaic_readl(ctx, 0x30));
	printf("AIP_PARAM3[0x%08x]\n", iaic_readl(ctx, 0x34));
	printf("AIP_PARAM4[0x%08x]\n", iaic_readl(ctx, 0x38));
	printf("AIP_PARAM5[0x%08x]\n", iaic_readl(ctx, 0x3c));
	printf("AIP_PARAM6[0x%08x]\n", iaic_readl(ctx, 0x40));
	printf("AIP_PARAM7[0x%08x]\n", iaic_readl(ctx, 0x44));
	printf("AIP_PARAM8[0x%08x]\n", iaic_readl(ctx, 0x48));
	printf("AIP_T_OFST[0x%08x]\n", iaic_readl(ctx, 0x4c));
}

void iaic_perspective_reg_read(iaic_ctx_t *ctx)
{
	printf("AIP_P_CTRL[0x%08x]\n", iaic_readl(ctx, 0x300));
	printf("AIP_P_IRQ [0x%08x]\n", iaic_readl(ctx, 0x304));
	printf("AIP_P_CFG [0x%08x]\n", iaic_readl(ctx, 0x308));
	printf("AIP_P_TIME[0x%08x]\n", iaic_readl(ctx, 0x30c));
	printf("AIP_P_MODE[0x%08x]\n", iaic_readl(ctx, 0x310));
	printf("AIP_SRC_YB[0x%08x]\n", iaic_readl(ctx, 0x314));
	printf("AIP_SRC_CB[0x%08x]\n", iaic_readl(ctx, 0x318));
	printf("AIP_SRTIDE[0x%08x]\n", iaic_readl(ctx, 0x31c));
	printf("AIP_DST_B [0x%08x]\n", iaic_readl(ctx, 0x320));
	printf("AIP_SRTIDE[0x%08x]\n", iaic_readl(ctx, 0x324));
	printf("AIP_DST_WH[0x%08x]\n", iaic_readl(ctx, 0x328));
	printf("AIP_SRC_WH[0x%08x]\n", iaic_readl(ctx, 0x32c));
	printf("AIP_DUMMY [0x%08x]\n", iaic_readl(ctx, 0x330));
	printf("AIP_COEF0 [0x%08x]\n", iaic_readl(ctx, 0x334));
	printf("AIP_COEF1 [0x%08x]\n", iaic_readl(ctx, 0x338));
	printf("AIP_COEF2 [0x%08x]\n", iaic_readl(ctx, 0x33c));
	printf("AIP_COEF3 [0x%08x]\n", iaic_readl(ctx, 0x340));
	printf("AIP_COEF4 [0x%08x]\n", iaic_readl(ctx, 0x344));
	printf("AIP_COEF5 [0x%08x]\n", iaic_readl(ctx, 0x348));
	printf("AIP_COEF6 [0x%08x]\n", iaic_readl(ctx, 0x34c));
	printf("AIP_COEF7 [0x%08x]\n", iaic_readl(ctx, 0x350));
	printf("AIP_COEF8 [0x%08x]\n", iaic_readl(ctx, 0x354));
	printf("AIP_COEF9 [0x%08x]\n", iaic_readl(ctx, 0x358));
	printf("AIP_COEFa [0x%08x]\n", iaic_readl(ctx, 0x35c));
	printf("AIP_COEFb [0x%08x]\n", iaic_readl(ctx, 0x360));
	printf("AIP_COEFc [0x%08x]\n", iaic_readl(ctx, 0x364));
	printf("AIP_COEFd [0x%08x]\n", iaic_readl(ctx, 0x368));
	printf("AIP_COEFe [0x%08x]\n", iaic_readl(ctx, 0x36c));
	printf("AIP_PARAM0[0x%08x]\n", iaic_readl(ctx, 0x370));
	printf("AIP_PARAM1[0x%08x]\n", iaic_readl(ctx, 0x374));
	printf("AIP_PARAM2[0x%08x]\n", iaic_readl(ctx, 0x378));
	printf("AIP_PARAM3[0x%08x]\n", iaic_readl(ctx, 0x37c));
	printf("AIP_PARAM4[0x%08x]\n", iaic_readl(ctx, 0x380));
	printf("AIP_PARAM5[0x%08x]\n", iaic_readl(ctx, 0x384));
	printf("AIP_PARAM6[0x%08x]\n", iaic_readl(ctx, 0x388));
	printf("AIP_PARAM7[0x%08x]\n", iaic_readl(ctx, 0x38c));
	printf("AIP_PARAM8[0x%08x]\n", iaic_readl(ctx, 0x390));
	printf("AIP_T_OFST[0x%08x]\n", iaic_readl(ctx, 0x394));
}


iaic_ret_code_t iaic_nndma_start(iaic_ctx_t *ctx,
		uint32_t bo_paddr, uint32_t oram_paddr,
		size_t size, uint32_t dma_oft)
{
	volatile uint32_t *desram_vbase =
		(volatile uint32_t *)ctx->nna_status.dma_desram_vbase;
	volatile uint32_t *nndma_vbase =
		(volatile uint32_t *)ctx->nna_status.dma_vbase;
	uint32_t oram_oft;
	uint32_t dma_size1, dma_size2;
	if ((size & 0x3f) != 0x0) {
		printf("IAIC error:The size of dma transport is not 64 aligned.\n");
		return -1;
	}
	if ((oram_paddr & 0x12000000) == 0x0) {
		printf("IAIC error:The third parameter is not the oram_paddr.\n");
		return -1;
	}

	size = size >> 6;
	size--;
	if (size > 0x20000) {
		printf("IAIC error:The size exceeds the limit.\n");
		return -1;
	}

	if (size > 0x1000) {
		dma_size1 = size & 0xfff;
		dma_size2 = ((size >> 12) & 0x1f) << 1;
	} else {
		dma_size1 = size;
		dma_size2 = 0;
	}

	oram_oft = oram_paddr - 0x12600000;

	desram_vbase[0] = bo_paddr | dma_size2;
	desram_vbase[1] = dma_size1 << 20 | oram_oft >> 6;
	*(volatile uint32_t *)((uint32_t)nndma_vbase + dma_oft) = 0x1 << 31;
	*(volatile uint32_t *)((uint32_t)nndma_vbase + dma_oft);

	return IAIC_CODE_OK;
}


iaic_ret_code_t iaic_create_fnode(iaic_ctx_t *ctx,
		int num, iaic_bo_t *bo)
{
	if (num < 1)
		return IAIC_CODE_OK;

	if (ctx->status.chip_version == 0xa1) {
		num++;
	}

	size_t size = num * sizeof(aip_v20_f_node_t);

	return iaic_create_bo(ctx, size, bo);
}

iaic_ret_code_t iaic_create_pnode(iaic_ctx_t *ctx,
		int num, iaic_bo_t *bo)
{
	if (num < 1)
		return IAIC_CODE_OK;

	if (ctx->status.chip_version == 0xa1) {
		num++;
	}

	size_t size = num * sizeof(aip_v20_p_node_t);

	return iaic_create_bo(ctx, size, bo);
}

int iaic_get_meminfo(iaic_ctx_t *ctx, int mode)
{
	int ret;
	int fd = ctx->aip_fd;
	aip_v20_ioctl_action_meminfo_t meminfo;
	meminfo.op = AIP_ACTION_GET_MEMINFO;

	ret = IAIC_IOCTL(fd, IOCTL_AIP_V20_ACTION, &meminfo);
	if (ret < 0) {
		perror("IAIC IOCTL_AIP_V20_ACTION failed");
		return -1;
	}

	switch (mode) {
	case 0:
		ret = meminfo.mem_total;
		break;
	case 1:
		ret = meminfo.mem_free;
		break;
	case 2:
		ret = meminfo.cma_total;
		break;
	case 3:
		ret = meminfo.cma_free;
		break;
	default:
		printf("no this mode\n");
	}

	return ret;
}




/*
 * Convenient for users to directly control AIP
 * for image processing.
 */
void get_img_info(iaic_img_format_t format,
		int wdt, int hgt, int *bpp, int *chn,
		int *stride, int *size)
{
	if (format >= IAIC_BGRA &&format <= IAIC_ARGB) {
		format = IAIC_BGRA;
	}
	switch (format) {
	case IAIC_NV12:
		*bpp = 12;
		*chn = 1;
		break;
	case IAIC_Y:
		*bpp = 8;
		*chn = 1;
		break;
	case IAIC_UV:
		*bpp = 8;
		*chn = 2;
		break;
	case IAIC_BGRA:
		*bpp = 8;
		*chn = 4;
		break;
	case IAIC_FEATURE2:
		*bpp = 2;
		*chn = 32;
		break;
	case IAIC_FEATURE4:
		*bpp = 4;
		*chn = 32;
		break;
	case IAIC_FEATURE8:
		*bpp = 8;
		*chn = 32;
		break;
	case IAIC_FEATURE8_H:
		*bpp = 8;
		*chn = 16;
		break;
	default:
		printf("IAIC error:This image format is not available.\n");
		*bpp = 0;
		*chn = 0;
	}
	if(format == IAIC_NV12) {
		*stride =  wdt;
		*size = (*stride) * hgt * (*bpp) >> 3;
	} else {
		*stride =  wdt * (*chn) * (*bpp) >> 3;
		*size = *stride * hgt;
	}
}

iaic_bo_t iaic_img_read(
		iaic_ctx_t *ctx, char *img_path,
		size_t wdt, size_t hgt,
		iaic_img_format_t format)
{
	FILE *img_fp;
	iaic_bo_t img_bo;
	int stride, size;
	int bpp, chn;
	get_img_info(format, wdt, hgt,
			&bpp, &chn, &stride, &size);
	if(iaic_create_bo(ctx, size, &img_bo)) {
		img_bo.vaddr = NULL;
		return img_bo;
	}
	img_fp = fopen(img_path, "r");
	if(img_fp == NULL) {
		perror("IAIC Image fopen failed");
		iaic_destroy_bo(ctx, &img_bo);
		img_bo.vaddr = NULL;
		return img_bo;
	}
	fread(img_bo.vaddr, 32, size/32, img_fp);
	if(fclose(img_fp) < 0){
		perror("IAIC Image fclose failed");
		iaic_destroy_bo(ctx, &img_bo);
		img_bo.vaddr = NULL;
		return img_bo;
	}
	return img_bo;
}

iaic_ret_code_t iaic_img_show(
		 iaic_bo_t img_bo, char *out_path)
{
	FILE *dst_fp;
	dst_fp = fopen(out_path, "w+");
	if(dst_fp == NULL) {
		perror("IAIC dst_image fopen error");
		return -1;
	}
	fwrite(img_bo.vaddr, 32, img_bo.size/32, dst_fp);
	if(fclose(dst_fp) != 0){
		perror("IAIC dst_image fclose error");
		return -1;
	}

	return IAIC_CODE_OK;
}

iaic_bo_t iaic_cvt_format(iaic_ctx_t *ctx,
		iaic_bo_t src_bo,
		size_t wdt, size_t hgt,
		iaic_img_format_t format)
{
	int ret;
	int src_bpp, src_chn, src_stride, src_size;
	int dst_bpp, dst_chn, dst_stride, dst_size;
	iaic_bo_t dst_bo;
	iaic_t_node_t tnode;
	uint32_t src_paddr_uv =
		(uint32_t)src_bo.paddr + wdt * hgt;
	uint32_t nv2bgr_coef[9] = {
		1220542, 2116026, 0,
		1220542, 409993, 852492,
		1220542, 0, 1673527
	};
	uint64_t job_seqno, job_timeout_ns;

	get_img_info(IAIC_NV12, wdt, hgt, &src_bpp,
			&src_chn, &src_stride, &src_size);
	get_img_info(format, wdt, hgt, &dst_bpp,
			&dst_chn, &dst_stride, &dst_size);

	ret = iaic_create_bo(ctx, dst_size, &dst_bo);
	if (ret) {
		perror("IAIC dst_tbo failed");
		return dst_bo;
	}

	if (format == IAIC_RGBA2BGRA) {
		for(int i = 0; i < src_bo.size; i += 4) {
			char *src_vaddr = (char *)(src_bo.vaddr + i);
			char *dst_vaddr = (char *)(dst_bo.vaddr + i);
			*dst_vaddr = *(src_vaddr + 2);
			*(dst_vaddr + 1) = *(src_vaddr + 1);
			*(dst_vaddr + 2) = *src_vaddr;
			*(dst_vaddr + 3) = *(src_vaddr + 3);
		}
		return dst_bo;
	}

	tnode.src_ybase = (uint32_t)src_bo.paddr;
	tnode.src_cbase = src_paddr_uv;
	tnode.dst_base = (uint32_t)dst_bo.paddr;
	tnode.src_h = hgt;
	tnode.src_w = wdt;
	tnode.src_stride = src_stride;
	tnode.dst_stride = dst_stride;
	tnode.task_len = hgt;
	iaic_aip_t(ctx,
			AIP_DAT_POS_DDR,
			AIP_DAT_POS_DDR,
			format & 0xf,
			255, 16, 128,
			nv2bgr_coef,
			(void *)(&tnode),
			NULL, &job_seqno);
	if (ret) {
		printf("IAIC convert failed, ret = %d\n", ret);
		iaic_destroy_bo(ctx, &dst_bo);
		return dst_bo;
	}
	job_timeout_ns = -1ll;
	ret = iaic_aip_t_wait(ctx, job_seqno, job_timeout_ns, NULL);
	if (ret) {
		printf("IAIC convert wait faild\n");
		iaic_destroy_bo(ctx, &dst_bo);
		return dst_bo;
	}

	return dst_bo;
}

iaic_bo_t iaic_resize(iaic_ctx_t *ctx,
		iaic_bo_t src_bo,
		size_t src_w, size_t src_h,
		size_t dst_w, size_t dst_h,
		iaic_img_format_t format)
{
	int ret;
	int src_bpp, src_chn, src_stride, src_size;
	int dst_bpp, dst_chn, dst_stride, dst_size;
	iaic_bo_t dst_bo;
	iaic_bo_t node_bo;
	uint32_t src_paddr_uv, dst_paddr_uv;
	uint32_t scale_x, scale_y;
	uint64_t job_seqno, job_timeout_ns;

	get_img_info(format, src_w, src_h, &src_bpp,
			&src_chn, &src_stride, &src_size);
	get_img_info(format, dst_w, dst_h, &dst_bpp,
			&dst_chn, &dst_stride, &dst_size);

	ret = iaic_create_bo(ctx, dst_size, &dst_bo);
	if (ret) {
		perror("IAIC dst_pbo failed");
		return dst_bo;
	}
	ret = iaic_create_fnode(ctx,
			sizeof(aip_v20_f_node_t), &node_bo);
	if (ret) {
		perror("IAIC fnode_bo failed");
		iaic_destroy_bo(ctx, &dst_bo);
		return dst_bo;
	}

	src_paddr_uv = (uint32_t)src_bo.paddr + src_w * src_h;
	dst_paddr_uv = (uint32_t)dst_bo.paddr + dst_w * dst_h;

	scale_x = (float)src_w / (float)dst_w;
	scale_y = (float)src_h / (float)dst_h;

	aip_v20_f_node_t *fnode = (aip_v20_f_node_t *)node_bo.vaddr;
	fnode->timeout[0] = 0xffffffff;
	fnode->scale_x[0] = (uint32_t)(scale_x * 65536);
	fnode->scale_y[0] = (uint32_t)(scale_y * 65536);
	fnode->trans_x[0] = (int32_t)((scale_x * 0.5 - 0.5) * 65536) + 16;
	fnode->trans_y[0] = (int32_t)((scale_y * 0.5 - 0.5) * 65536) + 16;
	fnode->src_base_y[0] = (uint32_t)src_bo.paddr;
	fnode->src_base_uv[0] = src_paddr_uv;
	fnode->dst_base_y[0] = (uint32_t)dst_bo.paddr;
	fnode->dst_base_uv[0] = dst_paddr_uv;
	fnode->src_size[0] = src_h<<16 | src_w;
	fnode->dst_size[0] = dst_h<<16 | dst_w;
	fnode->stride[0] = dst_stride<<16 | src_stride;

	ret = iaic_aip_f_chain(ctx,
			(format >> 4) & 0xf,
			AIP_DAT_POS_DDR,
			AIP_DAT_POS_DDR,
			&node_bo, 1,
			NULL, &job_seqno);
	if (ret) {
		printf("IAIC resize failed, ret = %d\n", ret);
		iaic_destroy_bo(ctx, &node_bo);
		iaic_destroy_bo(ctx, &dst_bo);
		return dst_bo;
	}
	job_timeout_ns = -1ll;
	ret = iaic_aip_f_wait(ctx, job_seqno, job_timeout_ns, NULL);
	if (ret) {
		printf("IAIC resize wait faild\n");
		iaic_destroy_bo(ctx, &node_bo);
		iaic_destroy_bo(ctx, &dst_bo);
		return dst_bo;
	}

	iaic_destroy_bo(ctx, &node_bo);
	return dst_bo;
}

iaic_bo_t iaic_perspective(iaic_ctx_t *ctx,
		iaic_bo_t src_bo,
		size_t src_w, size_t src_h,
		size_t dst_w, size_t dst_h,
		iaic_img_format_t format,
		float matrix[9])
{
	int ret;
	int src_bpp, src_chn, src_stride, src_size;
	int dst_bpp, dst_chn, dst_stride, dst_size;
	iaic_bo_t dst_bo;
	iaic_bo_t node_bo;
	uint32_t src_paddr_uv =
		(uint32_t)src_bo.paddr + src_w * src_h;
	uint32_t nv2bgr_coef[9] = {
		1220542, 2116026, 0,
		1220542, 409993, 852492,
		1220542, 0, 1673527
	};
	double Dxx, A11, A22, b1, b2;
	double t0, t1, t2, t3, dst_m[9] = {0};
	int64_t ax,bx,cx,ay,by,cy,az,bz,cz;
	uint64_t job_seqno, job_timeout_ns;

	if (format == IAIC_NV12) {
		get_img_info(format, src_w, src_h, &src_bpp,
				&src_chn, &src_stride, &src_size);
		get_img_info(IAIC_BGRA, dst_w, dst_h, &dst_bpp,
				&dst_chn, &dst_stride, &dst_size);
	} else {
		get_img_info(format, src_w, src_h, &src_bpp,
				&src_chn, &src_stride, &src_size);
		get_img_info(format, dst_w, dst_h, &dst_bpp,
				&dst_chn, &dst_stride, &dst_size);
	}


	ret = iaic_create_bo(ctx, dst_size, &dst_bo);
	if (ret) {
		perror("IAIC dst_pbo failed");
		return dst_bo;
	}
	ret = iaic_create_pnode(ctx,
			sizeof(aip_v20_p_node_t), &node_bo);
	if (ret) {
		perror("IAIC pnode_bo failed");
		iaic_destroy_bo(ctx, &dst_bo);
		return dst_bo;
	}

	if (iaic_fabs(matrix[6]) < 1e-10 && iaic_fabs(matrix[7]) < 1e-10) {
		Dxx = matrix[0] * matrix[4] - matrix[1] * matrix[3];
		Dxx = Dxx != 0? 1.0/Dxx : 0;
		A11 = matrix[4] * Dxx , A22 = matrix[0] * Dxx;
		matrix[0] = A11;
		matrix[1] *= -Dxx;
		matrix[3] *= -Dxx;
		matrix[4] = A22;
		b1 = -matrix[0] * matrix[2] - matrix[1] * matrix[5];
		b2 = -matrix[3] * matrix[2] - matrix[4] * matrix[5];
		matrix[2] = b1;
		matrix[5] = b2;
		ax = (int64_t)iaic_round(matrix[0] * 65536 * 1024);
		bx = (int64_t)iaic_round(matrix[1] * 65536 * 1024);
		cx = (int64_t)iaic_round(matrix[2] * 65536 * 1024);
		ay = (int64_t)iaic_round(matrix[3] * 65536 * 1024);
		by = (int64_t)iaic_round(matrix[4] * 65536 * 1024);
		cy = (int64_t)iaic_round(matrix[5] * 65536 * 1024);
		az = (int64_t)iaic_round(matrix[6] * 65536 * 1024);
		bz = (int64_t)iaic_round(matrix[7] * 65536 * 1024);
		cz = (int64_t)iaic_round(matrix[8] * 65536 * 1024);
	} else {
		t0 = matrix[0] * (matrix[4] * matrix[8] - matrix[5] * matrix[7]);
		t1 = matrix[1] * (matrix[3] * matrix[8] - matrix[5] * matrix[6]);
		t2 = matrix[2] * (matrix[3] * matrix[7] - matrix[4] * matrix[6]);
		t3 = t0 - t1 + t2;
		if(t3 != 0.0) {
			float def = 1.0 / t3;
			dst_m[0] = (matrix[4] * matrix[8] - matrix[5] * matrix[7]) * def;
			dst_m[1] = (matrix[2] * matrix[7] - matrix[1] * matrix[8]) * def;
			dst_m[2] = (matrix[1] * matrix[5] - matrix[2] * matrix[4]) * def;

			dst_m[3] = (matrix[5] * matrix[6] - matrix[3] * matrix[8]) * def;
			dst_m[4] = (matrix[0] * matrix[8] - matrix[2] * matrix[6]) * def;
			dst_m[5] = (matrix[2] * matrix[3] - matrix[0] * matrix[5]) * def;

			dst_m[6] = (matrix[3] * matrix[7] - matrix[4] * matrix[6]) * def;
			dst_m[7] = (matrix[1] * matrix[6] - matrix[0] * matrix[7]) * def;
			dst_m[8] = (matrix[0] * matrix[4] - matrix[1] * matrix[3]) * def;
		}
		ax = (int64_t)iaic_round(dst_m[0] * 65536 * 1024);
		bx = (int64_t)iaic_round(dst_m[1] * 65536 * 1024);
		cx = (int64_t)iaic_round(dst_m[2] * 65536 * 1024);
		ay = (int64_t)iaic_round(dst_m[3] * 65536 * 1024);
		by = (int64_t)iaic_round(dst_m[4] * 65536 * 1024);
		cy = (int64_t)iaic_round(dst_m[5] * 65536 * 1024);
		az = (int64_t)iaic_round(dst_m[6] * 65536 * 1024);
		bz = (int64_t)iaic_round(dst_m[7] * 65536 * 1024);
		cz = (int64_t)iaic_round(dst_m[8] * 65536 * 1024);
	}

	aip_v20_p_node_t *pnode = (iaic_p_node_t *)node_bo.vaddr;
	pnode->timeout[0]      = 0xffffffff;
	pnode->src_base_y[0]   = (uint32_t)src_bo.paddr;
	pnode->src_base_uv[0]  = src_paddr_uv;
	pnode->src_stride[0]   = src_stride;
	pnode->dst_base[0]     = (uint32_t)dst_bo.paddr;
	pnode->dst_stride[0]   = dst_stride;
	pnode->dst_size[0]     = dst_h<<16 | dst_w;
	pnode->src_size[0]     = src_h<<16 | src_w;
	pnode->dummy_val[0]    = 0x80801010;
	pnode->coef0[0]        = (ax & 0xffffffff);
	pnode->coef1[0]        = ((bx & 0xffff) << 16) | ((ax & 0xffff00000000) >> 32);
	pnode->coef2[0]        = (bx & 0xffffffff0000) >> 16;
	pnode->coef3[0]        = (cx & 0xffffffff);
	pnode->coef4[0]        = (cx & 0x7ffffff00000000) >> 32;
	pnode->coef5[0]        = (ay & 0xffffffff);
	pnode->coef6[0]        = ((by & 0xffff) << 16) | ((ay & 0xffff00000000) >> 32);
	pnode->coef7[0]        = (by & 0xffffffff0000) >> 16;
	pnode->coef8[0]        = (cy & 0xffffffff);
	pnode->coef9[0]        = (cy & 0x7ffffff00000000) >> 32;
	pnode->coef10[0]       = (az & 0xffffffff);
	pnode->coef11[0]       = ((bz & 0xffff) << 16) | ((az & 0xffff00000000) >> 32);
	pnode->coef12[0]       = (bz & 0xffffffff0000) >> 16;
	pnode->coef13[0]       = (cz & 0xffffffff);
	pnode->coef14[0]       = (cz & 0x7ffffff00000000) >> 32;

	ret = iaic_aip_p_chain(ctx,
			(format >> 8) & 0xf,
			AIP_DAT_POS_DDR,
			AIP_DAT_POS_DDR,
			AIP_ORDER_BGRA,
			0, 16, 128,
			nv2bgr_coef,
			&node_bo, 1,
			NULL, &job_seqno);
	if (ret) {
		printf("IAIC perspective failed, ret = %d\n", ret);
		iaic_destroy_bo(ctx, &node_bo);
		iaic_destroy_bo(ctx, &dst_bo);
		return dst_bo;
	}
	job_timeout_ns = -1ll;
	ret = iaic_aip_p_wait(ctx, job_seqno, job_timeout_ns, NULL);
	if (ret) {
		printf("IAIC perspective wait faild\n");
		iaic_destroy_bo(ctx, &node_bo);
		iaic_destroy_bo(ctx, &dst_bo);
		return dst_bo;
	}

	return dst_bo;
}
