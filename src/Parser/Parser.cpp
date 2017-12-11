
#include <boost/optional.hpp>
#include "Parser.hpp"
#include "Lexer.hpp"

namespace fcl
{

boost::optional<Link> parse_link(std::vector<Token>::iterator& it, std::vector<Token>::iterator end)
{
	Link result;

	if (it == end || *it != Tokens::ID)
		return boost::none;
	result.node_name = it->contents_;
	it++;

	if (it == end || *it != Tokens::DOT_OP)
		return boost::none;
	it++;

	if (it == end || *it != Tokens::NUM)
		return boost::none;
	result.index = std::stoi(it->contents_.c_str());
	it++;

	return result;
}

boost::optional<NodeConstructor> parse_constructor(std::vector<Token>::iterator& it, std::vector<Token>::iterator end)
{
	NodeConstructor result;

	if (it == end || *it != Tokens::ID)
		return boost::none;
	result.function_name = it->contents_;
	it++;

	if (it == end || *it != Tokens::LEFT_PARENTHESIS)
		return boost::none;
	it++;

	while (it != end)
	{
		auto maybe_link = parse_link(it, end);
		if (maybe_link)
		{
			result.links.push_back(maybe_link.get());

			if (it == end)
				return boost::none;

			if (*it == Tokens::RIGHT_PARENTHESIS)
			{
				it++;
				break;
			}

			if (*it == Tokens::COMMA_OP)
			{
				it++;
				continue;
			}
			return boost::none;
		}

		if (*it == Tokens::RIGHT_PARENTHESIS)
		{
			it++;
			break;
		}

		return boost::none;
	}

	return result;
}

boost::optional<Declaration> parse_declaration(std::vector<Token>::iterator& begin, std::vector<Token>::iterator end)
{
	auto it = begin;
	if (it == end || *it != Tokens::ID)
		return boost::none;
	std::string name = it->contents_;
	it++;

	if (it == end || *it != Tokens::EQUALS_OP)
		return boost::none;
	it++;

	auto maybe_constructor = parse_constructor(it, end);
	if (not maybe_constructor)
		return boost::none;

	begin = it;
	return Declaration{name, maybe_constructor.get()};
}

boost::optional<Definition> parse_definition(std::vector<Token>::iterator& begin, std::vector<Token>::iterator end)
{
	auto it = begin;
	Definition result;
	if (it == end || *it != Tokens::ID)
		return boost::none;
	result.type = it->contents_;
	it++;

	if (it == end || *it != Tokens::ID)
		return boost::none;
	result.node_name = it->contents_;
	it++;

	if (it == end || *it != Tokens::EQUALS_OP)
		return boost::none;
	it++;

	if (it == end || *it != Tokens::STRING)
		return boost::none;
	result.initialization_string = it->contents_;
	it++;

	begin = it;
	return result;
}

Program parse(const std::string& text)
{
	auto tokens = tokenize(text);
	auto it = tokens.begin();

	Program result;
	while (it != tokens.end())
	{
		auto maybe_declaration = parse_declaration(it, tokens.end());
		if (maybe_declaration)
		{
			result.push_back(maybe_declaration.get());
			continue;
		}

		auto maybe_definition = parse_definition(it, tokens.end());
		if (maybe_definition)
		{
			result.push_back(maybe_definition.get());
			continue;
		}

		return {};
	}


	return result;
}

}
