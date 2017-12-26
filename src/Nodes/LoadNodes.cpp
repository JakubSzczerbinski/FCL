
#include <map>
#include <stdexcept>

#include <boost/optional.hpp>

#include <Parser/Parser.hpp>
#include <Serialization/ISerializer.hpp>
#include <Functions/FunctionHelpers.hpp>
#include <Utils/LambdaVisitor.hpp>

#include "Node.hpp"
#include "LoadNodes.hpp"

namespace fcl
{

Nodes loadNodes(
	const Program& program,
	const Functions& functions,
	const Serializers& serializers)
{
	Nodes nodes;
	auto visitor = make_lambda_visitor<int>(
	[&](const Definition& definition) -> int
	{
		auto it = std::find_if(serializers.cbegin(), serializers.cend(), 
			[&](std::shared_ptr<ISerializer> serializer)
			{
				return serializer->typeName() == definition.type;
			});

		if (it == serializers.end())
			throw std::runtime_error("Unable to deserialize type " + definition.type);

		auto serializer = *it;
		auto function = serializer->deserialize(definition.initialization_string);

		nodes.push_back(std::make_unique<Node>(function, definition.node_name));
		return 0;
	},
	[&](const Declaration& declaration) -> int
	{
		auto& function_name = declaration.constructor.function_name;
		auto it = std::find_if(functions.cbegin(), functions.cend(), 
			[&](std::shared_ptr<IFunction> function)
			{
				return function->name() == function_name;
			});

		if (it == functions.cend())
			throw std::runtime_error("No function named " + function_name);

		auto node = std::make_unique<Node>(*it, declaration.node_name);
		unsigned i = 0;

		for (auto&& link : declaration.constructor.links)
		{
			auto sourceNodeName = link.node_name;
			auto it = std::find_if(nodes.cbegin(), nodes.cend(), 
				[&](const std::unique_ptr<Node>& n) -> bool
				{
					return n->name == sourceNodeName;
				});

			if (it == nodes.cend())
				throw std::runtime_error("No node named " + sourceNodeName);
			
			auto& sourceNode = *it;
			node->setTargetEndpoint(i++, sourceNode->sourceEndpoint(link.index));
		}

		nodes.push_back(std::move(node));
		return 0;
	});

	for (auto&& statement : program)
	{
		boost::apply_visitor(visitor, statement);
	}
	return nodes;
}

Nodes readNodes(
	const std::string& program_text,
	const Functions& functions,
	const Serializers& serializers)
{
	auto program = parse(program_text);
	return loadNodes(program, functions, serializers);
}

}