CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -g -O0 -lm

HW5_DEPS=hw5.c eqn2solve.c ClassErrors.h rootfinding.h rootfinding.c
INTTEST_DEPS=intTest.c ClassErrors.h rootfinding.h rootfinding.c

BINARIES=hw5 intTest
OUT_FILES=out.txt

.PHONY: all
all: $(BINARIES)


hw5: $(HW5_DEPS)
	$(CC) $(CFLAGS) $(HW5_DEPS) -o hw5

intTest: $(INTTEST_DEPS)
	$(CC) $(CFLAGS) $(INTTEST_DEPS) -o intTest

.PHONY: hw5_time
hw5_time: $(HW5_DEPS)
	$(CC) $(CFLAGS) -DEN_TIME $(HW5_DEPS) -o hw5

.PHONY: intTest_time
intTest_time: $(INTTEST_DEPS)
	$(CC) $(CFLAGS) -DEN_TIME $(INTTEST_DEPS) -o intTest

.PHONY: test
test: hw5_time intTest_time
	@echo 'Bisection:' > out.txt
	./hw5 -bisection -tol 1E-6 -ga -2.5 -gb 2.5 >> out.txt 2>&1
	@echo -e '\nSecant:' >> out.txt
	./hw5 -secant -tol 1E-6 -ga -1.5 -gb 1.5 >> out.txt 2>&1
	@echo -e '\nNewton:' >> out.txt
	./hw5 -newton -tol 1E-6 -ga -1.5 -gb 1.5 >> out.txt 2>&1
	@echo -e '\nInteger:' >> out.txt
	./intTest >> out.txt 2>&1
	@cat out.txt

.PHONY: bisection
bisection: hw5_time
	./hw5 -bisection -tol 1E-6 -ga -2.5 -gb 2.5 > out.txt 2>&1
	@cat out.txt

.PHONY: secant
secant: hw5_time
	./hw5 -secant -tol 1E-6 -ga -1.5 -gb 1.5 > out.txt 2>&1
	@cat out.txt

.PHONY:newton
newton: hw5_time
	./hw5 -newton -tol 1E-6 -ga -1.5 -gb 1.5 > out.txt 2>&1
	@cat out.txt

.PHONY:integer
integer: intTest_time
	./intTest > out.txt 2>&1
	@cat out.txt

.PHONY:opts
opts: clean hw5
	./hw5 -b -tol 1E-6 -ga -2 -gb 3 -verb > out.txt 2>&1
	./hw5 -b -t 1E-6 -g -2 -u 3 -v >> out.txt 2>&1
	./hw5 -b -t 1E-6 -ba -2 -bb 3 -v >> out.txt 2>&1
	@cat out.txt

.PHONY: clean
clean:
	$(RM) $(BINARIES) $(OUT_FILES)
