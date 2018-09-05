CC=gcc
VALGRIND=valgrind
ECHO=echo -e
VALGRIND_FLAGS=--tool=memcheck --leak-check=yes --track-origins=yes
TEST_OUT=out.txt
MEM_OUT=mem.txt

all: qs val

qs: QuadraticSolver.c
	$(CC) -o qs QuadraticSolver.c -lm

val: val.c
	$(CC) -o val val.c

test: all
	./qs 2 2392.2 -766267.2 > $(TEST_OUT)
	./qs 1 -1 1 >> $(TEST_OUT)
	./qs 0 1 -10 >> $(TEST_OUT)
	cat $(TEST_OUT)

mem1: all
	$(VALGRIND) $(VALGRIND_FLAGS) ./qs 2 2392.2 -766267.2 > $(MEM_OUT) 2>&1
	cat $(MEM_OUT)

mem2: all
	$(VALGRIND) ./val > $(MEM_OUT) 2>&1
	cat $(MEM_OUT)

.PHONY: help
help:
	@$(ECHO)	'Makefile targets:'
	@$(ECHO) '\tall: build QuadraticSolver.c and val.c'
	@$(ECHO) '\ttest: run QuadraticSolver.c with three different polynomial equations'
	@$(ECHO) '\tmem1: run QuadraticSolver.c through valgrind'
	@$(ECHO) '\tmem2: run val.c through valgrind'
	@$(ECHO) '\thelp: display this dialog'
	@$(ECHO) '\tclean: remove all temporary and intermediate files'

.PHONY: clean
clean:
	$(RM) qs val $(TEST_OUT) $(MEM_OUT)
