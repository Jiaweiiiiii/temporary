#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <sys/time.h>
#include "iaic.h"



int nnav20_dma(int run_cnt, pid_t pid, uint64_t *nna_time, uint64_t *nna_time_lock)
{
	int ret = 0;
	iaic_ctx_t iaic_ctx;
	size_t op_size = 1024 * 8;
	size_t nnadma_length;
	iaic_bo_t bo;
	iaic_oram_t oram;
	char res;

	cpu_set_t cpumask;
	struct timeval time0, time1; 
	struct timeval time2, time3; 
	uint64_t time0_us, time1_us, run_time;
	uint64_t time2_us, time3_us, run_time_lock;

	volatile uint32_t *nnadma_desram;
	volatile uint32_t *nnadma_cfg;


	CPU_ZERO(&cpumask);
	CPU_SET(0, &cpumask);
	sched_setaffinity(0, sizeof(cpumask), &cpumask);

	ret = iaic_ctx_init(&iaic_ctx);
	iaic_set_nice(&iaic_ctx, -19);
	if (ret) {
		printf("iaic_ctx_init failed, ret = %d\n", ret);
		return -1;
	}
	//printf("nndma vbase :0x%08x\n",
	//		(int)iaic_ctx.nna_status.dma_vbase);
	//printf("nndma pbase :0x%08x\n",
	//		(int)iaic_ctx.nna_status.dma_pbase);
	//printf("nndma desram vbase :0x%08x\n",
	//		(int)iaic_ctx.nna_status.dma_desram_vbase);
	//printf("nndma desram pbase :0x%08x\n",
	//		(int)iaic_ctx.nna_status.dma_desram_pbase);

	// Create BO
	ret = iaic_create_bo(&iaic_ctx, op_size, &bo);
	if (ret) {
		printf("bo alloc failed\n");
		goto unref;
	}

	//printf("bo paddr:0x%08x 0x%08x\n", (int)bo.vaddr, (int)bo.paddr);

	// Alloc ORAM
	ret = iaic_nna_oram_alloc(&iaic_ctx, op_size, &oram);
	if (ret) {
		printf("oram alloc failed\n");
		goto fail;
	}
	//printf("oram paddr:0x%08x offset:0x%08x\n",
	//		(int)oram.paddr,
	//		oram.offset);

	for (int i = 0; i < run_cnt; i++) {

	memset(bo.vaddr, 0xa, op_size);
	ret = iaic_cacheflush(&iaic_ctx,
			bo.vaddr, op_size, DMA_TO_DEVICE);

	gettimeofday(&time0, NULL);
	iaic_nna_mutex_lock(&iaic_ctx);
	gettimeofday(&time1, NULL);

	//iaic_nna_mutex_lock(&iaic_ctx);
	//usleep(100);
	//iaic_nna_mutex_unlock(&iaic_ctx);


	//iaic_nna_mutex_lock(&iaic_ctx);
#if 1
	// NNA DMA
	nnadma_desram = (volatile uint32_t *)iaic_ctx.nna_status.dma_desram_vbase;
	nnadma_cfg = (volatile uint32_t *)iaic_ctx.nna_status.dma_vbase;

	nnadma_length = (op_size >> 6) - 1;
	*nnadma_desram = ((uint32_t)(bo.paddr) & ~(0x3f)) | (((nnadma_length >> 12) & 0x1f) << 1);
	nnadma_desram++;
	*nnadma_desram = (oram.offset >> 6) | ((nnadma_length & 0xfff) << 20);
	*nnadma_cfg = 1<<31;
	iaic_fast_iob(&iaic_ctx);

	iaic_nndma_rch0_wait;

#if 1
	// Check
	for (int i = 0; i < op_size; i++) {
		res = ((char *)oram.vaddr)[i];
		if (res != 0xa) {
			printf("test0 result error i=%d 0x%x 0x%08x\n", i, res,
					(oram.offset + i));
			goto err;
		}
	}
#endif
#endif


	memset(bo.vaddr, 0xb, op_size);
	ret = iaic_cacheflush(&iaic_ctx,
			bo.vaddr, op_size, DMA_TO_DEVICE);

	// NNA DMA
	nnadma_desram = (volatile uint32_t *)iaic_ctx.nna_status.dma_desram_vbase;
	nnadma_cfg = (volatile uint32_t *)iaic_ctx.nna_status.dma_vbase;

	nnadma_length = (op_size >> 6) - 1;
	*nnadma_desram = ((uint32_t)(bo.paddr) & ~(0x3f)) | (((nnadma_length >> 12) & 0x1f) << 1);
	nnadma_desram++;
	*nnadma_desram = (oram.offset >> 6) | ((nnadma_length & 0xfff) << 20);
	*nnadma_cfg = 0x80000000;
	asm volatile("lw $0, 0(%0)"::"r"(nnadma_cfg));

	iaic_nndma_rch0_wait;
	iaic_fast_iob(&iaic_ctx);

#if 1
	// Check
	for (int i = 0; i < op_size; i++) {
		res = ((char *)oram.vaddr)[i];
		if (res != 0xb) {
			printf("test1 result error i=%d 0x%x 0x%08x\n", i, res,
					(oram.offset + i));
			goto err;
		}
	}
#endif

	//printf("result ok!\n");

err:
	//sleep(2);
	gettimeofday(&time2, NULL);
	iaic_nna_mutex_unlock(&iaic_ctx);
	gettimeofday(&time3, NULL);
	//iaic_nna_mutex_unlock_set(&iaic_ctx, 1);

	time0_us = time0.tv_sec * 1000000 + time0.tv_usec;
	time1_us = time1.tv_sec * 1000000 + time1.tv_usec;
	time2_us = time2.tv_sec * 1000000 + time2.tv_usec;
	time3_us = time3.tv_sec * 1000000 + time3.tv_usec;
	run_time = time2_us - time1_us;
	run_time_lock = time3_us - time0_us;
	nna_time[i] = run_time;
	nna_time_lock[i] = run_time_lock;


	}

	ret = iaic_nna_oram_free(&iaic_ctx, &oram);
fail:
	ret = iaic_destroy_bo(&iaic_ctx, &bo);
unref:
	ret = iaic_ctx_destroy(&iaic_ctx);

	return ret;
}

int main(int argc, char**argv)
{
	int ret;
	int run_cnt = 1;
	int sched_priority = 0;
	struct timeval time0, time1; 
	uint64_t time0_us, time1_us, total_time = 0ll,
		 max_run_time = 0ll, min_run_time = -1ll,
		 max_run_time_lock = 0ll, min_run_time_lock = -1ll;
	uint64_t *nna_time, *nna_time_lock;
	pid_t pid = getpid();

	for (int i = 1; i < argc; i++) {
		if (i == 1) {
			sscanf(argv[i], "%d", &run_cnt);
			printf("PID:%d run cnt:%d\n", pid, run_cnt);
		} else if (i == 2) {
			sscanf(argv[i], "%d", &sched_priority);
			printf("PID:%d sched priority:%d\n", pid, sched_priority);
		}
	}

	nna_time = (uint64_t *)malloc(sizeof(uint64_t) * run_cnt * 2);
	if (nna_time == NULL) {
		printf("nna time buffer alloc fail\n");
		return -1;
	}
	nna_time_lock = nna_time + run_cnt;

	gettimeofday(&time0, NULL);

	ret = nnav20_dma(run_cnt, pid, nna_time, nna_time_lock);
	if (ret < 0)
		printf("nnav20_dma failed!\n");

	gettimeofday(&time1, NULL);

	time0_us = time0.tv_sec * 1000000 + time0.tv_usec;
	time1_us = time1.tv_sec * 1000000 + time1.tv_usec;
	total_time = time1_us - time0_us;

	for (int i = 0; i < run_cnt; i++) {
		if (nna_time[i] > max_run_time)
			max_run_time = nna_time[i];
		if (nna_time[i] < min_run_time)
			min_run_time = nna_time[i];
		//printf("run time:%llu\n", nna_time[i]);

		if (nna_time_lock[i] > max_run_time_lock)
			max_run_time_lock = nna_time_lock[i];
		if (nna_time_lock[i] < min_run_time_lock)
			min_run_time_lock = nna_time_lock[i];
		//printf("run time lock:%llu\n", nna_time_lock[i]);
	}

	printf("PID:%d total time %llu agv:%llu\n", pid, total_time, total_time / run_cnt);
	printf("PID:%d max:%llu min:%llu\n", pid, max_run_time, min_run_time);
	printf("PID:%d max-lock:%llu min-lock:%llu\n", pid, max_run_time_lock, min_run_time_lock);



	free(nna_time);

	return 0;
}
