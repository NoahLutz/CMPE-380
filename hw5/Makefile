CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -g -O0 -lm

HW5_DEPS=hw5.c eqn2solve.c rootfinding.h rootfinding.c

BINARIES=hw5
OUT_FILES=out.txt

.PHONY: all
all: $(BINARIES)


hw5: $(HW5_DEPS)
	$(CC) $(CFLAGS) $(HW5_DEPS) -o hw5

.PHONY: bisection
bisection:
	$(CC) $(CFLAGS) -DEN_TIME $(HW5_DEPS) -o hw5
	./hw5 -bisection -tol 1E-6 -ga -2.5 -gb 2.5 >> out.txt 2>&1
	@cat out.txt

.PHONY: clean
clean:
	$(RM) $(BINARIES) $(OUT_FILES)
