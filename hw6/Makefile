CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -lm

VALGRIND=valgrind
VALGRIND_FLAGS=--tool=memcheck --leak-check=yes --track-origins=yes

HW6_DEPS=hw6.c poly.h poly.c ClassErrors.h

INFILE=polynomials.txt

BINARIES=hw6
OUTFILES=out.txt mem.txt

.PHONY: all
all: $(BINARIES)

hw6: $(HW6_DEPS)
	$(CC) $(CFLAGS) $(HW6_DEPS) -o hw6

.PHONY: roots
roots: $(BINARIES)
	./hw6 -input $(INFILE) -verbose > out.txt 2>&1
	@cat out.txt

.PHONY: mem
mem: $(BINARIES)
	$(VALGRIND) $(VALGRIND_FLAGS) ./hw6 -i $(INFILE) > mem.txt 2>&1
	@cat mem.txt

.PHONY: clean
clean:
	$(RM) $(BINARIES) $(OUTFILES)
