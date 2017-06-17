#include <set>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <iostream>

#include <boost/optional.hpp>

enum Tokens
{
	WHITESPACE,
	PRINT,
	DOT_OP,
	EQ_OP,
	STRING,
	NUM,
	VAR
};

std::map<Tokens, std::string> tokensToString =
{ {Tokens::WHITESPACE, "whitespace"}
, {Tokens::PRINT, "print"}
, {Tokens::DOT_OP, "."}
, {Tokens::EQ_OP, "="}
, {Tokens::VAR, "var"}
, {Tokens::STRING, "string"}
, {Tokens::NUM, "num"}
};


class Token
{
public:
	Token(Tokens type, std::string contents) : type_(type), contents_(contents) {}
	Token(Tokens type) : Token(type, "") {}
	std::string contents_;
	Tokens type_;
};

std::ostream& operator << (std::ostream& os, Token token)
{
	os << "(" << tokensToString[token.type_] << (token.contents_ == "" ? "" : " " + token.contents_) << ")";
	return os;
}

template<typename T>
class Lexer
{
public:
	using lexerFun = std::function<boost::optional<T>(std::string::iterator&, std::string::iterator)>;
	Lexer() : Lexer(std::vector<lexerFun>{}) {}
	Lexer(std::vector<lexerFun> funcs) : funcs_(funcs) {}
	std::vector<T> tokenize(std::string& str)
	{
		std::vector<T> result;
		auto it = str.begin();
		while (it != str.end())
		{
			std::string tmp(it, str.end());
			std::cout << tmp << std::endl;
			auto token = tokenize_(it, str.end());
			if (token)
			{
				result.push_back(*token);
			}
			else
			{
				std::cout << "Parse error" << std::endl;
				return result;
			}
		}
		return result;
	}
protected:
	boost::optional<T> tokenize_(std::string::iterator& it, std::string::iterator end)
	{
		for (auto& fun : funcs_)
		{
			auto result = fun(it, end);
			if (result)
			{
				return result;
			}
		}
		return boost::none;
	}
	std::vector<lexerFun> funcs_;
};

class FCLLexer : public Lexer<Token>
{
public:
	FCLLexer()
	{
		for (char c = 'a'; c <= 'z'; c++)
		{
			letters.insert(c);
		}		
		for (char c = 'A'; c <= 'Z'; c++)
		{
			letters.insert(c);
		}
		for (char c = '0'; c <= '9'; c++)
		{
			digits.insert(c);
		}
		lexerFun whitespace = 
		[](std::string::iterator& it, std::string::iterator end) -> boost::optional<Token>
		{
			if (it == end)
			{
				return boost::none;
			}
			if (*it != '\n' && *it != ' ' && *it != '\t')
			{
				return boost::none;
			}
			while (*it == '\n' || *it == ' ' || *it == '\t')
			{
				if (it == end)
				{
					return Token{Tokens::WHITESPACE};
				}
				it++;
			}
			return Token{Tokens::WHITESPACE};
		};
		lexerFun vars = 
		[this](std::string::iterator& it, std::string::iterator end) -> boost::optional<Token>
		{
			if (it == end)
			{
				return boost::none;
			}
			if (letters.count(*it) || *it == '_')
			{
				auto begin = it;
				it++;
				while (letters.count(*it) || digits.count(*it) || *it == '_')
				{
					if (it == end)
					{
						return Token(Tokens::VAR, std::string(begin, it));
					}
					it++;
				}
				return Token(Tokens::VAR, std::string(begin, it));
			}
			return boost::none;
		};
		lexerFun print = 
		[this](std::string::iterator& it, std::string::iterator end) -> boost::optional<Token>
		{
			size_t size = it - end;
			std::string expectedToken = "print";
			if (size >= expectedToken.size())
			{
				std::string possibleToken(it, it + expectedToken.size());
				if (possibleToken == expectedToken)
				{
					it = it + expectedToken.size();
					return Token(Tokens::PRINT);
				}
			}
			return boost::none;
		};
		lexerFun dot_op = 
		[this](std::string::iterator& it, std::string::iterator end) -> boost::optional<Token>
		{
			if (it == end)
			{
				return boost::none;
			}
			if (*it == '.')
			{
				it++;
				return Token(Tokens::DOT_OP);
			}
			return boost::none;
		};
		lexerFun eq_op = 
		[this](std::string::iterator& it, std::string::iterator end) -> boost::optional<Token>
		{
			if (it == end)
			{
				return boost::none;
			}
			if (*it == '=')
			{
				it++;
				return Token(Tokens::EQ_OP);
			}
			return boost::none;
		};
		lexerFun string = 
		[this](std::string::iterator& it, std::string::iterator end) -> boost::optional<Token>
		{
			if (it == end)
			{
				return boost::none;
			}
			if (*it == '\"')
			{
				it++;
				auto begin = it;
				while(*it != '\"')
				{
					if (it == end)
					{
						return boost::none;
					}
					it++;
				}
				return Token(Tokens::STRING, std::string(begin, it++));
			}
			return boost::none;
		};
		lexerFun num =
		[this](std::string::iterator& it, std::string::iterator end) -> boost::optional<Token>
		{
			if (it == end)
			{
				return boost::none;
			}
			if (digits.count(*it))
			{
				auto begin = it;
				it++;
				while (digits.count(*it) || *it == '.')
				{
					if (it == end)
					{
						return boost::none;
					}
					it++;
				}
				return Token(Tokens::NUM, std::string(begin, it));
			}
			return boost::none;
		};
		funcs_ = {whitespace, dot_op, eq_op, print, string, num, vars};
	}
	std::set<char> letters;
	std::set<char> digits;

	
};
