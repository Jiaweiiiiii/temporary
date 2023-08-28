#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <sys/time.h>
#include "iaic.h"


void *pthread1(void *args) {
	iaic_ctx_t *ctx = (iaic_ctx_t *)args;
	iaic_bo_t bo;
	iaic_bo_t bo1;

	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(0, &mask);
	if (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) < 0) {
		printf("set thread affinity failed\n");
	}


	struct timeval start_time, end_time;
	double total_time;
	gettimeofday(&start_time, NULL);

	iaic_create_bo(ctx, 0x100, &bo);
	memset(bo.vaddr, 0x00, 0x100);
	iaic_create_bo(ctx, 0x100, &bo1);
	memcpy(bo1.vaddr, bo.vaddr, 0x100);

	gettimeofday(&end_time, NULL);
	long time = start_time.tv_sec * 1000000 + start_time.tv_usec;
	time = end_time.tv_sec * 1000000 + end_time.tv_usec - time;
	total_time = time *1.0 / 1000;
	printf("Running time of pthread1 is %.03lf ms\n", total_time);

	iaic_destroy_bo(ctx, &bo);
	while(1){
		printf("1111\n");
		sleep(1);
	}
	pthread_exit(NULL);
}

void *pthread2(void *args) {
	iaic_ctx_t *ctx = (iaic_ctx_t *)args;
	iaic_bo_t bo;
	iaic_bo_t bo1;

	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(0, &mask);
	if (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) < 0) {
		printf("set thread affinity failed\n");
	}

	struct timeval start_time, end_time;
	double total_time;
	gettimeofday(&start_time, NULL);

	iaic_create_bo(ctx, 0x100, &bo);
	memset(bo.vaddr, 0x00, 0x100);
	iaic_create_bo(ctx, 0x100, &bo1);
	memcpy(bo1.vaddr, bo.vaddr, 0x100);

	gettimeofday(&end_time, NULL);
	long time = start_time.tv_sec * 1000000 + start_time.tv_usec;
	time = end_time.tv_sec * 1000000 + end_time.tv_usec - time;
	total_time = time *1.0 / 1000;
	printf("Running time of pthread2 is %.03lf ms\n", total_time);

	iaic_destroy_bo(ctx, &bo);
	while(1) {
		printf("2222\n");
		sleep(1);
	}
	pthread_exit(NULL);
}

int aipv20_test()
{
	iaic_ctx_t ctx;
	iaic_ctx_init(&ctx, 0x1000);

	int ret;
	pthread_t tid1, tid2;
	ret = pthread_create(&tid1, NULL, pthread1, (void *)&ctx);
	if (ret != 0) {
		printf("Create thread1 failed!\n");
		return -1;
	}
	ret = pthread_create(&tid2, NULL, pthread2, (void *)&ctx);
	if (ret != 0) {
		printf("Create thread2 failed!\n");
		return -1;
	}

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	printf("===========================\n");

	ret = pthread_create(&tid1, NULL, pthread1, (void *)&ctx);
	if (ret != 0) {
		printf("Create thread1 failed!\n");
		return -1;
	}
	ret = pthread_create(&tid2, NULL, pthread2, (void *)&ctx);
	if (ret != 0) {
		printf("Create thread2 failed!\n");
		return -1;
	}

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);


	iaic_ctx_destroy(&ctx);
	return 0;
}

int main(int argc, char *argv[])
{
	int ret;
	ret = aipv20_test();
	if (ret < 0)
		printf("aipv20_test failed!\n");
	return 0;
}

