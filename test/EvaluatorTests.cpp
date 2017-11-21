#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Evaluator/Evaluator.hpp>
#include <AST/AST.hpp>

#include <fakes/functions/AddFunction.hpp>
#include <fakes/functions/MultiplyFunction.hpp>


namespace fcl
{

std::string get_function_with_name(const AST& ast, std::string name)
{
	auto functions = ast.get_functions();
	error_code ec = error_code::success;

	for (auto& fun_hdl : functions)
	{
		auto func = ast.get_function_from_handle(fun_hdl, ec);
		if (ec) return "";
		if (func->name() == name) return fun_hdl;
	}

	return "";
}

TEST(EvaluatorTests, shouldEvaluateValueFunction)
{
	AST ast{};
	Evaluator eval(ast);
	error_code ec = error_code::success;
	auto function_hdl = ast.create_value_function<int>(10, ec);
	ASSERT_EQ(ec, error_code::success);
	auto node_hdl = ast.create_node(function_hdl, ec);
	ASSERT_EQ(ec, error_code::success);
	auto result = eval.evaluate<int>(node_hdl, 0, ec);
	ASSERT_EQ(ec, error_code::success);
	ASSERT_EQ(10, get_return<int>(result));
}


TEST(EvaluatorTests, shouldAddTwoNumbers)
{
	std::vector<std::unique_ptr<IFunction>> funcs{};
	funcs.emplace_back(std::make_unique<AddFunction>());
	
	AST ast{std::move(funcs)};
	Evaluator eval(ast);
	error_code ec = error_code::success;

	auto funcs_vec = ast.get_functions();
	ASSERT_EQ(funcs_vec.size(), 1);

	auto add_func_hdl = funcs_vec[0];
	auto add_function = ast.get_function_from_handle(add_func_hdl, ec);
	ASSERT_EQ("int_add", add_function->name());

	auto value_func_hdl = ast.create_value_function<int>(14, ec);
	ASSERT_EQ(ec, error_code::success);
	
	auto value_node_hdl = ast.create_node(value_func_hdl, ec);
	auto add_node_hdl = ast.create_node(add_func_hdl, ec);
	ASSERT_EQ(ec, error_code::success);

	ast.create_link(value_node_hdl, 0, add_node_hdl, 0, ec);
	ASSERT_EQ(ec, error_code::success);

	ast.create_link(value_node_hdl, 0, add_node_hdl, 1, ec);
	ASSERT_EQ(ec, error_code::success);
	
	auto result = eval.evaluate<int>(add_node_hdl, 0, ec);
	ASSERT_EQ(ec, error_code::success);
	ASSERT_EQ(28, get_return<int>(result));
}

TEST(EvaluatorTests, shouldAddFourNumbers)
{
	std::vector<std::unique_ptr<IFunction>> funcs{};
	funcs.emplace_back(std::make_unique<AddFunction>());
	
	AST ast{std::move(funcs)};
	Evaluator eval(ast);
	error_code ec = error_code::success;

	auto funcs_vec = ast.get_functions();
	ASSERT_EQ(funcs_vec.size(), 1);

	auto add_func_hdl = funcs_vec[0];
	auto add_function = ast.get_function_from_handle(add_func_hdl, ec);
	ASSERT_EQ("int_add", add_function->name());

	auto value_func_hdl = ast.create_value_function<int>(14, ec);
	ASSERT_EQ(ec, error_code::success);
	
	auto value_node_hdl = ast.create_node(value_func_hdl, ec);
	auto add_node_hdl = ast.create_node(add_func_hdl, ec);
	auto snd_add_node_hdl = ast.create_node(add_func_hdl, ec);
	ASSERT_EQ(ec, error_code::success);

	ast.create_link(value_node_hdl, 0, add_node_hdl, 0, ec);
	ASSERT_EQ(ec, error_code::success);

	ast.create_link(value_node_hdl, 0, add_node_hdl, 1, ec);
	ASSERT_EQ(ec, error_code::success);


	ast.create_link(add_node_hdl, 0, snd_add_node_hdl, 0, ec);
	ASSERT_EQ(ec, error_code::success);

	ast.create_link(add_node_hdl, 0, snd_add_node_hdl, 1, ec);
	ASSERT_EQ(ec, error_code::success);


	auto result1 = eval.evaluate<int>(add_node_hdl, 0, ec);
	ASSERT_EQ(ec, error_code::success);
	ASSERT_EQ(28, get_return<int>(result1));

	auto result2 = eval.evaluate<int>(snd_add_node_hdl, 0, ec);
	ASSERT_EQ(ec, error_code::success);
	ASSERT_EQ(56, get_return<int>(result2));
}

TEST(EvaluatorTests, shouldDoArithmetic)
{
	std::vector<std::unique_ptr<IFunction>> funcs{};
	funcs.emplace_back(std::make_unique<AddFunction>());
	funcs.emplace_back(std::make_unique<MultiplyFunction>());

	AST ast{std::move(funcs)};
	Evaluator eval(ast);
	error_code ec = error_code::success;

	auto funcs_vec = ast.get_functions();
	ASSERT_EQ(funcs_vec.size(), 2);

	auto add_func_hdl = get_function_with_name(ast, "int_add");
	ASSERT_NE(add_func_hdl, "");
	auto multiply_func_hdl = get_function_with_name(ast, "int_multiply");
	ASSERT_NE(multiply_func_hdl, "");

	auto value_func_hdl = ast.create_value_function<int>(14, ec);
	ASSERT_EQ(ec, error_code::success);
	
	auto value_node_hdl = ast.create_node(value_func_hdl, ec);
	auto add_node_hdl = ast.create_node(add_func_hdl, ec);
	auto multiply1_node_hdl = ast.create_node(multiply_func_hdl, ec);
	auto multiply2_node_hdl = ast.create_node(multiply_func_hdl, ec);
	ASSERT_EQ(ec, error_code::success);

	ast.create_link(value_node_hdl, 0, add_node_hdl, 0, ec);
	ASSERT_EQ(ec, error_code::success);

	ast.create_link(value_node_hdl, 0, add_node_hdl, 1, ec);
	ASSERT_EQ(ec, error_code::success);


	ast.create_link(value_node_hdl, 0, multiply1_node_hdl, 0, ec);
	ASSERT_EQ(ec, error_code::success);

	ast.create_link(add_node_hdl, 0, multiply1_node_hdl, 1, ec);
	ASSERT_EQ(ec, error_code::success);


	ast.create_link(multiply1_node_hdl, 0, multiply2_node_hdl, 0, ec);
	ASSERT_EQ(ec, error_code::success);

	ast.create_link(add_node_hdl, 0, multiply2_node_hdl, 1, ec);
	ASSERT_EQ(ec, error_code::success);


	auto result1 = eval.evaluate<int>(add_node_hdl, 0, ec);
	ASSERT_EQ(ec, error_code::success);
	ASSERT_EQ(28, get_return<int>(result1));

	auto result2 = eval.evaluate<int>(multiply2_node_hdl, 0, ec);
	ASSERT_EQ(ec, error_code::success);
	ASSERT_EQ(10976, get_return<int>(result2));

	auto result3 = eval.evaluate<int>(multiply1_node_hdl, 0, ec);
	ASSERT_EQ(ec, error_code::success);
	ASSERT_EQ(392, get_return<int>(result3));
}

}
