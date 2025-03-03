# Makefile for pgm-image-rotation project

CC = /opt/homebrew/bin/gcc-14  # Use GCC 14 from Homebrew
CFLAGS = -O0 -fopenmp -Wall
LDFLAGS = -fopenmp

CFILES = matrix_rotation.c
OBJS = $(CFILES:.c=.o)
TARGET = matrix_rotate

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $(TARGET) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	-rm -f *.o *.d $(TARGET)

distclean: clean
