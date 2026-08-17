#include <backend/monomial.h>
#include <backend/polynomial.h>

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <vector>

namespace solver
{

Polynomial::Polynomial(const std::vector<Monomial>& monomials)
{
    if (monomials.size() == 0) {
        throw std::runtime_error(
            "a polynomial cannot be constructed with an empty list of monomials");
    }

    int order = 0;
    for (auto term : monomials) {
        order = std::max(order, term.Exponent());
    }

    m_terms = std::vector<int>(order + 1, 0);
    for (auto term : monomials) {
        if (term.Exponent() >= 0) {
            m_terms[term.Exponent()] += term.Coefficient();
        }
    }
}

Polynomial::Polynomial(size_t order)
    : m_terms(order + 1, 0)
{}

Polynomial::Polynomial(std::initializer_list<int> coeffs)
    : Polynomial(coeffs.size() - 1)
{
    if (coeffs.size() == 0) {
        throw std::runtime_error(
            "a polynomial cannot be constructed with an empty list of coefficients");
    }
    std::copy_n(coeffs.begin(), coeffs.size(), m_terms.begin());
}

auto Polynomial::operator[](size_t i) const -> int
{
    CheckBounds(i);
    return m_terms[i];
}

auto Polynomial::operator[](size_t i) -> int&
{
    CheckBounds(i);
    return m_terms[i];
}

auto Polynomial::Order() const -> size_t
{
    return m_terms.size() - 1;
}

auto Polynomial::CheckBounds(size_t i) const -> void
{
    if (i > m_terms.size()) {
        throw std::runtime_error("out of bounds access in polynomial");
    }
}

auto operator+(const Polynomial& a, const Polynomial& b) -> Polynomial
{
    auto order = std::max(a.Order(), b.Order());
    auto c = Polynomial(order);
    for (size_t i = 0; i < order; ++i) {
        if (i < a.Order()) {
            c[i] += a[i];
        }
        if (i < b.Order()) {
            c[i] += b[i];
        }
    }
    return c;
}

auto operator-(const Polynomial& a, const Polynomial& b) -> Polynomial
{
    throw std::runtime_error("Polynomial::operator- not implemented");
}

auto operator*(const Polynomial& a, const Polynomial& b) -> Polynomial
{
    throw std::runtime_error("Polynomial::operator* not implemented");
}

auto operator/(const Polynomial& a, const Polynomial& b) -> Polynomial
{
    throw std::runtime_error("Polynomial::operator/ not implemented");
}

} // namespace solver
