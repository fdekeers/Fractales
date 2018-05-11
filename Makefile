export CC = gcc
export CFLAGS = -g -Wall -W -lpthread -lSDL -I/usr/include/SDL -I libfractal -std=c99
LIB_DIR = libfractal
TEST_DIR = tests
LIB = ./libfractal/libfractal.a
EXE = main

main: main.o lib
	@$(CC) -o $@ $< $(LIB) -lpthread -lSDL

main.o: main.c lib
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: lib clean tests

lib:
	@(cd $(LIB_DIR) && $(MAKE))

clean:
	@(rm -rf *.o $(EXE) *.bmp && cd $(LIB_DIR) && rm -rf *.o *.a && cd ../tests && rm -rf *.o test)
	
tests:
	@(cd $(TEST_DIR) && $(MAKE))