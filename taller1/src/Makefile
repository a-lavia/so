CFLAGS= -Wall -O0 -g -ggdb
NASMFLAGS= -g -ggdb
LDFLAGS=-static
CC=gcc

BINS=launch haiclone

OBJS=$(BINS:=.o)

all: $(BINS)

# Dependencias
launch.o: launch.c
haiclone.o: haiclone.c

clean:
	rm -rf $(OBJS) $(BINS)
