" Vim syntax file
" Language:     S/SL
" Author/Maintainer:    Jim Cordy <cordy@cs.queensu.ca>
" Version:              6.2/7.2(synched), 2009 Dec 12

" Remove any old syntax stuff hanging around
syn clear

" Comments
syn match sslComment            "%.*$"

" Keywords
syn keyword sslKeyword          input output error type mechanism rules end
syn keyword sslKeyword          Input Output Error Type Mechanism Rules End
syn keyword sslKeyword          INPUT OUTPUT ERROR TYPE MECHANISM RULES END
syn keyword sslKeyword          do od if fi Do Od If Fi DO OD IF FI
syn match sslKeyword            "{"
syn match sslKeyword            "}"
syn match sslKeyword            "\["
syn match sslKeyword            "]"
syn match sslKeyword            "|"
syn match sslKeyword            "\*"
syn match sslKeyword            ":"
syn match sslKeyword            ";"
syn match sslKeyword            "@"
syn match sslKeyword            "?"
syn match sslKeyword            "#"
syn match sslKeyword            ">"
syn match sslKeyword            ">>"
syn match sslKeyword            "\."

" Literal values
syn match sslLiteral            "'[^']*'"               " string
syn match sslLiteral            "\<\d\+\>"              " integer

" Link to color categories
" At present, identical to MPW colors

hi link sslKeyword Keyword
hi link sslComment Comment
hi link sslLiteral Constant

"hi ruleContext guibg=black guifg=white

