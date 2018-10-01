CC=gcc
CFLAGS=-Wall -g

VALGRIND=valgrind
VALGRIND_FLAGS=--tool=memcheck --leak-check=yes --track-origins=yes

ECHO=echo -e

all: TestDArray

TestDArray: TestDarray_hw.c DynamicArrays.c DynamicArrays.h
	$(CC) -o TestDArray TestDarray_hw.c DynamicArrays.c DynamicArrays.h $(CFLAGS)

.PHONY: test
test:
	./TestDArray us-eng-words.txt > out.txt 2>&1
	cat out.txt

.PHONY: mem
mem:
	$(VALGRIND) $(VALGRIND_FLAGS) ./TestDArray us-eng-words.txt > mem.txt 2>&1
	cat mem.txt

.PHONY: clean
clean: 
	$(RM) TestDArray out.txt mem.txt

.PHONY: help
help:
	@$(ECHO) 'Makefile targets:'
	@$(ECHO) '\tall: build DynamicArrays.c and TestDArray.c'
	@$(ECHO) '\ttest: run TestDArray'
	@$(ECHO) '\tmem: run TestDArray through valgrind'
	@$(ECHO) '\thelp: display this dialog'
	@$(ECHO) '\tclean: remove all temporary and intermediate files'

