CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -O0

CPP=g++
CPPFLAGS=-Wall -pedantic

IN_FILE=data.txt
OUT_FILE=out.txt

all: hw3c_c hw3c_cpp hw3opt

hw3c_c: hw3c.c
	$(CC) $(CFLAGS) hw3c.c -o hw3c_c

hw3c_cpp: hw3cpp.cpp
	$(CPP) $(CPPFLAGS) hw3cpp.cpp -o hw3c_cpp

hw3opt: hw3opt.c
	$(CC) $(CFLAGS) hw3opt.c -o hw3opt

.PHONY: test
test: 
	$(CC) $(CFLAGS) hw3c.c -o hw3c_c -DEN_TIME
	$(CPP) $(CPPFLAGS) -DEN_TIME hw3cpp.cpp -o hw3c_cpp
	$(CC) $(CFLAGS) -Ofast hw3opt.c -o hw3opt -DEN_TIME
	@echo -e 'hw3c_c:\n' > $(OUT_FILE)
	./hw3c_c $(IN_FILE) >> $(OUT_FILE) 2>&1
	@echo -e 'hw3c_cpp:\n' >> $(OUT_FILE)
	./hw3c_cpp $(IN_FILE) >> $(OUT_FILE) 2>&1
	@echo -e 'hw3opt:\n' >> $(OUT_FILE)
	./hw3opt $(IN_FILE) >> $(OUT_FILE) 2>&1
	
.PHONY: clean
clean:
	$(RM) $(OUT_FILE) hw3c_c hw3c_cpp
