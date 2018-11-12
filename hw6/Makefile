CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -lm -O0 -g

HW6_DEPS=hw6.c poly.h poly.c ClassErrors.h

BINARIES=hw6
OUTFILES=

.PHONY: all
all: $(BINARIES)

hw6: $(HW6_DEPS)
	$(CC) $(CFLAGS) $(HW6_DEPS) -o hw6

.PHONY: clean
clean:
	$(RM) $(BINARIES) $(OUTFILES)
