#pragma once

#include <memory>

namespace solver
{

class Expression
{
public:
    virtual ~Expression() = default;
};

class Function : public Expression
{
private:

    std::unique_ptr<Expression> m_expr;

public:

    Function(std::unique_ptr<Expression> expr);

    auto Expr() const -> Expression const*;

};

class Binary : public Expression
{
private:

    std::unique_ptr<Expression> m_left;
    std::unique_ptr<Expression> m_right;
    char m_op;

public:

    Binary(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right, char op);

    auto Left() const -> Expression const*;
    auto Right() const -> Expression const*;
    auto Op() const -> char;

};

class Monomial : public Expression
{
public:

    int m_coefficient;
    int m_exponent;

    Monomial(int coefficient, int exponent);

    auto Coefficient() const -> int;
    auto Exponent() const -> int;

};

} // namespace solver
