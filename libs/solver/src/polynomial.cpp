#include <monomial.h>
#include <polynomial.h>

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <vector>

namespace
{

auto ZeroExtend(const solver::Polynomial& p, size_t n) -> solver::Polynomial
{
    if (n <= p.Size()) {
        return p;
    }

    auto extended = solver::Polynomial(std::vector<int>(n, 0));
    for (size_t i = 0; i < p.Size(); ++i) {
        extended[i] = p[i];
    }

    return extended;
}

} // namespace

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

Polynomial::Polynomial(std::vector<int> coeffs)
    : m_terms(coeffs)
{
    if (coeffs.size() == 0) {
        throw std::runtime_error(
            "a polynomial cannot be constructed with an empty list of coefficients");
    }
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

auto Polynomial::Size() const -> size_t
{
    return m_terms.size();
}

auto Polynomial::CheckBounds(size_t i) const -> void
{
    if (i >= m_terms.size()) {
        throw std::runtime_error("out of bounds access in polynomial");
    }
}

auto operator+(const Polynomial& a, const Polynomial& b) -> Polynomial
{
    auto n = std::max(a.Size(), b.Size());

    auto aa = ZeroExtend(a, n);
    auto bb = ZeroExtend(b, n);

    auto c = Polynomial(std::vector<int>(n, 0));

    for (size_t i = 0; i < n; ++i) {
        c[i] = aa[i] + bb[i];
    }

    return c;
}

auto operator-(const Polynomial& a, const Polynomial& b) -> Polynomial
{
    auto n = std::max(a.Size(), b.Size());

    auto aa = ZeroExtend(a, n);
    auto bb = ZeroExtend(b, n);

    auto c = Polynomial(std::vector<int>(n, 0));

    for (size_t i = 0; i < n; ++i) {
        c[i] = aa[i] - bb[i];
    }

    return c;
}

auto operator*(const Polynomial& a, const Polynomial& b) -> Polynomial
{
    auto n = a.Size() + b.Size() - 1;

    auto aa = ZeroExtend(a, n);
    auto bb = ZeroExtend(b, n);

    auto c = Polynomial(std::vector<int>(n, 0));

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j <= i; ++j) {
            c[i] += aa[j] * bb[i - j];
        }
    }

    return c;
}

auto operator/(const Polynomial& a, const Polynomial& b) -> Polynomial
{
    (void)a;
    (void)b;
    throw std::runtime_error("Polynomial::operator/ not implemented");
}

} // namespace solver
