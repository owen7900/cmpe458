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

## Phase 3
**semantic.ssl:**
- Upate input tokens in semantic.ssl to match output tokens in parser.ssl -- DONE Matthew
- Change output tokens for Char operations to String operattions (replace "Char" with "String"). Add the new T-codes for `tConcatenate`, `tSubstring`, `tLength`, `tIndex` and `tStringEqual`. Remove the old T-codes for the while operations, and replace the old T-codes for the repeat operations with the new do loop operations `tDoBegin`, `tDoBreakIf`, `tDoTest` and `tDoEnd`. Add the new `tCaseElse` T-code -- DONE Matthew
- Add a definition for `stringSize` to the type Integer. The value is 1024 -- DONE Matthew
- In type StdType, change `stdChar` to `stdString` -- DONE Matthew
- Add the new `oSymbolTblStripScope` and `oSymbolTblMergeScope` operations to the `SymbolTable` mechanism
- Add a kind for `syModule` to the type `SymbolKind`. If you are using a special kind for public procedures, add a kind for them also (e.g. `syPublicProcedure`), otherwise add a public attribute in another way -- DONE Matthew
- In type `TypeKind`, change the type kind for char (`tpChar`) to be for string (`tpString`) -- DONE Matthew
-  In type `TrapKind`, change the names of the traps for read and write char to be for string, and change their trap numbers to 108 for `trReadString` and 109 for `trWriteString` (which are the trap numbers assigned to them in the Quby runtime library). Remove the redundant extra `trWriteString`. Change all uses of the char traps in the S/SL to use the string traps instead -- DONE Matthew
- Add a new rule `ModuleDefinition` to handle modules. A `ModuleDefinition` is much like Program except it has no program parameters and no halt. At the end of the module it uses the new `oSymbolTblStripScope` and `oSymbolTblMergeScope` symbol table operations to promote all public symbols to the enclosing scope
-  Merge the alternatives of the `Statement` rule into the `Block` rule and modify the `Statement` rule to push a new scope, call the `Block` rule, then pop the scope -- DONE Matthew
- Modify handling of constant definitions to allow only one per definition -- DONE Owen Hooper
- Modify handling of type definitions to allow only one per definition -- DONE Owen Hooper
- Modify handling of variable declarations to allow multiple identifiers declared using one type, but only one declaration per definition. You will have to push all the declared identifiers on the Symbol Stack and keep count of how many there are using the Count Stack, and then accept the type and use it to set the type and enter in the SymbolTable all the identifiers you pushed, one at a time. Remember to keep the stacks straight and clean up after you're done. Watch out for the fact that the code to do this for one variable in PT swaps the type stack but forgets to swap it back after entering the variable type - this will have to be fixed when you do it for more than one variable. (Note: If you’re finding this change too challenging, try just doing the single identifier case with no counting first.)
- Change handling of procedure definitions to recognize public procedures and store them with the special public attribute or special symbol kind `syPublicProcedure`
- Remove the handling of repeat statements and add handling of the Quby general do loop statement. Handling do statements is just like while statements except that the T-codes are different and there is a statement allowed before the conditional exit -- DONE Matthew
- Change case statement handling to handle the Quby optional else clause. The else clause is much like another case alternative, except emitted after the `tCaseEnd`
-  Add handling of ternary (three-operand) operators (e.g. substring) to the `Expression` rule. Add a new `TernaryOperator` rule to handle substring operations. Look at the `BinaryOperator` rule as a model
- Add handling of the string index (`?`) operator to the `BinaryOperator` rule. Be careful to get the type checking right
- Add handling of string concatenation to the `sAdd` part of the `BinaryOperator` rule. Remember that strings are first class values in Quby, so string concatenation is just like integer addition in terms of what to do, except the T-codes are different -- DONE Owen Hooper
- Change `UnaryOperator` rule to handle the string length operation as well. Be careful to get the type checking right -- DONE Owen Hooper
- Strings are first class values in Quby, so we no longer need the `tSkipString` and `tStringDescriptor` stuff in the T-code for string literals. The T-code for a string literal in any context should simply be `tLiteralString`
- Change handling of string constants to act like vars instead

**semantic.pt:**
- Copy and paste the entire contents of semantic.def where indicated by the comments in the semantic.pt source code
- Change the predefined type for Char to be a predefined type for String in the predefined type table entries and their initialization. Change all references to the Char type ref in the program to reference String instead
- Change the predefined type "text" to reference String instead of Char
- Add cases for the new semantic operations `oSymbolTblStripScope` and `oSymbolTblMergeScope` to the `SslWalker`. The implementation of `oSymbolTblStripScope` is like `oSymbolTblPopScope` except that it should not decrement the lexical level. That is, it just changes all the `identSymbolTblRefs` for the symbols in the top scope to their `symbolTblLink` values, and that's all. (This has the effect of removing them from visibility even though they are technically still in the table. A bit of a hack, but easy and correct.) Unlike `oSymbolTblPopScope`, be careful not to change `symbolTableTop`, `typeTableTop` and `lexicLevelStackTop` in `oSymbolTblStripScope` since we don’t want to remove anything from the tables in this case. The implementation of `oSymbolTblMergeScope` is easy - it just has to decrement the lexical level without changing any ident links
- Change `oAllocateVariable` to handle allocation of Strings (size 1024) -- DONE Matthew
- Change all the assertions that insist on the top of the `SymbolStack` being `syProcedure` to allow for `syPublicProcedure` as well

## Phase 2
- Tokens -- DONE Matthew Thompson -- TESTS DONE Owen Hooper
   - Remove old parser output tokens --- sRepeatStmt, sRepeatEnd
   - Add new parser output tokens    --- sPublic, sModule, sDoStmt, sBreakIf, sSubstring, sLength, sIndex
- change main parser loop to allow declarations and statements to be intermixed -- DONE Matthew Thompson -- TESTS DONE Owen Hooper
   - keep begin .... end statement
- Modify parsing of constant, type and variable declarations -- DONE Matthew Thompson -- TEST DONE Owen Hooper
- Routines (procedures) -- DONE Matthew Thompson -- TESTS DONE Owen Hooper
   - Public vs private functions
   - emit begin ... end
- Modules -- DONE Matthew Thompson -- TESTS DONE Owen Hooper
   - parse input to output required modules and their requirements
- Statements
   - modify the parsing of `if`, `case`, `while`, `repeat`, and `begin` to meet the lang spec for Quby's `if`, `unless`, `case`, and `do`
   - for `if` and `case` the goal is to have the output token stream match that of PTPascal
- If Statements -- DONE Alan -- TESTS DONE Owen
   - Implemented elsif in the nested method
- Unless Statements -- DONE Alan -- TESTS DONE Owen
   - make them look like `if not` 
- Case Statements -- DONE Alan -- TESTS DONE Owen
   - output should look the same as PTPascal --- using `sCase`, `sLabelEnd` and `sCaseEnd`
   - Re-use the the main block rule in here
   - add else statements 
- Do Statements -- DONE Matthew Thompson -- TEST DONE Owen
   - remove the `repeat` statement add handling of the `do` statement
   - Should use `sDo` to mark the beginning of the do statement
      - *Note: Tokens section of assignment said to add `sDoStmt`, not `sDo`. Currently using `sDoStmt`*
   - Should use the `sBreakIf` token for `break if`
- The String type -- DONE Matthew Thompson -- TESTS DONE Owen Hooper
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
