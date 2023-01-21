# Makefile for the PT run-time library
# Usage:  make [ptruntime.o]

all:  install

ptruntime.o:	ptruntime.c
	gcc -m32 -w -c ptruntime.c

install: ptruntime.o
	/bin/cp ptruntime.o ../lib/pt

clean:
	/bin/rm -f ptruntime.o
