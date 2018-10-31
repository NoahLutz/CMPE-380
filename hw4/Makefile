CC=gcc
CFLAGS=-Wall -std=c99 -pedantic
DEBUG=-g -DMDEBUG

TESTLIST_DEP=TestList.c ClassErrors.h LinkedLists.h LinkedLists.c
TESTSEARCH_DEP=TestSearch.c ClassErrors.h LinkedLists.h LinkedLists.c

all: TestList TestSearch


TestList: $(TESTLIST_DEP)
	$(CC) $(CFLAGS) $(TESTLIST_DEP) -o TestList

TestSearch: $(TESTSEARCH_DEP)
	$(CC) $(CFLAGS) $(TESTSEARCH_DEP) -o TestSearch


.PHONY: clean
clean:
	$(RM) TestList TestSearch
