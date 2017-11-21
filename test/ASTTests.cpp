
#include <algorithm>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <AST/AST.hpp>
#include <Common/Logger.hpp>
#include <Functions/FunctionHelpers.hpp>

#include <fakes/functions/AddFunction.hpp>
#include <fakes/functions/MultiplyFunction.hpp>

namespace fcl
{

TEST(ASTTests, hasFunctions)
{
	std::vector<std::unique_ptr<IFunction>> funcs{};
	funcs.emplace_back(std::make_unique<AddFunction>());
	funcs.emplace_back(std::make_unique<MultiplyFunction>());
	AST ast(std::move(funcs));

	auto function_hdls = ast.get_functions();
	ASSERT_EQ(function_hdls.size(), 2);

	auto it = std::find_if(function_hdls.begin(), function_hdls.end(),
		[&ast](std::string hdl) -> bool
		{
			error_code ec = error_code::success;
			auto func = ast.get_function_from_handle(hdl, ec);
			if (ec) return false;
			return "int_add" == func->name();
		});
	ASSERT_NE(it, function_hdls.end());

	it = std::find_if(function_hdls.begin(), function_hdls.end(),
		[&ast](std::string hdl) -> bool
		{
			error_code ec = error_code::success;
			auto name = ast.get_function_pretty_name(hdl, ec);
			if (ec) return false;
			return "int_multiply" == name;
		});
	ASSERT_NE(it, function_hdls.end());
}

TEST(ASTTests, hasTypeInformation)
{
	std::vector<std::unique_ptr<IFunction>> funcs{};
	funcs.emplace_back(std::make_unique<AddFunction>());
	AST ast(std::move(funcs));

	auto func_hdl = ast.get_functions()[0];
	error_code ec = error_code::success;

	auto ret_types = ast.get_return_types(func_hdl, ec);
	ASSERT_EQ(ec, error_code::success);
	ASSERT_EQ(ret_types.size(), 1);

	ASSERT_TRUE(ast.has_type<int>(ret_types[0], ec));
	ASSERT_EQ(ec, error_code::success);

	auto arg_types = ast.get_arg_types(func_hdl, ec);
	ASSERT_EQ(ec, error_code::success);
	ASSERT_EQ(arg_types.size(), 2);

	ASSERT_TRUE(ast.has_type<int>(arg_types[0], ec));
	ASSERT_EQ(ec, error_code::success);

	ASSERT_TRUE(ast.has_type<int>(arg_types[1], ec));
	ASSERT_EQ(ec, error_code::success);
}

TEST(ASTTests, canCreateValueFunction)
{
	AST ast;
	error_code ec = error_code::success;

	auto empty_func_hdls = ast.get_functions();
	ASSERT_EQ(empty_func_hdls.size(), 0);

	auto value_func_hdl = ast.create_value_function<std::string>("my string", ec);
	ASSERT_EQ(ec, error_code::success);

	auto func_hdls = ast.get_functions();
	ASSERT_EQ(func_hdls.size(), 1);
	ASSERT_EQ(func_hdls[0], value_func_hdl);

	auto ret_types = ast.get_return_types(value_func_hdl, ec);
	ASSERT_EQ(ec, error_code::success);
	ASSERT_EQ(ret_types.size(), 1);

	ASSERT_TRUE(ast.has_type<std::string>(ret_types[0], ec));
	ASSERT_EQ(ec, error_code::success);

	auto arg_types = ast.get_arg_types(value_func_hdl, ec);
	ASSERT_EQ(ec, error_code::success);
	ASSERT_EQ(arg_types.size(), 0);
}

TEST(ASTTests, canCreateAndDeleteNode)
{
	AST ast;
	error_code ec = error_code::success;

	auto value_func_hdl = ast.create_value_function<std::string>("my string", ec);
	ASSERT_EQ(ec, error_code::success);

	auto node_hdls = ast.get_nodes();
	ASSERT_EQ(node_hdls.size(), 0);

	auto value_node_hdl = ast.create_node(value_func_hdl, ec);
	ASSERT_EQ(ec, error_code::success);

	node_hdls = ast.get_nodes();
	ASSERT_EQ(node_hdls.size(), 1);

	bool deleted = ast.delete_node(value_node_hdl, ec);
	ASSERT_TRUE(deleted);
	ASSERT_EQ(ec, error_code::success);

	node_hdls = ast.get_nodes();
	ASSERT_EQ(node_hdls.size(), 0);
}

}  // namespace fcl