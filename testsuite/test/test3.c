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

	iaic_process_lock(&ctx);
	sleep(1);
	iaic_process_unlock(&ctx);



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

