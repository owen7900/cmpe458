# Makefile for PT Scanner Parser Version 5.0
# Usage:  make [scanner | parser]

# Revised v5.0 - 10 Dec 2020

PASS = parser
SUFFIXES = .ssl .sst .pt .out
.SUFFIXES: ${SUFFIXES}

all:  parser

install:  parser

parser.out:  parser.pt scan.sst parser.sst scan.consistency parser.consistency
	@echo "ptc parser.pt"
	@ptc parser.pt > parser.err
	@cat parser.err
	-@(grep -v Warning parser.err > parser.err.nowarn || true)
	@if (! cmp -s parser.err.nowarn /dev/null) ; \
	then \
	    rm -f parser.err parser.err.nowarn  ; \
	    exit 1 ; \
	else \
	    rm -f parser.err parser.err.nowarn  ; \
	fi

scanner.out:  parser.pt scan.sst scan.consistency
	@echo "ptc parser.pt"
	@ptc parser.pt > parser.err
	@cat parser.err
	-@(grep -v Warning parser.err > parser.err.nowarn || true)
	@if (! cmp -s parser.err.nowarn /dev/null) ; \
	then \
	    rm -f parser.err parser.err.nowarn  ; \
	    exit 2 ; \
	else \
	    rm -f parser.err parser.err.nowarn  ; \
	    mv parser.out scanner.out ; \
	fi


# S/SL make procedure and dependency analysis

scan.sst: scan.ssl
	@echo "ssl scan.ssl"
	@ssl scan.ssl > scan.err
	@cat scan.err
	-@(grep -v Warning scan.err > scan.err.nowarn || true)
	@if (! cmp -s scan.err.nowarn /dev/null) ; \
	then \
	    rm -f scan.err scan.err.nowarn  ; \
	    rm -f scan.def scan.sst ; \
	    exit 3 ; \
	else \
	    rm -f scan.err scan.err.nowarn  ; \
	fi 

parser.sst: parser.ssl
	@echo "ssl parser.ssl"
	@ssl parser.ssl > parser.err
	@cat parser.err
	-@(grep -v Warning parser.err > parser.err.nowarn || true)
	@if (! cmp -s parser.err.nowarn /dev/null) ; \
	then \
	    rm -f parser.err parser.err.nowarn  ; \
	    rm -f parser.def parser.sst ; \
	    exit 3 ; \
	else \
	    rm -f parser.err parser.err.nowarn  ; \
	    mv parser.def parser.rawdef ; \
	    echo '	}' > parser.eol ; \
	    sed -e 's/{ Input Tokens }/& {/' -e '/lastSyntaxToken.*;/r parser.eol' \
	        -e 's/{ S\/SL Table Parameters }/& {/' -e '/maxSslTableValue.*/r parser.eol' < parser.rawdef > parser.def ; \
	    rm -f parser.rawdef parser.eol ; \
	fi 

scan.consistency: scan.sst parser.pt
	@ # scanner character classes
	@inconsistent=0 ; \
	defcounts=`cat scan.def parser.pt | grep -e "firstCharClass =" -e "lastCharClass =" -e "lLeftParen =" | expand | sort -u | wc -l` ; \
	if [ ! $${defcounts} -eq  3 ] ; \
	then \
	    inconsistent=1 ; \
	fi ; \
	if [ ! $${inconsistent} -eq 0 ] ; \
	then \
	    echo "ERROR: Scanner character class definitions out of date. Update parser.pt with new definitions from scan.def." ; \
	    exit 4 ; \
	fi
	@ # scanner syntax tokens
	@cat parser.pt | sed '/===== End of contents of scan.def/,999999d' > temp.pt
	@inconsistent=0 ; \
	defcounts=`cat scan.def temp.pt | grep -e "firstKeywordToken =" -e "lastKeywordToken =" -e "pIf =" -e "firstCompoundInputToken =" -e "lastCompoundInputToken =" -e "pLeftParen =" -e "lastSyntaxToken =" | expand | sort -u | wc -l` ; \
        /bin/rm -f temp.pt ; \
	if [ ! $${defcounts} -eq  7 ] ; \
	then \
	    inconsistent=1 ; \
	fi ; \
	if [ ! $${inconsistent} -eq 0 ] ; \
	then \
	    echo "ERROR: Scanner/parser syntax token definitions out of date. Update parser.pt with new definitions from scan.def." ; \
	    exit 5 ; \
	fi

parser.consistency: scan.sst parser.sst parser.pt
	@ # scanner/parser syntax tokens
	@inconsistent=0 ; \
	defcounts=`cat scan.def parser.def | grep -e "firstKeywordToken =" -e "lastKeywordToken =" -e "pIf =" -e "firstCompoundInputToken =" -e "lastCompoundInputToken =" -e "pLeftParen =" -e "lastSyntaxToken =" | expand | sort -u | wc -l` ; \
	if [ ! $${defcounts} -eq  7 ] ; \
	then \
	    inconsistent=1 ; \
	fi ; \
	if [ ! $${inconsistent} -eq 0 ] ; \
	then \
	    echo "ERROR: Scanner/parser syntax token definitions inconsistent. Update parser.ssl input tokens to match scan.ssl output tokens." ; \
	    exit 6 ; \
	fi
	@ # parser semantic tokens
	@inconsistent=0 ; \
	defcounts=`cat parser.pt parser.def | grep -e "firstSemanticToken =" -e "firstCompoundSemanticToken =" -e "lastCompoundSemanticToken =" -e "sExpnEnd =" -e "sAdd =" -e "lastSemanticToken =" | expand | sort -u | wc -l` ; \
	if [ ! $${defcounts} -eq  6 ] ; \
	then \
	    inconsistent=1 ; \
	fi ; \
	if [ ! $${inconsistent} -eq 0 ] ; \
	then \
	    echo "ERROR: Parser semantic token definitions out of date. Update parser.pt with new definitions from parser.def." ; \
	    exit 7 ; \
	fi
	
parser: parser.out parser.sst parser.def scan.sst scan.def stdIdentifiers
	/bin/cp parser.out parser.sst parser.def scan.sst scan.def stdIdentifiers ../lib/pt


scanner: scanner.out parser.sst parser.def scan.sst scan.def stdIdentifiers
	/bin/cp scanner.out ../lib/pt/parser.out
	/bin/cp parser.sst parser.def scan.sst scan.def stdIdentifiers ../lib/pt

clean:
	/bin/rm -f parser.err parser.p parser.out parser.sst parser.def 
	/bin/rm -f scanner.out scan.sst scan.def
