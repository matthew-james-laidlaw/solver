mkdir -p build/coverage

llvm-profdata merge -sparse build/**/*.profraw \
    -o build/coverage/coverage.profdata

llvm-cov show \
    build/libsolver_tests \
    -instr-profile=build/coverage/coverage.profdata \
    -format=html \
    -output-dir=build/coverage
