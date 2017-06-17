#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <vector>
#include <string>

#include <Lexer.hpp>

class LexerTests : public ::testing::TestWithParam<std::pair<std::string, std::vector<fcl::Token>>>
{
public:
	LexerTests() 
		: lexerPtr_(new fcl::FCLLexer())
	{
		auto param = GetParam();
		inputProgram = param.first;
		lexedTokens = param.second;
	}
	std::string inputProgram;
	std::vector<fcl::Token> lexedTokens;
	std::unique_ptr<fcl::Lexer<fcl::Token>> lexerPtr_;
};

TEST_P (LexerTests, ShouldParseProgramProperly)
{
	auto tokens = lexerPtr_->tokenize(inputProgram);

	ASSERT_EQ(tokens.size(), lexedTokens.size());

	auto size = tokens.size();
	for (int i = 0; i < size; i++)
	{
		EXPECT_EQ(tokens[i], lexedTokens[i]);
	}
}

std::vector<std::pair<std::string, std::vector<fcl::Token>>> getTestArgs()
{
	std::vector<std::pair<std::string, std::vector<fcl::Token>>> res;
	res.push_back(
	{
	R"***(
		ConcatNode concat 
	)***",
	{ {fcl::Tokens::WHITESPACE}
	, {fcl::Tokens::VAR, "ConcatNode"}
	, {fcl::Tokens::WHITESPACE}
	, {fcl::Tokens::VAR, "concat"}
	, {fcl::Tokens::WHITESPACE}}
	});
	return res;
}

INSTANTIATE_TEST_CASE_P(ValidTests,
                        LexerTests,
                        ::testing::ValuesIn(getTestArgs()));


