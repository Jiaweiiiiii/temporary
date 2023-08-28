/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : nna_reg_default.h
 * Authors    : jzhang@elim.ic.jz.com
 * Create Time: 2019-12-25:14:40:58
 * Description: set NNA reg constant
 *              set all needed REG_<reg-name>'s value by
 * #define REG_<reg-name> <const>
 *              Where <const> must be a constant. Then include this file
 *              This may clob all VWR registers
 */

#define DEFAULT_WR_RADDR 0
#define DEFAULT_WR_RADDR_RST 0
#define DEFAULT_RCFL_E 1
#define DEFAULT_PAD_VALUE 0
#define DEFAULT_FP_H 2047
#define DEFAULT_FP_W 2047
#define DEFAULT_MACTL_FP_Y -1
#define DEFAULT_MACTL_FP_X -1
#define DEFAULT_TLYP_ACC 0
#define DEFAULT_SF_MODE 0
#define DEFAULT_MIN_FPN_O 0
#define DEFAULT_FR_RB_E 1
#define DEFAULT_F_BIT_GSIZE 1
#define DEFAULT_FR_FP_W 20
#define DEFAULT_FR_FP_STRIDE 64
#define DEFAULT_MAC_STRIDE 0
#define DEFAULT_MAC_FP 2
#define DEFAULT_MAC_BIT 0
#define DEFAULT_FP_NUM0 0
#define DEFAULT_FP_NUM1 0
#define DEFAULT_MAC_W_MASK 0x1ff
#define DEFAULT_MAC_P_MASK 0xff

#define DEFAULT_BTR_WADDR 0
#define DEFAULT_BTR_RADDR 0
#define DEFAULT_MSF_MODE 0
#define DEFAULT_Q_BIT 0
#define DEFAULT_QD_CONFIG 0
#define DEFAULT_RDND_NUM 0
#define DEFAULT_OC_LSH 0
#define DEFAULT_PIXEL_OUT0 0x3210
#define DEFAULT_PIXEL_OUT1 0x7654
#define DEFAULT_FR_WADDR0 0
#define DEFAULT_FR_WADDR1 0
#define DEFAULT_FR_CONFIG 0
#define DEFAULT_F_BIT 0
#define DEFAULT_WFCW_PN 0

// assume less than or equal to 16 regs needs to set

uint32_t reg_value_0[23];
uint32_t *reg_value = (uint32_t *)(((uint32_t)(uintptr_t)(reg_value_0) + 28) / 32 * 32);
#ifdef DEBUG_INFO
printf("reg_value_0=%x reg_value=%x\n", reg_value_0, reg_value);
#endif
int reg_index;

reg_index = 0;

#define SET_REG_CONST(reg)                                                                         \
    if ((REG_##reg) != DEFAULT_##reg) {                                                            \
        reg_value[reg_index] = REG_##reg;                                                          \
        reg_index++;                                                                               \
    }

#ifdef REG_WR_RADDR
SET_REG_CONST(WR_RADDR);
#endif
#ifdef REG_WR_RADDR_RST
SET_REG_CONST(WR_RADDR_RST);
#endif
#ifdef REG_RCFL_E
SET_REG_CONST(RCFL_E);
#endif
#ifdef REG_PAD_VALUE
SET_REG_CONST(PAD_VALUE);
#endif
#ifdef REG_FP_H
SET_REG_CONST(FP_H);
#endif
#ifdef REG_FP_W
SET_REG_CONST(FP_W);
#endif
#ifdef REG_MACTL_FP_Y
SET_REG_CONST(FP_MACTL_FP_Y);
#endif
#ifdef REG_MACTL_FP_X
SET_REG_CONST(FP_MACTL_FP_X);
#endif
#ifdef REG_TLYP_ACC
SET_REG_CONST(TLYP_ACC);
#endif
#ifdef REG_SF_MODE
SET_REG_CONST(SF_MODE);
#endif
#ifdef REG_MIN_FPN_O
SET_REG_CONST(MIN_FPN_O);
#endif
#ifdef REG_FR_RB_E
SET_REG_CONST(FR_RB_E);
#endif
#ifdef REG_F_BIT_GSIZE
SET_REG_CONST(F_BIT_GSIZE);
#endif
#ifdef REG_FR_FP_W
SET_REG_CONST(FR_FP_W);
#endif
#ifdef REG_FR_FP_STRIDE
SET_REG_CONST(FR_FP_STRIDE);
#endif
#ifdef REG_MAC_STRIDE
SET_REG_CONST(MAC_STRIDE);
#endif
#ifdef REG_MAC_FP
SET_REG_CONST(MAC_FP);
#endif
#ifdef REG_MAC_BIT
SET_REG_CONST(MAC_BIT);
#endif
#ifdef REG_FP_NUM0
SET_REG_CONST(FP_NUM0);
#endif
#ifdef REG_FP_NUM1
SET_REG_CONST(FP_NUM1);
#endif
#ifdef REG_MAC_W_MASK
SET_REG_CONST(MAC_W_MASK);
#endif
#ifdef REG_MAC_P_MASK
SET_REG_CONST(MAC_P_MASK);
#endif

#ifdef REG_BTR_WADDR
SET_REG_CONST(BTR_WADDR);
#endif
#ifdef REG_BTR_RADDR
SET_REG_CONST(BTR_RADDR);
#endif
#ifdef REG_MSF_MODE
SET_REG_CONST(MSF_MODE);
#endif
#ifdef REG_Q_BIT
SET_REG_CONST(Q_BIT);
#endif
#ifdef REG_QD_CONFIG
SET_REG_CONST(QD_CONFIG);
#endif
#ifdef REG_RDND_NUM
SET_REG_CONST(RDND_NUM);
#endif
#ifdef REG_OC_LSH
SET_REG_CONST(OC_LSH);
#endif
#ifdef REG_PIXEL_OUT0
SET_REG_CONST(PIXEL_OUT0);
#endif
#ifdef REG_PIXEL_OUT1
SET_REG_CONST(PIXEL_OUT1);
#endif
#ifdef REG_FR_WADDR0
SET_REG_CONST(FR_WADDR0);
#endif
#ifdef REG_FR_WADDR1
SET_REG_CONST(FR_WADDR1);
#endif
#ifdef REG_FR_CONFIG
SET_REG_CONST(FR_CONFIG);
#endif
#ifdef REG_F_BIT
SET_REG_CONST(F_BIT);
#endif
#ifdef REG_WFCW_PN
SET_REG_CONST(WFCW_PN);
#endif

#undef SET_REG_CONST

#ifdef DEBUG_INFO
printf("Constant %d NNA reg set\n", reg_index);
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

#define NNRW_REG_CONST(type, reg)                                                                  \
    if ((REG_##reg) != DEFAULT_##reg) {                                                            \
        NNRW_VWR(type, NNARA_##reg);                                                               \
        reg_index++;                                                                               \
    }

reg_index = 0;

#ifdef REG_WR_RADDR
NNRW_REG_CONST(M, WR_RADDR);
#undef REG_WR_RADDR
#endif
#ifdef REG_WR_RADDR_RST
NNRW_REG_CONST(M, WR_RADDR_RST);
#undef REG_WR_RADDR_RST
#endif
#ifdef REG_RCFL_E
NNRW_REG_CONST(M, RCFL_E);
#undef REG_RCFL_E
#endif
#ifdef REG_PAD_VALUE
NNRW_REG_CONST(M, PAD_VALUE);
#undef REG_PAD_VALUE
#endif
#ifdef REG_FP_H
NNRW_REG_CONST(M, FP_H);
#undef REG_FP_H
#endif
#ifdef REG_FP_W
NNRW_REG_CONST(M, FP_W);
#undef REG_FP_W
#endif
#ifdef REG_MACTL_FP_Y
NNRW_REG_CONST(M, MACTL_FP_Y);
#undef REG_MACTL_FP_Y
#endif
#ifdef REG_MACTL_FP_X
NNRW_REG_CONST(M, MACTL_FP_X);
#undef REG_MACTL_FP_X
#endif
#ifdef REG_TLYP_ACC
NNRW_REG_CONST(M, TLYP_ACC);
#undef REG_TLYP_ACC
#endif
#ifdef REG_SF_MODE
NNRW_REG_CONST(M, SF_MODE);
#undef REG_SF_MODE
#endif
#ifdef REG_MIN_FPN_O
NNRW_REG_CONST(M, MIN_FPN_O);
#undef REG_MIN_FPN_O
#endif
#ifdef REG_FR_RB_E
NNRW_REG_CONST(M, FR_RB_E);
#undef REG_FR_RB_E
#endif
#ifdef REG_F_BIT_GSIZE
NNRW_REG_CONST(M, F_BIT_GSIZE);
#undef REG_F_BIT_GSIZE
#endif
#ifdef REG_FR_FP_W
NNRW_REG_CONST(M, FR_FP_W);
#undef REG_FR_FP_W
#endif
#ifdef REG_FR_FP_STRIDE
NNRW_REG_CONST(M, FR_FP_STRIDE);
#undef REG_FR_FP_STRIDE
#endif
#ifdef REG_MAC_STRIDE
NNRW_REG_CONST(M, MAC_STRIDE);
#undef REG_MAC_STRIDE
#endif
#ifdef REG_MAC_FP
NNRW_REG_CONST(M, MAC_FP);
#undef REG_MAC_FP
#endif
#ifdef REG_MAC_BIT
NNRW_REG_CONST(M, MAC_BIT);
#undef REG_MAC_BIT
#endif
#ifdef REG_FP_NUM0
NNRW_REG_CONST(M, FP_NUM0);
#undef REG_FP_NUM0
#endif
#ifdef REG_FP_NUM1
NNRW_REG_CONST(M, FP_NUM1);
#undef REG_FP_NUM1
#endif
#ifdef REG_MAC_W_MASK
NNRW_REG_CONST(M, MAC_W_MASK);
#undef REG_MAC_W_MASK
#endif
#ifdef REG_MAC_P_MASK
NNRW_REG_CONST(M, MAC_P_MASK);
#undef REG_MAC_P_MASK
#endif

#ifdef REG_BTR_WADDR
NNRW_REG_CONST(D, BTR_WADDR);
#undef REG_BTR_WADDR
#endif
#ifdef REG_BTR_RADDR
NNRW_REG_CONST(D, BTR_RADDR);
#undef REG_BTR_RADDR
#endif
#ifdef REG_MSF_MODE
NNRW_REG_CONST(D, MSF_MODE);
#undef REG_MSF_MODE
#endif
#ifdef REG_Q_BIT
NNRW_REG_CONST(D, Q_BIT);
#undef REG_Q_BIT
#endif
#ifdef REG_QD_CONFIG
NNRW_REG_CONST(D, QD_CONFIG);
#undef REG_QD_CONFIG
#endif
#ifdef REG_RDND_NUM
NNRW_REG_CONST(D, RDND_NUM);
#undef REG_RDND_NUM
#endif
#ifdef REG_OC_LSH
NNRW_REG_CONST(D, OC_LSH);
#undef REG_OC_LSH
#endif
#ifdef REG_PIXEL_OUT0
NNRW_REG_CONST(D, PIXEL_OUT0);
#undef REG_PIXEL_OUT0
#endif
#ifdef REG_PIXEL_OUT1
NNRW_REG_CONST(D, PIXEL_OUT1);
#undef REG_PIXEL_OUT1
#endif
#ifdef REG_FR_WADDR0
NNRW_REG_CONST(D, FR_WADDR0);
#undef REG_FR_WADDR0
#endif
#ifdef REG_FR_WADDR1
NNRW_REG_CONST(D, FR_WADDR1);
#undef REG_FR_WADDR1
#endif
#ifdef REG_FR_CONFIG
NNRW_REG_CONST(D, FR_CONFIG);
#undef REG_FR_CONFIG
#endif
#ifdef REG_F_BIT
NNRW_REG_CONST(D, F_BIT);
#undef REG_F_BIT
#endif
#ifdef REG_WFCW_PN
NNRW_REG_CONST(D, WFCW_PN);
#undef REG_WFCW_PN
#endif

#undef NNRW_REG_CONST
#undef NNRW_VWR
