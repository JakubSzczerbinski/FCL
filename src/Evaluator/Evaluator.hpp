
#pragma once

#include <AST/Node.hpp>
#include <Functions/FunctionHelpers.hpp>

namespace fcl
{

inline ReturnVector get_value(Node* node)
{
	ReturnVector args;
	args.resize(node->endpoints.size());

	for (size_t i = 0; i < node->endpoints.size(); i++)
	{
		auto endpoint = node->endpoints[i];
		if (endpoint == null_endpoint)
			throw std::runtime_error("Unable to calculate node, endpoint " + std::to_string(i) + " not connected");

		auto sourceNode = endpoint.node();
		auto endpointIndex = endpoint.index();

		auto sourceNodeReturns = get_value(sourceNode);
		args[i] = std::move(sourceNodeReturns[endpointIndex]);
	}

	return node->function->call(to_arg_vector(args));
}

template<typename T>
inline Return evaluate(const SourceEndpoint& source)
{
	auto node = source.node();
	auto index = source.index();

	auto returnTypes = node->function->outputArgs();
	auto expectedType = bti::type_id<T>();
	if (returnTypes[index] != expectedType)
		throw std::runtime_error("Unable to evaluate endpoint as " + expectedType.pretty_name());

	ReturnVector returns = get_value(node);	
	return std::move(returns[index]);
}

};