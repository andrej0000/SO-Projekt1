CC=gcc
CCFLAGS = -Wall

all: executor manager

shared.o: shared.c shared.h
	$(CC) $(CCFLAGS) shared.c -c -o shared.o

err.o: err.h err.c
	$(CC) $(CCFLAGS) err.c -c -o err.o

executor: executor.c shared.o err.o
	$(CC) $(CCFLAGS) err.o shared.o executor.c -o executor

manager: manager.c shared.o err.o
	$(CC) $(CCFLAGS) manager.c err.o shared.o -o manager

clean:
	rm -rf *.o manager executor
