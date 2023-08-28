#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "iaic.h"


int aipv20_convert()
{
	int ret, i, j, task_h;
	FILE *img_fp;
	iaic_ctx_t iaic_ctx;
	iaic_bo_t src_bo;
	iaic_bo_t dst_bo;
	iaic_t_node_t *tnode;
	uint64_t job_seqno, job_timeout_ns;
	uint64_t task_y_paddr, task_c_paddr, task_dst_paddr;

	uint32_t src_w, src_h, src_bpp, dst_bpp, src_stride, dst_stride;
	uint32_t box_w, box_h, box_x, box_y, box_num;
	size_t src_size, dst_size, dst_total_size;
	uint32_t src_pbase_y, src_pbase_uv, offset_y, offset_uv;
	uint32_t nv2bgr_coef[9] = {
		1220542, 2116026, 0,
		1220542, 409993, 852492,
		1220542, 0, 1673527
	};

	src_w = 1024;
	src_h = 576;

	box_w = src_w;
	box_h = src_h;
	box_x = 0;
	box_y = 0;
	box_num = 1;

	src_bpp = 12;
	dst_bpp = 32;
	src_stride = src_w;
	dst_stride = box_w * dst_bpp >> 3;
	src_size = src_stride * src_h * src_bpp >> 3;
	dst_size = dst_stride * box_h;

	ret = iaic_ctx_init(&iaic_ctx, 0x600000);
	if (ret) {
		printf("iaic_ctx_init failed, ret = %d\n", ret);
		return -1;
	}
	ret = iaic_create_bo(&iaic_ctx, src_size, &src_bo);
	if (ret) {
		perror("src_bo failed");
		return -1;
	}
	dst_total_size = dst_size * box_num;
	ret = iaic_create_bo(&iaic_ctx, dst_total_size, &dst_bo);
	if (ret) {
		perror("dst_bo failed");
		return -1;
	}
	img_fp = fopen("./model/day_w1024_h576.nv12","r");
	if(img_fp == NULL) {
		perror("image fopen failed");
		return -1;
	}
	fread(src_bo.vaddr, 32, src_size/32, img_fp);
	if(fclose(img_fp) < 0){
		perror("image fclose failed");
		return -1;
	}

	src_pbase_y = (uint32_t)src_bo.paddr;
	src_pbase_uv = src_pbase_y + src_w * src_h;
	offset_y = src_stride * box_y + box_x;
	offset_uv = src_stride * box_y / 2 + box_x;

			struct timeval start_time, end_time;
			double total_time;
			gettimeofday(&start_time, NULL);

	job_timeout_ns = -1ll;
	task_h = box_h;
	//task_h = 2;
	tnode = (iaic_t_node_t *)malloc(sizeof(iaic_t_node_t) * box_num);
	for (i = 0; i < box_num; i++) {
		for ( j = 0; j < box_h; j += task_h) {
			if (task_h > (box_h - j) - task_h)
				task_h = box_h - j;
			task_y_paddr = src_pbase_y + (src_stride * j);
			task_c_paddr = src_pbase_uv + (src_stride/2 * j);
			task_dst_paddr = (uint32_t)dst_bo.paddr + (dst_size * i) + (dst_stride * j);
 			tnode[i].src_ybase 		= task_y_paddr + offset_y;
			tnode[i].src_cbase 		= task_c_paddr + offset_uv;
			tnode[i].dst_base  		= task_dst_paddr;
			tnode[i].src_h     		= box_h;
			tnode[i].src_w     		= box_w;
			tnode[i].src_stride     = src_stride;
			tnode[i].dst_stride     = dst_stride;
			tnode[i].task_len       = task_h;

			ret = iaic_aip_t(&iaic_ctx,
					AIP_DAT_POS_DDR,
					AIP_DAT_POS_DDR,
					AIP_ORDER_BGRA,
					255, 16, 128,
					nv2bgr_coef,
					(void *)(&tnode[i]),
					NULL, &job_seqno);


			if (ret) {
				printf("Failed to submit IO request, ret = %d\n", ret);
				return -1;
			}
			iaic_convert_reg_read(&iaic_ctx);
			ret = iaic_aip_t_wait(&iaic_ctx, job_seqno, job_timeout_ns, NULL);
			if (ret) printf("aip_t wait faild\n");
		}
	}

			gettimeofday(&end_time, NULL);
			long time = start_time.tv_sec * 1000000 + start_time.tv_usec;
			time = end_time.tv_sec * 1000000 + end_time.tv_usec - time;
			total_time = time *1.0 / 1000;
			printf("Running time of iaic_aip_t is %.03lf ms\n", total_time);

#if 1
	void *dst_vbase = NULL;
	dst_vbase = dst_bo.vaddr;
	FILE *dst_fp;
	char name [32];
	for (i = 0; i < box_num; i++){
		sprintf(name, "%s%d", "./dst_image", i);
		dst_fp = fopen(name, "w+");
		if(dst_fp == NULL) {
			perror("dst_image fopen");
			break;
		}
		fwrite(dst_vbase + (dst_size * i), 32, dst_size/32, dst_fp);
		printf("Successfully obtained image[%d] information\n", i);
		if(fclose(dst_fp) != 0){
			perror("dst_image fclose");
			break;
		}
	}
#endif

	//free(tsubmit);
	free(tnode);
	ret = iaic_destroy_bo(&iaic_ctx, &dst_bo);
	ret = iaic_destroy_bo(&iaic_ctx, &src_bo);
	ret = iaic_ctx_destroy(&iaic_ctx);

	return 0;
}

int main(int argc, char *argv[])
{
	int ret;
	ret = aipv20_convert();
	if (ret < 0)
		printf("aipv20_convert failed!\n");
	return 0;
}

