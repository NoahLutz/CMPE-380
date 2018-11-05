CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -g -O0 -lm

HW5_DEPS=hw5.c eqn2solve.c rootfinding.h rootfinding.c

BINARIES=hw5

.PHONY: all
all: $(BINARIES)


hw5: $(HW5_DEPS)
	$(CC) $(CFLAGS) $(HW5_DEPS) -o hw5


.PHONY: clean
clean:
	$(RM) $(BINARIES)
