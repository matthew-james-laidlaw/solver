#include "Lex.hpp"

auto Main() -> void
{
	PrintTokens(Lex("(1 + 2)"));
}

auto main() -> int
{
	try
	{
		Main();
	}
	catch (std::exception const& e)
	{
		std::println(std::cerr, "{}", e.what());
		return 1;
	}

	return 0;
}
