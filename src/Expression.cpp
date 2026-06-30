#include <Expression.h>

Term::Term(double coefficient, int exponent)
    : m_coefficient(coefficient), m_exponent(exponent)
{}

auto Term::Coefficient() const -> double
{
    return m_coefficient;
}

auto Term::Exponent() const -> int
{
    return m_exponent;
}

Unary::Unary(char op, Expression* operand)
    : m_op(op), m_operand(operand)
{}

auto Unary::Op() const -> char
{
    return m_op;
}

auto Unary::Operand() const -> Expression const*
{
    return m_operand;
}

Binary::Binary(char op, Expression* lhs, Expression* rhs)
    : m_op(op), m_lhs(lhs), m_rhs(rhs)
{}

auto Binary::Op() const -> char
{
    return m_op;
}

auto Binary::Left() const -> Expression const*
{
    return m_lhs;
}

auto Binary::Right() const -> Expression const*
{
    return m_rhs;
}

Equality::Equality(Expression* lhs, Expression* rhs)
    : m_lhs(lhs), m_rhs(rhs)
{}

auto Equality::Left() const -> Expression const*
{
    return m_lhs;
}

auto Equality::Right() const -> Expression const*
{
    return m_rhs;
}
