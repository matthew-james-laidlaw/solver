#include <gtest/gtest.h>

#include <backend/monomial.h>
#include <backend/polynomial.h>

#include <initializer_list>
#include <vector>

using namespace solver;

TEST(PolynomialTests, construct_constant_monomial)
{
    const auto monomial = Monomial::Constant(3);
    ASSERT_EQ(monomial.Coefficient(), 3);
    ASSERT_EQ(monomial.Exponent(), 0);
}

TEST(PolynomialTests, construct_variable_monomial)
{
    const auto monomial = Monomial::Variable(3);
    ASSERT_EQ(monomial.Coefficient(), 1);
    ASSERT_EQ(monomial.Exponent(), 3);
}

TEST(PolynomialTests, construct_monomial)
{
    const auto monomial = Monomial(3, 2);
    ASSERT_EQ(monomial.Coefficient(), 3);
    ASSERT_EQ(monomial.Exponent(), 2);
}

TEST(PolynomialTests, negate_monomial)
{
    const auto monomial = -Monomial(3, 2);
    ASSERT_EQ(monomial.Coefficient(), -3);
    ASSERT_EQ(monomial.Exponent(), 2);
}

TEST(PolynomialTests, construct_polynomial_from_empty_monomial_list)
{
    const auto monomials = std::vector<Monomial>{};
    ASSERT_THROW(const auto polynomial = Polynomial(monomials), std::runtime_error);
}

TEST(PolynomialTests, construct_polynomial_from_monomials)
{
    const auto monomials = std::vector<Monomial>{{3, 5}};
    const auto polynomial = Polynomial(monomials);

    ASSERT_EQ(polynomial.Size(), 6);
    ASSERT_EQ(polynomial[0], 0);
    ASSERT_EQ(polynomial[1], 0);
    ASSERT_EQ(polynomial[2], 0);
    ASSERT_EQ(polynomial[3], 0);
    ASSERT_EQ(polynomial[4], 0);
    ASSERT_EQ(polynomial[5], 3);
}

TEST(PolynomialTests, construct_polynomial_from_empty_coefficient_list)
{
    ASSERT_THROW(const auto polynomial = Polynomial(std::initializer_list<int>{}),
                 std::runtime_error);
}

TEST(PolynomialTests, construct_polynomial_from_coefficients)
{
    const auto polynomial = Polynomial({1, 2, 3});

    ASSERT_EQ(polynomial.Size(), 3);
    ASSERT_EQ(polynomial[0], 1);
    ASSERT_EQ(polynomial[1], 2);
    ASSERT_EQ(polynomial[2], 3);
}

TEST(PolynomialTests, polynomial_bounds_checking)
{
    const auto polynomial = Polynomial(std::vector<int>(3, 0));
    ASSERT_THROW(polynomial[10], std::runtime_error);
}

TEST(PolynomialTests, add_polynomials)
{
    {
        const auto p1 = Polynomial({1, 2, 3});
        const auto p2 = Polynomial({4, 5, 6});
        const auto p3 = p1 + p2;
        ASSERT_EQ(p3.Size(), 3);
        ASSERT_EQ(p3[0], 5);
        ASSERT_EQ(p3[1], 7);
        ASSERT_EQ(p3[2], 9);
    }

    {
        const auto p1 = Polynomial({1, 2, 3});
        const auto p2 = Polynomial({4, 5});
        const auto p3 = p1 + p2;
        ASSERT_EQ(p3.Size(), 3);
        ASSERT_EQ(p3[0], 5);
        ASSERT_EQ(p3[1], 7);
        ASSERT_EQ(p3[2], 3);
    }

    {
        const auto p1 = Polynomial({1, 2});
        const auto p2 = Polynomial({4, 5, 6});
        const auto p3 = p1 + p2;
        ASSERT_EQ(p3.Size(), 3);
        ASSERT_EQ(p3[0], 5);
        ASSERT_EQ(p3[1], 7);
        ASSERT_EQ(p3[2], 6);
    }
}

TEST(PolynomialTests, subtract_polynomials)
{
    {
        const auto p1 = Polynomial({3, 2, 1});
        const auto p2 = Polynomial({4, 5, 6});
        const auto p3 = p2 - p1;
        ASSERT_EQ(p3.Size(), 3);
        ASSERT_EQ(p3[0], 1);
        ASSERT_EQ(p3[1], 3);
        ASSERT_EQ(p3[2], 5);
    }

    {
        const auto p1 = Polynomial({3, 2, 1});
        const auto p2 = Polynomial({4, 5});
        const auto p3 = p2 - p1;
        ASSERT_EQ(p3.Size(), 3);
        ASSERT_EQ(p3[0], 1);
        ASSERT_EQ(p3[1], 3);
        ASSERT_EQ(p3[2], -1);
    }

    {
        const auto p1 = Polynomial({3, 2});
        const auto p2 = Polynomial({4, 5, 6});
        const auto p3 = p2 - p1;
        ASSERT_EQ(p3.Size(), 3);
        ASSERT_EQ(p3[0], 1);
        ASSERT_EQ(p3[1], 3);
        ASSERT_EQ(p3[2], 6);
    }
}

TEST(PolynomialTests, multiply_polynomials)
{
    {
        const auto p1 = Polynomial({1, 2, 3});
        const auto p2 = Polynomial({4, 5, 6});
        const auto p3 = p2 * p1;
        ASSERT_EQ(p3.Size(), 5);
        ASSERT_EQ(p3[0], 4);
        ASSERT_EQ(p3[1], 13);
        ASSERT_EQ(p3[2], 28);
        ASSERT_EQ(p3[3], 27);
        ASSERT_EQ(p3[4], 18);
    }

    {
        const auto p1 = Polynomial({1, 2, 3});
        const auto p2 = Polynomial({4, 5});
        const auto p3 = p2 * p1;
        ASSERT_EQ(p3.Size(), 4);
        ASSERT_EQ(p3[0], 4);
        ASSERT_EQ(p3[1], 13);
        ASSERT_EQ(p3[2], 22);
        ASSERT_EQ(p3[3], 15);
    }

    {
        const auto p1 = Polynomial({1, 2});
        const auto p2 = Polynomial({4, 5, 6});
        const auto p3 = p2 * p1;
        ASSERT_EQ(p3.Size(), 4);
        ASSERT_EQ(p3[0], 4);
        ASSERT_EQ(p3[1], 13);
        ASSERT_EQ(p3[2], 16);
        ASSERT_EQ(p3[3], 12);
    }
}

TEST(PolynomialTests, DISABLED_divide_polynomials)
{
    ASSERT_TRUE(false);
}
