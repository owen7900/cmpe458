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

# TODO 

## Phase 1
- Determine why the screener/scanner changes aren't changing the ptc command even when invoked with `-L` 
- Figure out how to automate testing of just the scanner/screener
   - using `ptc -t1` or `ptc -o1` will just invoke the screener/scanner
- Remove old keywords `not until program const procedure begin repeat`
   - Bandid over the problems in the parser.ssl file
   - Documentation
- Add new Quby keywords `using val def break when module unless elsif`
   - Tests
   - Documentation
- Replace `char` type with `string` type
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
