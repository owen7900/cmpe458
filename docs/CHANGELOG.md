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
