
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <AST.hpp>
#include <Logger.hpp>
#include <FunctionHelpers.hpp>

namespace fcl
{

std::unique_ptr<IFunction> addFun = Input<int, int>::Output<int>::makeFunction(
	[](int rhs, int lhs)
	{
		return new int(rhs + lhs);
	}, "add");

int* multiply(int rhs, int lhs)
{
	return new int(rhs + lhs);
}

std::unique_ptr<IFunction> multiplyFun = Input<int, int>::Output<int>::makeFunction(
	std::function<int*(int, int)>(multiply), "multiply");

bool hasFunctionWithName(
	const AST& ast,
	const std::string& pretty_name)
{
	auto functions = ast.get_functions();
	auto it = std::find_if(functions.begin(), functions.end(), [&](FunctionHandle hdl) -> bool
	{
		error_code ec;
		auto name = ast.get_function_pretty_name(hdl, ec);
		if (ec)
		{
			return false;
		}
		return name == pretty_name;
	});
	return it != functions.end();
}

// TEST(ASTTests, hasAddedFunctions)
// {
// 	Logger logger{"fcl", "ASTTests"};
// 	AST ast({std::move(multiplyFun), addFun.get()});
// 	ASSERT_TRUE(hasFunctionWithName(ast, "add"));
// 	ASSERT_TRUE(hasFunctionWithName(ast, "multiply"));
// }


}  // namespace fcl