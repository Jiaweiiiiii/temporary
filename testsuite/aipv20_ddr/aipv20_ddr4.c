#include <stdio.h>
#include <sys/time.h>
#include "iaic.h"

int aipv20_ddr()
{

	int ret, i;
	iaic_ctx_t iaic_ctx;

	ret = iaic_ctx_init(&iaic_ctx, 0x1000000);


	int num = 1000;
	iaic_bo_t bo[num];

struct timeval start_time, end_time;
double total_time;
gettimeofday(&start_time, NULL);
	for (i = 0; i < num; i++) {
		ret = iaic_create_bo(&iaic_ctx, 1024, &bo[i]);
		if (ret != 0) {
			printf("create_bo error!\n");
			return ret;
		}
	}
gettimeofday(&end_time, NULL);
long time = start_time.tv_sec * 1000000 + start_time.tv_usec;
time = end_time.tv_sec * 1000000 + end_time.tv_usec - time;
total_time = time *1.0 / 1000;
printf("Running time of create_new_bo is %.03lf ms\n", total_time);

gettimeofday(&start_time, NULL);
	for (i = 0; i < num; i++) {
		iaic_destroy_bo(&iaic_ctx, &bo[i]);
	}
gettimeofday(&end_time, NULL);
time = start_time.tv_sec * 1000000 + start_time.tv_usec;
time = end_time.tv_sec * 1000000 + end_time.tv_usec - time;
total_time = time *1.0 / 1000;
printf("Running time of create_new_free is %.03lf ms\n", total_time);


gettimeofday(&start_time, NULL);
	for (i = 0; i < num; i++) {
		ret = iaic_alloc_bo(&iaic_ctx, 1024, &bo[i]);
		if (ret != 0) {
			printf("create_bo error!\n");
			return ret;
		}
	}
gettimeofday(&end_time, NULL);
time = start_time.tv_sec * 1000000 + start_time.tv_usec;
time = end_time.tv_sec * 1000000 + end_time.tv_usec - time;
total_time = time *1.0 / 1000;
printf("Running time of create_old_bo is %.03lf ms\n", total_time);



gettimeofday(&start_time, NULL);
	for (i = 0; i < num; i++) {
		iaic_free_bo(&iaic_ctx, &bo[i]);
	}
gettimeofday(&end_time, NULL);
time = start_time.tv_sec * 1000000 + start_time.tv_usec;
time = end_time.tv_sec * 1000000 + end_time.tv_usec - time;
total_time = time *1.0 / 1000;
printf("Running time of create_old_free is %.03lf ms\n", total_time);

	ret = iaic_ctx_destroy(&iaic_ctx);

	return ret;
}

int main()
{
	int ret;
	ret = aipv20_ddr();
	if (ret != 0)
		printf("aipv20_ddr failed!\n");
	return 0;
}
