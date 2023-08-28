# aipv20_nna Makefile

CC = mips-linux-gnu-gcc
INC_DIR = -I./common -I../../libaip/__release/include
CFLAGS = -EL -O3 -mfp64 -mnan=2008 -mabs=2008 -march=mips32r2 -fvisibility=hidden -flax-vector-conversions -Wall -Wextra 
LDFLAGS = -Wl,--wrap=memcpy,--wrap=memset,--wrap=strcpy -lmxu3
LDLIBS = -pthread -laip_v20

USE_UCLIBC ?= 1
ifeq ($(USE_UCLIBC),1)
	LIB_DIR = -L../../libaip/__release/lib/uclibc -muclibc
else
	LIB_DIR = -L../../libaip/__release/lib/glibc
endif

USE_STATIC ?= 1
ifeq ($(USE_STATIC),1)
	LDLIBS += -static
endif


SRC = nna
TARGET = $(SRC).exe
#SRCS = $(wildcard *.c)
SRCS = $(SRC).cc
OBJS = $(SRCS:.c=.o)
MXU = $(SRC).mxu3

all: $(TARGET)

$(TARGET): $(SRC).s
	$(CC) $(CFLAGS) $(LIB_DIR) -s $^ -o $@ $(LDLIBS)

%.s: %.mxu3
	python3 ./python/mxu3_asm.py $(MXU) >| $(SRC).s

%.mxu3: %.cc
	$(CC) $(CFLAGS) $(LDFLAGS) $(INC_DIR) -S -o $(MXU) $^

.PHONY: clean
clean:
	rm -f $(SRC).s $(SRC).mxu3 $(TARGET) 
