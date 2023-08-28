#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct {
    long type;
    int mem_type;
}Msg;
int main(int argc, char *argv) {
	int res;
    int id = msgget(0x8888, IPC_CREAT | 0664);
	if(id < 0)
	{
		printf("create message queue error\n");
		return -1;
	}
	Msg msg;
	msg.type = 2;
	msg.mem_type = 2;
		res = msgsnd(id, &msg, sizeof(Msg), 0);
		if(res < 0)
		{
			printf("sed message is error\n");
			return -1;
		}
    return 0;
}
