export CC = gcc
export CFLAGS = -g -Wall -W -lpthread -lSDL -I/usr/include/SDL -I ../libfractal
LIB_DIR = ../libfractal
LIB = ../libfractal/libfractal.a
EXE = main

test: test.o
	@gcc -o test test.o $(LIB) -lpthread -lSDL

test.o: test.c lib
	@gcc -o test.o -c test.c $(CFLAGS)
	
.PHONY : lib clean debug

debug: test.o
	@gcc -o test test.o $(LIB) -lpthread -lSDL -g

lib:
	@(cd $(LIB_DIR) && $(MAKE))

clean:
	@(rm -rf *.o test && cd $(LIB_DIR) && rm -rf *.o *.a)