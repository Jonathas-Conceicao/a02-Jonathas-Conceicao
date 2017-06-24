CC=gcc

CFLAGS=-g -O1 -std=c99 -pthread -Wall -Wextra -Werror

all: test grade

memvirt.o:
	$(CC) $(CFLAGS) -c memvirt.c -o memvirt.o -lm

teste: memvirt.o teste.c
	$(CC) $(CFLAGS) teste.c -o teste -lm

proftest: memvirt.o proftest.c


test: teste
	./teste

grade: proftest
	./proftest

clean:
	rm -rf *.o teste proftest
