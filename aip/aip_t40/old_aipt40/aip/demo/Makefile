CONFIG_UCLIBC_BUILD=y
CUR_DIR=$(shell pwd)

CROSS_COMPILE ?= mips-linux-gnu-

CC = $(CROSS_COMPILE)gcc
CPLUSPLUS = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)ld
AR = $(CROSS_COMPILE)ar cr
STRIP = $(CROSS_COMPILE)strip

INCLUDE = -I$(CUR_DIR)/../include/3rd_party/ -I$(CUR_DIR)/../include/api/hal -I$(CUR_DIR)/../include/api -I$(CUR_DIR)/../include/model/ -I$(CUR_DIR)/../include/platform/ -I$(CUR_DIR)/../include/wrap/ -I/tmp/to_ATD/nna/include/
CFLAGS = $(INCLUDE) -O2 -Wall -march=mips32r2
LDFLAG = -Wall -Wl,-gc-sections -lpthread -lm -lrt -lstdc++ -std=c99 -D_GNU_SOURCE

ifeq ($(CONFIG_UCLIBC_BUILD), y)
CFLAGS += -muclibc
LDFLAG += -muclibc
endif

ifeq ($(CONFIG_UCLIBC_BUILD), y)
AIP_LIB_DIR = $(CUR_DIR)/../_install_uclibc/lib/api
MDL_LIB_DIR = $(CUR_DIR)/../_install_uclibc/lib/mdl
NNA_LIB_DIR = /tmp/to_ATD/nna/lib
endif

LIBS = $(AIP_LIB_DIR)/libbscaler_api.a $(MDL_LIB_DIR)/libbscaler_mdl.a \
	   $(NNA_LIB_DIR)/libnna.a


SAMPLES = resize_demo

all: $(SAMPLES)

resize_demo: resize_demo.o
	$(CPLUSPLUS) -o $@ $^ $(CFLAGS) $(LIBS) $(LDFLAG)
	$(STRIP) $@

resize_demo.o: resize_demo.cpp
	$(CPLUSPLUS) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o *~

distclean: clean
	rm -rf $(SAMPLES)


