#
#        (C) COPYRIGHT Ingenic Limited.
#             ALL RIGHTS RESERVED
# 
# File       :  Makefile
# Authors    :  jin.zhang@ingenic.com
# Create Time:  2019-12-04:10:24:00 
# Description:
#  NNA lib makefile at ITE
# 
# make OPT=I
# make OPT=V
# make OPT=IV
ifeq ($(OPT),I)
GCC_EXT_OPT=-D__AIE_INFO__ 
endif
ifeq ($(OPT),V)
GCC_EXT_OPT=-D__AIE_VALID_CHECK__ 
endif
ifeq ($(OPT),IV)
GCC_EXT_OPT=-D__AIE_INFO__ -D__AIE_VALID_CHECK__ 
endif

LIB_NAME=libnna
#
# toolchain config
#
OPT_LEVEL=-O2

# if with "-mmsa", will generate the instruction LSA
GCC=/tools/mips-gcc720-glibc226/bin/mips-linux-gnu-gcc
OBJDUMP=mips-linux-gnu-objdump
AR=mips-linux-gnu-ar

#
# base options
#
#CSE_CFLAGS=-DUSE_SIM
# GCC_EXT_OPT=-mmsa
GCC_OPT=$(OPT_LEVEL) $(CSE_CFLAGS) $(GCC_EXT_OPT) -EL -mips32r2 -mfp64 -mabs=2008 -mnan=2008 -static \
	-I../include 

#
# target
#
C_SOURCE=$(wildcard *.c)
C_TARGET_OBJ=$(patsubst %.c, %.o, $(C_SOURCE))
S_SOURCE=$(wildcard *.S)
S_TARGET_OBJ=$(patsubst %.S, %.o, $(S_SOURCE))

.PHONY:all

all: $(C_TARGET_OBJ) $(S_TARGET_OBJ)
	$(AR) rcs $(LIB_NAME).a $^
	cp -f -p $(LIB_NAME).a ../lib

%.o: %.S
	$(GCC) -c -o $@ $^ $(GCC_OPT) $(GCC_EXT_OPT)

%.o: %.c
	$(GCC) -c -o $@ $^ $(GCC_OPT) $(GCC_EXT_OPT)

.PHONY:dump
dump:
	$(OBJDUMP) -D $(LIB_NAME).a >| $(LIB_NAME).dump

.PHONY:clean
clean:
	rm -f $(LIB_NAME).a *.o
