#Compiler settings
CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -O0 -g -DHW8 -lm
CFLAGS_GSL=-lgsl -lgslcblas

#valgrind settings
VAL=valgrind
VAL_FLAGS=--tool=memcheck --leak-check=full --track-origins=yes

#Verbose variables
#VERBOSE=-v
VERBOSE=

#Dependencies for binaries
HW8_DEPS=hw8.c DynamicArrays.h DynamicArrays.c ClassErrors.h

#Clean files
OUTFILES=out.txt mem.txt
BINARIES=hw8

.PHONY: clean
all: $(BINARIES)

hw8: $(HW8_DEPS)
	$(CC) $(CFLAGS) $(CFLAGS_GSL) $(HW8_DEPS) -o hw8


.PHONY: test
test: hw8
	@cat out.txt

.PHONY: mem
mem: hw8
	$(VAL) $(VAL_FLAGS) ./hw7 -i ge2.txt > mem.txt 2>&1
	@cat mem.txt

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

