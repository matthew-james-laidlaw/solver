mkdir -p build/coverage

llvm-profdata merge -sparse build/libs/solver/default.profraw \
    -o build/coverage/coverage.profdata

llvm-cov show \
    build/bin/Debug/solver_tests \
    -instr-profile=build/coverage/coverage.profdata \
    -format=html \
    -output-dir=build/coverage
