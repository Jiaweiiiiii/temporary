/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : bscaler_reg_rw.c
 * Authors    : lzhu@levi
 * Create Time: 2020-07-23:16:56:50
 * Description:
 *
 */
//#define CHIP_SIM_ENV
#include <stdio.h>
#include <stdlib.h>
#include "bscaler_hal.h"

#ifdef CHIP_SIM_ENV
#include "bscaler_hal.c"
#include "platform.c"
#endif

uint32_t bscaler_reg_addr[58] = {
    BSCALER_FRMC_CTRL,        //0
    BSCALER_FRMC_MODE,        //1
    BSCALER_FRMC_YBASE_SRC,   //2
    BSCALER_FRMC_CBASE_SRC,   //3
    BSCALER_FRMC_WH_SRC,      //4
    BSCALER_FRMC_PS_SRC,      //5
    BSCALER_FRMC_BASE_DST,    //6
    BSCALER_FRMC_WH_DST,      //7
    BSCALER_FRMC_PS_DST,      //8
    BSCALER_FRMC_BOX_BASE,    //9
    BSCALER_FRMC_MONO,        //10
    BSCALER_FRMC_EXTREME_Y,   //11
    BSCALER_FRMC_TIMEOUT,     //12
    BSCALER_FRMC_CHAIN_CTRL,  //13
    BSCALER_FRMC_CHAIN_BASE,  //14
    BSCALER_FRMC_CHAIN_LEN,   //15
    BSCALER_FRMC_BOX0,        //16
    BSCALER_FRMC_BOX1,        //17
    BSCALER_FRMC_BOX2,        //18
    BSCALER_FRMC_BOX3,        //19
    BSCALER_FRMC_BOX4,        //20
    BSCALER_FRMC_BOX5,        //21
    BSCALER_FRMC_BOX6,        //22
    BSCALER_FRMC_BOX7,        //23
    BSCALER_FRMC_BOX8,        //24
    BSCALER_FRMC_BOX9,        //25
    BSCALER_FRMC_BOX10,       //26
    BSCALER_FRMT_CTRL,        //27
    BSCALER_FRMT_TASK,        //28
    BSCALER_FRMT_YBASE_SRC,   //29
    BSCALER_FRMT_CBASE_SRC,   //30
    BSCALER_FRMT_WH_SRC,      //31
    BSCALER_FRMT_PS_SRC,      //32
    BSCALER_FRMT_YBASE_DST,   //33
    BSCALER_FRMT_CBASE_DST,   //34
    BSCALER_FRMT_FS_DST,      //35
    BSCALER_FRMT_PS_DST,      //36
    BSCALER_FRMT_DUMMY,       //37
    BSCALER_FRMT_FORMAT,      //38
    BSCALER_FRMT_CHAIN_CTRL,  //39
    BSCALER_FRMT_CHAIN_BASE,  //40
    BSCALER_FRMT_CHAIN_LEN,   //41
    BSCALER_FRMT_TIMEOUT,     //42
    BSCALER_PARAM0,           //43
    BSCALER_PARAM1,           //44
    BSCALER_PARAM2,           //45
    BSCALER_PARAM3,           //46
    BSCALER_PARAM4,           //47
    BSCALER_PARAM5,           //48
    BSCALER_PARAM6,           //49
    BSCALER_PARAM7,           //50
    BSCALER_PARAM8,           //51
    BSCALER_PARAM9,           //52
    BSCALER_FRMT_ISUM,        //54
    BSCALER_FRMT_OSUM,        //55
    BSCALER_FRMC_ISUM,        //56
    BSCALER_FRMC_OSUM         //57
};

uint32_t bscaler_reg_default[58] = {
    0x00000000,      //0
    0x00010000,      //1
    0x00000000,      //2
    0x00000000,      //3
    0x00000000,      //4
    0x00000000,      //5
    0x00000000,      //6
    0x00000000,      //7
    0x00000000,      //8
    0x00000000,      //9
    0x00000000,      //10
    0x00000000,      //11
    0xffffffff,      //12
    0x00000000,      //13
    0x00000000,      //14
    0x00000000,      //15
    0x00000000,      //16
    0x00000000,      //17
    0x00000000,      //18
    0x00000000,      //19
    0x00000000,      //20
    0x00000000,      //21
    0x00000000,      //22
    0x00000000,      //23
    0x00000000,      //24
    0x00000000,      //25
    0x00000001,      //26
    0x00000000,      //27
    0x00000000,      //28
    0x00000000,      //29
    0x00000000,      //30
    0x00000000,      //31
    0x0000000l,      //32
    0x00000000,      //33
    0x00000000,      //34
    0x00000000,      //35
    0x00000000,      //36
    0xffffffff,      //37
    0x00000050,      //38
    0x00000000,      //39
    0x00000000,      //40
    0x00000000,      //41
    0xffffffff,      //42
    0x00000000,      //43
    0x00000000,      //44
    0x00000000,      //45
    0x00000000,      //46
    0x00000000,      //47
    0x00000000,      //48
    0x00000000,      //49
    0x00000000,      //50
    0x00000000,      //51
    0x00000000,      //52
    0x00000000,      //53
    0x00000000,      //54
    0x00000000,      //55
    0x00000000,      //56
    0x00000000       //57
};
uint32_t bscaler_wff_value[58] = {
    0x00001e00,      //0   dumy
    0x0007ff3f,      //1
    0xffffffff,      //2
    0xffffffff,      //3
    0xffffffff,      //4
    0x0000ffff,      //5
    0xffffffff,      //6
    0x01ff01ff,      //7
    0x0000ffff,      //8
    0xffffffff,      //9
    0x00ff00ff,      //10
    0xffffffff,      //11
    0xffffffff,      //12
    0x0000010a,      //13  dumy
    0xffffffff,      //14
    0xfffffff8,      //15
    0xffffffff,      //16
    0xffffffff,      //17
    0xffffffff,      //18
    0xffffffff,      //19
    0xffffffff,      //20
    0xffffffff,      //21
    0xffffffff,      //22
    0xffffffff,      //23
    0xffffffff,      //24
    0xffffffff,      //25
    0xffffffff,      //26
    0x0000e000,      //27  dumy
    0xffff0002,      //28  dumy
    0xffffffff,      //29
    0xffffffff,      //30
    0xfffefffe,      //31
    0x0000ffff,      //32
    0xffffffff,      //33
    0xffffffff,      //34
    0xffffffff,      //35
    0x0000ffff,      //36
    0xffffffff,      //37
    0x777703fd,      //38
    0x0000000a,      //39  dumy
    0xffffffff,      //40
    0xfffffff8,      //41
    0xffffffff,      //42
    0xffffffff,      //43
    0xffffffff,      //44
    0xffffffff,      //45
    0xffffffff,      //46
    0xffffffff,      //47
    0xffffffff,      //48
    0xffffffff,      //49
    0xffffffff,      //50
    0xffffffff,      //51
    0xffffff0f,      //52
    0x0000000d,      //53 //reset dumy
    0x00000000,      //54
    0x00000000,      //55
    0x00000000,      //56
    0x00000000       //57
};

uint32_t bscaler_w5a_value[58] = {
    0x00001e00,      //0   dumy
    0x00025a1a,      //1
    0x5a5a5a5a,      //2
    0x5a5a5a5a,      //3
    0x5a5a5a5a,      //4
    0x00005a5a,      //5
    0x5a5a5a5a,      //6
    0x005a005a,      //7
    0x00005a5a,      //8
    0x5a5a5a5a,      //9
    0x005a005a,      //10
    0x5a5a5a5a,      //11
    0x5a5a5a5a,      //12
    0x0000010a,      //13 dumy
    0x5a5a5a5a,      //14
    0x5a5a5a58,      //15
    0x5a5a5a5a,      //16
    0x5a5a5a5a,      //17
    0x5a5a5a5a,      //18
    0x5a5a5a5a,      //19
    0x5a5a5a5a,      //20
    0x5a5a5a5a,      //21
    0x5a5a5a5a,      //22
    0x5a5a5a5a,      //23
    0x5a5a5a5a,      //24
    0x5a5a5a5a,      //25
    0x5a5a5a5a,      //26
    0x0000e000,      //27 dumy
    0x5a5a0002,      //28 dumy
    0x5a5a5a5a,      //29
    0x5a5a5a5a,      //30
    0x5a5a5a5a,      //31
    0x00005a5a,      //32
    0x5a5a5a5a,      //33
    0x5a5a5a5a,      //34
    0x5a5a5a5a,      //35
    0x00005a5a,      //36
    0x5a5a5a5a,      //37
    0x52520258,      //38
    0x0000000a,      //39 dumy
    0x5a5a5a5a,      //40
    0x5a5a5a58,      //41
    0x5a5a5a5a,      //42
    0x5a5a5a5a,      //43
    0x5a5a5a5a,      //44
    0x5a5a5a5a,      //45
    0x5a5a5a5a,      //46
    0x5a5a5a5a,      //47
    0x5a5a5a5a,      //48
    0x5a5a5a5a,      //49
    0x5a5a5a5a,      //50
    0x5a5a5a5a,      //51
    0x5a5a5a0a,      //52
    0x00000008,      //53 //reset
    0x00000000,      //54
    0x00000000,      //55
    0x00000000,      //56
    0x00000000       //57
};

uint32_t bscaler_wa5_value[58] = {
    0x00001e00,      //0 dummy
    0x0005a525,      //1
    0xa5a5a5a5,      //2
    0xa5a5a5a5,      //3
    0xa5a5a5a5,      //4
    0x0000a5a5,      //5
    0xa5a5a5a5,      //6
    0x01a501a5,      //7
    0x0000a5a5,      //8
    0xa5a5a5a5,      //9
    0x00a500a5,      //10
    0xa5a5a5a5,      //11
    0xa5a5a5a5,      //12
    0x0000010a,      //13 dummy
    0xa5a5a5a5,      //14
    0xa5a5a5a0,      //15
    0xa5a5a5a5,      //16
    0xa5a5a5a5,      //17
    0xa5a5a5a5,      //18
    0xa5a5a5a5,      //19
    0xa5a5a5a5,      //20
    0xa5a5a5a5,      //21
    0xa5a5a5a5,      //22
    0xa5a5a5a5,      //23
    0xa5a5a5a5,      //24
    0xa5a5a5a5,      //25
    0xa5a5a5a5,      //26
    0x0000a000,      //27 dumy
    0xa5a50000,      //28 dumy
    0xa5a5a5a5,      //29
    0xa5a5a5a5,      //30
    0xa5a4a5a4,      //31
    0x0000a5a5,      //32
    0xa5a5a5a5,      //33
    0xa5a5a5a5,      //34
    0xa5a5a5a5,      //35
    0x0000a5a5,      //36
    0xa5a5a5a5,      //37
    0x252501a5,      //38
    0x0000000a,      //39
    0xa5a5a5a5,      //40
    0xa5a5a5a0,      //41
    0xa5a5a5a5,      //42
    0xa5a5a5a5,      //43
    0xa5a5a5a5,      //44
    0xa5a5a5a5,      //45
    0xa5a5a5a5,      //46
    0xa5a5a5a5,      //47
    0xa5a5a5a5,      //48
    0xa5a5a5a5,      //49
    0xa5a5a5a5,      //50
    0xa5a5a5a5,      //51
    0xa5a5a505,      //52
    0x0000000d,      //53 //reset  dummy
    0x00000000,      //54
    0x00000000,      //55
    0x00000000,      //56
    0x00000000       //57
};

#ifdef CHIP_SIM_ENV
#define TEST_NAME  t_bscaler
#else
#define TEST_NAME  main
#endif
int TEST_NAME()
{
    pf_printf("------reg case begin------!\n");
    // 1. platform initialized
    pf_init();
    //case
    uint8_t i,k,size=1;
    uint32_t read_default;
    uint32_t check_bypass = 0;
    //comp default
    pf_printf("Step1---\n");
    pf_printf("Step1: check the default value\n");
    pf_printf("Step1---\n");
    for (i = 0; i < 58; i++) {
        check_bypass = (i == 6);
        if (check_bypass) {
            pf_printf("NOTE:default check bypass %d\n", i);
        } else {
            if (i == 6) {
                size = 64;//fix me init lut_ram
            } else if (i == 11) {
                size = 16;
            } else {
                size = 1;
            }

            for (k = 0; k < size; k++) {
                read_default = pf_read_reg(BSCALER_BASE + bscaler_reg_addr[i]);
                if (read_default == bscaler_reg_default[i]) {
                    //pf_printf("success:%d(%d); 0x%x\n",i,k,read_default);
                } else {
                    pf_printf("error:%d(%d); GLD:0x%x, DUT:0x%x\n",i,k,bscaler_reg_default[i],read_default);
                    pf_deinit();
                    return 1;
                }
            }
        }
    }
    //set intc_mask
    //comp 0xffffffff
    check_bypass = 0;
    uint32_t read_ff;
    pf_printf("Step2---\n");
    pf_printf("Step2: check the ff_value\n");
    pf_printf("Step2---\n");

    for (i = 0; i < 58; i++) {
        check_bypass = (i==0 | i==13 | i==27 |
                        i==28 | i==39 | i==53);
        if (check_bypass) {
            pf_printf("NOTE:ff_value check bypass CTRL reg %d\n", i);
        } else {
            if (i == 6) {
                size = 64;//fix me init lut_ram
            } else if (i == 11) {
                size = 16;
            } else {
                size = 1;
            }
            for (k=0; k<size;k++) {
                pf_write_reg(BSCALER_BASE + bscaler_reg_addr[i],0xffffffff);
                read_ff = pf_read_reg(BSCALER_BASE + bscaler_reg_addr[i]);
                if (read_ff == bscaler_wff_value[i]) {
                    //pf_printf("success:%d(%d); 0x%x\n",i,k,read_ff);
                } else {
                    pf_printf("error:%d(%d); GLD:0x%x, DUT:0x%x\n",i,k,bscaler_wff_value[i],read_ff);
                    pf_deinit();
                    return 1;
                }
            }
        }
    }

    check_bypass=0;
    //comp 0x5a5a5a5a
    uint32_t read_5a;
    pf_printf("Step3---\n");
    pf_printf("Step3: check the 5a_value\n");
    pf_printf("Step3---\n");

    for(i=0;i<58;i++){
        check_bypass = (i==0 | i==13 | i==27 |
                        i==28 | i==39);
        if(check_bypass){
            pf_printf("NOTE:5a_value check bypass CTRL reg %d\n", i);
        }else{
            if( i == 6 ){
                size = 64;//fix me init lut_ram
            }
            else if(i == 11 ){
                size = 16;
            }
            else{
                size = 1;
            }
            for(k=0; k<size;k++){
                pf_write_reg(BSCALER_BASE + bscaler_reg_addr[i],0x5a5a5a5a);
                read_5a = pf_read_reg(BSCALER_BASE + bscaler_reg_addr[i]);
                if(read_5a == bscaler_w5a_value[i]){
                    //pf_printf("success:%d(%d); 0x%x\n",i,k,read_5a);
                }else{
                    pf_printf("error:%d(%d); GLD:0x%x, DUT:0x%x\n",i,k,bscaler_w5a_value[i],read_5a);
                    pf_deinit();
                    return 1;
                }
            }
        }
    }
    check_bypass=0;
    //comp 0xa5a5a5a5
    uint32_t read_a5;
    pf_printf("Step4---\n");
    pf_printf("Step4: check the a5_value\n");
    pf_printf("Step4---\n");

    for(i=0;i<58;i++){
        check_bypass = (i==0 | i==13 | i==27 |
                        i==28 | i==39  | i==53);
        if(check_bypass){
            pf_printf("NOTE:a5_value check bypass CTRL reg %d\n", i);
        }else{
            if( i == 6 ){
                size = 64;//fix me init lut_ram
            }
            else if(i == 11 ){
                size = 16;
            }
            else{
                size = 1;
            }
            for(k=0; k<size;k++){
                pf_write_reg(BSCALER_BASE + bscaler_reg_addr[i],0xa5a5a5a5);
                read_a5 = pf_read_reg(BSCALER_BASE + bscaler_reg_addr[i]);
                if(read_a5 == bscaler_wa5_value[i]){
                    //pf_printf("success:%d(%d); 0x%x\n",i,k,read_a5);
                }else{
                    pf_printf("error:%d(%d); GLD:0x%x, DUT:0x%x\n",i,k,bscaler_wa5_value[i],read_a5);
                    pf_deinit();
                    return 1;
                }
            }
        }
    }
    pf_printf("reg_rw is passed!\n");
    pf_deinit();

    return 0;
}
#ifdef CHIP_SIM_ENV
TEST_MAIN(TEST_NAME);
#endif
