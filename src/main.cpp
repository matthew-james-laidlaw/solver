#include <Expression.h>
#include <Parser.h>

#include <iostream>

auto Show(Expression* e) -> void
{
    if (auto* term = dynamic_cast<Term*>(e))
    {
        std::cout << "Term(";
        if (term->coefficient == 1 && term->exponent == 0)
        {
            std::cout << '1';
        }
        else if (term->coefficient == 1 && term->exponent == 1)
        {
            std::cout << 'x';
        }
        else if (term->exponent == 0)
        {
            std::cout << term->coefficient;
        }
        else if (term->exponent == 1)
        {
            std::cout << term->coefficient << 'x';
        }
        else if (term->coefficient == 1)
        {
            std::cout << "x^" << term->exponent;
        }
        else
        {
            std::cout << term->coefficient << "x^" << term->exponent;
        }
        std::cout << ")";
    }
    else if (auto* un = dynamic_cast<Unary*>(e))
    {
        std::cout << "Unary(" << un->op << ", ";
        Show(un->operand);
        std::cout << ")";
    }
    else if (auto* bin = dynamic_cast<Binary*>(e))
    {
        std::cout << "Binary(" << bin->op << ", ";
        Show(bin->lhs);
        std::cout << ", ";
        Show(bin->rhs);
        std::cout << ")";
    }
    else if (auto* eq = dynamic_cast<Equality*>(e))
    {
        std::cout << "Equality(";
        Show(eq->lhs);
        std::cout << ", ";
        Show(eq->rhs);
        std::cout << ")";
    }
    else
    {
        std::cout << "<unknown Expression>";
    }
}

auto PrintTokens(std::span<std::string> const& tokens) -> void
{
    for (auto const& token : tokens)
    {
        std::cout << '"' << token << "\", ";
    }
    std::cout << '\n';
}

auto main() -> int
{
    std::vector<std::vector<std::string>> tests = {
        { "42" },
        { "-42" },
        { "-42x" },
        { "1", "+", "2" },
        { "3", "*", "2", "+", "1" },
        { "x", "+", "5", "=", "8" },
        { "1", "+", "2", "+", "3" },
        { "3x", "+", "5" },
        { "-x" },
        { "-3x" },
        { "x^0" },
        { "x^1" },
        { "x^2" },
        { "x^-3" },
        { "x^2", "+", "6x", "+", "5", "=", "0" },
    };

    for (auto& test : tests)
    {
        PrintTokens(test);
        auto term = ParseEquality()(std::span(test));
        if (term)
        {
            Show(term->result);
        }
        else
        {
            std::cout << "parse error";
        }
        std::cout << "\n\n";
    }
	return 0;
}
