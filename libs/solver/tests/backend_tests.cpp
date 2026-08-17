#include <gtest/gtest.h>

#include <backend/monomial.h>
#include <backend/polynomial.h>

#include <initializer_list>
#include <vector>

using namespace solver;

TEST(BackendTests, construct_constant_monomial)
{
    const auto monomial = Monomial::Constant(3);
    ASSERT_EQ(monomial.Coefficient(), 3);
    ASSERT_EQ(monomial.Exponent(), 0);
}

TEST(BackendTests, construct_variable_monomial)
{
    const auto monomial = Monomial::Variable(3);
    ASSERT_EQ(monomial.Coefficient(), 1);
    ASSERT_EQ(monomial.Exponent(), 3);
}

TEST(BackendTests, construct_monomial)
{
    const auto monomial = Monomial(3, 2);
    ASSERT_EQ(monomial.Coefficient(), 3);
    ASSERT_EQ(monomial.Exponent(), 2);
}

TEST(BackendTests, negate_monomial)
{
    const auto monomial = -Monomial(3, 2);
    ASSERT_EQ(monomial.Coefficient(), -3);
    ASSERT_EQ(monomial.Exponent(), 2);
}

TEST(BackendTests, construct_polynomial_from_empty_monomial_list)
{
    const auto monomials = std::vector<Monomial>{};
    ASSERT_THROW(const auto polynomial = Polynomial(monomials), std::runtime_error);
}

TEST(BackendTests, construct_polynomial_from_monomials)
{
    const auto monomials = std::vector<Monomial>{{3, 5}};
    const auto polynomial = Polynomial(monomials);

    ASSERT_EQ(polynomial.Order(), 5);
    ASSERT_EQ(polynomial[0], 0);
    ASSERT_EQ(polynomial[1], 0);
    ASSERT_EQ(polynomial[2], 0);
    ASSERT_EQ(polynomial[3], 0);
    ASSERT_EQ(polynomial[4], 0);
    ASSERT_EQ(polynomial[5], 3);
}

TEST(BackendTests, construct_polynomial_nonzero_order)
{
    const auto polynomial = Polynomial(3);

    ASSERT_EQ(polynomial.Order(), 3);
    ASSERT_EQ(polynomial[0], 0);
    ASSERT_EQ(polynomial[1], 0);
    ASSERT_EQ(polynomial[2], 0);
    ASSERT_EQ(polynomial[3], 0);
}

TEST(BackendTests, construct_polynomial_from_empty_coefficient_list)
{
    ASSERT_THROW(const auto polynomial = Polynomial(std::initializer_list<int>{}),
                 std::runtime_error);
}

TEST(BackendTests, construct_polynomial_from_coefficients)
{
    const auto polynomial = Polynomial({1, 2, 3});

    ASSERT_EQ(polynomial.Order(), 2);
    ASSERT_EQ(polynomial[0], 1);
    ASSERT_EQ(polynomial[1], 2);
    ASSERT_EQ(polynomial[2], 3);
}

TEST(BackendTests, polynomial_bounds_checking)
{
    const auto polynomial = Polynomial(3);
    ASSERT_THROW(polynomial[10], std::runtime_error);
}
