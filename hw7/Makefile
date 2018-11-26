CC=gcc
CFLAGS=-Wall -pedantic -O0 -g

HW7_DEPS=hw7.c apmatrix.h apmatrix.c ClassErrors.h

BINARIES=hw7

.PHONY: clean
all: $(BINARIES)

hw7: $(HW7_DEPS)
	$(CC) $(CFLAGS) $(HW7_DEPS) -o hw7


.PHONY: clean
clean:
	$(RM) $(BINARIES)
