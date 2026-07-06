#pragma once

#include <memory>

namespace solver
{

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

    auto AddCoefficient(int coefficient) -> void;

    auto operator-() const -> Monomial;

};

} // namespace solver
