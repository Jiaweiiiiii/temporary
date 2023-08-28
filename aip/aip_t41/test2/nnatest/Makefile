#----------------------------------
#-----------aip_v20_test-----------
#----------------------------------

CC = mips-linux-uclibc-gcc
C++ = mips-linux-uclibc-g++

CFLAGS = -Wall -O0

#LIBS_AIP =  ../../__release_aip/lib/uclibc/ 
#LIBS_DRI = ../model/libdrivers/uclibc/

#INCLUDE_COMMON = -I ../model/include/drivers/

OUTPUT_NAME := v20test
$(OUTPUT_NAME): $(OUTPUT_NAME).o
	$(CC) $(CFLAGS) -o $@ $^ -L$(LIBS_AIP) -laip -L$(LIBS_DRI) -ldrivers $(INCLUDE_COMMON) -lpthread
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE_COMMON)
%.o:%.cc
	$(C++) $(CFLAGS) -c $< -o $@ $(INCLUDE_COMMON)

.PHONY: clean
clean:
	rm -f $(OUTPUT_NAME) $(OUTPUT_NAME).o

