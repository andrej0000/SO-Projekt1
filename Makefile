CC=gcc
CCFLAGS = -Wall

all: executor manager

onpcalc.o: onpcalc.c onpcalc.h
	$(CC) $(CCFLAGS) onpcalc.c -c -o onpcalc.o

err.o: err.h err.c
	$(CC) $(CCFLAGS) err.c -c -o err.o

executor.o: executor.c err.o onpcalc.o
	$(CC) $(CCFLAGS) executor.c -c -o executor.o

executor: executor.o
	$(CC) $(CCFLAGS) executor.o err.o onpcalc.o -o executor

manager.o: manager.c err.o onpcalc.o
	$(CC) $(CCFLAGS) manager.c -c -o manager.o

manager: manager.o executor
	$(CC) $(CCFLAGS) manager.o err.o onpcalc.o -o manager
