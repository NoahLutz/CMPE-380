CC=gcc
QS_EXEC=qs
VAL_EXEC=val

all: QuadraticSolver.c val.c
	$(CC) -o $(QS_EXEC) QuadraticSolver.c -lm
	$(CC) -o $(VAL_EXEC) val.c

test: all
	./$(QS_EXEC) 2 2392.2 -766267.2 > out.txt
	./$(QS_EXEC) 1 -1 1 >> out.txt
	./$(QS_EXEC) 0 1 -10 >> out.txt
	cat out.txt
