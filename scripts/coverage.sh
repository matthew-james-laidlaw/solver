mkdir -p build/coverage

llvm-profdata merge -sparse build/libs/libsolver/default.profraw \
    -o build/coverage/coverage.profdata

llvm-cov show \
    build/bin/Debug/libsolver_tests \
    -instr-profile=build/coverage/coverage.profdata \
    -format=html \
    -output-dir=build/coverage
