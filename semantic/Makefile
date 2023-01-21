# Makefile for PT Semantic pass Version 5.0
# Usage:  make [semantic]

# Revised v5.0 - 10 Dec 2020

PASS = semantic
SUFFIXES = .ssl .sst .pt .out
.SUFFIXES: ${SUFFIXES}

all:  semantic

install:  semantic

semantic.out:  semantic.pt semantic.sst semantic.consistency
	@echo "ptc semantic.pt"
	@ptc semantic.pt > semantic.err
	@cat semantic.err
	-@(grep -v Warning semantic.err > semantic.err.nowarn || true)
	@if (! cmp -s semantic.err.nowarn /dev/null) ; \
	then \
	    rm -f semantic.err semantic.err.nowarn  ; \
	    exit 1 ; \
	else \
	    rm -f semantic.err semantic.err.nowarn  ; \
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

semantic.consistency: semantic.sst semantic.pt
	@ # parser/semantic semantic tokens
	@inconsistent=0 ; \
	defcounts=`cat ../parser/parser.def semantic.def | grep -e "firstSemanticToken =" -e "firstCompoundSemanticToken =" -e "lastCompoundSemanticToken =" -e "sExpnEnd =" -e "sAdd =" -e "lastSemanticToken =" | expand | sort -u | wc -l` ; \
	if [ ! $${defcounts} -eq  6 ] ; \
	then \
	    inconsistent=1 ; \
	fi ; \
	if [ ! $${inconsistent} -eq 0 ] ; \
	then \
	    echo "ERROR: Parser/semantic semantic token definitions inconsistent. Update semantic.ssl input tokens to match parser.ssl output tokens." ; \
	    exit 3 ; \
	fi
	@ # semantic tokens
	@inconsistent=0 ; \
	defcounts=`cat semantic.pt semantic.def | grep -e "firstSemanticToken =" -e "firstCompoundSemanticToken =" -e "lastCompoundSemanticToken =" -e "sExpnEnd =" -e "sAdd =" -e "lastSemanticToken =" -e "firstTcode =" -e "tAdd =" -e "tNot =" -e "tParmEnd =" -e "tTrap =" -e "firstCompoundTcode =" -e "lastCompoundTcode =" -e "lastTcode =" -e "lastSslErrorCode =" -e "trAssign =" -e "oSymbolTblEnter =" -e "oSymbolStkPush =" -e "oSymbolStkChooseKind =" -e "oSymbolStkPop =" -e "oTypeTblEnter =" -e "oTypeStkPush =" -e "oTypeStkChooseKind =" -e "oTypeStkPop =" -e "oValuePop =" -e "oCountPop =" -e "oFixSwap =" | expand | sort -u | wc -l` ; \
	if [ ! $${defcounts} -eq  27 ] ; \
	then \
	    inconsistent=1 ; \
	fi ; \
	if [ ! $${inconsistent} -eq 0 ] ; \
	then \
	    echo "ERROR: Semantic token definitions out of date. Update semantic.pt with new definitions from semantic.def." ; \
	    exit 4 ; \
	fi
	
semantic: semantic.out semantic.sst semantic.def 
	/bin/cp semantic.out semantic.sst semantic.def ../lib/pt/

clean:
	/bin/rm -f semantic.err semantic.p semantic.out semantic.sst semantic.def 
