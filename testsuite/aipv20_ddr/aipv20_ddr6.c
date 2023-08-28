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

#if 0
	iaic_bo_t bo1;
	ret = iaic_create_bo(&iaic_ctx, 0x400, &bo1);
	memset(bo1.vaddr, 0x11, 0x400);
	printf("1:paddr=%p,vaddr=%p,size=0x%x,value=0x%x\n",
			bo1.paddr, bo1.vaddr,
			bo1.size, *(int *)bo1.vaddr);

	printf("=========\n");
	iaic_destroy_bo(&iaic_ctx, &bo1);
	printf("=========\n");
#endif
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
