#pragma once

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

    /** @brief Construct a constant monomial of form cx^0.
     */
    static auto Constant(int coefficient) -> Monomial;

    /** @brief Construct a monomial with form x^n.
     */
    static auto Variable(int exponent = 1) -> Monomial;

    /** @brief Construct a monomial with both its coefficient and exponent.
     */
    Monomial(int coefficient, int exponent);

    auto Coefficient() const -> int;
    auto Exponent() const -> int;

    auto operator+(int coefficient) const -> Monomial;
    auto operator-() const -> Monomial;
};

} // namespace solver
