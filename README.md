# Solver

A simple polynomial solver application

## Grammar

function   ::= "f(x)" "=" expression
expression ::= term { ("+" | "-") term }
term       ::= factor { ("*" | "/") factor }
factor     ::= [ "-" ] primary
primary    ::= number [ variable ] | variable | "(" expression ")"
number     ::= digit { digit } [ "." digit { digit } ]
digit      ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
variable   ::= "x"
