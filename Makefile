CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g
LIBS=-lm

test: bitpack.o test.o
	$(CC) $(LIBS) -o test bitpack.o test.o
	./test

bitpack.o: bitpack.h bitpack.c
	$(CC) $(CFLAGS) -c -o bitpack.o bitpack.c

test.o: test.c
	$(CC) $(CFLAGS) -c -o test.o test.c

clean:
	rm -f bitpack.o test.o test
