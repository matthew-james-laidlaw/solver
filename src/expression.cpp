#include <expression.h>

namespace solver
{

Function::Function(std::unique_ptr<Expression> expr)
    : m_expr(std::move(expr))
{}

auto Function::Expr() const -> Expression const*
{
    return m_expr.get();
}

Binary::Binary(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right, char op)
    : m_left(std::move(left))
    , m_right(std::move(right))
    , m_op(op)
{}

auto Binary::Left() const -> Expression const*
{
    return m_left.get();
}

auto Binary::Right() const -> Expression const*
{
    return m_right.get();
}

auto Binary::Op() const -> char
{
    return m_op;
}

Monomial::Monomial(int coefficient, int exponent)
    : m_coefficient(coefficient)
    , m_exponent(exponent)
{}

auto Monomial::Coefficient() const -> int
{
    return m_coefficient;
}

auto Monomial::Exponent() const -> int
{
    return m_exponent;
}

} // namespace solver
