# aipv20_resize Makefile


CC = mips-linux-gnu-gcc
#CC = mips-linux-uclibc-gcc
CFLAGS = -Wall -I../../libaip/__release/include
LIBDIR = ../../libaip/__release/lib/uclibc
LDFLAGS = -L$(LIBDIR) -laip_v20 -lpthread

USE_STATIC ?= 1
ifeq ($(USE_STATIC),1)
	LDFLAGS += -static
endif

USE_UCLIBC ?= 1
ifeq ($(USE_STATIC),1)
    LIBDIR += -muclibc
endif

SRC = aip_test
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

