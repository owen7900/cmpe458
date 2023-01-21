# Makefile for ptc command

all: install 

ptc:	ptc.c
	cc -w -o ptc ptc.c

install: ptc pti
	/bin/cp ptc ../bin
	/bin/cp pti ../bin

clean:
	/bin/rm -f ptc *.o *.out
