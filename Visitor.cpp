#include "Expression.hpp"
#include "Visitor.hpp"

Evaluator::Evaluator(std::unordered_map<string, f64> const& variables)
	: m_variables(variables)
{}

auto Evaluator::VisitLiteral(Literal* expression) -> f64 
{
	return expression->m_value;
}

auto Evaluator::VisitVariable(Variable* expression) -> f64 
{
	if (!m_variables.contains(expression->m_name))
	{
		throw std::runtime_error(std::format("evaluator error: unknown variable '{}'", expression->m_name));
	}
	return m_variables[expression->m_name];
}

auto Evaluator::VisitAddition(Addition* expression) -> f64 
{
	auto left = expression->m_left->Visit(this);
	auto right = expression->m_right->Visit(this);
	return left + right;
}

auto Evaluator::VisitSubtraction(Subtraction* expression) -> f64 
{
	auto left = expression->m_left->Visit(this);
	auto right = expression->m_right->Visit(this);
	return left - right;
}

auto Evaluator::VisitMultiplication(Multiplication* expression) -> f64 
{
	auto left = expression->m_left->Visit(this);
	auto right = expression->m_right->Visit(this);
	return left * right;
}

auto Evaluator::VisitDivision(Division* expression) -> f64 
{
	auto left = expression->m_left->Visit(this);
	auto right = expression->m_right->Visit(this);
	return left / right;
}
