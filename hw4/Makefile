CC=gcc
CFLAGS=-Wall -std=c99 -pedantic
DEBUG=-g -DMDEBUG

TESTLIST_DEP=TestList.c ClassErrors.h LinkedLists.h LinkedLists.c

all: TestList


TestList: $(TESTLIST_DEP)
	$(CC) $(CFLAGS) $(TESTLIST_DEP) -o TestList

