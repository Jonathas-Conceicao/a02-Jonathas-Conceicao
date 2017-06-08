CC=gcc

CFLAGS=-g -O1 -std=c99 -pthread -lm

all: test grade 

memvirt.o: 

teste: memvirt.o teste.c

proftest: memvirt.o proftest.c


test: teste
	./teste

grade: proftest
	./proftest

clean:
	rm -rf *.o teste proftest
