#pragma once

#include <complex>
#include <expected>
#include <string>
#include <vector>

namespace solver
{

class Error
{
public:

    enum class Type
    {
        LexerError,
        ParserError,
        SolverError,
        NotImplemented,
    };

    Type type;
    std::string message;

    Error(Type type, const std::string& message);
};

auto operator==(const Error& a, const Error& b) -> bool;

using Solution = std::vector<std::complex<double>>;

/** @brief Calculate all solutions for a given polynomial of order <= 3.
 * @param[in] equation The polynomial to solve written as a string. Should be a sum of
 * terms of the form cx^n with c as an integer and n as a non-negative integer.
 * @returns The list of solutions to the provided polynomial, or an error.
 */
auto Solve(const std::string& equation) -> std::expected<Solution, Error>;

} // namespace solver
