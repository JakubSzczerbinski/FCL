
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <AST/LoadProgram.hpp>
#include <Functions/FunctionHelpers.hpp>

#include <fakes/functions/AddFunction.hpp>


namespace fcl
{

TEST(LoadProgramTests, testIntDefinition)
{
	Program program = {Definition{"xd", "int", "123"}};
	AST ast{};
	ProgramLoader loader(ast);
	loader.load_program(program);

	auto nodes = ast.get_nodes();
	ASSERT_EQ(nodes.size(), 1);
	error_code ec = error_code::success;
	auto f_hdl = ast.get_node_function(nodes[0], ec);
	ASSERT_EQ(ec, error_code::success);
	auto func = ast.get_function_from_handle(f_hdl, ec);
	ASSERT_EQ(ec, error_code::success);
	auto ret = get_return<int>(func->call({}), 0);
	ASSERT_EQ(ret, 123);
}

TEST(LoadProgramTests, testStringDefinition)
{
	Program program = {Definition{"xd", "string", "123"}};
	AST ast{};
	ProgramLoader loader(ast);
	loader.load_program(program);

	auto nodes = ast.get_nodes();
	ASSERT_EQ(nodes.size(), 1);
	error_code ec = error_code::success;
	auto f_hdl = ast.get_node_function(nodes[0], ec);
	ASSERT_EQ(ec, error_code::success);
	auto func = ast.get_function_from_handle(f_hdl, ec);
	ASSERT_EQ(ec, error_code::success);
	auto ret = get_return<std::string>(func->call({}), 0);
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

	std::vector<std::unique_ptr<IFunction>> funcs{};
	funcs.emplace_back(std::make_unique<AddFunction>());
	AST ast(std::move(funcs));
	ProgramLoader loader(ast);
	loader.load_program(program);

	auto nodes = ast.get_nodes();
	ASSERT_EQ(nodes.size(), 3);
	
	auto it = std::find_if(nodes.begin(), nodes.end(),
		[&](auto&& node_hdl){ error_code ec; return ast.get_node_pretty_name(node_hdl, ec) == "a2";});
	ASSERT_TRUE(it != nodes.end());

	it = std::find_if(nodes.begin(), nodes.end(),
		[&](auto&& node_hdl){ error_code ec; return ast.get_node_pretty_name(node_hdl, ec) == "a1";});
	ASSERT_TRUE(it != nodes.end());
	
	it = std::find_if(nodes.begin(), nodes.end(),
		[&](auto&& node_hdl){ error_code ec; return ast.get_node_pretty_name(node_hdl, ec) == "add";});
	ASSERT_TRUE(it != nodes.end());

	error_code ec = error_code::success;
	auto link_hdls = ast.get_arg_links(*it, ec);
	ASSERT_EQ(ec, error_code::success);
	ASSERT_EQ(link_hdls.size(), 2);
}

}
