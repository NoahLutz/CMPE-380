CC=gcc
CFLAGS=-Wall -std=c99 -pedantic
DEBUG=-g -DMDEBUG
VALGRIND=valgrind
VAL_FLAGS=

TESTLIST_DEP=TestList.c ClassErrors.h LinkedLists.h LinkedLists.c
TESTSEARCH_DEP=TestSearch.c ClassErrors.h LinkedLists.h LinkedLists.c
TESTTIME_DEP=TestTime.c ClassErrors.h Timers.h LinkedLists.h LinkedLists.c DynamicArrays.h DynamicArrays.c

BINARIES=TestList TestSearch TestTime

IN_FILE=us-eng-words.txt
SEARCHWORD=space

TEST_OUT=out.txt
SEARCH_OUT=search.txt
TIME_OUT=time.txt
MEM_OUT=mem.txt
OUTFILES=$(TEST_OUT) $(SEARCH_OUT) $(TIME_OUT) $(MEM_OUT)


all: $(BINARIES)

TestList: $(TESTLIST_DEP)
	$(CC) $(CFLAGS) $(TESTLIST_DEP) -o TestList

TestSearch: $(TESTSEARCH_DEP)
	$(CC) $(CFLAGS) $(TESTSEARCH_DEP) -o TestSearch

TestTime: $(TESTTIME_DEP)
	$(CC) $(CFLAGS) -DEN_TIME $(TESTTIME_DEP) -o TestTime

.PHONY: test
test: TestList
	./TestList $(IN_FILE) > $(TEST_OUT) 2>&1
	@cat $(TEST_OUT)

.PHONY: search
search: TestSearch
	./TestSearch $(IN_FILE) $(SEARCHWORD) > $(SEARCH_OUT) 2>&1
	@cat $(SEARCH_OUT)

.PHONY: time
time: TestTime
	./TestTime $(IN_FILE) > $(TIME_OUT) 2>&1
	@cat $(TIME_OUT)

.PHONY: mem
mem: TestList
	$(VALGRIND) $(VAL_FLAGS) ./TestList $(IN_FILE) > $(MEM_OUT) 2>&1
	@cat $(MEM_OUT)

.PHONY: clean
clean:
	$(RM) $(BINARIES) $(OUTFILES)
