#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "iaic.h"

typedef struct {
	pthread_mutex_t lock;
	int value;
} shm_data_t;

int aipv20_test()
{
	iaic_ctx_t ctx;
	//iaic_ctx_init(&ctx, 0x100);

	int shm_id, flag = 0;
	pthread_mutexattr_t attr;
	shm_data_t *shm_data = NULL;


	key_t key = ftok("/dev/aip2.0", 99);
	if (key < 0) {
		perror("ftok");
		return -1;
	}
	shm_id = shmget(key, sizeof(shm_data_t), IPC_EXCL | IPC_CREAT | 0666);
	if (shm_id < 0) {
		perror("shmget");
		shm_id = shmget(key, sizeof(shm_data_t), IPC_CREAT | 0666);
		flag = 1;
	}
	shm_data = (shm_data_t *)shmat(shm_id, NULL, 0);

	if (flag == 0) {
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
		pthread_mutex_init(&shm_data->lock, &attr);
	}

	pthread_mutex_lock(&shm_data->lock);
	shm_data->value++;
	printf("value[%d]\n", shm_data->value);
	sleep(1);
	pthread_mutex_unlock(&shm_data->lock);

	shmctl(shm_id, IPC_RMID, NULL);
	//iaic_ctx_destroy(&ctx);
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

