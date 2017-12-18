
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <AST/LoadProgram.hpp>
#include <Functions/FunctionHelpers.hpp>

#include <fakes/functions/AddFunction.hpp>
#include <fakes/serializers/Serializers.hpp>


namespace fcl
{

TEST(LoadProgramTests, testIntDefinition)
{
	Program program = {Definition{"xd", "int", "123"}};
	Nodes nodes = loadNodes(program, {}, serializers());

	ASSERT_EQ(nodes.size(), 1);
	auto ret = get_return<int>(nodes[0]->function->call({}), 0);
	ASSERT_EQ(ret, 123);
}

TEST(LoadProgramTests, testStringDefinition)
{
	Program program = {Definition{"xd", "string", "123"}};
	Nodes nodes = loadNodes(program, {}, serializers());

	ASSERT_EQ(nodes.size(), 1);
	auto ret = get_return<std::string>(nodes[0]->function->call({}), 0);
	ASSERT_EQ(ret, "123");
}

TEST(LoadProgramTests, testTwoDeclarationAndAdditions)
{
	Program program = 
	{ Definition{"a1", "int", "12"} 
	, Definition{"a2", "int", "13"}
	, Declaration{"add", 
		NodeConstructor{"int_add", 
			{{"a2", 0}, {"a1", 0}}}
		} 
	};
	Nodes nodes = loadNodes(program, {std::make_shared<AddFunction>()}, serializers());
	ASSERT_EQ(nodes.size(), 3);
	
	auto it = std::find_if(nodes.begin(), nodes.end(),
		[&](auto&& node){ return node->name == "a2";});
	ASSERT_TRUE(it != nodes.end());

	it = std::find_if(nodes.begin(), nodes.end(),
		[&](auto&& node){ return node->name == "a1";});
	ASSERT_TRUE(it != nodes.end());
	
	it = std::find_if(nodes.begin(), nodes.end(),
		[&](auto&& node){ return node->name == "add";});
	ASSERT_TRUE(it != nodes.end());

	auto endpoint = (*it)->endpoints[0];
	ASSERT_NE(endpoint, null_endpoint);
	ASSERT_EQ(endpoint.node()->name, "a2");

	endpoint = (*it)->endpoints[1];
	ASSERT_NE(endpoint, null_endpoint);
	ASSERT_EQ(endpoint.node()->name, "a1");
}

}
