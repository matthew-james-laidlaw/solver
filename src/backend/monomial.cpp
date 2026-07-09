#include <backend/monomial.h>

namespace solver
{

Monomial::Monomial(int coefficient, int exponent)
    : m_coefficient(coefficient), m_exponent(exponent)
{}

auto Monomial::Constant(int coefficient) -> Monomial
{
    return Monomial{coefficient, 0};
}

auto Monomial::Variable(int exponent) -> Monomial
{
    return Monomial{1, exponent};
}

auto Monomial::Coefficient() const -> int
{
    return m_coefficient;
}

auto Monomial::Exponent() const -> int
{
    return m_exponent;
}

auto Monomial::operator+(int coefficient) const -> Monomial
{
    return Monomial{m_coefficient + coefficient, m_exponent};
}

auto Monomial::operator-() const -> Monomial
{
    return Monomial{-m_coefficient, m_exponent};
}

} // namespace solver
