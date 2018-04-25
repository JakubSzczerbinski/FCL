
#include <ostream>

#include <gtest/gtest.h>

#include <libFCL/Parser/Parser.hpp>


namespace fcl
{

TEST(ParserTests, shouldParseDefinitionsAndDeclaration)
{
	Program result = parse("int l1 = '10'  int l2 = '9'  add_node = add(l1.0, l2.0)");
	ASSERT_EQ(result.size(), 3);
	ASSERT_EQ(result[0].which(), 1);
	ASSERT_EQ(result[1].which(), 1);
	ASSERT_EQ(result[2].which(), 0);
}

TEST(ParserTests, shouldParseDefinition)
{
	Program result = parse("string l1 = '200'");
	ASSERT_EQ(result.size(), 1);
	ASSERT_EQ(result[0].which(), 1);
}

TEST(ParserTests, shouldParseDeclaration)
{
	Program result = parse("gen_node = gen()");
	ASSERT_EQ(result.size(), 1);
	ASSERT_EQ(result[0].which(), 0);
}

}
