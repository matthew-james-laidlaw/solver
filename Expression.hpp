#pragma once

#include "Foundation.hpp"
#include "ExpressionForward.hpp"
#include "VisitorForward.hpp"

#include <functional>

class IExpression
{
public:

	virtual ~IExpression() = default;
	virtual auto Visit(IVisitor* visitor) -> f64 = 0;

};

class Literal : public IExpression
{
public:

	f64 m_value;
	Literal(f64 value);
	auto Visit(IVisitor* visitor) -> f64 override;

};

class Variable : public IExpression
{
public:

	string m_name;
	Variable(string name);
	auto Visit(IVisitor* visitor) -> f64 override;

};

class Addition : public IExpression
{
public:

	IExpression* m_left;
	IExpression* m_right;
	Addition(IExpression* left, IExpression* right);
	auto Visit(IVisitor* visitor) -> f64 override;

};

class Subtraction : public IExpression
{
public:

	IExpression* m_left;
	IExpression* m_right;
	Subtraction(IExpression* left, IExpression* right);
	auto Visit(IVisitor* visitor) -> f64 override;

};

class Multiplication : public IExpression
{
public:

	IExpression* m_left;
	IExpression* m_right;
	Multiplication(IExpression* left, IExpression* right);
	auto Visit(IVisitor* visitor) -> f64 override;

};

class Division : public IExpression
{
public:

	IExpression* m_left;
	IExpression* m_right;
	Division(IExpression* left, IExpression* right);
	auto Visit(IVisitor* visitor) -> f64 override;

};
