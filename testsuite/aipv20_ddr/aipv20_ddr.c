#include <stdio.h>
#include "iaic.h"

int aipv20_ddr()
{

	int ret;
	iaic_ctx_t iaic_ctx;
	iaic_bo_t src_bo;
	char * dst;

	// step 1: Init
	ret = iaic_ctx_init(&iaic_ctx);

	// step 2: Applying for space from ddr
	ret = iaic_create_bo(&iaic_ctx, 1024 * 1024, &src_bo);
	printf("ddr_size = %d, ddr_pbase = %p, ddr_vbase = %p %p\n",
			src_bo.size, src_bo.paddr, src_bo.vaddr, src_bo.evaddr);

	memset(src_bo.vaddr, 0xa, src_bo.size);

	dst = (char *)src_bo.vaddr;

	for (int i = 0; i < src_bo.size; i++) {
		if (dst[i] != 0xa) {
			printf("check err %d %d\n", i, dst[i]);
			break;
		}
	}
	printf("ext read:%d\n", *(dst - PAGE_SIZE));
	printf("ext read:%d\n", *(dst + (1024 * 1024) + PAGE_SIZE));

	// step 3: Destroy space
	ret = iaic_destroy_bo(&iaic_ctx, &src_bo);
	ret = iaic_ctx_destroy(&iaic_ctx);


	return ret;
}

int main()
{
	int ret;
	ret = aipv20_ddr();
	if (ret < 0)
		printf("aipv20_ddr failed!\n");
	return 0;
}
