#pragma once

#include "Foundation.hpp"

enum class Kind
{
	Number, Variable,
	Plus, Minus, Multiplies, Divides,
	OpenParenthesis, ClosedParenthesis,
};

struct Token
{
	Kind kind;
	string lexeme;
};

template <typename Predicate>
auto Consume(string& state, Predicate predicate) -> string
{
	auto end = std::find_if_not(state.begin(), state.end(), [&](u8 c) {
		return predicate(c);
	});
	auto lexeme = string(state.begin(), end);
	state.erase(state.begin(), end);
	return lexeme;
}

auto ConsumeWhitespace(string& state) -> void
{
	Consume(state, [](u8 c) { return std::isspace(c); });
}

auto ConsumeNumber(string& state) -> Token
{
	auto lexeme = Consume(state, [](u8 c) { return std::isdigit(c); });
	return {
		.kind = Kind::Number,
		.lexeme = lexeme,
	};
}

auto ConsumeVariable(string& state) -> Token
{
	auto lexeme = Consume(state, [](u8 c) { return std::isalpha(c); });
	return {
		.kind = Kind::Variable,
		.lexeme = lexeme,
	};
}

auto ConsumeOperator(string& state) -> Token
{
	Token token;
	switch (state[0])
	{
	case '+': token = { .kind = Kind::Plus, .lexeme = "+" }; break;
	case '-': token = { .kind = Kind::Minus, .lexeme = "-" }; break;
	case '*': token = { .kind = Kind::Multiplies, .lexeme = "*" }; break;
	case '/': token = { .kind = Kind::Divides, .lexeme = "/" }; break;
	}
	state.erase(0, 1);
	return token;
}

auto LexExpression(string& state, std::vector<Token>& tokens) -> void
{
	if (std::isdigit(state[0]))
	{
		tokens.push_back({ ConsumeNumber(state) });
	}
	else if (std::isalpha(state[0]))
	{
		tokens.push_back({ ConsumeVariable(state) });
	}
	else if (string("+-*/").contains(state[0]))
	{
		tokens.push_back({ ConsumeOperator(state) });
	}
	else if (state[0] == '(')
	{
		tokens.push_back({ .kind = Kind::OpenParenthesis, .lexeme = "(" });
		state.erase(0, 1);
	}
	else if (state[0] == ')')
	{
		tokens.push_back({ .kind = Kind::ClosedParenthesis, .lexeme = ")" });
		state.erase(0, 1);
	}
	else
	{
		throw std::runtime_error(std::format("lexer error: unexpected character '{}'", state[0]));
	}
}

auto Lex(string& state, std::vector<Token>& tokens) -> void
{
	ConsumeWhitespace(state);
	while (!state.empty())
	{
		LexExpression(state, tokens);
		ConsumeWhitespace(state);
	}
}

auto Lex(string const& source) -> std::vector<Token>
{
	auto state = source;
	auto tokens = std::vector<Token>();
	Lex(state, tokens);
	return tokens;
}

auto PrintTokens(std::vector<Token> const& tokens) -> void
{
	for (auto const& token : tokens)
	{
		std::println("{{ .kind = {}, .lexeme = {} }}", static_cast<int>(token.kind), token.lexeme);
	}
}
