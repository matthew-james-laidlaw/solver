### Grammar Rules

```
equation   ::= "f(x)" "=" expression
expression ::= unary { ( "+" | "-" ) unary }
unary      ::= [ "-" ] term
term       ::= posint [ variable ] | variable
variable   ::= "x" [ "^" posint ]
```

### Lexographical Rules

```
posint  ::= nonzero { digit }
nonzero ::= "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" 
digit   ::= "0" | nonzero
```

### Key

```
( ) -- grouping
{ } -- zero or more
[ ] -- zero or one
 |  -- choice
```
