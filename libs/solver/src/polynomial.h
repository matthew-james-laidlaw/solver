#pragma once

#include <cstddef>
#include <initializer_list>
#include <vector>

namespace solver
{

class Monomial;

/** @brief A polynomial is a collection of monomials where each index in the list
 * corresponds to an exponent value. E.g., 3x^2 + 5x - 7 is stored as { -7, 5, 3 }.
 */
class Polynomial
{
private:

    std::vector<int> m_terms;

public:

    /** @brief Sort and combine a list of arbitrary monomials into a single polynomial.
     */
    Polynomial(const std::vector<Monomial>& monomials);

    /** @brief Create a polynomial from a list of coefficients. For example { 1, 2, 3 }
     * becomes the polynomial 1 + 2x + 3x^2.
     */
    Polynomial(std::vector<int> coeffs);

    auto operator[](size_t i) const -> int;
    auto operator[](size_t i) -> int&;

    /** @brief The number of terms in the polynomial.
     */
    auto Size() const -> size_t;

    friend auto operator+(const Polynomial& a, const Polynomial& b) -> Polynomial;
    friend auto operator-(const Polynomial& a, const Polynomial& b) -> Polynomial;
    friend auto operator*(const Polynomial& a, const Polynomial& b) -> Polynomial;
    friend auto operator/(const Polynomial& a, const Polynomial& b) -> Polynomial;

private:

    auto CheckBounds(size_t i) const -> void;
};

} // namespace solver
