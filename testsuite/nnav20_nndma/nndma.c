#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include "common.h"

int main(void)
{
	int ret;
	iaic_ctx_t iaic_ctx;
	iaic_bo_t  bo;
	iaic_oram_t oram;


	/* DDR init and malloc */
	ret = iaic_ctx_init(&iaic_ctx, 0x10000);
	if (ret) {
		printf("iaic failed!, ret = %d\n", ret);
		return 0;
	}
	ret = iaic_create_bo(&iaic_ctx, 0x1000, &bo);
	if (ret) {
		printf("iaic failed!, ret = %d\n", ret);
		ret = iaic_ctx_destroy(&iaic_ctx);
		return 0;
	}
	memset(bo.vaddr, 0xa, 0x1000);

	/* ORAM init and malloc */
	ret = iaic_nna_oram_alloc(&iaic_ctx, 0x1000, &oram);
	if (ret) {
		printf("iaic failed!, ret = %d\n", ret);
		ret = iaic_destroy_bo(&iaic_ctx, &bo);
		ret = iaic_ctx_destroy(&iaic_ctx);
		return 0;
	}

	/* Desram information  */
	volatile uint32_t *nndma_vbase = (volatile uint32_t *)iaic_ctx.nna_status.dma_vbase;
	volatile uint32_t *dsram_vbase = (volatile uint32_t *)iaic_ctx.nna_status.dma_desram_vbase;

	/* IF W OF parameter */
	int ic = 32, iw = 8, ih = 2, ibit = 2;
	int kx = 1, ky = 1, sx = 1, sy = 1, wbit = 2;
	int oc = 32, ow = 8, oh = 2, obit = 32;
	/* IF information */
	uint32_t if_size = ic * iw * ih * ibit >> 3;
	uint8_t *if_ddr_vaddr = bo.vaddr;
	uint8_t *if_ddr_paddr = bo.paddr;
	uint8_t *if_oram_vaddr = oram.vaddr;
	uint32_t if_oram_ofs = oram.offset;
	memset(if_ddr_vaddr, 0xe4, if_size);

	/*	W information */
	uint32_t w_size = ic * oc * kx * ky * wbit >> 3;
	uint8_t *w_ddr_vaddr = bo.vaddr + if_size;
	uint8_t *w_ddr_paddr = bo.paddr + if_size;
	uint8_t *w_oram_vaddr = oram.vaddr + if_size;
	uint32_t w_oram_ofs = oram.offset + if_size;
	memset(w_ddr_vaddr, 0xe4, w_size);

	/* OF information */
	uint32_t of_size = oc * ow * oh * obit >> 3;
	uint8_t *of_ddr_vaddr = bo.vaddr + if_size + w_size;
	uint8_t *of_ddr_paddr = bo.paddr + if_size + w_size;
	uint8_t *of_oram_vaddr = oram.vaddr + if_size + w_size;
	uint32_t of_oram_ofs = oram.offset + if_size + w_size;


	/* Brush cache */
	iaic_cacheflush(&iaic_ctx, bo.vaddr, 0x1000, 0);

	ret = iaic_nna_mutex_lock(&iaic_ctx);
	if (ret != 0) {
		printf("lock failed!\n");
		iaic_nna_oram_free(&iaic_ctx, &oram);
		iaic_destroy_bo(&iaic_ctx, &bo);
		iaic_ctx_destroy(&iaic_ctx);
	}

	/* ---- nndma W ---- */
	dsram_vbase[0] = (uint32_t)(w_ddr_paddr) & ~(0x3f);
	dsram_vbase[1] = (((w_size >> 6)-1) << 20) | w_oram_ofs >> 6;
	*nndma_vbase = 0x1 << 31;
	*nndma_vbase;
	iaic_nndma_rch0_wait;

	for (int i = 0; i < (int)w_size; i++){
		if (w_oram_vaddr[i] != 0xe4) 
			printf("w[%d]=0x%x\n", i, w_oram_vaddr[i]);
	}
	
	/* ---- nndma if ----*/
	dsram_vbase[0] = (uint32_t)(if_ddr_paddr) & ~(0x3f);
	dsram_vbase[1] = (((if_size >> 6)-1) << 20) | if_oram_ofs >> 6;
	*nndma_vbase = 0x1 << 31;
	*nndma_vbase;
	iaic_nndma_rch0_wait;

	for (int i = 0; i < (int)if_size; i++){
		if (if_oram_vaddr[i] != 0xe4) 
			printf("if[%d]=0x%x\n", i, if_oram_vaddr[i]);
	}

	/* NNA regs */
	uint32_t data;
	NNRST;
	LIWR(VWR0, 1); // k11
	NNRWM(VWR0, NNARA_MAC_W_MASK);
	LIWR(VWR1, ((1<<1)|1));
	LIWR(VWR0, ((1<<4)|2)); //mac_fp=2x4
	NNRWM(VWR1, NNARA_MIN_FPN_O);
	NNRWM(VWR0, NNARA_MAC_FP);

	LIWR(VWR1, 0);
	LIWR(VWR0, 8);
	NNRWD(VWR1, NNARA_MSF_MODE);
	NNRWM(VWR1, NNARA_SF_MODE);
	NNRWM(VWR0, NNARA_FR_FP_W);

	LIWR(VWR1, 8*2); // fr_fp_w*fr_fp_h
	NNRWM(VWR1, NNARA_FR_FP_STRIDE);
	//data = ((param.sy-1)<<4) | (param.sx-1);
	data = ((sy-1)<<4) | (sx-1);
	LA(w, VR31, VWR0, &data, 0);
	NNRWM(VWR0, NNARA_MAC_STRIDE); //sx=sy=1

	data = (wbit-2)*8 + (ibit-2)/2;
	LA(w, VR31, VWR1, &data, 0);
	NNRWM(VWR1, NNARA_MAC_BIT);
	data = (obit-2)/2;
	LA(w, VR31, VWR0, &data, 0);
	NNRWD(VWR0, NNARA_Q_BIT);

	data = ih - 1;
	LA(w, VR31, VWR1, &data, 0);
	NNRWM(VWR1, NNARA_FP_H);
	data = iw - 1;
	LA(w, VR31, VWR0, &data, 0);
	NNRWM(VWR0, NNARA_FP_W);

	LIWR(VWR1, 0);
	NNRWD(VWR1, NNARA_OC_LSH);
	NNRWD(VWR1, NNARA_QD_CONFIG);
	NNRWD(VWR1, NNARA_F_BIT);
	NNRWM(VWR1, NNARA_PAD_VALUE);

	data = ibit/2 - 1;
	LA(w, VR31, VWR0, &data, 0);
	NNRWM(VWR0, NNARA_F_BIT_GSIZE);
	//data = sx - 1;
	data = sx - 1;
	LA(w, VR31, VWR0, &data, 0);
	NNRWD(VWR0, NNARA_WFCW_PN);

	uint32_t fram_line = 8/4;
	uint32_t fr_fpm1_stride = fram_line * (2-1);
	uint32_t next_col = sx;
	uint32_t wfcw_data[8];
	wfcw_data[0] = (0xf << 11) | (2 << 8) | 0;
	wfcw_data[1] = (1 << 11) | (0 << 10) | fram_line;
	wfcw_data[2] = (1 << 11) | (0 << 10) | ((next_col-fr_fpm1_stride) & 0x3ff);
	wfcw_data[3] = 8;
	wfcw_data[4] = (3 << 4) | 3;

	LA(o, VR31, 0, wfcw_data, 0);
	WVWR_WFCW_INP(VWR0);
	WVWR_WFCW_INP(VWR1);
	WVWR_WFCW_INP(VWR2);

	WVWR_WFCW_WPTR(VWR3);
	WVWR_WFCW_INP(VWR0);
	WVWR_WFCW_INP(VWR1);
	WVWR_WFCW_INP(VWR2);
	WVWR_WFCW_END(VWR4);
	NNINITCW;

	/* write all W to WRAM */
	LIWR(FWR_VWR0, 0);
	NNRWD(FWR_VWR0, NNARA_WR_WADDR);
	uint8_t *wbuf_ptr = w_oram_vaddr;
	LA(o, VR0, 0, wbuf_ptr,  0);
	LA(o, VR0, 1, wbuf_ptr, 32);
	LA(o, VR1, 0, wbuf_ptr, 64);
	LA(o, VR1, 1, wbuf_ptr, 96);
	NNDWW(VR0, OPT_NNDWRW_NONINCA, OPT_NNDWRW_PN0);
	NNDWW(VR1, OPT_NNDWRW_NONINCA, OPT_NNDWRW_PN1);

	LA(o, VR0, 0, wbuf_ptr, 128);
	LA(o, VR0, 1, wbuf_ptr, 160);
	LA(o, VR1, 0, wbuf_ptr, 192);
	LA(o, VR1, 1, wbuf_ptr, 224);
	NNDWW(VR0, OPT_NNDWRW_NONINCA, OPT_NNDWRW_PN2);
	NNDWW(VR1, OPT_NNDWRW_INCA,    OPT_NNDWRW_PN3); //wr_waddr + 1


	int32_t mactl_fp_x = 0;
	int32_t mactl_fp_y = 0;
	LA(w, VR31, TLX_VWR, &mactl_fp_x, 0);
	LA(w, VR31, TLY_VWR, &mactl_fp_y, 0);

	NNRWD(FWR_VWR0, NNARA_FR_WADDR0);
	NNRWM(FWR_VWR0, NNARA_WR_RADDR_RST);
	NNRWM(FWR_VWR0, NNARA_WR_RADDR);
	NNRWM(TLX_VWR, NNARA_MACTL_FP_X);
	NNRWM(TLY_VWR, NNARA_MACTL_FP_Y);

	uint8_t *obuf_l0 = (uint8_t *)of_oram_vaddr;
	uint8_t *obuf_l1 = obuf_l0 + ow * oc * 32 / 8;

	int32_t mac_vwr0 = 0;
	LA(w, VR31, MAC_VWR0, &mac_vwr0, 0);

	uint32_t ibuf_line_size = iw * ic * ibit / 8;
	uint32_t next_pixel = (ic * ibit / 8) * 4 * sx;
	uint8_t *frame0_l0 = if_oram_vaddr;
	uint8_t *frame0_l1 = frame0_l0 + ibuf_line_size;

	/* write first 4-pixel */
	LA(o, VR0, 0, frame0_l0, 0);
	LA(o, VR0, 1, frame0_l1, 0);
	NNDWF0(VR0);
	frame0_l0 += next_pixel;
	frame0_l1 += next_pixel;

	/* write next 4-pixel */
	LA(o, VR1, 0, frame0_l0, 0);
	LA(o, VR1, 1, frame0_l1, 0);
	NNDWF0(VR1);

	/* OFP0 L0 & L1, first 4-pixel */
	NNMAC0(MAC_VWR0, OPT_NNMAC_WRARST);
	ADDIW(MAC_VWR0, MAC_VWR0, 4);
	NNCCYMXP;

	/* MAC result */
	NNDROCB(VR8,  OPT_NNDRDOCB_PIXEL0, OPT_NNDRDOCB_LO);
	NNDROCB(VR9,  OPT_NNDRDOCB_PIXEL0, OPT_NNDRDOCB_HI);
	NNDROCB(VR10, OPT_NNDRDOCB_PIXEL1, OPT_NNDRDOCB_LO);
	NNDROCB(VR11, OPT_NNDRDOCB_PIXEL1, OPT_NNDRDOCB_HI);
	NNDROCB(VR12, OPT_NNDRDOCB_PIXEL2, OPT_NNDRDOCB_LO);
	NNDROCB(VR13, OPT_NNDRDOCB_PIXEL2, OPT_NNDRDOCB_HI);
	NNDROCB(VR14, OPT_NNDRDOCB_PIXEL3, OPT_NNDRDOCB_LO);
	NNDROCB(VR15, OPT_NNDRDOCB_PIXEL3, OPT_NNDRDOCB_HI);
	NNDROCB(VR16, OPT_NNDRDOCB_PIXEL4, OPT_NNDRDOCB_LO);
	NNDROCB(VR17, OPT_NNDRDOCB_PIXEL4, OPT_NNDRDOCB_HI);
	NNDROCB(VR18, OPT_NNDRDOCB_PIXEL5, OPT_NNDRDOCB_LO);
	NNDROCB(VR19, OPT_NNDRDOCB_PIXEL5, OPT_NNDRDOCB_HI);
	NNDROCB(VR20, OPT_NNDRDOCB_PIXEL6, OPT_NNDRDOCB_LO);
	NNDROCB(VR21, OPT_NNDRDOCB_PIXEL6, OPT_NNDRDOCB_HI);
	NNDROCB(VR22, OPT_NNDRDOCB_PIXEL7, OPT_NNDRDOCB_LO);
	NNDROCB(VR23, OPT_NNDRDOCB_PIXEL7, OPT_NNDRDOCB_HI);

	/* to ORAM */
	SA(o, VR8, 0, obuf_l0, 0);
	SA(o, VR8, 1, obuf_l0, 32);
	SA(o, VR9, 0, obuf_l0, 64);
	SA(o, VR9, 1, obuf_l0, 96);
	obuf_l0 += 128;
	SA(o, VR10, 0, obuf_l0, 0);
	SA(o, VR10, 1, obuf_l0, 32);
	SA(o, VR11, 0, obuf_l0, 64);
	SA(o, VR11, 1, obuf_l0, 96);
	obuf_l0 += 128;
	SA(o, VR12, 0, obuf_l0, 0);
	SA(o, VR12, 1, obuf_l0, 32);
	SA(o, VR13, 0, obuf_l0, 64);
	SA(o, VR13, 1, obuf_l0, 96);
	obuf_l0 += 128;
	SA(o, VR14, 0, obuf_l0, 0);
	SA(o, VR14, 1, obuf_l0, 32);
	SA(o, VR15, 0, obuf_l0, 64);
	SA(o, VR15, 1, obuf_l0, 96);
	obuf_l0 += 128;
	/* L1 */
	SA(o, VR16, 0, obuf_l1, 0);
	SA(o, VR16, 1, obuf_l1, 32);
	SA(o, VR17, 0, obuf_l1, 64);
	SA(o, VR17, 1, obuf_l1, 96);
	obuf_l1 += 128;
	SA(o, VR18, 0, obuf_l1, 0);
	SA(o, VR18, 1, obuf_l1, 32);
	SA(o, VR19, 0, obuf_l1, 64);
	SA(o, VR19, 1, obuf_l1, 96);
	obuf_l1 += 128;
	SA(o, VR20, 0, obuf_l1, 0);
	SA(o, VR20, 1, obuf_l1, 32);
	SA(o, VR21, 0, obuf_l1, 64);
	SA(o, VR21, 1, obuf_l1, 96);
	obuf_l1 += 128;
	SA(o, VR22, 0, obuf_l1, 0);
	SA(o, VR22, 1, obuf_l1, 32);
	SA(o, VR23, 0, obuf_l1, 64);
	SA(o, VR23, 1, obuf_l1, 96);
	obuf_l1 += 128;

	/* OFP0 L0 & L1, next 4-pixel */
	NNMAC0(MAC_VWR0, OPT_NNMAC_WRARST);
	ADDIW(MAC_VWR0, MAC_VWR0, 4);
	NNCCYMXP;

	/* MAC result */
	NNDROCB(VR8,  OPT_NNDRDOCB_PIXEL0, OPT_NNDRDOCB_LO);
	NNDROCB(VR9,  OPT_NNDRDOCB_PIXEL0, OPT_NNDRDOCB_HI);
	NNDROCB(VR10, OPT_NNDRDOCB_PIXEL1, OPT_NNDRDOCB_LO);
	NNDROCB(VR11, OPT_NNDRDOCB_PIXEL1, OPT_NNDRDOCB_HI);
	NNDROCB(VR12, OPT_NNDRDOCB_PIXEL2, OPT_NNDRDOCB_LO);
	NNDROCB(VR13, OPT_NNDRDOCB_PIXEL2, OPT_NNDRDOCB_HI);
	NNDROCB(VR14, OPT_NNDRDOCB_PIXEL3, OPT_NNDRDOCB_LO);
	NNDROCB(VR15, OPT_NNDRDOCB_PIXEL3, OPT_NNDRDOCB_HI);
	NNDROCB(VR16, OPT_NNDRDOCB_PIXEL4, OPT_NNDRDOCB_LO);
	NNDROCB(VR17, OPT_NNDRDOCB_PIXEL4, OPT_NNDRDOCB_HI);
	NNDROCB(VR18, OPT_NNDRDOCB_PIXEL5, OPT_NNDRDOCB_LO);
	NNDROCB(VR19, OPT_NNDRDOCB_PIXEL5, OPT_NNDRDOCB_HI);
	NNDROCB(VR20, OPT_NNDRDOCB_PIXEL6, OPT_NNDRDOCB_LO);
	NNDROCB(VR21, OPT_NNDRDOCB_PIXEL6, OPT_NNDRDOCB_HI);
	NNDROCB(VR22, OPT_NNDRDOCB_PIXEL7, OPT_NNDRDOCB_LO);
	NNDROCB(VR23, OPT_NNDRDOCB_PIXEL7, OPT_NNDRDOCB_HI);

	/* to ORAM */
	SA(o, VR8, 0, obuf_l0, 0);
	SA(o, VR8, 1, obuf_l0, 32);
	SA(o, VR9, 0, obuf_l0, 64);
	SA(o, VR9, 1, obuf_l0, 96);
	obuf_l0 += 128;
	SA(o, VR10, 0, obuf_l0, 0);
	SA(o, VR10, 1, obuf_l0, 32);
	SA(o, VR11, 0, obuf_l0, 64);
	SA(o, VR11, 1, obuf_l0, 96);
	obuf_l0 += 128;
	SA(o, VR12, 0, obuf_l0, 0);
	SA(o, VR12, 1, obuf_l0, 32);
	SA(o, VR13, 0, obuf_l0, 64);
	SA(o, VR13, 1, obuf_l0, 96);
	obuf_l0 += 128;
	SA(o, VR14, 0, obuf_l0, 0);
	SA(o, VR14, 1, obuf_l0, 32);
	SA(o, VR15, 0, obuf_l0, 64);
	SA(o, VR15, 1, obuf_l0, 96);
	obuf_l0 += 128;
	/* L1 */
	SA(o, VR16, 0, obuf_l1, 0);
	SA(o, VR16, 1, obuf_l1, 32);
	SA(o, VR17, 0, obuf_l1, 64);
	SA(o, VR17, 1, obuf_l1, 96);
	obuf_l1 += 128;
	SA(o, VR18, 0, obuf_l1, 0);
	SA(o, VR18, 1, obuf_l1, 32);
	SA(o, VR19, 0, obuf_l1, 64);
	SA(o, VR19, 1, obuf_l1, 96);
	obuf_l1 += 128;
	SA(o, VR20, 0, obuf_l1, 0);
	SA(o, VR20, 1, obuf_l1, 32);
	SA(o, VR21, 0, obuf_l1, 64);
	SA(o, VR21, 1, obuf_l1, 96);
	obuf_l1 += 128;
	SA(o, VR22, 0, obuf_l1, 0);
	SA(o, VR22, 1, obuf_l1, 32);
	SA(o, VR23, 0, obuf_l1, 64);
	SA(o, VR23, 1, obuf_l1, 96);
	obuf_l1 += 128;

	dsram_vbase[0] = (uint32_t)(of_ddr_paddr) & ~(0x3f);
	dsram_vbase[1] = (((of_size >> 6)-1) << 20) | of_oram_ofs >> 6;
	*(volatile uint32_t *)((uint32_t)nndma_vbase + 0x10) = 0x1 << 31;
	*(volatile uint32_t *)((uint32_t)nndma_vbase + 0x10);
	iaic_nndma_wch0_wait;

	//iaic_cacheflush(&iaic_ctx, bo.vaddr, 0x1000, DMA_FROM_DEVICE);
	int result1 = ((uint32_t *)of_ddr_vaddr)[1];
	int result2 = ((uint32_t *)of_ddr_vaddr)[32];
	int result3 = ((uint32_t *)of_ddr_vaddr)[64];
	int result4 = ((uint32_t *)of_ddr_vaddr)[128];

	ret = iaic_nna_mutex_unlock(&iaic_ctx);

	if (result1 == 0x70 && result2 == 0x70 && result3 == 0x70 && result4 == 0x70){
		printf("The result is correct.\n");
	} else {
		printf("ERROR!, result1 = 0x%x, result2 = 0x%x, result3 = 0x%x, result4 = 0x%x\n",
				result1, result2, result3, result4);
	}
	
	/* free */
	iaic_nna_oram_free(&iaic_ctx, &oram);
	iaic_destroy_bo(&iaic_ctx, &bo);
	iaic_ctx_destroy(&iaic_ctx);

	return 0;
}

