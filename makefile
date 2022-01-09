alphanim: nim.o nimhelp.o alphanim.o
	clang nim.o nimhelp.o alphanim.o -o alphanim

all: nim.o

nim.o: nim.c
	clang -Wall -std=c99 -pedantic -c nim.c -o nim.o

nimhelp.o: nimhelp.c
	clang -Wall -std=c99 -pedantic -c nimhelp.c -o nimhelp.o

alphanim.o: alphanim.c
	clang -Wall -std=c99 -pedantic -c alphanim.c -o alphanim.o

clean:
	rm *.o alphanim

