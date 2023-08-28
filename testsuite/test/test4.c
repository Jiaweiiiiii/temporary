#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include "iaic.h"

union semun {
    int                 val;
    struct semid_ds     *buf;
    unsigned short      *array;
    struct seminfo      *_buf;
};

int create_sem(key_t key)
{
    int sem_id = semget(key, 1, IPC_CREAT | IPC_EXCL | 0660);
    if(sem_id >= 0)
    {
        union semun arg;
        arg.val = 1;
        if(semctl(sem_id, 0, SETVAL, arg) < 0)
        {
            semctl(sem_id, 0, IPC_RMID);
            sem_id = -1;
        }
    }

    return sem_id;
}

void lock_sem(int sem_id)
{
    struct sembuf buf;

    buf.sem_num = 0;
    buf.sem_op = -1;
    buf.sem_flg = SEM_UNDO;

    semop(sem_id, &buf, 1);
}

void unlock_sem(int sem_id)
{
    struct sembuf buf;

    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = SEM_UNDO;

    semop(sem_id, &buf, 1);
}

int aipv20_test()
{
	iaic_ctx_t ctx;
	iaic_ctx_init(&ctx, 0x1000);

	// 获取key
	key_t key = ftok("/dev/aip2.0", 99);
	int sem_id = create_sem(key);

	lock_sem(sem_id);

	printf("start !\n");
	sleep(1);
	printf("end !!!\n");

	unlock_sem(sem_id);

	
	// 销毁共享空间
	semctl(sem_id, 0, IPC_RMID);

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

