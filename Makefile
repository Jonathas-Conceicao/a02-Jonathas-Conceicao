CC=gcc

CFLAGS=-g -O1 -std=c99 -Wall -Werrno
LDFLAGS=-lm 


all: grade 

memvirt.o: memvirt.c

test.o: test.c

test: memvirt.o test.o
	$(CC) $(CFLAGS) memvirt.o test.o -o test $(LDFLAGS)


grade: test
	./test

clean:
	rm -rf *.o test
