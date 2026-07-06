### Grammar Rules

```
equation ::= "f(x)" "=" expression
expression ::= unary { ( "+" | "-" ) unary }
unary ::= [ "-" ] term
term ::= number [ power ] | power
power ::= variable [ "^" number ]
```

### Lexographical Rules

```
number ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
variable ::= "x"
```

### Key

```
( ) -- grouping
{ } -- zero or more
[ ] -- zero or one
 |  -- choice
```
