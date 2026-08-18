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
    if (i > m_terms.size()) {
        throw std::runtime_error("out of bounds access in polynomial");
    }
}

auto operator+(const Polynomial& a, const Polynomial& b) -> Polynomial
{
    auto n = std::max(a.Size(), b.Size());
    auto c = Polynomial(std::vector<int>(n, 0));
    for (size_t i = 0; i < n; ++i) {
        if (i < a.Size()) {
            c[i] += a[i];
        }
        if (i < b.Size()) {
            c[i] += b[i];
        }
    }
    return c;
}

auto operator-(const Polynomial& a, const Polynomial& b) -> Polynomial
{
    auto n = std::max(a.Size(), b.Size());
    auto c = Polynomial(std::vector<int>(n, 0));
    for (int i = n - 1; i >= 0; --i) {
        if (i < a.Size() && i < b.Size()) {
            c[i] = a[i] - b[i];
        }
        else if (i < a.Size()) {
            c[i] = a[i];
        }
        else if (i < b.Size()) {
            c[i] = -b[i];
        }
    }
    return c;
}

auto operator*(const Polynomial& a, const Polynomial& b) -> Polynomial
{
    if (a.Size() != b.Size()) {
        throw std::runtime_error("Polynomial::operator* operands must have same order");
    }

    // TODO
    // * Allow for different sized polynomials
    // * Rather than an if statement in the hot loop, we can zero extend both inputs to
    // n_c and let them get evaluated to 0
    //      (this also fixes the previous bullet)?

    auto n_a = a.Size();
    auto n_b = b.Size();
    auto n_c = n_a + n_b - 1;

    auto c = Polynomial(std::vector<int>(n_c, 0));

    for (size_t i = 0; i < n_c; ++i) {
        for (size_t j = 0; j <= i; ++j) {
            if (j >= n_a || (i - j) >= n_b) {
                continue;
            }
            c[i] += a[j] * b[i - j];
        }
    }

    return c;
}

auto operator/(const Polynomial& a, const Polynomial& b) -> Polynomial
{
    throw std::runtime_error("Polynomial::operator/ not implemented");
}

} // namespace solver
