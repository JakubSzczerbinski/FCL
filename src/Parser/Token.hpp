#pragma once 

#include <ostream>
#include <map>

namespace fcl
{

enum Tokens
{
    NONE,
    ID,
    NUM,
    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,
    DOT_OP,
    EQUALS_OP,
    COMMA_OP,
    STRING,
};

inline std::ostream& operator << (std::ostream& os, const Tokens& token)
{
    static std::map<Tokens, std::string> tokensToNames =
    {
        {NONE, "NONE"},
        {ID, "ID"},
        {NUM, "NUM"},
        {LEFT_PARENTHESIS, "LEFT_PARENTHESIS"},
        {RIGHT_PARENTHESIS, "RIGHT_PARENTHESIS"},
        {DOT_OP, "DOT_OP"},
        {EQUALS_OP, "EQUALS_OP"},
        {COMMA_OP, "COMMA_OP"},
        {STRING , "STRING"}
    };
    os << tokensToNames[token];
    return os;
}


class Token
{
public:
    Token(Tokens type, std::string contents) : type_(type), contents_(contents) {}
    Token(Tokens type) : Token(type, "") {}

    Tokens type_;
    std::string contents_;
};

inline std::ostream& operator << (std::ostream& os, const Token& token)
{
    os << token.type_;

    if (token.contents_ == "")
    {
        return os;
    }
    
    os << "(" << token.contents_ << ")";
    return os;
}

inline bool operator == (const Token& lhs, const Token& rhs)
{
    return lhs.type_ == rhs.type_ && lhs.contents_ == rhs.contents_;
}

inline bool operator != (const Token& lhs, const Token& rhs)
{
    return !(lhs == rhs);
}

inline bool operator == (const Token& lhs, Tokens rhs)
{
    return lhs.type_ == rhs;
}

inline bool operator == (Tokens lhs, const Token& rhs)
{
    return rhs == lhs;
}

inline bool operator != (const Token& lhs, Tokens rhs)
{
    return !(lhs == rhs);
}

inline bool operator != (Tokens lhs, const Token& rhs)
{
    return !(lhs == rhs);
}
	
} // namespace fcl
