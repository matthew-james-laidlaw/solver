#include <gtest/gtest.h>

#include <backend/polynomial.h>
#include <backend/solver.h>

#include <algorithm>
#include <complex>
#include <cstddef>
#include <string>

using namespace solver;

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
    auto equation = Polynomial{5}; // f(x) = 5

    auto expected_solution = Solution{};
    auto actual_solution = Solve(equation);

    AssertSolutionsEquivalent(expected_solution, actual_solution);
}

TEST(SolverTests, solve_first_order_polynomial)
{
    auto equation = Polynomial{5, 3}; // f(x) = 3x + 5

    auto expected_solution = Solution{-5.0 / 3.0};
    auto actual_solution = Solve(equation);

    AssertSolutionsEquivalent(expected_solution, actual_solution);
}

TEST(SolverTests, solve_second_order_polynomial)
{
    auto equation = Polynomial{2, -3, 1}; // f(x) = x^2 - 3x + 1

    auto expected_solution = Solution{2.0, 1.0};
    auto actual_solution = Solve(equation);

    AssertSolutionsEquivalent(expected_solution, actual_solution);
}

TEST(SolverTests, solve_third_order_polynomial)
{
    auto equation = Polynomial{-6, 11, -6, 1}; // f(x) = x^3 - 6x^2 + 11x - 6

    auto expected_solution = Solution{3.0, 2.0, 1.0};
    auto actual_solution = Solve(equation);

    AssertSolutionsEquivalent(expected_solution, actual_solution);
}

TEST(SolverTests, DISABLED_solve_fourth_order_polynomial)
{
    ASSERT_TRUE(false) << "not implemented";
}
