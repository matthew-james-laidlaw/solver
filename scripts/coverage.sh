mkdir -p build/coverage

# each ctest-launched test process is a separate binary invocation, so give
# each one a unique profile filename (%p = pid) or later runs clobber earlier
# ones' coverage data
LLVM_PROFILE_FILE="%m_%p.profraw" ctest --preset coverage

shopt -s globstar

# create a .profdata file by merging every test process's profile
llvm-profdata merge -sparse build/libs/**/*.profraw \
    -o build/coverage/coverage.profdata

# report coverage to stdout
llvm-cov report                                              \
    build/bin/Debug/solver_tests                             \
    -instr-profile=build/coverage/coverage.profdata          \
    -ignore-filename-regex='(.*/tests/.*|.*/build/_deps/.*)'

# also save coverage to an html file
llvm-cov show                                                \
    build/bin/Debug/solver_tests                             \
    -instr-profile=build/coverage/coverage.profdata          \
    -format=html                                             \
    -output-dir=build/coverage                               \
    -ignore-filename-regex='(.*/tests/.*|.*/build/_deps/.*)'
