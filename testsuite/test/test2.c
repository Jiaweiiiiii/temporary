#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include "iaic.h"

typedef struct {
	    int locked;
} Lock;

int aipv20_test()
{
	iaic_ctx_t ctx;
	iaic_ctx_init(&ctx, 0x1000);

	// 获取key
	key_t key = ftok("/dev/aip2.0", 99);
	int shm_id = shmget(key, sizeof(Lock), IPC_CREAT | 0644);
	Lock *lock = (Lock*) shmat(shm_id, (void*)0, 0);

	int expected = 0;
	while (!__sync_bool_compare_and_swap(&lock->locked, expected, 1)) {
		expected = 0;
		sched_yield();
	}

	printf("start !\n");
	sleep(1);
	printf("end !!!\n");


	__sync_bool_compare_and_swap(&lock->locked, 1, 0);


	
	// 销毁共享空间
	shmctl(shm_id, IPC_RMID, NULL);

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

