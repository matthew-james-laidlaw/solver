#pragma once

#include <optional>

struct Expression
{
	virtual ~Expression() = default;
};

struct Term : public Expression
{
	int coefficient;
	int exponent;

	Term(int coefficient, int exponent)
		: coefficient(coefficient), exponent(exponent)
	{}
};

struct Unary : public Expression
{
	char op;
	Expression* operand;

	Unary(char op, Expression* operand)
		: op(op), operand(operand)
	{}
};

struct Binary : public Expression
{
	char op;
	Expression* lhs;
	Expression* rhs;

	Binary(char op, Expression* lhs, Expression* rhs)
		: op(op), lhs(lhs), rhs(rhs)
	{}
};

struct Equality : public Expression
{
	Expression* lhs;
	Expression* rhs;

	Equality(Expression* lhs, Expression* rhs)
		: lhs(lhs), rhs(rhs)
	{}
};
