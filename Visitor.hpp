#pragma once

#include "ExpressionForward.hpp"
#include "Foundation.hpp"

#include <unordered_map>

class IVisitor
{
public:
	virtual ~IVisitor() = default;
	virtual auto VisitLiteral(Literal* expression) -> f64 = 0;
	virtual auto VisitVariable(Variable* expression) -> f64 = 0;
	virtual auto VisitAddition(Addition* expression) -> f64 = 0;
	virtual auto VisitSubtraction(Subtraction* expression) -> f64 = 0;
	virtual auto VisitMultiplication(Multiplication* expression) -> f64 = 0;
	virtual auto VisitDivision(Division* expression) -> f64 = 0;
};

class Evaluator : public IVisitor
{
public:

	Evaluator(std::unordered_map<string, f64> const& variables = {});

	std::unordered_map<string, f64> m_variables;

	auto VisitLiteral(Literal* expression) -> f64 override;
	auto VisitVariable(Variable* expression) -> f64 override;
	auto VisitAddition(Addition* expression) -> f64 override;
	auto VisitSubtraction(Subtraction* expression) -> f64 override;
	auto VisitMultiplication(Multiplication* expression) -> f64 override;
	auto VisitDivision(Division* expression) -> f64 override;

};
