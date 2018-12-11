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

#Debug variables
#MYPLOT_DEBUG=-DDEBUG
MYPLOT_DEBUG=

#Dependencies for binaries
HW8_DEPS=hw8.c DynamicArrays.h DynamicArrays.c ClassErrors.h
CORRECTION_DEPS=correction.c
MYPLOT_DEPS=myplot.c

#Clean files
OUTFILES=realdevice.txt correction.txt alldata.txt alldata.png mem.txt
BINARIES=hw8 correction myplot

.PHONY: clean
all: $(BINARIES)

hw8: $(HW8_DEPS)
	$(CC) $(CFLAGS) $(CFLAGS_GSL) $(HW8_DEPS) -o hw8

correction: $(CORRECTION_DEPS)
	$(CC) $(CFLAGS) $(CORRECTION_DEPS) -o correction

myplot: $(MYPLOT_DEPS)
	$(CC) $(CFLAGS) $(MYPLOT_DEPS) $(MYPLOT_DEBUG) -o myplot

.PHONY: base
base: 
	./realDevice | ./detError

.PHONY: correct
correct: correction
	./realDevice | ./correction | ./detError

.PHONY: plot
plot: myplot
	./realDevice > realdevice.txt
	./realDevice | ./correction > correction.txt
	paste realdevice.txt correction.txt > alldata.txt
	./myplot -i alldata.txt -o alldata.png

.PHONY: mem
mem: hw8
	$(VAL) $(VAL_FLAGS) ./hw8 -n -o 3 -p data.txt > mem.txt 2>&1
	@cat mem.txt

.PHONY: test
test: hw8
	@cat out.txt

.PHONY: help
help:
	@echo -e "Makefile usage:\n\
		all: build all binaries\n\
		hw8: build just hw8 binary\n\
		correction: build corretion.c\n\
		myplot: build myplot.c\n\
		base: generate device output and view error\n\
		correct: generate device output and pipe through correction and view error\n\
		plot: using real device data and corrected device data, plot curves using gnuplot\n\
		mem: run hw8 with valgrind \n\
		clean: Clean all binarys and temporary files\n"

.PHONY: clean
clean:
	$(RM) $(BINARIES) $(OUTFILES)

