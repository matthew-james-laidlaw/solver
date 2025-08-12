#include "Expression.hpp"
#include "Foundation.hpp"
#include "Visitor.hpp"

Literal::Literal(f64 value)
	: m_value(value)
{}

auto Literal::Visit(IVisitor* visitor) -> f64
{
	return visitor->VisitLiteral(this);
}

Variable::Variable(string name)
	: m_name(name)
{}

auto Variable::Visit(IVisitor* visitor) -> f64
{
	return visitor->VisitVariable(this);
}

Addition::Addition(IExpression* left, IExpression* right)
	: m_left(left), m_right(right)
{}

auto Addition::Visit(IVisitor* visitor) -> f64
{
	return visitor->VisitAddition(this);
}

Subtraction::Subtraction(IExpression* left, IExpression* right)
	: m_left(left), m_right(right)
{}

auto Subtraction::Visit(IVisitor* visitor) -> f64
{
	return visitor->VisitSubtraction(this);
}

Multiplication::Multiplication(IExpression* left, IExpression* right)
	: m_left(left), m_right(right)
{}

auto Multiplication::Visit(IVisitor* visitor) -> f64
{
	return visitor->VisitMultiplication(this);
}

Division::Division(IExpression* left, IExpression* right)
	: m_left(left), m_right(right)
{}

auto Division::Visit(IVisitor* visitor) -> f64
{
	return visitor->VisitDivision(this);
}
