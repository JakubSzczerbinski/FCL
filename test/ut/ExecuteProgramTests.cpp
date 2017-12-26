
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <Nodes/LoadNodes.hpp>
#include <Functions/FunctionHelpers.hpp>

#include <Parser/Parser.hpp>
#include <Parser/Lexer.hpp>
#include <Evaluator/Evaluator.hpp>

#include <fakes/functions/Functions.hpp>
#include <fakes/serializers/Serializers.hpp>


namespace fcl
{

template <typename T>
T run(std::string program_text, Functions&& functions, std::string nodeName, int output)
{
	auto program = parse(program_text);
	assert(program.size() != 0);

	Nodes nodes = loadNodes(program, functions, serializers());

	auto it = std::find_if(nodes.begin(), nodes.end(),
		[&](auto& node){ return node->name == nodeName;});
	assert(it != nodes.end());

	auto& node = *it;
	auto result = evaluate<T>(node->sourceEndpoint(output));
	return get_return<T>(result);
}


TEST(ExecuteProgramTests, addTwoNumbers)
{
	std::string program_text = 
R"program(
	int a1 = '10'
	int a2 = '20'
	add = int_add(a1.0, a2.0)
)program";

	auto result = run<int>(program_text, functions(), "add", 0);
	ASSERT_EQ(result, 30);
}

TEST(ExecuteProgramTests, concatStrings)
{
	std::string program_text = 
R"program(
	string a1 = '10'
	string a2 = '20'
	concat = concat(a1.0, a2.0)
)program";

	auto result = run<std::string>(program_text, functions(), "concat", 0);
	ASSERT_EQ(result, "1020");
}

TEST(ExecuteProgramTests, doMath)
{
	std::string program_text = 
R"program(
	int a = '8'
	int b = '4'
	int two = '2'
	pole = int_multiply(a.0, b.0)
	pol_obwod = int_add(a.0, b.0)
	obwod = int_multiply(pol_obwod.0, two.0)
)program";

	{
		auto result = run<int>(program_text, functions(), "pole", 0);
		ASSERT_EQ(result, 32);
	}

	{
		auto result = run<int>(program_text, functions(), "obwod", 0);
		ASSERT_EQ(result, 24);
	}
}

TEST(ExecuteProgramTests, helloMath)
{
	std::string program_text = 
R"program(
	string hello = 'Hello'
	string math = 'Math'
	string space = ' '
	string equals = ' = '
	string plus = ' + '
	int a = '2'
	int b = '3'
	concat1 = concat(hello.0, space.0)
	helloMath = concat(concat1.0, math.0)
	add = int_add(a.0, b.0)
	stringResult = int_to_string(add.0)
	aString = int_to_string(a.0)
	bString = int_to_string(b.0)
	aAndAdd = concat(aString.0, plus.0)
	operation = concat(aAndAdd.0, bString.0)
	aPlusbEq = concat(operation.0, equals.0)
	equation = concat(aPlusbEq.0, stringResult.0)
	helloMathSpace = concat(helloMath.0, space.0)
	result = concat(helloMathSpace.0, equation.0)
	resultLowercase = lowercase(result.0)
	resultUppercase = uppercase(result.0)
)program";

	{
		auto result = run<std::string>(program_text, functions(), "result", 0);
		ASSERT_EQ(result, "Hello Math 2 + 3 = 5");
	}

	{
		auto result = run<std::string>(program_text, functions(), "resultLowercase", 0);
		ASSERT_EQ(result, "hello math 2 + 3 = 5");
	}

	{
		auto result = run<std::string>(program_text, functions(), "resultUppercase", 0);
		ASSERT_EQ(result, "HELLO MATH 2 + 3 = 5");
	}

	{
		auto result = run<std::string>(program_text, functions(), "helloMath", 0);
		ASSERT_EQ(result, "Hello Math");
	}
	{
		auto result = run<std::string>(program_text, functions(), "equation", 0);
		ASSERT_EQ(result, "2 + 3 = 5");
	}

	{
		auto result = run<int>(program_text, functions(), "add", 0);
		ASSERT_EQ(result, 5);
	}
}

TEST(ExecuteProgramTests, moreMath)
{
	std::string program_text = 
R"program(
	int a = '30'
	int b = '42'
	nwd_nww = nwd_nww(a.0, b.0)
	multiple = int_multiply(a.0, b.0)
	nww = int_divide(multiple.0, nwd_nww.0)
)program";

	{
		auto result = run<int>(program_text, functions(), "nwd_nww", 0);
		ASSERT_EQ(result, 6);
	}

	{
		auto result = run<int>(program_text, functions(), "nww", 0);
		ASSERT_EQ(result, 210);
	}

	{
		auto result = run<int>(program_text, functions(), "nwd_nww", 1);
		ASSERT_EQ(result, 210);
	}
}

}
