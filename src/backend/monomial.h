#pragma once

#include <memory>

namespace solver
{

/** @brief Represents a single term of a polynomial with form cx^n.
 */
class Monomial
{
private:

    int m_coefficient;
    int m_exponent;

public:

    static auto Constant(int coefficient) -> Monomial;
    static auto Variable(int exponent = 1) -> Monomial;

    Monomial(int coefficient, int exponent);

    auto Coefficient() const -> int;
    auto Exponent() const -> int;

    auto operator+(int coefficient) const -> Monomial;
    auto operator-() const -> Monomial;

};

} // namespace solver
