#include <stdio.h>
#include <unistd.h>
#include "iaic.h"

int aipv20_ddr()
{

	int ret;
	iaic_ctx_t iaic_ctx;
	iaic_oram_t src_oram;

	// step 1: Init
	ret = iaic_ctx_init(&iaic_ctx);

	// step 2: Applying for space from oram
	ret = iaic_nna_oram_alloc(&iaic_ctx, 0x1000, &src_oram);
	printf("oram_size = %d, oram_pbase = %p, oram_vbase = %p\n", src_oram.size, src_oram.paddr, src_oram.vaddr);

	// step 3: Use
	iaic_nna_mutex_lock(&iaic_ctx);
	*(int *)src_oram.vaddr = 0x1111;
	printf("oram_value = 0x%x\n", *(int *)src_oram.vaddr);
	sleep(5);
	iaic_nna_mutex_unlock(&iaic_ctx);

	// step 4: Destroy space
	ret = iaic_nna_oram_free(&iaic_ctx, &src_oram);
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
