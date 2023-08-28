
# CC = mips-linux-gnu-g++
# C++ = mips-linux-gnu-g++
CC = mips-linux-uclibc-g++ 
#mips-linux-uclibc-gnu-gcc
C++ = mips-linux-uclibc-gcc

# LIBS = ./nna/glibc/libnna.a
#LIBS =  ../../src/uclibc/libdrivers.a ../../src/uclibc/libaip.a
LIBS_AIP =  ../../__release_aip/lib/uclibc/ 
LIBS_DRI = ../model/libdrivers/uclibc/


#src := $(wildcard ./sample_aip.c ./src/*.c)
#objs += $(patsubst %.c, %.o, $(src))
model := $(wildcard ../../model/*.c)
objs += $(patsubst %.c, %.o, $(model))
objs += ../model/bs_src.o ../model/include/3rd_party/Matrix.o

INCLUDE_COMMON = -I ../../include/jz_aip -I ../model/include/model/ -I ../model/include/3rd_party/ -I ../model/include/drivers/

OUTPUT_NAME := aip_covent_test

$(OUTPUT_NAME): aip_covent_test.o
	$(CC) -o $@ $^ -L$(LIBS_AIP) -laip -L$(LIBS_DRI) -ldrivers $(INCLUDE_COMMON) -lpthread
%.o:%.c
	$(CC) -c -o $@ $< $(INCLUDE_COMMON)
%.o:%.cc
	$(C++) -c -o $@ $< $(INCLUDE_COMMON)

.PHONY: clean
clean:
	rm $(objs) $(OUTPUT_NAME) $(OUTPUT_NAME).o
