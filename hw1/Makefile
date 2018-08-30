CC=gcc
VALGRIND=valgrind
ECHO=echo -e
VALGRIND_FLAGS=--tool=memcheck --leak-check=yes --track-origins=yes
QS_EXEC=qs
VAL_EXEC=val
TEST_OUT=out.txt
MEM_OUT=mem.txt

all: QuadraticSolver.c val.c
	$(CC) -o $(QS_EXEC) QuadraticSolver.c -lm
	$(CC) -o $(VAL_EXEC) val.c

test: all
	./$(QS_EXEC) 2 2392.2 -766267.2 > $(TEST_OUT)
	./$(QS_EXEC) 1 -1 1 >> $(TEST_OUT)
	./$(QS_EXEC) 0 1 -10 >> $(TEST_OUT)
	cat $(TEST_OUT)

mem1: all
	$(VALGRIND) $(VALGRIND_FLAGS) ./$(QS_EXEC) 2 2392.2 -766267.2 > $(MEM_OUT) 2>&1
	cat $(MEM_OUT)

mem2: all
	$(VALGRIND) ./$(VAL_EXEC) > $(MEM_OUT) 2>&1
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
	$(RM) $(QS_EXEC) $(VAL_EXEC) $(TEST_OUT) $(MEM_OUT)
