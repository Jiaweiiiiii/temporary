
/*
 * =======================================================================
 *       Filename:  aipv20_resize.c
 *
 *    Description: This test case is used to verify whether the results
 *                 of the AIP_F and the opencv on processing FMU2 images
 *                 are consistent.
 *
 *        Created:  06/19/2023 04:47:57 PM
 *       Compiler:  mips-linux-gnu-gcc & -static & -muclibc
 *
 *         Author: jwzhang 
 * =======================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include "aipv20_resize_lib.h"

int aipv20_resize()
{
	int ret;
	iaic_ctx_t ctx;
	img_info_t src_img;
	img_info_t dst_img;
	int src_w, src_h, dst_w, dst_h;

	ret = iaic_ctx_init(&ctx, 0x400000);
	if (ret != 0) {
		printf("iaic_ctx_init failed, ret = %d\n", ret);
		return -ret;
	}

	src_w = 1024;
	src_h = 576;
	dst_w = 512;
	dst_h = 512;
	
	src_img = img_read(&ctx, "./model/day_w1024_h576.nv12",
				src_w, src_h, AIP_F_FORMAT_NV12);
		
	dst_img = img_resize(&ctx, src_img, dst_w, dst_h,
				AIP_F_FORMAT_NV12);

	ret = img_show(dst_img, "./dst_image");

	ret = iaic_destroy_bo(&ctx, &dst_img.bo);
	ret = iaic_destroy_bo(&ctx, &src_img.bo);
	ret = iaic_ctx_destroy(&ctx);

	return ret;

}


int main(int argc, char *argv[])
{
	int ret;

	ret = aipv20_resize();
	if (ret != 0)
		printf("aipv20_resize failed!\n");

	return 0;
}
