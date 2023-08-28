/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : nna_dma_driver_v3.c
 * Authors    : xqwu@aram.ic.jz.com
 * Create Time: 2019-07-08:11:29:54
 * Description:
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "nna_dma_driver_v4.h"

#ifdef NNDMA_DRIVER_DEBUG
  FILE *nnadma_fp;
#endif
static int nna_addr_align_check(unsigned int d_va_addr, unsigned int o_va_addr);
static int nna_des_analysis(uint32_t cmd_num, uint32_t cmd_st_idx, nndma_cmd_buf * cmd_buf, des_info * des_buf);
static void nna_desram_update(des_info * des_rd_buf, des_info * des_wr_buf, uint32_t * dma_cmd_buf, des_gen_result * des_gen_result);

int nna_dma_driver(uint32_t cmd_info[4], nndma_cmd_buf * cmd_buf, uint32_t * dma_cmd_buf, des_gen_result * des_gen_result) {

    des_info des_rd_buf;
    des_info des_wr_buf;
    int ret = 0;

    des_rd_buf.chain_num = 0;
    des_wr_buf.chain_num = 0;

#ifdef NNDMA_DRIVER_DEBUG
    nnadma_fp = fopen("./describe_result.dump","aw");
    fprintf(nnadma_fp,"DMA driver: START\n");
    fprintf(nnadma_fp,"\n");
    fprintf(nnadma_fp,"RD CH START\n");
    fprintf(nnadma_fp,"RD CH start rdch cmd_num_0=%d, wrch cmd_num_1=%d\n",cmd_info[0],cmd_info[1]);
#endif
    //DMA RD CH describe generate
    ret = nna_des_analysis(cmd_info[0], cmd_info[2], cmd_buf, &des_rd_buf);

#ifdef NNDMA_DRIVER_DEBUG
    fprintf(nnadma_fp,"WR CH START\n");
    fprintf(nnadma_fp,"WR CH start rdch cmd_num_1=%d, wrch cmd_num_1=%d\n",cmd_info[0],cmd_info[1]);
#endif

    //DMA WR CH describe generate
    ret = nna_des_analysis(cmd_info[1], cmd_info[3], (cmd_buf+cmd_info[0]), &des_wr_buf);

    //Wrt describe to desram
    nna_desram_update(&des_rd_buf, &des_wr_buf, dma_cmd_buf, des_gen_result);

#ifdef NNDMA_DRIVER_DEBUG
    fprintf(nnadma_fp,"\n");
    fprintf(nnadma_fp,"DMA driver: END\n");
    fclose(nnadma_fp);
#endif

    return ret;
}

int nna_des_analysis(uint32_t cmd_num, uint32_t cmd_st_idx, nndma_cmd_buf * cmd_buf, des_info * des_buf)
{
    int ret = 0;
    uint32_t des_link      = 0;
    uint32_t d_va_st_addr  = 0; //ddr  start va addr in every describe
    uint32_t d_pa_st_addr  = 0; //ddr start pa addr in every describe
    uint32_t o_pa_st_addr  = 0; //oram start va addr in every describe
    uint32_t o_mlc_st_addr = 0;
    uint32_t o_mlc_ed_addr = 0;
    uint32_t o_mlc_bytes   = 0;
    uint32_t trans_bytes   = 0;
    uint32_t data_block_bytes = 0;
    uint32_t bytes_remain  = 0;
    uint32_t c64k_num      = 0;
    uint32_t c64k_idx      = 0;

    uint32_t cur_trans_bytes = 0;
    uint32_t o_cur_ed_addr  = 0;
    uint32_t o_next_st_addr = 0;
    uint32_t d_pa_addr    = 0;
    uint32_t o_pa_addr    = 0;
    uint32_t data_length  = 0;
    uint32_t last_des     = 0;
    uint32_t cfg_flag     = 0;

    uint32_t des_num      = 0;
    uint32_t wptr         = 0;
    uint32_t chain_num    = 0;
    uint32_t total_bytes  = 0;
    uint32_t cmd_idx      = 0;
    uint32_t chain_st_idx = 0;

#ifdef NNDMA_DRIVER_DEBUG
    fprintf(nnadma_fp,"    ======================\n");
    fprintf(nnadma_fp,"    dma_des_analysis start\n");
    fprintf(nnadma_fp,"    ======================\n");
#endif

    chain_st_idx = cmd_st_idx;

    for(cmd_idx = cmd_st_idx; cmd_idx < cmd_num; cmd_idx++) {
    //read the cmd buf
    o_mlc_bytes   = cmd_buf[cmd_idx].o_mlc_bytes;
    data_block_bytes = cmd_buf[cmd_idx].data_bytes;
    des_link      = cmd_buf[cmd_idx].des_link;
    d_pa_st_addr  = (unsigned int)nndma_ddr_vir_to_phy((void *)cmd_buf[cmd_idx].d_va_st_addr);
    o_pa_st_addr  = (unsigned int)nndma_oram_vir_to_phy((void *)cmd_buf[cmd_idx].o_va_st_addr);
    o_mlc_st_addr = (unsigned int)nndma_oram_vir_to_phy((void *)cmd_buf[cmd_idx].o_va_mlc_addr);
    o_mlc_ed_addr = o_mlc_st_addr + o_mlc_bytes;
    c64k_num      = ((data_block_bytes - 1) >> 16) + 1;
    bytes_remain  = data_block_bytes;

    //64Bytes align check
#ifdef NNDMA_DRIVER_DEBUG
    ret = nna_addr_align_check(cmd_buf[cmd_idx].d_va_st_addr, cmd_buf[cmd_idx].o_va_st_addr);

    if(ret!=0)
        goto nna_des_analysis_end;
#endif

#ifdef NNDMA_DRIVER_DEBUG
    fprintf(nnadma_fp,"      ======================\n");
    fprintf(nnadma_fp,"      dma_des_analysis start\n");
    fprintf(nnadma_fp,"      ======================\n");
    fprintf(nnadma_fp,"      DATA block input cmd info\n");
    fprintf(nnadma_fp,"      \n");
    fprintf(nnadma_fp,"      info-> chain_st_idx =%d, cmd_idx=%d,cmd_num=%d\n",chain_st_idx, cmd_idx, cmd_num);
    fprintf(nnadma_fp,"      info-> DDR  phy_addr = 0x%x, vir_addr = 0x%x\n",d_pa_st_addr, cmd_buf[cmd_idx].d_va_st_addr);
    fprintf(nnadma_fp,"      info-> ORAM st phy_addr = 0x%x\n",o_pa_st_addr);
    fprintf(nnadma_fp,"      info-> ORAM mlc st phy_addr = 0x%x\n",o_mlc_st_addr);
    fprintf(nnadma_fp,"      info-> ORAM mlc ed phy_addr = 0x%x\n",o_mlc_ed_addr);
    fprintf(nnadma_fp,"      info-> current trans total bytes = %d\n",data_block_bytes);
    fprintf(nnadma_fp,"      info-> des_link = %d\n",des_link);
    fprintf(nnadma_fp,"      info-> c64k num = %d\n",c64k_num);
    fprintf(nnadma_fp,"      info-> bytes_remain = 0x%x\n",bytes_remain);
    fprintf(nnadma_fp,"      ======================\n");
#endif
    //==============================
    //describe loop gen in one chain
    //==============================
    for(c64k_idx = 0; c64k_idx < c64k_num; c64k_idx++) {
        last_des = (c64k_idx == (c64k_num - 1)) ? 1 : 0;

        if(last_des)
        trans_bytes = bytes_remain;
        else
        trans_bytes = 65536;

        //===============
        //oram loop check
        //===============
        o_cur_ed_addr = o_pa_st_addr + trans_bytes;

        //ddr phy addr
#if 0
        d_pa_addr = d_pa_st_addr >> 6;
#else
        d_pa_addr = d_pa_st_addr;
#endif
        //the oram phy addr
        o_pa_addr = o_pa_st_addr >> 6;

         if(o_cur_ed_addr <= o_mlc_ed_addr) {
        //the data length
        data_length = (trans_bytes / 64) - 1;
                //cfg flag compute
//         if((last_des != 1) | (des_link == 1))
//             cfg_flag = 0x1;
//         else
//             cfg_flag = 0x2;

         if((last_des != 1) | (des_link == 1))
             cfg_flag = 0x1;
         else
             cfg_flag = 0x0;

#ifdef NNDMA_DRIVER_DEBUG
    fprintf(nnadma_fp,"      =======================\n");
    fprintf(nnadma_fp,"      oram loop compute result\n");
    fprintf(nnadma_fp,"      =======================\n");
    fprintf(nnadma_fp,"      info-> c64k_num=%d, c64k_idx=%d\n", c64k_num, c64k_idx);
    fprintf(nnadma_fp,"      info-> Input info\n");
    fprintf(nnadma_fp,"      info-> o_pa_st_addr = 0x%x, trans_bytes=%d\n", o_pa_st_addr, trans_bytes);
    fprintf(nnadma_fp,"      info-> o_mlc_st_addr=0x%x, o_mlc_ed_addr=0x%x\n", o_mlc_st_addr, o_mlc_ed_addr);
    fprintf(nnadma_fp,"      \n");
    fprintf(nnadma_fp,"      info-> Output info\n");
    fprintf(nnadma_fp,"      info-> d_pa_st_addr=0x%x\n", d_pa_st_addr);
    fprintf(nnadma_fp,"      info-> d_pa_st_addr >> 6=0x%x\n", d_pa_addr);
    fprintf(nnadma_fp,"      info-> o_pa_st_addr=0x%x\n", o_pa_st_addr);
    fprintf(nnadma_fp,"      info-> o_pa_st_addr >> 6 =0x%x\n", o_pa_addr);
    fprintf(nnadma_fp,"      info-> 64bytes num = %d\n", data_length);
    fprintf(nnadma_fp,"      =======================\n");
#endif

#if 0
        des_buf[0].des_data[wptr    ] = ((o_pa_addr & 0x3F) << 26) | (d_pa_addr & 0x3FFFFFF);
//        des_buf[0].des_data[wptr + 1] = ((cfg_flag & 0x3) << 18) |
        des_buf[0].des_data[wptr + 1] = ((cfg_flag & 0x1) << 18) |
                        ((data_length & 0x3FF) << 8) |
                        ((o_pa_addr & 0x3FFF) >> 6);
#else
        des_buf[0].des_data[wptr    ] = d_pa_addr;
        des_buf[0].des_data[wptr + 1] = ((cfg_flag & 0x1) << 31) |
                        ((data_length & 0x3FF) << 16) |
                        ((o_pa_addr & 0x3FFF) << 0);
#endif
#ifdef NNDMA_DRIVER_DEBUG
    fprintf(nnadma_fp, "%s(%d)des_buf[0].des_data[wptr]=0x%x, des_buf[0].des_data[wptr + 1]=0x%x\n", __func__, __LINE__, des_buf[0].des_data[wptr], des_buf[0].des_data[wptr + 1]);
#endif
                des_num = des_num + 2;
                wptr = wptr + 2;
        o_pa_st_addr = o_pa_st_addr + trans_bytes;

            }
        else {
         cur_trans_bytes = o_mlc_ed_addr - o_pa_st_addr;
        //the data length
               data_length = (cur_trans_bytes / 64) - 1;
        //1st describe
        cfg_flag = 0x1;

#if 0
                des_buf[0].des_data[wptr    ] = ((o_pa_addr & 0x3F) << 26) | (d_pa_addr & 0x3FFFFFF);
//        des_buf[0].des_data[wptr + 1] = ((cfg_flag & 0x3) << 18) |
        des_buf[0].des_data[wptr + 1] = ((cfg_flag & 0x1) << 18) |
                                                ((data_length & 0x3FF) << 8) |
                        ((o_pa_addr & 0x3FFF) >> 6);
#else
        des_buf[0].des_data[wptr    ] = d_pa_addr;
        des_buf[0].des_data[wptr + 1] = ((cfg_flag & 0x1) << 31) |
                        ((data_length & 0x3FF) << 16) |
                        ((o_pa_addr & 0x3FFF) << 0);
#endif
#ifdef NNDMA_DRIVER_DEBUG
    fprintf(nnadma_fp, "%s(%d)des_buf[0].des_data[wptr]=0x%x, des_buf[0].des_data[wptr + 1]=0x%x\n", __func__, __LINE__, des_buf[0].des_data[wptr], des_buf[0].des_data[wptr + 1]);
#endif
                des_num = des_num + 2;
                wptr = wptr + 2;

        //2nd describe
        //the ddr start phy addr in 2nd describe
#if 0
        d_pa_addr = (d_pa_st_addr + cur_trans_bytes) >> 6;
#else
        d_pa_addr = (d_pa_st_addr + cur_trans_bytes);
#endif
        //the oram start phy addr in 2nd describe
        o_pa_addr = o_mlc_st_addr >> 6;
        cur_trans_bytes = trans_bytes - cur_trans_bytes;
        //the data length
        data_length = (cur_trans_bytes / 64) - 1;

        //cfg flag compute
//         if((last_des != 1) | (des_link == 1))
//             cfg_flag = 0x1;
//         else
//             cfg_flag = 0x2;

        if((last_des != 1) | (des_link == 1))
            cfg_flag = 0x1;
        else
            cfg_flag = 0x0;

#if 0
                des_buf[0].des_data[wptr    ] = ((o_pa_addr & 0x3F) << 26) | (d_pa_addr & 0x3FFFFFF);
//        des_buf[0].des_data[wptr + 1] = ((cfg_flag & 0x3) << 18) |
        des_buf[0].des_data[wptr + 1] = ((cfg_flag & 0x1) << 18) |
                                                ((data_length & 0x3FF) << 8) |
                        ((o_pa_addr & 0x3FFF) >> 6);
#else
        des_buf[0].des_data[wptr    ] = d_pa_addr;
        des_buf[0].des_data[wptr + 1] = ((cfg_flag & 0x1) << 31) |
                        ((data_length & 0x3FF) << 16) |
                        ((o_pa_addr & 0x3FFF) << 0);
#endif
#ifdef NNDMA_DRIVER_DEBUG
    fprintf(nnadma_fp, "%s(%d)des_buf[0].des_data[wptr]=0x%x, des_buf[0].des_data[wptr + 1]=0x%x\n", __func__, __LINE__, des_buf[0].des_data[wptr], des_buf[0].des_data[wptr + 1]);
#endif
                des_num = des_num + 2;
                wptr = wptr + 2;
        o_pa_st_addr = o_mlc_st_addr + cur_trans_bytes;
        }

        //compute the ddr and oram phy addr in next loop describe
        //the ddr start phy addr
        d_pa_st_addr = d_pa_st_addr + trans_bytes;
        //the oram start phy addr
        bytes_remain = bytes_remain - trans_bytes;
    }

    total_bytes = total_bytes + data_block_bytes;

    if(des_link!=1) {
            des_buf[0].chain_st_idx[chain_num] = chain_st_idx;
        des_buf[0].des_num[chain_num] = des_num;
        des_buf[0].total_bytes[chain_num] = total_bytes;
        chain_num = chain_num + 1;
        des_buf[0].chain_num = chain_num;
        chain_st_idx = cmd_idx + 1;
        des_num = 0;
        total_bytes = 0;
    }
    }

#ifdef NNDMA_DRIVER_DEBUG
    printf("    ********************\n");
    printf("    dma_des_analysis end\n");

    fprintf(nnadma_fp,"    ********************\n");
    fprintf(nnadma_fp,"    dma_des_analysis end\n");
    nna_des_analysis_end:{};
#endif

    return ret;
}

void nna_desram_update(des_info * des_rd_buf, des_info * des_wr_buf, uint32_t * dma_cmd_buf, des_gen_result * des_gen_result) {

    uint32_t i=0;
    uint32_t j=0;
    uint32_t des_total_num = 0;
    uint32_t des_rdch_num  = 0;
    uint32_t des_wrch_num  = 0;
    uint32_t desram_remain = 4096;
    uint32_t rd_des_wptr = 0;
    uint32_t wr_des_wptr = 0;
    uint32_t dma_cmd_wptr = 0;
    uint32_t * addr = 0;
    uint32_t finish_chain_num = 0;
    uint32_t desgen_err = 0;
    uint32_t phy_addr =0;
    uint32_t des_rd_buf_st_ptr=0;
    uint32_t des_wr_buf_st_ptr=0;

#ifdef NNDMA_DRIVER_DEBUG
    printf("      DMA driver: wr des to desram start\n");
    printf("\n");
    fprintf(nnadma_fp,"      DMA driver: wr des to desram start\n");
    fprintf(nnadma_fp,"\n");
    fprintf(nnadma_fp,"%s(%d):des_rd_buf[0].chain_num=%d\n", __func__, __LINE__, des_rd_buf[0].chain_num);
#endif

    for(i = 0; i < des_rd_buf[0].chain_num; i++) {
//    des_total_num = des_rd_buf[0].des_num[i] + des_wr_buf[0].des_num[i] + 4;
    des_total_num = des_rd_buf[0].des_num[i] + des_wr_buf[0].des_num[i];

    if(des_total_num <= desram_remain) {
        //================================
        //wrt the rd ch describe to desram
        //================================
        addr = desram_rvaddr + rd_des_wptr;

//         *(addr) = des_rd_buf[0].total_bytes[i];
//         *(addr+1) = 0;
//         addr = addr + 2;
        des_rdch_num = des_rd_buf[0].des_num[i];
#ifdef NNDMA_DRIVER_DEBUG
      fprintf(nnadma_fp,"%s(%d):des_rdch_num=%d, des_rd_buf[0].des_num[%d]=%d,rd_des_wptr=%d\n", __func__, __LINE__, des_rdch_num, i, des_rd_buf[0].des_num[i], rd_des_wptr);
#endif

        for(j=0; j < des_rdch_num; j++) {
        *(addr) = des_rd_buf[0].des_data[des_rd_buf_st_ptr + j];
#ifdef NNDMA_DRIVER_DEBUG
      fprintf(nnadma_fp,"%s(%d):i=%d, des_rd_buf[0].des_data[des_rd_buf_st_ptr + %d]=0x%x\n", __func__, __LINE__, i, j, des_rd_buf[0].des_data[des_rd_buf_st_ptr + j]);
#endif
        addr = addr + 1;
        }

        des_rd_buf_st_ptr = des_rd_buf_st_ptr + des_rdch_num;
        //for current wr des wptr
//        wr_des_wptr = rd_des_wptr + des_rdch_num + 2;
        wr_des_wptr = rd_des_wptr + des_rdch_num;
        //================================
        //wrt the wr ch describe to desram
        //================================
        addr = desram_rvaddr + wr_des_wptr;
//         *(addr) = des_wr_buf[0].total_bytes[i];
//         *(addr+1) = 0;
//        addr = addr + 2;
        des_wrch_num = des_wr_buf[0].des_num[i];
#ifdef NNDMA_DRIVER_DEBUG
      fprintf(nnadma_fp,"%s(%d):des_wrch_num=%d, des_wr_buf[0].des_num[%d]=%d\n", __func__, __LINE__, des_wrch_num, i, des_wr_buf[0].des_num[i]);
#endif

        for(j=0; j < des_wrch_num; j++) {
        *(addr) = des_wr_buf[0].des_data[des_wr_buf_st_ptr + j];
#ifdef NNDMA_DRIVER_DEBUG
    fprintf(nnadma_fp,"%s(%d):i=%d, des_wr_buf[0].des_data[des_wr_buf_st_ptr + %d]=0x%x, wr_des_wptr=%d\n", __func__, __LINE__, i, j, des_wr_buf[0].des_data[des_wr_buf_st_ptr + j], wr_des_wptr);
#endif
        addr = addr + 1;
        }

        des_wr_buf_st_ptr = des_wr_buf_st_ptr + des_wrch_num;
        desram_remain = desram_remain - des_total_num;
        finish_chain_num = finish_chain_num + 1;

        //dma cmd buf
//         dma_cmd_buf[dma_cmd_wptr    ] = (des_rd_buf[0].total_bytes[i]&0x3FFF)<<12 | (rd_des_wptr>>1) & 0xFFF;
//         dma_cmd_buf[dma_cmd_wptr + 1] = (des_wr_buf[0].total_bytes[i]&0x3FFF)<<12 | (wr_des_wptr>>1) & 0xFFF;
        dma_cmd_buf[dma_cmd_wptr    ] = ((des_rd_buf[0].total_bytes[i]&0x3FFF)<<12) | (rd_des_wptr / 2) & 0xFFF;
        dma_cmd_buf[dma_cmd_wptr + 1] = ((des_wr_buf[0].total_bytes[i]&0x3FFF)<<12) | (wr_des_wptr / 2) & 0xFFF;
#ifdef NNDMA_DRIVER_DEBUG
      fprintf(nnadma_fp,"%s(%d):i=%d,  dma_cmd_buf[dma_cmd_wptr    ]=0x%x, dma_cmd_buf[dma_cmd_wptr  + 1]=0x%x\n", __func__, __LINE__, i, dma_cmd_buf[dma_cmd_wptr    ], dma_cmd_buf[dma_cmd_wptr  + 1]);
#endif
        dma_cmd_wptr = dma_cmd_wptr + 2;

        //for next rd des wptr
//        rd_des_wptr = wr_des_wptr + des_wrch_num + 2;
        rd_des_wptr = wr_des_wptr + des_wrch_num;
    }
    else {
        des_gen_result[0].rcmd_st_idx = des_rd_buf[0].chain_st_idx[i];
        des_gen_result[0].wcmd_st_idx = des_wr_buf[0].chain_st_idx[i];
        desgen_err = 1;
        goto nna_wrt_desram_end;
    }
    }

    nna_wrt_desram_end:{
    //return the cmd chain info for user
    des_gen_result[0].dma_cmd_num = finish_chain_num;

    if(desgen_err) {
        des_gen_result[0].finish = 0;
    }
    else {
        des_gen_result[0].rcmd_st_idx = 0;
        des_gen_result[0].wcmd_st_idx = 0;
        des_gen_result[0].finish = 1;
    }
    };

#ifdef NNDMA_DRIVER_DEBUG
    printf("      DMA driver: wr des to desram end\n");
    printf("\n");
    fprintf(nnadma_fp,"      DMA driver: wr des to desram end\n");
    fprintf(nnadma_fp,"\n");
#endif
}

int nna_addr_align_check(unsigned int d_va_addr, unsigned int o_va_addr) {
    if(((d_va_addr & 0x3F) != 0) | ((o_va_addr & 0x3F) != 0)) {
    if((d_va_addr & 0x3F) != 0){
        printf("Error: NNA-DMA Malloc DDR address is not 64Bytes align, vir_addr = %x\n",d_va_addr);
    }

    if((o_va_addr & 0x3F) != 0) {
        printf("Error: NNA-DMA Malloc ORAM address is not 64Bytes align, oram_addr = %x\n",o_va_addr);
    }

    return 1;
    }
    else {
    return 0;
    }
}
