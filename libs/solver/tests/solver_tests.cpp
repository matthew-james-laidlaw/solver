#include <gtest/gtest.h>

#include <polynomial.h>
#include <solver/solver.h>

#include <algorithm>
#include <complex>
#include <cstddef>
#include <string>

using namespace solver;
using namespace std::string_literals;

auto AssertSolutionsEquivalent(const Solution& a, const Solution& b,
                               double tolerance = 1e-9) -> void
{
    ASSERT_EQ(a.size(), b.size());

    auto by_components = [](const std::complex<double>& lhs,
                            const std::complex<double>& rhs) -> bool
    {
        if (lhs.real() != rhs.real()) {
            return lhs.real() < rhs.real();
        }
        return lhs.imag() < rhs.imag();
    };

    auto a_sorted = a;
    std::sort(a_sorted.begin(), a_sorted.end(), by_components);

    auto b_sorted = b;
    std::sort(b_sorted.begin(), b_sorted.end(), by_components);

    bool equivalent = true;

    for (std::size_t i = 0; i < a_sorted.size(); ++i) {
        if (std::abs(a_sorted[i] - b_sorted[i]) > tolerance) {
            equivalent = false;
            break;
        }
    }

    ASSERT_TRUE(equivalent);
}

TEST(SolverTests, constant_function_has_no_solutions)
{
    auto equation = "f(x) = 5"s;

    auto expected_solution = Solution{};
    auto actual_solution = Solve(equation);
    ASSERT_TRUE(actual_solution);

    AssertSolutionsEquivalent(expected_solution, *actual_solution);
}

TEST(SolverTests, solve_first_order_polynomial)
{
    auto equation = "f(x) = 3x + 5"s;

    auto expected_solution = Solution{-5.0 / 3.0};
    auto actual_solution = Solve(equation);
    ASSERT_TRUE(actual_solution);

    AssertSolutionsEquivalent(expected_solution, *actual_solution);
}

TEST(SolverTests, solve_second_order_polynomial)
{
    auto equation = "f(x) = x^2 - 3x + 2"s;

    auto expected_solution = Solution{2.0, 1.0};
    auto actual_solution = Solve(equation);
    ASSERT_TRUE(actual_solution);

    AssertSolutionsEquivalent(expected_solution, *actual_solution);
}

TEST(SolverTests, solve_third_order_polynomial)
{
    auto equation = "f(x) = x^3 - 6x^2 + 11x - 6"s;

    auto expected_solution = Solution{3.0, 2.0, 1.0};
    auto actual_solution = Solve(equation);
    ASSERT_TRUE(actual_solution);

    AssertSolutionsEquivalent(expected_solution, *actual_solution);
}

TEST(SolverTests, solve_fourth_order_polynomial_fails)
{
    auto equation = "f(x) = x^4 - 6x^2 + 11x - 6"s;
    auto solution = Solve(equation);
    ASSERT_FALSE(solution);
    ASSERT_EQ(solution.error(),
              Error(Error::Type::NotImplemented,
                    "solving for polynomials of order 4 is not implemented"));
}

TEST(SolverTests, solve_fifth_order_polynomial_fails)
{
    auto equation = "f(x) = x^5 - 6x^2 + 11x - 6"s;
    auto solution = Solve(equation);
    ASSERT_FALSE(solution);
    ASSERT_EQ(solution.error(),
              Error(Error::Type::NotImplemented,
                    "solving for polynomials of order > 4 is not implemented"));
}
