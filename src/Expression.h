#pragma once

#include <optional>

struct Expression
{
	virtual ~Expression() = default;
};

struct Term : public Expression
{
	int number;
	std::optional<char> variable;

	Term(int number, std::optional<char> variable)
		: number(number), variable(variable)
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
