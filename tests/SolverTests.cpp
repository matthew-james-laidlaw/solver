#include <gtest/gtest.h>

#include <Expression.h>
#include <Parser.h>

#include <span>
#include <sstream>
#include <string>
#include <vector>

namespace
{

// Same logic as the old Show(), but writes into a stream so it can be
// captured and compared instead of going to std::cout.
auto Serialize(Expression* e, std::ostream& out) -> void
{
    if (auto* term = dynamic_cast<Term*>(e))
    {
        out << "Term(";
        if (term->coefficient == 1 && term->exponent == 0)
            out << '1';
        else if (term->coefficient == 1 && term->exponent == 1)
            out << 'x';
        else if (term->exponent == 0)
            out << term->coefficient;
        else if (term->exponent == 1)
            out << term->coefficient << 'x';
        else if (term->coefficient == 1)
            out << "x^" << term->exponent;
        else
            out << term->coefficient << "x^" << term->exponent;
        out << ")";
    }
    else if (auto* un = dynamic_cast<Unary*>(e))
    {
        out << "Unary(" << un->op << ", ";
        Serialize(un->operand, out);
        out << ")";
    }
    else if (auto* bin = dynamic_cast<Binary*>(e))
    {
        out << "Binary(" << bin->op << ", ";
        Serialize(bin->lhs, out);
        out << ", ";
        Serialize(bin->rhs, out);
        out << ")";
    }
    else if (auto* eq = dynamic_cast<Equality*>(e))
    {
        out << "Equality(";
        Serialize(eq->lhs, out);
        out << ", ";
        Serialize(eq->rhs, out);
        out << ")";
    }
    else
    {
        out << "<unknown Expression>";
    }
}

auto Serialize(Expression* e) -> std::string
{
    std::ostringstream out;
    Serialize(e, out);
    return out.str();
}

struct ParserCase
{
    std::string              name;      // valid identifier for test output
    std::vector<std::string> tokens;
    std::string              expected;
};

} // namespace

class ParserTest : public testing::TestWithParam<ParserCase> {};

TEST_P(ParserTest, ParsesToExpected)
{
    // Copy out of the param: State is a span over mutable strings.
    std::vector<std::string> tokens = GetParam().tokens;

    auto result = ParseEquality()(std::span(tokens));

    ASSERT_TRUE(result.has_value()) << "parser returned no result";
    EXPECT_EQ(Serialize(result->result), GetParam().expected);
}

INSTANTIATE_TEST_SUITE_P(
    Expressions,
    ParserTest,
    testing::Values(
        ParserCase{ "Constant",           { "42" },                                       "Term(42)" },
        ParserCase{ "NegativeConstant",   { "-42" },                                      "Term(-42)" },
        ParserCase{ "NegativeCoeffVar",   { "-42x" },                                     "Term(-42x)" },
        ParserCase{ "Addition",           { "1", "+", "2" },                              "Binary(+, Term(1), Term(2))" },
        ParserCase{ "MulThenAdd",         { "3", "*", "2", "+", "1" },                    "Binary(+, Binary(*, Term(3), Term(2)), Term(1))" },
        ParserCase{ "EqualityWithVar",    { "x", "+", "5", "=", "8" },                    "Equality(Binary(+, Term(x), Term(5)), Term(8))" },
        ParserCase{ "LeftAssocAddition",  { "1", "+", "2", "+", "3" },                    "Binary(+, Binary(+, Term(1), Term(2)), Term(3))" },
        ParserCase{ "CoeffVarPlusConst",  { "3x", "+", "5" },                             "Binary(+, Term(3x), Term(5))" },
        ParserCase{ "NegBareVar",         { "-x" },                                       "Term(-1x)" },
        ParserCase{ "NegCoeffVar",        { "-3x" },                                      "Term(-3x)" },
        ParserCase{ "VarExpZero",         { "x^0" },                                      "Term(1)" },
        ParserCase{ "VarExpOne",          { "x^1" },                                      "Term(x)" },
        ParserCase{ "VarExpTwo",          { "x^2" },                                      "Term(x^2)" },
        ParserCase{ "VarNegExp",          { "x^-3" },                                     "Term(x^-3)" },
        ParserCase{ "Quadratic",          { "x^2", "+", "6x", "+", "5", "=", "0" },       "Equality(Binary(+, Binary(+, Term(x^2), Term(6x)), Term(5)), Term(0))" }
    ),
    [](const testing::TestParamInfo<ParserCase>& info)
    {
        return info.param.name;
    });
