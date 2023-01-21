# Makefile for PT and S/SL 
# Usage:  make 

all:
	cd cmd; make $(ACTION)
	cd parser; make $(ACTION)
	cd semantic; make $(ACTION)
	cd coder; make $(ACTION)
	cd ptruntime; make $(ACTION)
	cd ptam; make $(ACTION)

scanner:
	cd cmd; make $(ACTION)
	cd parser; make scanner

install: 
	make all ACTION=install

clean:
	make all ACTION=clean
	/bin/rm -f bin/* lib/pt/*
