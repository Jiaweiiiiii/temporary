CC = mips-linux-uclibc-g++ 
C++ = mips-linux-uclibc-gcc

LIBS_AIP =  ../../../__release_aip/lib/uclibc 
LIBS_DIR =  ../../drivers_src/uclibc/


#src := $(wildcard ./sample_aip.c ./src/*.c)
#objs += $(patsubst %.c, %.o, $(src))
model := $(wildcard ../../model/*.c)
objs += $(patsubst %.c, %.o, $(model))
objs += ../../model/bs_src.o ../../model/3rd_party/Matrix.o

INCLUDE_COMMON = -I ../../../include/jz_aip -I ../../include/model/ -I ../../include/3rd_party -I ../../include/drivers/

OUTPUT_NAME := aip_resize_test

$(OUTPUT_NAME): $(objs) aip_resize_test.o
	$(CC) -o $@ $^ -L$(LIBS_AIP) -laip -L$(LIBS_DIR) -ldrivers  $(INCLUDE_COMMON) -lpthread
%.o:%.c
	$(CC) -c -o $@ $< $(INCLUDE_COMMON)
%.o:%.cc
	$(C++) -c -o $@ $< $(INCLUDE_COMMON)

.PHONY: clean
clean:
	rm $(objs) $(OUTPUT_NAME) $(OUTPUT_NAME).o
