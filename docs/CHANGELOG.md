# Phase 3
## semantic.ssl
- Update input tokens to match parser.ssl (lines 33-76)
- Remove old T-codes and add new T-codes (lines 108-170)
- Add string size (line 232)
- Change `stdChar` to `stdString` (line 277)
- Add new symbol table operations `oSymbolTblStripScope` and `oSymbolTblMergeScope` (lines 305, 309)
- Add symbol kinds `syModule` and `syPublicProcedure` (lines 328-329)
- Remove `tpChar` (line 447)
- Change char traps to be string traps (lines 545-552)
- Add rule `ModuleDefinition` (lines 1430-1458)
- Update statment and block rules to allow declaration and statement intermixing (lines 811-841, 1461-1480)
- Update constant definitions (lines 844-869)
- Update type definitions (lines 936-956)
- Update variable declarations (lines 1139-1183)
- Recognize public procedures (lines 1296-1302)
- Remove repeat statement and add do loop (lines 1696-1724)
- Handle else clause within case statements (lines 1736-1758)
- Handle ternary operators (lines 1841, 2079-2113)
- Handle string index (lines 2054-2072)
- Handle string concatenation (lines 1986-1993)
- Handle string (in)equality (lines 2011-2028)
- Handle string length (lines 1962-1973)
- Update string T-codes (lines 1886-1894, 1909-1947)
- Update string constants (lines 859-867, 911-932, 2330-2333)

## semantic.pt
- Pasted contents of `semantic.def` (lines 54-501)
- Change predefined Char type to predefined String type (line 790)
- Replace Char references with String references (lines 1020-1035, 2081)
- Handle allocation of strings (lines 2396-2397)
- Add semantic operations `oSymbolTblStripScope` and `oSymbolTblMergeScope` (lines 1793-1824)
- Update assertions that require `syProcedure` on top of the Symbol stack to allow for `syPublicProcedure` (lines 1830, 1943, 1992, 2111, 2120)

# Phase 2
## parser.ssl
- Update input tokens to match scan.ssl. (lines 20-94)
- Removed old keywords: `sRepeatStmt`, `sRepeatEnd`. (lines 132-133)
- Added Quby keywords: `sPublic`, `sModule`, `sDoStmt`, `sBreakIf`, `sSubstring`, `sLength`, `sIndex`. (lines 111-154)
- Changed main parser loop to allow declarations and statements to be intermixed. (lines 212-226)
- Modified parsing of constant, type and variable declarations. (lines 234-340)
- Changed procedures. (lines 196-207 and 344-392)
- Added parsing of modules. (lines 208-211)
- Updated If Statements and implemented elsif. (lines 478-487)
- Added Unless Statements to make them work like if not. (lines 492-505)
- Modified Case Statements. (lines 511-554)
- Implemented Do Statements. (lines 577-582)
- Modified handling of the String type. (lines 625-662)
  - Added a new rule for a new precedence level for the `$` operator. (lines 639-646)
  - Added the `?` and `#` operators. (lines 633-634 and 660-662)
- Modified assignment, compare and not sign. (lines 414, 425, 587, 589 and 657)

## parser.pt
- Pasted contents of `parser.def` (lines 190-352)

# Phase 1
## scan.ssl
- Added input tokens for `%`, `$`, `#`, `?` and `!` (line 28-42)
- Removed output tokens for `not`, `until`, `program`, `const`, `procedure`, `begin` and `repeat` (line 57-83)
- Added output tokens for `elsif`, `unless`, `break`, `module`, `using`, `val`, `when` and `def` (line 57-83)
- Added scanner (output) tokens for `{`, `}`, `$`, `#`, `?` and `!` (line 116-132)
- Updated choice body for `(` to remove old pascal alternate comments (line 163) 
- Added choices for `#`, `$` and `?` (line 172-177)
- Updated choice body for `:` to remove support for `:=` (line 178)
- Updated choice body for `=` to add support for `==` (line 190-196)
- Updated choice body for `<` to remove support for `<>` (line 210)
- Added choice for `!` to add support for `!=` and `!` (the not operator) (line 224-230)
- Added choice body for `{` and `}` (line 231-234)
- Added choice body for `%` calling the `@Comment` rule (line 235)
- Modified choice body in `StringLiteral` that forces `'''` and an empty string. Added empty string support (line 275)
- Modified `@Comment` body to end comments at newline (line 299) 
- Removed rule `@AlternateComment` as it is no longer needed
  
## parser.pt
- Pastes of constants from `scan.def` 
- updated quote char (line 425)
- Added entries to `charClassMap` for `%`, `$`, `#`, `?` and `!` (line 935)

## stdIdentifiers
- removed `not`, `until`, `program`, `const`, `procedure`, `begin` and `repeat`
- added `elsif`, `unless`, `break`, `module`, `using`, `val`, `when`, `def`
- renamed `char` predefined type name to `string`
