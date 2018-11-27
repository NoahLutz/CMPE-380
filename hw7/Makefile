#Compiler settings
CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -O0 -g
CFLAGS_GSL=-lgsl -lgslcblas

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

gsl_test: $(GSL_DEPS)
	$(CC) $(CFLAGS) $(CFLAGS_GSL) $(GSL_DEPS) -o gsl_test

.PHONY: test
test: hw7
	./hw7 $(VERBOSE) -i ge1.txt > out.txt 2>&1
	./hw7 $(VERBOSE) -i ge2.txt >> out.txt 2>&1
	./hw7 $(VERBOSE) -i ge3.txt >> out.txt 2>&1
	./hw7 $(VERBOSE) -i ge4.txt >> out.txt 2>&1
	@cat out.txt

.PHONY: gsl
gsl: gsl_test
	./gsl_test $(VERBOSE) -i ge1.txt > out.txt 2>&1
	./gsl_test $(VERBOSE) -i ge2.txt > out.txt 2>&1
	./gsl_test $(VERBOSE) -i ge3.txt > out.txt 2>&1
	./gsl_test $(VERBOSE) -i ge4.txt > out.txt 2>&1
	@cat out.txt

.PHONY: mem
mem: 
.PHONY: clean
clean:
	$(RM) $(BINARIES) $(OUTFILES)
