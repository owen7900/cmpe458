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

## Phase 4
**coder.ssl**
- Update input tokens in *coder.ssl* to match output T-codes from *semantic.ssl* -- DONE Matthew
- Add the trap codes for the new Quby run time monitor string operation traps - make sure that the values are the ones listed in the phase 4 handout (i.e., `trAssignString` = 101, `trWriteString` = 109, and so on). Make sure that your semantic phase is also using the correct trap numbers (old `trWriteString` was 8, it's now 109)
- Add a `string` kind to the `DataKinds`. Value doesn't matter, 3 works
- Remove the `oOperandPushChar` semantic operation. Add the `oOperandPushString` operation
- Modify the `Block` rule to accept all the statement T-codes directly in its main declaration-handling loop by copying all the alternatives in the `Statements` rule directly into it instead of calling `Statements` afterwards. Conversely, wherever statements can appear in Quby, declarations can, so replace the entire body of the `Statements` rule with a simple call to the new `Block` rule
- Remove handling of `tLiteralChar` and `tStoreChar` whereever they occur. Add handling of `tLiteralString` and `tStoreString` instead. Remove handling of `tStringDescriptor` and `tSkipString` whereever they occur. The new `tLiteralString` can be handled just like `tSkipString` used to be, except that no `tStringDescriptor` T-code is accepted. Remove handling of Char operations `tAssignChar` and `tSubscriptChar`. Don't remove the rules they call (because they are also used to handle Booleans)
- Add handling of String operations `tAssignString`, `tStoreString`, `tSubscriptString`, `tConcatenate` etc. Remember that the operand length for string operations is Data Kind `string`. DO NOT make coalescence optimized
cases (ones that look for immediate assignment) for String operations
- Add handling of String parameters in the `Routine` rule - instead of using `EmitMove`, they must use your new `OperandAssignStringPopPop` rule
- Add subscripting of String arrays. Subscripting of strings is EXACTLY like subscripting of Integer arrays except that the scaling factor is 1024 (2^10) instead of 4 (2^2). (Remember that you can multiply by 1024 by shifting left 10 bits.)
- Add the new rules to be used by your string operations, `OperandAssignStringPopPop`, `OperandConcatenatePop`, `OperandSubstringPopPop`, `OperandIndexPop`, `OperandLength`, `OperandChr`, `OperandOrd` and `OperandStringEqualPop`. DO NOT make optimized versions such as `OperandConcatenateAssignPopPopPop`. The string operations all call trap routines at run time, so the optimizations are irrelevant. Remove the optimized cases for `tChr` and `tOrd`, which are now String operations. To implement Ord, for which we have no trap routine, simply force the address of the string into a temporary, change the mode to `mTempIndirect` and the length to `byte`, then force to a temporary to get the result integer (word)
- Strings are passed to the string traps by address always. So to pass a String argument to the String operation traps, just force the address of the String to a temporary (`OperandForceAddresIntoTemp`), force the result to the stack (`OperandForceToStack`), then pop and free the temporary (`OperandPopAndFreeTemp`). The String result of a String trap routine is returned as the address of the result in the result register. To represent that, simply push the result register (`mResultReg`) on the Operand Stack and generate code to copy it into a scratch register (`mScratchReg1`) before restoring the temporary registers (`RestoreTempRegsFromStack`), then force the scratch register to a temporary (`OperandForceIntoTemp`). Make sure that you generate code to call the string traps exactly as other traps are called - see `OperandEofFunction` for an example
- Add handling of `do` statements by copying the `WhileStmt` rule to a new `DoStmt` rule to handle Quby do statements, and delete `RepeatStmt`. Remember that Quby do loops are just like PT while statements except that statements may appear between the beginning of the loop and the `break if` condition
- Modify the `EmitDefaultCaseAbort` rule to check for an else clause (beginning with the `tCaseElse` T-code) before emitting the trap call. If there is an else clause, handle it just like a CaseVariant (but without a case label), otherwise emit the abort trap just as before

**coder.pt**
-  Change the semantic operations, type values, input tokens, output tokens, etc. to those generated in *coder.def* when *coder.ssl* is compiled by S/SL
- Make sure that the `firstCompoundToken` and `lastCompoundToken` values correspond exactly to the first and last T-code that have associated values generated by the semantic phase (these must be in a contiguous range). Make sure that the value of `tEndOfFile` and `lastInputToken` is exactly one more than the `lastOutputToken` T-code generated by your semantic phase
- In `OperandFoldManifestSubscript`, add a case to scale the subscript by 1024 if the element size is string (in the same way it scales by 4 if the element size is word). (You only need to do this if you chose to optimize manifest subscripts of string arrays in your S/SL.)
- Remove all references and alternatives for `tStringDescriptor` and `tSkipString` wherever they occur. Change the Assert statements that reference `tStringDescriptor` to say `tLiteralString` instead
- Remove the case alternative for the `oOperandPushChar` semantic operation, and change the case alternative for the `oOperandPushStringDescriptor` semantic operation to be `oOperandPushString`
- Remove the call to `AssertAllTempsAreFree` in the `oEmitMergeSourceCoordinate` semantic operation. Now that we have string traps, it’s going to fail

## Phase 3
**semantic.ssl:**
- Upate input tokens in semantic.ssl to match output tokens in parser.ssl -- DONE Matthew
- Change output tokens for Char operations to String operattions (replace "Char" with "String"). Add the new T-codes for `tConcatenate`, `tSubstring`, `tLength`, `tIndex` and `tStringEqual`. Remove the old T-codes for the while operations, and replace the old T-codes for the repeat operations with the new do loop operations `tDoBegin`, `tDoBreakIf`, `tDoTest` and `tDoEnd`. Add the new `tCaseElse` T-code -- DONE Matthew
- Add a definition for `stringSize` to the type Integer. The value is 1024 -- DONE Matthew
- In type StdType, change `stdChar` to `stdString` -- DONE Matthew
- Add the new `oSymbolTblStripScope` and `oSymbolTblMergeScope` operations to the `SymbolTable` mechanism -- DONE Matthew
- Add a kind for `syModule` to the type `SymbolKind`. If you are using a special kind for public procedures, add a kind for them also (e.g. `syPublicProcedure`), otherwise add a public attribute in another way -- DONE Matthew
- In type `TypeKind`, change the type kind for char (`tpChar`) to be for string (`tpString`) -- DONE Matthew
-  In type `TrapKind`, change the names of the traps for read and write char to be for string, and change their trap numbers to 108 for `trReadString` and 109 for `trWriteString` (which are the trap numbers assigned to them in the Quby runtime library). Remove the redundant extra `trWriteString`. Change all uses of the char traps in the S/SL to use the string traps instead -- DONE Matthew
- Add a new rule `ModuleDefinition` to handle modules. A `ModuleDefinition` is much like Program except it has no program parameters and no halt. At the end of the module it uses the new `oSymbolTblStripScope` and `oSymbolTblMergeScope` symbol table operations to promote all public symbols to the enclosing scope -- DONE Matthew
-  Merge the alternatives of the `Statement` rule into the `Block` rule and modify the `Statement` rule to push a new scope, call the `Block` rule, then pop the scope -- DONE Matthew
- Modify handling of constant definitions to allow only one per definition -- DONE Owen Hooper
- Modify handling of type definitions to allow only one per definition -- DONE Owen Hooper
- Modify handling of variable declarations to allow multiple identifiers declared using one type, but only one declaration per definition. You will have to push all the declared identifiers on the Symbol Stack and keep count of how many there are using the Count Stack, and then accept the type and use it to set the type and enter in the SymbolTable all the identifiers you pushed, one at a time. Remember to keep the stacks straight and clean up after you're done. Watch out for the fact that the code to do this for one variable in PT swaps the type stack but forgets to swap it back after entering the variable type - this will have to be fixed when you do it for more than one variable. (Note: If you’re finding this change too challenging, try just doing the single identifier case with no counting first.) -- DONE Matthew
- Change handling of procedure definitions to recognize public procedures and store them with the special public attribute or special symbol kind `syPublicProcedure` -- DONE Matthew
- Remove the handling of repeat statements and add handling of the Quby general do loop statement. Handling do statements is just like while statements except that the T-codes are different and there is a statement allowed before the conditional exit -- DONE Matthew
- Change case statement handling to handle the Quby optional else clause. The else clause is much like another case alternative, except emitted after the `tCaseEnd` -- DONE Matthew
-  Add handling of ternary (three-operand) operators (e.g. substring) to the `Expression` rule. Add a new `TernaryOperator` rule to handle substring operations. Look at the `BinaryOperator` rule as a model -- DONE Matthew
- Add handling of the string index (`?`) operator to the `BinaryOperator` rule. Be careful to get the type checking right -- DONE Matthew
- Add handling of string concatenation to the `sAdd` part of the `BinaryOperator` rule. Remember that strings are first class values in Quby, so string concatenation is just like integer addition in terms of what to do, except the T-codes are different -- DONE Owen Hooper
- Change `UnaryOperator` rule to handle the string length operation as well. Be careful to get the type checking right -- DONE Owen Hooper
- Strings are first class values in Quby, so we no longer need the `tSkipString` and `tStringDescriptor` stuff in the T-code for string literals. The T-code for a string literal in any context should simply be `tLiteralString` -- DONE Matthew
- Change handling of string constants to act like vars instead -- DONE Matthew

**semantic.pt:**
- Copy and paste the entire contents of semantic.def where indicated by the comments in the semantic.pt source code -- DONE Matthew
- Change the predefined type for Char to be a predefined type for String in the predefined type table entries and their initialization. Change all references to the Char type ref in the program to reference String instead -- DONE Matthew
- Change the predefined type "text" to reference String instead of Char -- DONE Matthew
- Add cases for the new semantic operations `oSymbolTblStripScope` and `oSymbolTblMergeScope` to the `SslWalker`. The implementation of `oSymbolTblStripScope` is like `oSymbolTblPopScope` except that it should not decrement the lexical level. That is, it just changes all the `identSymbolTblRefs` for the symbols in the top scope to their `symbolTblLink` values, and that's all. (This has the effect of removing them from visibility even though they are technically still in the table. A bit of a hack, but easy and correct.) Unlike `oSymbolTblPopScope`, be careful not to change `symbolTableTop`, `typeTableTop` and `lexicLevelStackTop` in `oSymbolTblStripScope` since we don’t want to remove anything from the tables in this case. The implementation of `oSymbolTblMergeScope` is easy - it just has to decrement the lexical level without changing any ident links -- DONE Matthew
- Change `oAllocateVariable` to handle allocation of Strings (size 1024) -- DONE Matthew
- Change all the assertions that insist on the top of the `SymbolStack` being `syProcedure` to allow for `syPublicProcedure` as well -- DONE Matthew

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
