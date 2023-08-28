/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : nna_reg_setv.h
 * Authors    : jzhang@elim.ic.jz.com
 * Create Time: 2020-04-24:15:40:50
 * Description: set NNA reg variable
 *              use after include nna_reg_set_c.h
 *              set all needed REG_<reg-name>'s value by
 * #define REG_<reg-name> <value>
 *              Where <value> can be a variable or an expression. Then include
 *              this file. This may clob all VWR registers
 */

reg_index = 0;

#define SET_REG_VALUE(reg)                                                                         \
    reg_value[reg_index] = REG_##reg;                                                              \
    reg_index++

#ifdef REG_WR_RADDR
SET_REG_VALUE(WR_RADDR);
#endif
#ifdef REG_WR_RADDR_RST
SET_REG_VALUE(WR_RADDR_RST);
#endif
#ifdef REG_RCFL_E
SET_REG_VALUE(RCFL_E);
#endif
#ifdef REG_PAD_VALUE
SET_REG_VALUE(PAD_VALUE);
#endif
#ifdef REG_FP_H
SET_REG_VALUE(FP_H);
#endif
#ifdef REG_FP_W
SET_REG_VALUE(FP_W);
#endif
#ifdef REG_MACTL_FP_Y
SET_REG_VALUE(FP_MACTL_FP_Y);
#endif
#ifdef REG_MACTL_FP_X
SET_REG_VALUE(FP_MACTL_FP_X);
#endif
#ifdef REG_TLYP_ACC
SET_REG_VALUE(TLYP_ACC);
#endif
#ifdef REG_SF_MODE
SET_REG_VALUE(SF_MODE);
#endif
#ifdef REG_MIN_FPN_O
SET_REG_VALUE(MIN_FPN_O);
#endif
#ifdef REG_FR_RB_E
SET_REG_VALUE(FR_RB_E);
#endif
#ifdef REG_F_BIT_GSIZE
SET_REG_VALUE(F_BIT_GSIZE);
#endif
#ifdef REG_FR_FP_W
SET_REG_VALUE(FR_FP_W);
#endif
#ifdef REG_FR_FP_STRIDE
SET_REG_VALUE(FR_FP_STRIDE);
#endif
#ifdef REG_MAC_STRIDE
SET_REG_VALUE(MAC_STRIDE);
#endif
#ifdef REG_MAC_FP
SET_REG_VALUE(MAC_FP);
#endif
#ifdef REG_MAC_BIT
SET_REG_VALUE(MAC_BIT);
#endif
#ifdef REG_FP_NUM0
SET_REG_VALUE(FP_NUM0);
#endif
#ifdef REG_FP_NUM1
SET_REG_VALUE(FP_NUM1);
#endif
#ifdef REG_MAC_W_MASK
SET_REG_VALUE(MAC_W_MASK);
#endif
#ifdef REG_MAC_P_MASK
SET_REG_VALUE(MAC_P_MASK);
#endif

#ifdef REG_BTR_WADDR
SET_REG_VALUE(BTR_WADDR);
#endif
#ifdef REG_BTR_RADDR
SET_REG_VALUE(BTR_RADDR);
#endif
#ifdef REG_MSF_MODE
SET_REG_VALUE(MSF_MODE);
#endif
#ifdef REG_Q_BIT
SET_REG_VALUE(Q_BIT);
#endif
#ifdef REG_QD_CONFIG
SET_REG_VALUE(QD_CONFIG);
#endif
#ifdef REG_RDND_NUM
SET_REG_VALUE(RDND_NUM);
#endif
#ifdef REG_OC_LSH
SET_REG_VALUE(OC_LSH);
#endif
#ifdef REG_PIXEL_OUT0
SET_REG_VALUE(PIXEL_OUT0);
#endif
#ifdef REG_PIXEL_OUT1
SET_REG_VALUE(PIXEL_OUT1);
#endif
#ifdef REG_FR_WADDR0
SET_REG_VALUE(FR_WADDR0);
#endif
#ifdef REG_FR_WADDR1
SET_REG_VALUE(FR_WADDR1);
#endif
#ifdef REG_FR_CONFIG
SET_REG_VALUE(FR_CONFIG);
#endif
#ifdef REG_F_BIT
SET_REG_VALUE(F_BIT);
#endif
#ifdef REG_WFCW_PN
SET_REG_VALUE(WFCW_PN);
#endif

#undef SET_REG_VALUE

#ifdef DEBUG_INFO
printf("Variable %d NNA reg set\n", reg_index);
#endif
if (reg_index > 0)
    LA(o, VR31, 0, reg_value, 0);
if (reg_index > 8)
    LA(o, VR31, 1, reg_value, 32);
if (reg_index > 16) {
    printf("Error: reg number need to set is %d > 16\n", reg_index);
    exit(-1);
}

#define NNRW_VWR(type, reg)                                                                        \
    if (reg_index == 0)                                                                            \
        NNRW##type(VWR0, reg);                                                                     \
    if (reg_index == 1)                                                                            \
        NNRW##type(VWR1, reg);                                                                     \
    if (reg_index == 2)                                                                            \
        NNRW##type(VWR2, reg);                                                                     \
    if (reg_index == 3)                                                                            \
        NNRW##type(VWR3, reg);                                                                     \
    if (reg_index == 4)                                                                            \
        NNRW##type(VWR4, reg);                                                                     \
    if (reg_index == 5)                                                                            \
        NNRW##type(VWR5, reg);                                                                     \
    if (reg_index == 6)                                                                            \
        NNRW##type(VWR6, reg);                                                                     \
    if (reg_index == 7)                                                                            \
        NNRW##type(VWR7, reg);                                                                     \
    if (reg_index == 8)                                                                            \
        NNRW##type(VWR8, reg);                                                                     \
    if (reg_index == 9)                                                                            \
        NNRW##type(VWR9, reg);                                                                     \
    if (reg_index == 10)                                                                           \
        NNRW##type(VWR10, reg);                                                                    \
    if (reg_index == 11)                                                                           \
        NNRW##type(VWR11, reg);                                                                    \
    if (reg_index == 12)                                                                           \
        NNRW##type(VWR12, reg);                                                                    \
    if (reg_index == 13)                                                                           \
        NNRW##type(VWR13, reg);                                                                    \
    if (reg_index == 14)                                                                           \
        NNRW##type(VWR14, reg);                                                                    \
    if (reg_index == 15)                                                                           \
    NNRW##type(VWR15, reg)

#define NNRW_REG_VALUE(type, reg)                                                                  \
    NNRW_VWR(type, NNARA_##reg);                                                                   \
    reg_index++

reg_index = 0;

#ifdef REG_WR_RADDR
NNRW_REG_VALUE(M, WR_RADDR);
#undef REG_WR_RADDR
#endif
#ifdef REG_WR_RADDR_RST
NNRW_REG_VALUE(M, WR_RADDR_RST);
#undef REG_WR_RADDR_RST
#endif
#ifdef REG_RCFL_E
NNRW_REG_VALUE(M, RCFL_E);
#undef REG_RCFL_E
#endif
#ifdef REG_PAD_VALUE
NNRW_REG_VALUE(M, PAD_VALUE);
#undef REG_PAD_VALUE
#endif
#ifdef REG_FP_H
NNRW_REG_VALUE(M, FP_H);
#undef REG_FP_H
#endif
#ifdef REG_FP_W
NNRW_REG_VALUE(M, FP_W);
#undef REG_FP_W
#endif
#ifdef REG_MACTL_FP_Y
NNRW_REG_VALUE(M, MACTL_FP_Y);
#undef REG_MACTL_FP_Y
#endif
#ifdef REG_MACTL_FP_X
NNRW_REG_VALUE(M, MACTL_FP_X);
#undef REG_MACTL_FP_X
#endif
#ifdef REG_TLYP_ACC
NNRW_REG_VALUE(M, TLYP_ACC);
#undef REG_TLYP_ACC
#endif
#ifdef REG_SF_MODE
NNRW_REG_VALUE(M, SF_MODE);
#undef REG_SF_MODE
#endif
#ifdef REG_MIN_FPN_O
NNRW_REG_VALUE(M, MIN_FPN_O);
#undef REG_MIN_FPN_O
#endif
#ifdef REG_FR_RB_E
NNRW_REG_VALUE(M, FR_RB_E);
#undef REG_FR_RB_E
#endif
#ifdef REG_F_BIT_GSIZE
NNRW_REG_VALUE(M, F_BIT_GSIZE);
#undef REG_F_BIT_GSIZE
#endif
#ifdef REG_FR_FP_W
NNRW_REG_VALUE(M, FR_FP_W);
#undef REG_FR_FP_W
#endif
#ifdef REG_FR_FP_STRIDE
NNRW_REG_VALUE(M, FR_FP_STRIDE);
#undef REG_FR_FP_STRIDE
#endif
#ifdef REG_MAC_STRIDE
NNRW_REG_VALUE(M, MAC_STRIDE);
#undef REG_MAC_STRIDE
#endif
#ifdef REG_MAC_FP
NNRW_REG_VALUE(M, MAC_FP);
#undef REG_MAC_FP
#endif
#ifdef REG_MAC_BIT
NNRW_REG_VALUE(M, MAC_BIT);
#undef REG_MAC_BIT
#endif
#ifdef REG_FP_NUM0
NNRW_REG_VALUE(M, FP_NUM0);
#undef REG_FP_NUM0
#endif
#ifdef REG_FP_NUM1
NNRW_REG_VALUE(M, FP_NUM1);
#undef REG_FP_NUM1
#endif
#ifdef REG_MAC_W_MASK
NNRW_REG_VALUE(M, MAC_W_MASK);
#undef REG_MAC_W_MASK
#endif
#ifdef REG_MAC_P_MASK
NNRW_REG_VALUE(M, MAC_P_MASK);
#undef REG_MAC_P_MASK
#endif

#ifdef REG_BTR_WADDR
NNRW_REG_VALUE(D, BTR_WADDR);
#undef REG_BTR_WADDR
#endif
#ifdef REG_BTR_RADDR
NNRW_REG_VALUE(D, BTR_RADDR);
#undef REG_BTR_RADDR
#endif
#ifdef REG_MSF_MODE
NNRW_REG_VALUE(D, MSF_MODE);
#undef REG_MSF_MODE
#endif
#ifdef REG_Q_BIT
NNRW_REG_VALUE(D, Q_BIT);
#undef REG_Q_BIT
#endif
#ifdef REG_QD_CONFIG
NNRW_REG_VALUE(D, QD_CONFIG);
#undef REG_QD_CONFIG
#endif
#ifdef REG_RDND_NUM
NNRW_REG_VALUE(D, RDND_NUM);
#undef REG_RDND_NUM
#endif
#ifdef REG_OC_LSH
NNRW_REG_VALUE(D, OC_LSH);
#undef REG_OC_LSH
#endif
#ifdef REG_PIXEL_OUT0
NNRW_REG_VALUE(D, PIXEL_OUT0);
#undef REG_PIXEL_OUT0
#endif
#ifdef REG_PIXEL_OUT1
NNRW_REG_VALUE(D, PIXEL_OUT1);
#undef REG_PIXEL_OUT1
#endif
#ifdef REG_FR_WADDR0
NNRW_REG_VALUE(D, FR_WADDR0);
#undef REG_FR_WADDR0
#endif
#ifdef REG_FR_WADDR1
NNRW_REG_VALUE(D, FR_WADDR1);
#undef REG_FR_WADDR1
#endif
#ifdef REG_FR_CONFIG
NNRW_REG_VALUE(D, FR_CONFIG);
#undef REG_FR_CONFIG
#endif
#ifdef REG_F_BIT
NNRW_REG_VALUE(D, F_BIT);
#undef REG_F_BIT
#endif
#ifdef REG_WFCW_PN
NNRW_REG_VALUE(D, WFCW_PN);
#undef REG_WFCW_PN
#endif

#undef NNRW_REG_VALUE
#undef NNRW_VWR
