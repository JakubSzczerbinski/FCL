
#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "Token.hpp"

namespace fcl
{

bool isLetter(char a)
{
    return ('a' <= a && 'z' >= a) || ('A' <= a && 'Z' >= a);
}

bool isWhitespace(char a)
{
    return a == ' ' || a == '\n' || a =='\t';
}

bool isNumber(char a)
{
    return '0' <= a && a <= '9';
}

bool isIdCharacter(char a)
{
    return isLetter(a) || isNumber(a) || a == '_';
}

bool parseID(std::vector<Token>& tokens, std::string::const_iterator& begin, std::string::const_iterator end)
{
    char firstCharacter = *begin;
    if (!isLetter(firstCharacter))
    {
        return false;
    }
    std::string result;
    result += firstCharacter;
    begin++;
    while(isIdCharacter(*begin) && begin != end)
    {
        result += *begin;
        begin++;
    }
    auto idToken = Token(Tokens::ID, result);
    tokens.push_back(idToken);
    return true;
}

bool parseNum(std::vector<Token>& tokens, std::string::const_iterator& begin, std::string::const_iterator end)
{
    char firstCharacter = *begin;
    if(!isNumber(firstCharacter))
    {
        return false;
    }

    std::string result;
    result += firstCharacter;
    begin++;
    while(isNumber(*begin) && begin != end)
    {
        result += *begin;
        begin++;
    }
    auto numToken = Token(Tokens::NUM, result);
    tokens.push_back(numToken);
    return true;


}

bool parseOp(std::vector<Token>& tokens, std::string::const_iterator& begin, std::string::const_iterator end)
{

    char firstCharacter = *begin;
    Token opToken = Token{Tokens::NONE};

    if(firstCharacter == '=')
    {
        opToken = Token{Tokens::EQUALS_OP};
    }

    if(firstCharacter == '.')
    {
        opToken = Token{Tokens::DOT_OP};
    }

    if(firstCharacter == ',')
    {
        opToken = Token{Tokens::COMMA_OP};
    }

    if(firstCharacter == '(')
    {
        opToken = Token{Tokens::LEFT_PARENTHESIS};
    }

    if(firstCharacter == ')')
    {
        opToken = Token{Tokens::RIGHT_PARENTHESIS};
    }

    if(opToken.type_ == Tokens::NONE) return false;
    begin++;
    tokens.push_back(opToken);
    return true;

} 

bool parseOp2(std::vector<Token>& tokens, std::string::const_iterator& begin, std::string::const_iterator end)
{

    char firstCharacter = *begin;

    static std::map<char, Token> charTokenMap =
    {
        {'=', Token(Tokens::EQUALS_OP)},
        {'.', Token(Tokens::DOT_OP)},
        {',', Token(Tokens::COMMA_OP)},
        {')', Token(Tokens::LEFT_PARENTHESIS)},
        {'(', Token(Tokens::RIGHT_PARENTHESIS)}
    };

    auto it = charTokenMap.find(firstCharacter);

    if (it == charTokenMap.end())
    {
        return false;
    }

    tokens.push_back(it->second);
    begin++;
    return true;

}


bool parseString(std::vector<Token>& tokens, std::string::const_iterator& begin, std::string::const_iterator end)
{
    char firstCharacter = *begin;



    if (firstCharacter != '\'')
        return false;

    auto it = begin + 1;
    if (it == end)
        return false;

    Token result(Tokens::STRING);
    while (*it != '\'')
    {
        result.contents_.push_back(*it);
        it++;
        if (it == end)
            return false;
    }
    begin = it + 1;
    tokens.push_back(result);
    return true;
}

std::vector<Token> tokenize(const std::string& text)
{
    std::vector<Token> result;

    auto it = text.begin();
    auto end = text.end();
    while (it != text.end())
    {
        if (isWhitespace(*it))
        {
            it++;
            continue;
        }

        if (parseID(result, it, end))
        {
            continue;
        }

        if (parseNum(result, it, end))
        {
            continue;
        }

        if (parseOp(result, it, end))
        {
            continue;
        }

        if (parseString(result, it, end))
        {
            continue;
        }

        std::cout << "Lexer error: Unable to parse character " << *it 
            << " with ASCII code " << static_cast<int>(*it) << std::endl;

        return {};

    }

    return result;
}

} // namespace