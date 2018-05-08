export CC = gcc
export CFLAGS = -g -Wall -W -lpthread -I/usr/include/SDL -L/usr/local/lib -lsdl -I libfractal
LIB_DIR = libfractal
LIB = ./libfractal/libfractal.a
EXE = main

main: main.o lib
	@$(CC) -o $@ $< $(LIB) -lpthread

main.o: main.c lib
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: lib clean tests

lib:
	@(cd $(LIB_DIR) && $(MAKE))

clean:
	@(rm -rf *.o $(EXE) && cd $(LIB_DIR) && rm -rf *.o *.a)
	
tests:
	//A completer
