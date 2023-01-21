# Makefile for PT Coder pass Version 5.0
# Usage:  make [coder]

# Revised v5.0 - 10 Dec 2020

PASS = coder
SUFFIXES = .ssl .sst .pt .out
.SUFFIXES: ${SUFFIXES}

all:  coder

install:  coder

coder.out:  coder.pt coder.sst coder.consistency
	@echo "ptc coder.pt"
	@ptc coder.pt > coder.err
	@cat coder.err
	-@(grep -v Warning coder.err > coder.err.nowarn || true)
	@if (! cmp -s coder.err.nowarn /dev/null) ; \
	then \
	    rm -f coder.err coder.err.nowarn  ; \
	    exit 1 ; \
	else \
	    rm -f coder.err coder.err.nowarn  ; \
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

coder.consistency: coder.sst coder.pt
	@ # coder/semantic T-codes
	@inconsistent=0 ; \
	defcounts=`cat ../semantic/semantic.def coder.def | grep -e "tMultiply =" -e "tSubtract =" -e "tVarParm =" -e "tSubscriptBegin =" -e "tTrapBegin =" -e "tLiteralInteger =" -e "tTrap =" | expand | sort -u | wc -l` ; \
	if [ ! $${defcounts} -eq  7 ] ; \
	then \
	    inconsistent=1 ; \
	fi ; \
	if [ ! $${inconsistent} -eq 0 ] ; \
	then \
	    echo "ERROR: Semantic/coder T-code token definitions inconsistent. Update coder.ssl input tokens to match semantic.ssl output tokens." ; \
	    exit 3 ; \
	fi
	@ # coder tokens
	@inconsistent=0 ; \
	defcounts=`cat coder.pt coder.def | grep -e "oTempAllocate =" -e "oOperandPushTrap =" -e "oOperandPushCopy =" -e "oOperandAddManifestValues =" -e "oOperandSwap =" -e "oOperandPop =" -e "oFixPushAddress =" -e "oFixPopAddress =" -e "oEmitNone =" -e "oEmitSourceCoordinate =" -e "oCasePush =" -e "oOptionTestChecking =" -e "firstTcode =" -e "tSubtract =" -e "tAssignBegin =" -e "tTrap =" -e "lastTcode =" -e "trHalt =" -e "trAssign =" -e "mManifest =" -e "mWord =" -e "iAdd =" -e "iJmp =" -e "iSpace =" | expand | sort -u | wc -l` ; \
	if [ ! $${defcounts} -eq  24 ] ; \
	then \
	    inconsistent=1 ; \
	fi ; \
	if [ ! $${inconsistent} -eq 0 ] ; \
	then \
	    echo "ERROR: Coder token definitions out of date. Update coder.pt with new definitions from coder.def." ; \
	    exit 4 ; \
	fi
	
coder: coder.out coder.sst coder.def 
	/bin/cp coder.out coder.sst coder.def ../lib/pt

clean:
	/bin/rm -f coder.err coder.p coder.out coder.sst coder.def 
