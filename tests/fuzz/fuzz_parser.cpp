#include <cstddef>
#include <cstdint>
#include <string>

#include <frontend/parser.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, std::size_t size)
{
    std::string input(reinterpret_cast<const char *>(data), size);

    try
    {
        solver::Parse(input);
    }
    catch (...)
    {
        // Safety net for std::stod / std::bad_alloc etc. on garbage input.
        // A thrown exception here is fine; an ASan report or hard crash is a bug.
    }

    return 0;
}
