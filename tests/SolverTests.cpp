#include <gtest/gtest.h>

#include <Expression.h>
#include <Parser.h>

#include <span>
#include <sstream>
#include <string>
#include <vector>

auto Serialize(Expression const* e, std::ostream& out) -> void
{
    if (auto* term = dynamic_cast<Term const*>(e))
    {
        out << "Term(";
        if (term->Coefficient() == 1 && term->Exponent() == 0)
            out << '1';
        else if (term->Coefficient() == 1 && term->Exponent() == 1)
            out << 'x';
        else if (term->Exponent() == 0)
            out << term->Coefficient();
        else if (term->Exponent() == 1)
            out << term->Coefficient() << 'x';
        else if (term->Coefficient() == 1)
            out << "x^" << term->Exponent();
        else
            out << term->Coefficient() << "x^" << term->Exponent();
        out << ")";
    }
    else if (auto* un = dynamic_cast<Unary const*>(e))
    {
        out << "Unary(" << un->Op() << ", ";
        Serialize(un->Operand(), out);
        out << ")";
    }
    else if (auto* bin = dynamic_cast<Binary const*>(e))
    {
        out << "Binary(" << bin->Op() << ", ";
        Serialize(bin->Left(), out);
        out << ", ";
        Serialize(bin->Right(), out);
        out << ")";
    }
    else if (auto* eq = dynamic_cast<Equality const*>(e))
    {
        out << "Equality(";
        Serialize(eq->Left(), out);
        out << ", ";
        Serialize(eq->Right(), out);
        out << ")";
    }
    else
    {
        out << "<unknown Expression>";
    }
}

auto Serialize(Expression const* e) -> std::string
{
    std::ostringstream out;
    Serialize(e, out);
    return out.str();
}

struct ParserCase
{
    std::string name;
    std::string source;
    std::string expected;
};

class ParserTest : public testing::TestWithParam<ParserCase> {};

TEST_P(ParserTest, ParsesToExpected)
{
    std::string source = GetParam().source;
    auto result = Parse(source);

    auto expected = GetParam().expected;
    auto actual   = Serialize(result);

    ASSERT_EQ(expected, actual);
}

INSTANTIATE_TEST_SUITE_P(
    Expressions,
    ParserTest,
    testing::Values(
        ParserCase{ "Constant",           "42",               "Term(42)"                                                              },
        ParserCase{ "NegativeConstant",   "-42",              "Term(-42)"                                                             },
        ParserCase{ "NegativeCoeffVar",   "-42x",             "Term(-42x)"                                                            },
        ParserCase{ "Addition",           "1 + 2",            "Binary(+, Term(1), Term(2))"                                           },
        ParserCase{ "MulThenAdd",         "3 * 2 + 1",        "Binary(+, Binary(*, Term(3), Term(2)), Term(1))"                       },
        ParserCase{ "EqualityWithVar",    "x + 5 = 8",        "Equality(Binary(+, Term(x), Term(5)), Term(8))"                        },
        ParserCase{ "LeftAssocAddition",  "1 + 2 + 3",        "Binary(+, Binary(+, Term(1), Term(2)), Term(3))"                       },
        ParserCase{ "CoeffVarPlusConst",  "3x + 5",           "Binary(+, Term(3x), Term(5))"                                          },
        ParserCase{ "NegBareVar",         "-x",               "Term(-1x)"                                                             },
        ParserCase{ "NegCoeffVar",        "-3x",              "Term(-3x)"                                                             },
        ParserCase{ "VarExpZero",         "x^0",              "Term(1)"                                                               },
        ParserCase{ "VarExpOne",          "x^1",              "Term(x)"                                                               },
        ParserCase{ "VarExpTwo",          "x^2",              "Term(x^2)"                                                             },
        ParserCase{ "VarNegExp",          "x^-3",             "Term(x^-3)"                                                            },
        ParserCase{ "Quadratic",          "x^2 + 6x + 5 = 0", "Equality(Binary(+, Binary(+, Term(x^2), Term(6x)), Term(5)), Term(0))" }
    ),
    [](const testing::TestParamInfo<ParserCase>& info)
    {
        return info.param.name;
    }
);
