mkdir -p build/coverage

# specify the naming convention for generated .profraw files
# and run the tests to generate them
LLVM_PROFILE_FILE="%m_%p.profraw" ctest --preset coverage

# allow recursive globbing '**'
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
