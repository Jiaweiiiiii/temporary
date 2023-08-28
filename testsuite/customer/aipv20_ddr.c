#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "iaic.h"


int aipv20_test()
{
	iaic_ctx_t ctx;
	iaic_bo_t bo1, bo2, bo3, bo4, bo5;
	// Set the continuous memory size used by this process
	iaic_ctx_init(&ctx, 0x1000);

	// Obtain mem information
	int memtotal = iaic_get_mem_total(&ctx);
	int memfree = iaic_get_mem_free(&ctx);
	int cmatotal = iaic_get_cma_total(&ctx);
	int cmafree = iaic_get_cma_free(&ctx);
	printf("MemTotal[%dkB], MemFree[%dkB]\n",
			memtotal, memfree);
	printf("CmaTotal[%dkB], CmaFree[%dkB]\n",
			cmatotal, cmafree);

	printf("BoFree[0x%x]\n", iaic_get_bo_free(&ctx));

	// Apply for continuous mem from bo
	iaic_create_bo(&ctx, 0x600, &bo1);
	printf("BoFree[0x%x]\n", iaic_get_bo_free(&ctx));

	iaic_create_bo(&ctx, 0x600, &bo2);
	printf("BoFree[0x%x]\n", iaic_get_bo_free(&ctx));

	iaic_destroy_bo(&ctx,&bo1); //->free
	iaic_create_bo(&ctx, 0x600, &bo3);
	printf("BoFree[0x%x]\n", iaic_get_bo_free(&ctx));

	iaic_destroy_bo(&ctx,&bo2); //->free
	iaic_create_bo(&ctx, 0x600, &bo4);
	printf("BoFree[0x%x]\n", iaic_get_bo_free(&ctx));

	//->wrong,but successful
	iaic_create_bo(&ctx, 0x600, &bo5);
	printf("BoFree[0x%x]\n", iaic_get_bo_free(&ctx));


	// Release application
	printf("----------\n");
	iaic_destroy_bo(&ctx,&bo4);
	printf("----------\n");
	iaic_destroy_bo(&ctx,&bo5);
	printf("----------\n");
	iaic_destroy_bo(&ctx,&bo3);

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

