#include <stdio.h>
#include "iaic.h"

int aipv20_ddr()
{

	int ret;
	iaic_ctx_t iaic_ctx;

	ret = iaic_ctx_init(&iaic_ctx);

	iaic_bo_t bo;

	ret = iaic_create_bo(&iaic_ctx, 1024*1024, &bo);
	memset(bo.vaddr, 0xaa, 0x800);

	printf("paddr=%p,vaddr=%p,size=0x%x,evaddr=%p,esize=0x%x,value=0x%x\n",
			bo.paddr, bo.vaddr, bo.size,
			bo.evaddr, bo.esize, *(int *)bo.vaddr);


	iaic_destroy_bo(&iaic_ctx, &bo);
	printf("release bo\n");


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
