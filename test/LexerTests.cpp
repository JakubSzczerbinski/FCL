
#include <ostream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <Lexer.hpp>


namespace fcl
{

TEST(LexerTests, shouldParseNumbers)
{
	std::vector<Token> result = tokenize("1223 12312312");

	ASSERT_EQ(result.size(), 2);

	ASSERT_EQ(result[0].type_, Tokens::NUM);
	ASSERT_EQ(result[0].contents_, "1223");

	ASSERT_EQ(result[1].type_, Tokens::NUM);	
	ASSERT_EQ(result[1].contents_, "12312312");
}

TEST(LexerTests, shouldParseID)
{
	std::vector<Token> result = tokenize("a124  A_D__	xxx	");

	ASSERT_EQ(result.size(), 3);

	ASSERT_EQ(result[0].type_, Tokens::ID);
	ASSERT_EQ(result[0].contents_, "a124");

	ASSERT_EQ(result[1].type_, Tokens::ID);	
	ASSERT_EQ(result[1].contents_, "A_D__");

	ASSERT_EQ(result[2].type_, Tokens::ID);	
	ASSERT_EQ(result[2].contents_, "xxx");
}

TEST(LexerTests, shouldParse)
{
	std::vector<Token> expected = 
	{
		{Tokens::ID, "a1"},
		{Tokens::LEFT_PARENTHESIS},
		{Tokens::NUM, "24"},
		{Tokens::RIGHT_PARENTHESIS}
	};
	std::vector<Token> result = tokenize("a11(24)");

	ASSERT_EQ(result, expected);
}

}