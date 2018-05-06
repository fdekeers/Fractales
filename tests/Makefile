test: test.o
	@gcc -o test test.o

test.o: test.c
	@gcc -o test.o -c test.c
	
.PHONY : clean

clean:
	@rm -rf *.o test