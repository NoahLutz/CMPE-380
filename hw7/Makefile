#Compiler settings
CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -O0 -g
CFLAGS_GSL=-lgsl -lgslcblas

#valgrind settings
VAL=valgrind
VAL_FLAGS=--tool=memcheck --leak-check=full --track-origins=yes

#Verbose variables
#VERBOSE=-v
VERBOSE=

#Dependencies for binaries
HW7_DEPS=hw7.c apmatrix.h apmatrix.c ClassErrors.h
GSL_DEPS=gsl_test.c ClassErrors.h

#Clean files
OUTFILES=out.txt mem.txt
BINARIES=hw7 gsl_test

.PHONY: clean
all: $(BINARIES)

hw7: $(HW7_DEPS)
	$(CC) $(CFLAGS) $(HW7_DEPS) -o hw7

.PHONY: hw7_time
hw7_time: $(HW7_DEPS)
	$(CC) $(CFLAGS) -DEN_TIME $(HW7_DEPS) -o hw7

gsl_test: $(GSL_DEPS)
	$(CC) $(CFLAGS) $(CFLAGS_GSL) $(GSL_DEPS) -o gsl_test

.PHONY: gsl_test_time
gsl_test_time: $(GSL_DEPS)
	$(CC) $(CFLAGS) $(CFLAGS_GSL) -DEN_TIME $(GSL_DEPS) -o gsl_test

.PHONY: test
test: hw7
	./hw7 $(VERBOSE) -i ge1.txt > out.txt 2>&1
	./hw7 $(VERBOSE) -i ge2.txt >> out.txt 2>&1
	-./hw7 $(VERBOSE) -i ge3.txt >> out.txt 2>&1
	./hw7 $(VERBOSE) -i ge4.txt >> out.txt 2>&1
	@cat out.txt

.PHONY: gsl
gsl: gsl_test
	./gsl_test $(VERBOSE) -i ge1.txt > out.txt 2>&1
	./gsl_test $(VERBOSE) -i ge2.txt >> out.txt 2>&1
	-./gsl_test $(VERBOSE) -i ge3.txt >> out.txt 2>&1
	./gsl_test $(VERBOSE) -i ge4.txt >> out.txt 2>&1
	@cat out.txt

.PHONY: mem
mem: hw7
	$(VAL) $(VAL_FLAGS) ./hw7 -i ge2.txt > mem.txt 2>&1
	@cat mem.txt

.PHONY: time
time: hw7_time gsl_test_time
	./hw7 $(VERBOSE) -i rand.txt > out.txt 2>&1
	./gsl_test $(VERBOSE) -i rand.txt >> out.txt 2>&1
	@cat out.txt

.PHONY: help
help:
	@echo -e "Makefile usage:\n\
		all: build all binaries\n\
		hw7: build just hw7 binary\n\
		hw7_time: build hw7 with timing macros enabled\n\
		gsl_test: build just gsl_test binary\n\
		gsl_test_time: build gsl_test with timing macros enabled\n\
		test: run hw7 with all input files\n\
		gsl: run gsl_test with all input files\n\
		mem: run hw7 with valgrind using ge2.txt\n\
		time: run hw7 and gsl_test with timing macros enabled\n"

.PHONY: clean
clean:
	$(RM) $(BINARIES) $(OUTFILES)

