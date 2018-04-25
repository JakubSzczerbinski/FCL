
#include <gtest/gtest.h>

#include <libFCL/Functions/ValueFunction.hpp>
#include <libFCL/Evaluator/Evaluator.hpp>
#include <libFCL/Nodes/Node.hpp>

#include <fakes/functions/AddFunction.hpp>
#include <fakes/functions/MultiplyFunction.hpp>


namespace fcl
{

TEST(EvaluatorTests, shouldEvaluateValueFunction)
{
	Nodes nodes;
	nodes.push_back(std::make_unique<Node>(make_value_function<int>(10), "returns_10"));
	auto result = evaluate(nodes[0]->sourceEndpoint(0));
	ASSERT_EQ(10, get_return<int>(result));
}


TEST(EvaluatorTests, shouldAddTwoNumbers)
{
	auto value_node = std::make_unique<Node>(make_value_function<int>(14), "returns_14");
	auto add_node = std::make_unique<Node>(std::make_shared<AddFunction>(), "add");
	add_node->setTargetEndpoint(0, value_node->sourceEndpoint(0));
	add_node->setTargetEndpoint(1, value_node->sourceEndpoint(0));

	auto result = evaluate(add_node->sourceEndpoint(0));
	ASSERT_EQ(28, get_return<int>(result));
}

TEST(EvaluatorTests, shouldAddFourNumbers)
{
	auto value_node = std::make_unique<Node>(make_value_function<int>(14), "returns_14");

	auto add_node1 = std::make_unique<Node>(std::make_shared<AddFunction>(), "add1");
	add_node1->setTargetEndpoint(0, value_node->sourceEndpoint(0));
	add_node1->setTargetEndpoint(1, value_node->sourceEndpoint(0));

	auto add_node2 = std::make_unique<Node>(std::make_shared<AddFunction>(), "add2");
	add_node2->setTargetEndpoint(0, add_node1->sourceEndpoint(0));
	add_node2->setTargetEndpoint(1, add_node1->sourceEndpoint(0));

	auto result1 = evaluate(add_node1->sourceEndpoint(0));
	ASSERT_EQ(28, get_return<int>(result1));

	auto result2 = evaluate(add_node2->sourceEndpoint(0));
	ASSERT_EQ(56, get_return<int>(result2));
}

TEST(EvaluatorTests, shouldDoArithmetic)
{
	auto value_node = std::make_unique<Node>(make_value_function<int>(14), "returns_14");

	auto add_node = std::make_unique<Node>(std::make_shared<AddFunction>(), "add");
	add_node->setTargetEndpoint(0, value_node->sourceEndpoint(0));
	add_node->setTargetEndpoint(1, value_node->sourceEndpoint(0));

	auto multiply_node1 = std::make_unique<Node>(std::make_shared<MultiplyFunction>(), "multiply1");
	multiply_node1->setTargetEndpoint(0, add_node->sourceEndpoint(0));
	multiply_node1->setTargetEndpoint(1, value_node->sourceEndpoint(0));

	auto multiply_node2 = std::make_unique<Node>(std::make_shared<MultiplyFunction>(), "multiply2");
	multiply_node2->setTargetEndpoint(0, multiply_node1->sourceEndpoint(0));
	multiply_node2->setTargetEndpoint(1, add_node->sourceEndpoint(0));

	auto result1 = evaluate(add_node->sourceEndpoint(0));
	ASSERT_EQ(28, get_return<int>(result1));

	auto result2 = evaluate(multiply_node2->sourceEndpoint(0));
	ASSERT_EQ(10976, get_return<int>(result2));

	auto result3 = evaluate(multiply_node1->sourceEndpoint(0));
	ASSERT_EQ(392, get_return<int>(result3));
}

}
