#pragma once

class Expression
{
public:
	virtual ~Expression() = default;
};

class Term : public Expression
{
private:

	int m_coefficient;
	int m_exponent;

public:

	Term(int coefficient, int exponent);

	auto Coefficient() const -> int;
	auto Exponent() const -> int;

};

class Unary : public Expression
{
private:

	char m_op;
	Expression* m_operand;

public:

	Unary(char op, Expression* operand);

	auto Op() const -> char;
	auto Operand() const -> Expression const*;

};

class Binary : public Expression
{
private:

	char m_op;
	Expression* m_lhs;
	Expression* m_rhs;

public:

	Binary(char op, Expression* lhs, Expression* rhs);

	auto Op() const -> char;
	auto Left() const -> Expression const*;
	auto Right() const -> Expression const*;

};

class Equality : public Expression
{
private:

	Expression* m_lhs;
	Expression* m_rhs;

public:

	Equality(Expression* lhs, Expression* rhs);

	auto Left() const -> Expression const*;
	auto Right() const -> Expression const*;

};
