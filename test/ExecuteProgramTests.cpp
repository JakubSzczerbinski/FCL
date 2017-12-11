
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <AST/LoadProgram.hpp>
#include <Functions/FunctionHelpers.hpp>

#include <Parser/Parser.hpp>
#include <Parser/Lexer.hpp>
#include <Evaluator/Evaluator.hpp>

#include <fakes/functions/AddFunction.hpp>


namespace fcl
{

TEST(ExecuteProgramTests, addTwoNumbers)
{
	std::string program_text = 
R"program(
	int a1 = '10'
	int a2 = '20'
	add = int_add(a1.0, a2.0)
)program";
	
	auto program = parse(program_text);
	ASSERT_NE(program.size(), 0);

	std::vector<std::unique_ptr<IFunction>> funcs{};
	funcs.emplace_back(std::make_unique<AddFunction>());
	AST ast(std::move(funcs));
	ProgramLoader loader(ast);
	loader.load_program(program);

	auto nodes = ast.get_nodes();
	ASSERT_EQ(nodes.size(), 3);

	auto it = std::find_if(nodes.begin(), nodes.end(),
		[&](auto&& node_hdl){ error_code ec; return ast.get_node_pretty_name(node_hdl, ec) == "add";});
	ASSERT_TRUE(it != nodes.end());

	auto eval = Evaluator(ast);
	error_code ec = error_code::success;
	auto result = eval.evaluate<int>(*it, 0, ec);
	ASSERT_EQ(ec, error_code::success);
	ASSERT_EQ(get_return<int>(result), 30);

}

}
