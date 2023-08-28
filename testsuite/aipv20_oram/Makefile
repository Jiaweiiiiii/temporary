# aipv20_oram Makefile


CC = mips-linux-gnu-gcc
CFLAGS = -Wall -I../../libaip/__release/include

USE_UCLIBC ?= 1
ifeq ($(USE_UCLIBC),1)
	LIBDIR = ../../libaip/__release/lib/uclibc -muclibc
else
	LIBDIR = ../../libaip/__release/lib/glibc
endif

LDFLAGS = -L$(LIBDIR) -laip_v20 -lpthread
USE_STATIC ?= 1
ifeq ($(USE_STATIC),1)
	LDFLAGS += -static
endif


SRC = aipv20_oram
TARGET = $(SRC).exe
SOURCES = $(SRC).c
OBJ = $(SOURCES:.c=.o)

all:$(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^  -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJ)

