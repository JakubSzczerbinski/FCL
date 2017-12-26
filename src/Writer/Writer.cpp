
#include <map>
#include <queue>
#include <vector>
#include <algorithm>

#include "Writer.hpp"

namespace fcl
{

std::string definition(Node* node, Serializers serializers)
{
	auto it = std::find_if(serializers.begin(), serializers.end(),
		[&](std::shared_ptr<ISerializer> serializer)
		{
			return serializer->typeName() == node->function->name();
		});
	auto serializer = *it;
	auto result = node->function->call({});
	std::string serialized = serializer->serialize(result[0].get());
	return node->function->name() + " " + node->name + " = \'" + serialized + '\'';
}

std::string declaration(Node* node)
{
	std::string result;
	result += node->name;
	result += " = ";
	result += node->function->name();
	result += "(";
	for (size_t i = 0; i < node->endpoints.size(); i++)
	{
		auto&& endpoint = node->endpoints[i];
		if (i != 0)
			result += ", ";

		result += endpoint.node()->name;
		result += ".";
		result += std::to_string(endpoint.index());
	}
	result += ")";
	return result;
}

bool is_value_node(Node* node)
{
	size_t inputs = node->function->argTypes().size();
	size_t outputs = node->function->returnTypes().size();
	return (inputs == 0 && outputs == 1);
}

std::string writeNodes(const Nodes& nodes, Serializers serializers)
{
	std::map<Node*, std::vector<Node*>> dependentNodes;
	std::map<Node*, unsigned> unsatisfiedDependencies;
	std::queue<Node*> queue;
	std::string result;
	for (size_t i = 0; i < nodes.size(); i++)
	{
		auto&& node = nodes[i];

		unsigned inputs = node->function->argTypes().size();
		unsatisfiedDependencies[node.get()] = inputs;
		if (inputs == 0)
			queue.push(node.get());

		for (auto&& endpoint : node->endpoints)
		{
			if (endpoint.node() == nullptr)
				throw std::runtime_error("Empty node target endpoint, unable to write to file.");

			dependentNodes[endpoint.node()].push_back(node.get());
		}
	}

	while (not queue.empty())
	{
		auto node = queue.front();
		queue.pop();
		result += (is_value_node(node) ? definition(node, serializers) : declaration(node)) + '\n';

		for (auto&& dependentNode : dependentNodes[node])
		{
			auto dependecies = unsatisfiedDependencies[dependentNode];
			if (dependecies == 0)
				throw std::logic_error("Trying to satisfy node dependecy while all satisfied");

			if (dependecies == 1)
				queue.push(dependentNode);

			unsatisfiedDependencies[dependentNode]--;
		}
	}

	return result;
}
}