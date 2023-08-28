#include <stdio.h>
#include "iaic.h"

int aipv20_ddr()
{

	int ret;
	iaic_ctx_t iaic_ctx;

	ret = iaic_ctx_init(&iaic_ctx, 0x1000);
	printf("size[0x%x], handle[0x%x]\n",
			iaic_ctx.shared_bo.size,
			iaic_ctx.shared_bo.handle);

	iaic_bo_t bo1;
	ret = iaic_create_bo(&iaic_ctx, 0x213, &bo1);
	memset(bo1.vaddr, 0x11, 0x400);
	printf("1:paddr=%p,vaddr=%p,size=0x%x,value=0x%x\n",
			bo1.paddr, bo1.vaddr,
			bo1.size, *(int *)bo1.vaddr);

	iaic_bo_t bo2;
	ret = iaic_create_bo(&iaic_ctx, 0x123, &bo2);
	memset(bo2.vaddr, 0x22, 0x100);
	printf("2:paddr=%p,vaddr=%p,size=0x%x,value=0x%x\n",
			bo2.paddr, bo2.vaddr,
			bo2.size, *(int *)bo2.vaddr);

	iaic_bo_t bo3;
	ret = iaic_create_bo(&iaic_ctx, 0x412, &bo3);
	memset(bo3.vaddr, 0x33, 0x400);
	printf("3:paddr=%p,vaddr=%p,size=0x%x,value=0x%x\n",
			bo3.paddr, bo3.vaddr,
			bo3.size, *(int *)bo3.vaddr);

#if 1
	printf("\n");
	printf("=========\n");
	iaic_destroy_bo(&iaic_ctx, &bo2);
	printf("=========\n");
	iaic_destroy_bo(&iaic_ctx, &bo1);
	printf("=========\n");
	iaic_destroy_bo(&iaic_ctx, &bo3);
	printf("=========\n");

	ret = iaic_ctx_destroy(&iaic_ctx);
#endif

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
