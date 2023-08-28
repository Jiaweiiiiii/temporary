#include <stdio.h>
#include "iaic.h"

int aipv20_ddr()
{

	int ret, i;
	iaic_ctx_t iaic_ctx;

	ret = iaic_ctx_init(&iaic_ctx);


	int num = 20;
	int cut_num = 5;
	iaic_bo_t bo[num];

	for (i = 0; i < num>>1; i++) {
		ret = iaic_create_bo(&iaic_ctx, 0x800, &bo[i]);
		if (ret != 0) {
			printf("create_bo error!\n");
			return ret;
		}
		memset(bo[i].vaddr, 0xaa, 0x800);
	}



	for (i = 0; i < num>>1; i++) {
		printf("[i:%d]paddr=%p,vaddr=%p,size=0x%x,evaddr=%p,esize=0x%x,value=0x%x\n",i,
				bo[i].paddr, bo[i].vaddr, bo[i].size,
				bo[i].evaddr, bo[i].esize, *(int *)bo[i].vaddr);
	}


	for (i = 0; i < cut_num; i++) {
		iaic_destroy_bo(&iaic_ctx, &bo[i]);
		printf("release bo[%d]\n", i);
	}

	for (i = num>>1; i < num; i++) {
		ret = iaic_create_bo(&iaic_ctx, 0x1500, &bo[i]);
		if (ret != 0) {
			printf("create_bo error!\n");
			return ret;
		}
		memset(bo[i].vaddr, 0xbb, 0x1800);
	}

	for (i = num>>1; i < num; i++) {
		printf("[i:%d]paddr=%p,vaddr=%p,size=0x%x,evaddr=%p,esize=0x%x, value0x%x\n",i,
				bo[i].paddr, bo[i].vaddr, bo[i].size,
				bo[i].evaddr, bo[i].esize, *(int *)bo[i].vaddr);
	}



	for (i = cut_num; i < num; i++) {
		iaic_destroy_bo(&iaic_ctx, &bo[i]);
		printf("release bo[%d]\n", i);
	}


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
