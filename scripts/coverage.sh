mkdir -p build/coverage

# create a .profdata file
llvm-profdata merge -sparse build/libs/solver/default.profraw \
    -o build/coverage/coverage.profdata

# report coverage to stdout
llvm-cov report                                     \
    build/bin/Debug/solver_tests                    \
    -instr-profile=build/coverage/coverage.profdata \
    -ignore-filename-regex='.*/build/_deps/.*'

# also save coverage to an html file
llvm-cov show                                       \
    build/bin/Debug/solver_tests                    \
    -instr-profile=build/coverage/coverage.profdata \
    -format=html                                    \
    -output-dir=build/coverage                      \
    -ignore-filename-regex='.*/build/_deps/.*'
