#include <backend/polynomial.h>

namespace solver
{

auto Canonicalize(std::vector<Monomial> monomials) -> Polynomial
{
    int order = 0;
    for (auto term : monomials) {
        order = std::max(order, term.Exponent());
    }

    Polynomial polynomial(order + 1, 0);
    for (auto term : monomials) {
        if (term.Exponent() >= 0) {
            polynomial[term.Exponent()] += term.Coefficient();
        }
    }

    return polynomial;
}

} // namespace solver
