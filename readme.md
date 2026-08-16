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

## ToDo

* ~~More expressive parser error reporting~~
* Column numbers in error reporting
* Fuzz testing
* Unit test coverage
* CI/CD automated tests for various platforms
* Complete the solving algorithms
