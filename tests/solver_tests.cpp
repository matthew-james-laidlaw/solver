#include <gtest/gtest.h>

#include <backend/monomial.h>
#include <backend/solver.h>

using namespace solver;

TEST(SolverTests, constant_function_has_no_solutions)
{
    auto equation = Polynomial{5}; // f(x) = 5
    auto solution = Solve(equation);
    ASSERT_EQ(solution, Solution{});
}

TEST(SolverTests, solve_first_order_polynomial)
{
    auto equation = Polynomial{5, 3}; // f(x) = 3x + 5
    auto solution = Solve(equation);
    ASSERT_EQ(solution, Solution{-5.0 / 3.0});
}

TEST(SolverTests, solve_second_order_polynomial)
{
    auto equation = Polynomial{2, -3, 1}; // f(x) = x^2 - 3x + 1
    auto solution = Solve(equation);
    ASSERT_EQ(solution, (Solution{2.0, 1.0}));
}

TEST(SolverTests, solve_third_order_polynomial)
{
    auto equation = Polynomial{1, -3, -1, 3}; // f(x) = x^3 - 3x^2 - x + 3
    auto solution = Solve(equation);
    ASSERT_EQ(solution, (Solution{3.0, 2.0, 1.0}));
}

TEST(SolverTests, solve_fourth_order_polynomial)
{
    ASSERT_TRUE(false) << "not implemented";
}
