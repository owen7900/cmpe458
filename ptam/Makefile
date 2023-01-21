# Makefile for PT Abstract Machine Version 5.0
# Usage:  make [ptam]

# Revised v5.0 - 10 Dec 2020

PASS = ptam
SUFFIXES = .ssl .sst .pt .out
.SUFFIXES: ${SUFFIXES}

all:  ptam

install:  ptam

ptam.out:  ptam.pt ptam.sst ptam.consistency
	@echo "ptc ptam.pt"
	@ptc ptam.pt > ptam.err
	@cat ptam.err
	-@(grep -v Warning ptam.err > ptam.err.nowarn || true)
	@if (! cmp -s ptam.err.nowarn /dev/null) ; \
	then \
	    rm -f ptam.err ptam.err.nowarn  ; \
	    exit 1 ; \
	else \
	    rm -f ptam.err ptam.err.nowarn  ; \
	fi


# S/SL make procedure and dependency analysis

.ssl.sst:  
	@echo "ssl $*.ssl"
	@ssl $*.ssl > $*.err
	@cat $*.err
	-@(grep -v Warning $*.err > $*.err.nowarn || true)
	@if (! cmp -s $*.err.nowarn /dev/null) ; \
	then \
	    rm -f $*.err $*.err.nowarn  ; \
	    rm -f $*.def $*.sst ; \
	    exit 2 ; \
	else \
	    rm -f $*.err $*.err.nowarn  ; \
	fi 

ptam.consistency: ptam.sst ptam.pt
	@ # ptam/semantic T-codes
	@inconsistent=0 ; \
	defcounts=`cat ../semantic/semantic.def ptam.def | grep -e "tMultiply =" -e "tSubtract =" -e "tVarParm =" -e "tSubscriptBegin =" -e "tTrapBegin =" -e "tLiteralInteger =" -e "tTrap =" | expand | sort -u | wc -l` ; \
	if [ ! $${defcounts} -eq  7 ] ; \
	then \
	    inconsistent=1 ; \
	fi ; \
	if [ ! $${inconsistent} -eq 0 ] ; \
	then \
	    echo "ERROR: Semantic/ptam T-code token definitions inconsistent. Update ptam.ssl input tokens to match semantic.ssl output tokens." ; \
	    exit 3 ; \
	fi
	@ # ptam tokens
	@inconsistent=0 ; \
	defcounts=`cat ptam.pt ptam.def | grep -e "oMemoryLoadCode =" -e "oALUpop =" -e "oALUpushNotR =" -e "oCUchooseInstruction =" -e "oCUincPC =" -e "oTrapHandlerTrap =" -e "firstTcode =" -e "tSubtract =" -e "tAssignBegin =" -e "tTrap =" -e "lastTcode =" -e "trHalt =" -e "trAssign =" -e "trFileBind =" | expand | sort -u | wc -l` ; \
	if [ ! $${defcounts} -eq  14 ] ; \
	then \
	    inconsistent=1 ; \
	fi ; \
	if [ ! $${inconsistent} -eq 0 ] ; \
	then \
	    echo "ERROR: PTAM token definitions out of date. Update ptam.pt with new definitions from ptam.def." ; \
	    exit 4 ; \
	fi
	
ptam: ptam.out ptam.sst ptam.def 
	/bin/cp ptam.out ptam.sst ptam.def ../lib/pt

clean:
	/bin/rm -f ptam.err ptam.p ptam.out ptam.sst ptam.def 
