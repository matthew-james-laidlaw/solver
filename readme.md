# Solver

A polynomial solver capable of solving polynomials up to the 3rd degree. Parses equations from the command line using a parser combinator.

See [grammar.md](docs/grammar.md) for the grammar rules.

## Architecture

`src/frontend` provides the lexing and parsing components that transform source strings into an AST. These components rely on `src/combinator` which provides a DSL-like interface for composing parsing primitives. `src/backend` provides the solving logic.

## Combinator

The combinator library consists of parsing primitives and overloaded operators to connect them. These are:
* Expect -- Expects a given token in the source stream, this is the lowest level primitive and does not compose any other parsers.
* Maybe -- Runs a given parser allowing for success or failure.
* Many (operator *) -- Runs a given parser indefinitely until it fails, allows for no matches.
* FoldRight (operator >>) -- Runs two parsers, expecting both to succeed but discarding the first result.
* Choice (operator |) -- Runs two parsers expecting at least one of them to succeed, returns the first successful match.
* Combine (operator &) -- Runs two parsers expecting them both to succeed and returns a tuple of their results.

## TODO

* Fuzz testing

1.0.0 features
* Can accept polynomials of simple format e.g.
*    f(x) = c0 + c1x + + c2x^n2 + c3x^n3 ...
*    order doesnt matter, and multiple terms of the same exponent can exist.
*    subtraction too
*    c and n are integers, n is nonnegative integer
* Solves up to 3rd order polynomials
* Proper documentation
* User testing
