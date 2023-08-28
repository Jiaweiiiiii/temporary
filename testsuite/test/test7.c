#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/time.h>
#include "iaic.h"

typedef struct {
	    int locked;
} Lock;

int aipv20_test()
{
	iaic_ctx_t ctx;
	iaic_ctx_init(&ctx, 0x1000);
	int i;

	struct timeval start_time, end_time;
	double total_time;
	long time;

	// --------------------------
	gettimeofday(&start_time, NULL);
	for (i = 0; i < 1000; i++) {
		iaic_process_lock(&ctx);
		iaic_process_unlock(&ctx);
	}
	gettimeofday(&end_time, NULL);
	time = (end_time.tv_sec - start_time.tv_sec) * 1000000 +
			(end_time.tv_usec - start_time.tv_usec);
	total_time = time * 1.0 / 1000;
	printf("Running time of iaic_process_lock is %lf ms\n", total_time / 1000);

	// --------------------------
	gettimeofday(&start_time, NULL);
	for (i = 0; i < 1000; i++) {
		iaic_mutex_lock(&ctx);
		iaic_mutex_unlock(&ctx);
	}
	gettimeofday(&end_time, NULL);
	time = (end_time.tv_sec - start_time.tv_sec) * 1000000 +
			(end_time.tv_usec - start_time.tv_usec);
	total_time = time * 1.0 / 1000;
	printf("Running time of iaic_mutex_lock is %lf ms\n", total_time / 1000);

	// --------------------------
	gettimeofday(&start_time, NULL);
	iaic_nna_mutex_lock(&ctx);
	for (i = 0; i < 999; i++) {
		iaic_nna_mutex_unlock_set(&ctx, 0);
		iaic_nna_mutex_lock(&ctx);
	}
	iaic_nna_mutex_unlock(&ctx);
	gettimeofday(&end_time, NULL);
	time = (end_time.tv_sec - start_time.tv_sec) * 1000000 +
			(end_time.tv_usec - start_time.tv_usec);
	total_time = time * 1.0 / 1000;
	printf("Running time of iaic_nna_mutex_lock&set is %lf ms\n", total_time / 1000);

	// --------------------------
	gettimeofday(&start_time, NULL);
	for (i = 0; i < 1000; i++) {
		iaic_nna_mutex_lock(&ctx);
		iaic_nna_mutex_unlock(&ctx);
	}
	gettimeofday(&end_time, NULL);
	time = (end_time.tv_sec - start_time.tv_sec) * 1000000 +
			(end_time.tv_usec - start_time.tv_usec);
	total_time = time * 1.0 / 1000;
	printf("Running time of iaic_nna_mutex_lock is %lf ms\n", total_time / 1000);


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

