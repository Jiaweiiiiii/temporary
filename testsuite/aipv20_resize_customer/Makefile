# aipv20_resize Makefile


CC = mips-linux-gnu-gcc
CFLAGS = -Wall -I../../libaip/__release/include
#CFLAGS = -Wall -I../../__release0710_aipv20/include

USE_UCLIBC ?= 1
ifeq ($(USE_UCLIBC),1)
	LIBDIR = ../../libaip/__release/lib/uclibc -muclibc
	#LIBDIR = ../../__release0710_aipv20/lib/uclibc -muclibc
else
	LIBDIR = ../../libaip/__release/lib/glibc
	#LIBDIR = ../../__release0710_aipv20/lib/glibc
endif

LDFLAGS = -L$(LIBDIR) -laip_v20 -lpthread
USE_STATIC ?= 1
ifeq ($(USE_STATIC),1)
	LDFLAGS += -static
endif


SRC = aipv20_resize
TARGET = $(SRC).exe
SOURCES = $(SRC).c
OBJ = $(SOURCES:.c=.o)
OBJ += aipv20_resize_lib.o

all:$(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^  -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJ)

