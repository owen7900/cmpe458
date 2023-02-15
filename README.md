PT Pascal v5.1 (c) 2021 Queen's University, (c) 1980 University of Toronto
February 2021

This directory contains the sources and build scripts for 
PT Pascal and S/SL, version 5.1 as of February 2021.

To make PT Pascal and S/SL on a machine where PT and S/SL 
are already installed, use the command "make install".

To make PT Pascal and S/SL on a machine where PT and S/SL 
are not already installed, download and install Gnu Pascal
(command gpc), and use the command "make -f Makefile.GPC install".

To test the PT Pascal you have made before installation,
run the tests in the test subdirectory using the ptc and pti
command scripts in that directory.

Once PT Pascal and S/SL have been made and tested, install them 
globally on your machine using the command "sudo ./Install.sh".

Editor plugins for the syntax colorization of PT and S/SL
source files using Vim are in the editors subdirectory.
See the README there for installation information.
# HINTS
- Run `ptc` or `pti` using `./ptc` or `./pti` in most directories to get the custom compiler

# TODO 

## Phase 2
- Tokens -- DONE Matthew Thompson
   - Remove old parser output tokens --- sRepeatStmt, sRepeatEnd
   - Add new parser output tokens    --- sPublic, sModule, sDoStmt, sBreakIf, sSubstring, sLength, sIndex
- change main parser loop to allow declarations and statements to be intermixed
   - keep begin .... end statement
- Modify parsing of constant, type and variable declarations
- Routines (procedures)
   - Public vs private functions
   - emit begin ... end
- Modules
   - parse input to output required modules and thier requirements
- Statements
   - modify the parsing of `if`, `case`, `while`, `repeat`, and `begin` to meet the lang spec for Quby's `if`, `unless`, `case`, and `do`
   - for `if` and `case` the goal is to have the output token stream match that of PTPascal
- Unless Statements 
   - make them look like `if not` 
- Case Statements 
   - output should look the same as PTPascal --- using `sCase`, `sLabelEnd` and `sCaseEnd`
   - Re-use the the main block rule in here
   - add else statements 
- Elsif Clauses
   - We can either create `sElsif` and modify the semandtic phase or look like a nested set of if statements
   - I think we want the `sElsif` option
- Do Statements
   - remove the `repeat` statement add handling of the `do` statement
   - Should use `sDo` to mark the beginning of the do statement
   - Should use the `sBreakIf` token for `break if`
- The String type
   - remove handling of the `char` type 
   - add handline of the `string` type
   - add handling of new operators `$` `?` `#` 
   - the `?` operator should have same precedence as the `*` operator
   - the `#` operator should have same precedence as not
   - the `$` operator requires adding a new level of precedence
   - Handle the fact that `$` operator requires 3 operators
- Other minor little details
   - `:=`
   - `==`
   - `!=` 
   - `!`

## Phase 1
- Figure out how to automate testing of just the scanner/screener
   - using `ptc -t1` or `ptc -o1` will just invoke the screener/scanner
- Remove old keywords `not until program const procedure begin repeat` -- DONE Owen Hooper
   - Bandid over the problems in the parser.ssl file
   - Documentation
- Add new Quby keywords `using val def break when module unless elsif` -- DONE Owen Hooper
   - Tests
   - Documentation
- Replace `char` type with `string` type -- DONE Owen Hooper. This doesn't actually effect the screener or the scanner. Only the semantics and onwards need this
   - Documentation
   - Tests
- Add new character classes to `scan.ssl` and `parser.pt` -- DONE Owen Hooper
   - Documentation
   - Tests
- Add new Syntax Tokens `using val def unless elsif break when module` -- DONE Owen Hooper
   - Add tests for this
   - Documentation
- String Literals 
   - Replace quote type -- DONE Owen Hooper
   - Add support for null string -- DONE Owen Hooper
   - Make sure all the old PTPascal string stuff is carried over
   - Write tests for this
   - Documentation
- Change comments to `% COMMENT` -- DONE Owen Hooper
   - Write automated tests for this 
   - Documentation
- Documentation of all changes
