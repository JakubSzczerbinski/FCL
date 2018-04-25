

#include <gtest/gtest.h>

#include <libFCL/Writer/Writer.hpp>
#include <libFCL/Parser/Parser.hpp>
#include <libFCL/Functions/FunctionHelpers.hpp>
#include <libFCL/Functions/ValueFunction.hpp>
#include <libFCL/Nodes/LoadNodes.hpp>

#include <fakes/functions/AddFunction.hpp>
#include <fakes/serializers/Serializers.hpp>


namespace fcl
{

TEST(WriterTests, writeNodesAndLoadResultInSameNodes)
{
	auto n1 = std::make_unique<Node>(make_value_function<int>(10), "ten");
	auto n2 = std::make_unique<Node>(std::make_shared<AddFunction>(), "add");
	n2->setTargetEndpoint(0, n1->sourceEndpoint(0));
	n2->setTargetEndpoint(1, n1->sourceEndpoint(0));
	std::string contents = writeNodes(make_nodes(std::move(n1), std::move(n2)), serializers());

	auto program = parse(contents);
	auto nodes = loadNodes(program, {std::make_shared<AddFunction>()}, serializers());

	auto it = std::find_if(nodes.first.begin(), nodes.first.end(), 
		[&](auto& node)
		{
			return node->name == "ten" && node->function->name() == "int";
		});

	ASSERT_NE(it, nodes.first.end());

	it = std::find_if(nodes.first.begin(), nodes.first.end(), 
		[&](auto& node)
		{
			return node->name == "add" && node->function->name() == "int_add";
		});
	ASSERT_NE(it, nodes.first.end());
}

}
