/*
 * =======================================================================
 *       Filename:  aipv20_resize_lib.h
 *
 *    Description:
 *
 *        Created:  07/10/2023 03:38 PM
 *       Compiler:  mips-linux-gnu-gcc & -static & -muclibc
 *
 *         Author: jwzhang 
 * =======================================================================
 */


#include "iaic.h"

typedef struct {
	iaic_bo_t bo;
	size_t wid;
	size_t hei;
	size_t stride;
	size_t size;
} img_info_t;


void format_set(aip_v20_ioctl_f_format_t format,
		int *src_bpp, int *dst_bpp, int *chn);
void set_bpp(aip_v20_ioctl_f_format_t format,
		int *bpp, int *chn);
img_info_t img_read(iaic_ctx_t *ctx, char *str,
	size_t wid, size_t hei,
	aip_v20_ioctl_f_format_t format);
img_info_t img_resize(iaic_ctx_t *ctx, img_info_t src_img,
	size_t wid, size_t hei,
	aip_v20_ioctl_f_format_t format);
int img_show(img_info_t dst_img, char *str);
