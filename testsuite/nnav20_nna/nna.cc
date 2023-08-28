#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <memory.h>/*strerror*/
#include "common/nna_app.h"
#include "common/nna_regs.h"
#include "iaic.h"
#include "time.h"

#define TLY_VWR VWR1
#define TLX_VWR VWR2
#define WWR_VWR VWR3
#define FWR_VWR VWR4
#define MAC_VWR00 VWR5
#define MAC_VWR01 VWR6
#define MAC_VWR02 VWR7
#define MAC_VWR03 VWR8

#define TEMP_VR0 VR0
#define TEMP_VR1 VR1
#define TEMP_VR2 VR2
#define TEMP_VR3 VR3
#define TEMP_VR4 VR4
#define TEMP_VR5 VR5
#define TEMP_VR6 VR6
#define TEMP_VR7 VR7
#define RESULT_VR0 VR8

int main()
{

	iaic_ret_code_t ret;
	iaic_ctx_t iaic_ctx;
	iaic_oram_t nna_oram;
	iaic_bo_t nna_bo;

	ret = iaic_ctx_init(&iaic_ctx);
	if (ret != 0) {
		printf("init failed!, ret = %d\n", ret);
		return 0;
	}

	ret = iaic_nna_oram_alloc(&iaic_ctx, 0x1000, &nna_oram);
	if (ret != 0) {
		printf("iaic_nna_oram_alloc faild!, ret = %d\n", ret);
		iaic_ctx_destroy(&iaic_ctx);
		return -ret;
	}

	ret = iaic_create_bo(&iaic_ctx, 256, &nna_bo);
	if (ret != 0) {
		printf("iaic_create_bo failed!, ret = %d\n", ret);
		iaic_ctx_destroy(&iaic_ctx);
		return -ret;
	}

	uint8_t *oram_ptr = (uint8_t *)nna_oram.vaddr;
	uint8_t *ddr_ptr = (uint8_t *)nna_bo.vaddr;

	printf("oram_pbase:%p  ddr_pbase:%p\n",nna_oram.paddr ,nna_bo.paddr);
	printf("oram_vbase:%p  ddr_vbase:%p\n",nna_oram.vaddr ,nna_bo.vaddr);

	/**printf ddr value**/
	for(int i = 0;i < 256;i ++){
		ddr_ptr[i] = i % 256;
	}
	LA(o, TEMP_VR0, 0, ddr_ptr, 0);
	LA(o, TEMP_VR0, 1, ddr_ptr, 32);
	PRINT8_VR(TEMP_VR0,64);
	iaic_destroy_bo(&iaic_ctx, &nna_bo);

	iaic_nna_mutex_lock(&iaic_ctx);

	struct timespec start, end;
	long mtime, seconds, nseconds;
	clock_gettime(CLOCK_MONOTONIC, &start);

	const int IH = 64;
	const int IW = 64;
	const int ifp_num = 1;
	const int input_bitwidth = 2;
	const int weight_bitwidth = 2;
	const int kernel_y = 3;
	const int kernel_x = 3;
	const int stride_y = 1;
	const int stride_x = 1;
	const int pad_top = 0;
	const int pad_left = 0;
	const int ifpg_h = 4;
	const int ifpg_w = 12;
	const uint8_t fr_fp_stride = ifpg_h * ifpg_w;
	uint32_t quant_bit = 0;
	int split_w = weight_bitwidth / 2;
	int split_if = input_bitwidth / 2;
	uint32_t macBit = ((split_w - 1) << 4) | (split_if - 1);
	uint32_t pad_val = 0;

	/* configure NNA regs */
	NNRST;
	LA(w, VR31, VWR0, &pad_val, 0);
	NNRWRM(VWR0, NNARA_PAD_VALUE);
	WV_MAC_STRIDE(stride_y, stride_x, VWR0);
	LA(w, VR31, VWR0, &quant_bit, 0);
	NNRWRD(VWR0, NNARA_Q_BIT);
	LIWR(VWR0, 0); //f: 2bit
	NNRWRD(VWR0, NNARA_F_BIT);
	LA(w, VR31, VWR0, &macBit, 0);
	NNRWRM(VWR0, NNARA_MAC_BIT);
	WV_FR_FP_W(ifpg_w, VWR0);
	WV_FR_FP_STRIDE(fr_fp_stride, VWR0);
	LIWR(VWR0, 2);
	NNRWRM(VWR0, NNARA_SF_MODE);
	NNRWRD(VWR0, NNARA_MSF_MODE);
	WV_FP_W(IW - 1, VWR0);
	WV_FP_H(IH - 1, VWR0);
	uint32_t data = 16 - 1;
	LA(w, VR31, VWR0, &data, 0);
	NNRWM(VWR0, NNARA_FP_NUM0);
	int32_t mactl_fp_y = -pad_top;
	int32_t mactl_fp_x = -pad_left;
	LA(w, VR31, TLX_VWR, &mactl_fp_x, 0);
	LA(w, VR31, TLY_VWR, &mactl_fp_y, 0);
	NNRWRM(TLY_VWR, NNARA_MACTL_FP_Y);
	NNRWRM(TLX_VWR, NNARA_MACTL_FP_X);
	LIWR(WWR_VWR, 0);
	LIWR(FWR_VWR, 0);
	NNRWRM(WWR_VWR, NNARA_WR_RADDR_RST);
	NNRWRM(WWR_VWR, NNARA_WR_RADDR);
	uint32_t mac_w_mask = 511;//k33
	LA(w, VR31, VWR0, &mac_w_mask, 0);
	NNRWRM(VWR0, NNARA_MAC_W_MASK);
	LIWR(VWR0, 2 | (1 << 4));
	NNRWM(VWR0, NNARA_MAC_FP);

	/*of_fmt: 0 full-bit, 1 2bit*/
	int of_fmt = 0;
	int of_invmsb = 0;
	uint32_t cfgv2 = (of_fmt | 1 << 1 | of_invmsb << 2 | 0 << 3);
	LA(w, VR31, VWR0, &cfgv2, 0);
	NNRWD(VWR0, NNARA_QD_CONFIG);
	int oc_lsh = 2; /*shift_count*/
	oc_lsh = oc_lsh == 2 ? 0 : oc_lsh == 6 ? 1 : oc_lsh == 11 ? 2 : 100;
	LA(w, VR31, VWR0, &oc_lsh, 0);
	NNRWD(VWR0, NNARA_OC_LSH);
	LIWR(VWR0, 0); /*2^(wfcw_pn+2)=4pixel*/
	NNRWD(VWR0, NNARA_WFCW_PN);

	/*SET WRAM WRITE CTORL WORD*/
	int ctrl, inc;
	uint32_t wfcw;
	int if_invmsb = 0;
	int word_cnt0 = 2, word_cnt1 = 0;
	LIWR(VWR0, 0);
	WVWR_WFCW_WPTR(VWR0); // set to WFCW[0] (wptr = 0)
	// ctrl_word0
	ctrl = 3; /*loop-num*/
	inc = ifpg_w / 4;
	wfcw = (((ctrl) << (FR_WABIT + 1)) | ((if_invmsb) << FR_WABIT) | ((inc)&FR_WA_MASK));
	LA(w, VR31, VWR0, &wfcw, 0);
	WVWR_WFCW_INP(VWR0);

	// ctrl_word1
	ctrl = 1;
	inc = ifpg_w / 4 + ifpg_h * ifpg_w / 4 * (split_if - 1);
	wfcw = (((ctrl) << (FR_WABIT + 1)) | ((if_invmsb) << FR_WABIT) | ((inc)&FR_WA_MASK));
	LA(w, VR31, VWR0, &wfcw, 0);
	WVWR_WFCW_INP(VWR0);

	/*END*/
	int word_cnt = ((word_cnt1 & 0x7) << 4) | (word_cnt0 & 0x7);
	LA(w, VR31, VWR0, &word_cnt, 0);
	WVWR_WFCW_END(VWR0);
	NNINITCW;

	LIW(TEMP_VR0, 0x87654321);
	LIW(TEMP_VR1, 0x12345678);
	LIW(TEMP_VR2, 0x43218765);
	LIW(TEMP_VR3, 0x56781234);

	NNRWD(WWR_VWR, NNARA_BTR_WADDR);
	LIW(TEMP_VR4, 656325);
	LIW(TEMP_VR5, 532485);
	LIW(TEMP_VR6, 3356);
	LIW(TEMP_VR7, 4254);
	NNDWBT(TEMP_VR4);
	NNDWBT(TEMP_VR5);
	NNDWBT(TEMP_VR6);
	NNDWBT(TEMP_VR7);

	LIWR(MAC_VWR00, 0);
	int mac_fp_stride = ifpg_h * ifpg_w / 4 * split_if * 16;
	LA(w, VR31, MAC_VWR01, &mac_fp_stride, 0);

	int w_entry = ifp_num * kernel_y * kernel_x * split_w;

	NNRWRD(FWR_VWR, NNARA_FR_WADDR0);
	for (int i = 0; i < ifp_num; i++) {
		NNDWF0(TEMP_VR0);
		NNDWF0(TEMP_VR1);
	}

	NNRWD(WWR_VWR, NNARA_WR_WADDR);
	for (int i_entry = 0; i_entry < w_entry; i_entry++) {
		NNDWW(TEMP_VR0, OPT_NNDWRW_NONINCA, OPT_NNDWRW_PN0);
		NNDWW(TEMP_VR1, OPT_NNDWRW_NONINCA, OPT_NNDWRW_PN1);
		NNDWW(TEMP_VR2, OPT_NNDWRW_NONINCA, OPT_NNDWRW_PN2);
		NNDWW(TEMP_VR3, OPT_NNDWRW_INCA, OPT_NNDWRW_PN3);
	}

	NNMACG(MAC_VWR00, OPT_NNMAC_WRAINC, 1, OPT_NNMAC_FP_NUM0);
	NNMACG(MAC_VWR01, OPT_NNMAC_WRARST, 1, OPT_NNMAC_FP_NUM0);


	NNRWD(WWR_VWR, NNARA_BTR_RADDR);
	NNRDBT;
	NNCCQ;
	NNDRQR(RESULT_VR0);
	SA(o, RESULT_VR0, 0, oram_ptr, 0);
	SA(o, RESULT_VR0, 1, oram_ptr, 32);


	clock_gettime(CLOCK_MONOTONIC, &end);
	seconds = end.tv_sec - start.tv_sec;
	nseconds = end.tv_nsec - start.tv_nsec;
	mtime = seconds * 1000000 + nseconds / 1000;

	iaic_nna_mutex_unlock(&iaic_ctx);
	printf("Time taken: %ld us\n", mtime);
	PRINT8_VR(RESULT_VR0,64);

	iaic_nna_oram_free(&iaic_ctx, &nna_oram);
	iaic_ctx_destroy(&iaic_ctx);


	return 0;
}
