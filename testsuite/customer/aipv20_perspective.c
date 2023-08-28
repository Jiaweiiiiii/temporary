/*
 * =====================================================================================
 *       Filename:  aipv20_convert.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  08/04/2023 02:54:48 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME ().
 *   Organization:
 * =====================================================================================
 */
#include <stdio.h>
#include "iaic.h"

int main(int argc, char *argv[])
{
	iaic_ctx_t iaic_ctx;
	iaic_bo_t img_nv12;
	iaic_bo_t img_bgra;


	float matrix[9] = {
		0.5, 0, 0,
		0, 0.8888, 0,
		0, 0, 1.0
	};

	/*
	 * 1024*576 NV12 --->size: 0xD8000
	 * 512*512  BGRA --->size: 0x100000
	*/
	if (iaic_ctx_init(&iaic_ctx, 0x200000)) {
		printf("iaic_ctx_init failed\n");
		return -1;
	}

	// Obtain image information
	img_nv12 = iaic_img_read(&iaic_ctx,
			"./model/day_w1024_h576.nv12",
			1024, 576, IAIC_NV12);
	// Reduce image size
	img_bgra = iaic_perspective(&iaic_ctx, img_nv12,
			1024, 576, 512, 512, IAIC_NV12, matrix);
	// Output converted files
	iaic_img_show(img_bgra, "./dst_img");


	iaic_destroy_bo(&iaic_ctx, &img_bgra);
	iaic_destroy_bo(&iaic_ctx, &img_nv12);
	iaic_ctx_destroy(&iaic_ctx);
	return 0;
}

