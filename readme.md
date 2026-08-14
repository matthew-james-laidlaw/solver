# Solver

A polynomial solver capable of solving polynomials up to the 3rd degree. Parses equations from the command line using a parser combinator.

See [grammar.md](docs/grammar.md) for the grammar rules.

## Architecture

`src/frontend` provides the lexing and parsing components that transform source strings into an AST. These components rely on the `src/combinator` component which provides a DSL-like interface for composing parsing primitives. `src/backend` provides the solving logic.


## ToDo

* Better error reporting
* Fuzz testing
* Unit test coverage
* CI/CD automated tests for various platforms
* Complete the solving algorithms
