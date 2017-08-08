vpath %.h = inc/
vpath %.c = src/

SRC += $(wildcard *.c)
SRC += $(wildcard src/*.c)

CC = arm-none-linux-gnueabi-gcc

CPPFLAGS += -I /usr/local/jpeg/include
CPPFLAGS += -I ./inc

LDFLAGS += -L /usr/local/jpeg/lib
LDFLAGS += -ljpeg
LDFLAGS += -lpthread

test:$(SRC)
	$(CC) $^ -o $@ $(CPPFLAGS) $(LDFLAGS)
	mv test /mnt/hgfs/arm/piano

clean:
	$(RM) test
