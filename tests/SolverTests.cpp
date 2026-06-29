#include <gtest/gtest.h>

#include <Parser.h>
#include <Solver.h>

TEST(SolverTests, SolveConstant)
{
    std::string equation = "5 = 0";
    auto expression = Parse(equation);

    auto expected_solutions = Solution{};
    auto actual_solutions = Solve(*dynamic_cast<Equality const*>(expression));

    ASSERT_EQ(actual_solutions, expected_solutions);
}

TEST(SolverTests, SolveLinear)
{
    std::string equation = "2x - 4 = 0";
    auto expression = Parse(equation);

    auto expected_solutions = Solution{ 2 };
    auto actual_solutions = Solve(*dynamic_cast<Equality const*>(expression));

    ASSERT_EQ(actual_solutions, expected_solutions);
}

TEST(SolverTests, SolveQuadratic)
{
    std::string equation = "x^2 - 5x + 6 = 0";
    auto expression = Parse(equation);

    auto expected_solutions = Solution{ 2, 3 };
    auto actual_solutions = Solve(*dynamic_cast<Equality const*>(expression));

    ASSERT_EQ(actual_solutions, expected_solutions);
}

TEST(SolverTests, SolveCubic)
{
    std::string equation = "x^3 - 6x^2 + 11x - 6 = 0";
    auto expression = Parse(equation);

    auto expected_solutions = Solution{ 1, 2, 3 };
    auto actual_solutions = Solve(*dynamic_cast<Equality const*>(expression));

    ASSERT_EQ(actual_solutions.size(), expected_solutions.size());
    for (size_t i = 0; i < actual_solutions.size(); ++i)
    {
        ASSERT_NEAR(actual_solutions[i].real(), expected_solutions[i].real(), 1e-9);
        ASSERT_NEAR(actual_solutions[i].imag(), expected_solutions[i].imag(), 1e-9);
    }
}

TEST(SolverTests, SolveQuartic)
{
    std::string equation = "x^4 - 10x^3 + 35x^2 - 50x + 24 = 0";
    auto expression = Parse(equation);

    auto expected_solutions = Solution{ 1, 2, 3, 4 };
    auto actual_solutions = Solve(*dynamic_cast<Equality const*>(expression));

    ASSERT_EQ(actual_solutions, expected_solutions);
}
