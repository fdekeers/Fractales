CC=gcc
AR=ar
CFLAGS=-g -Wall -W -lSDL -I/usr/include/SDL
LDFLAGS=
SRC=fractal.c tools.c
OBJ=$(SRC:.c=.o)
LIBFILE=libfractal.a

all: $(SRC) $(LIBFILE)

$(LIBFILE): $(OBJ)
	@$(AR) r $@ $(OBJ)

*.o:
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean

clean:
	@rm -rf *.o *.a
