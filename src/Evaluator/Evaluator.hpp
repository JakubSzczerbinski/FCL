
#pragma once

#include <Nodes/Node.hpp>
#include <Functions/FunctionHelpers.hpp>

namespace fcl
{

namespace impl
{

ReturnVector get_value(Node* node);

}

template<typename T>
Return evaluate(const SourceEndpoint& source)
{
	auto node = source.node();
	auto index = source.index();

	auto returnTypes = node->function->returnTypes();
	auto expectedType = bti::type_id<T>();
	if (returnTypes[index] != expectedType)
		throw std::runtime_error("Unable to evaluate endpoint as " + expectedType.pretty_name());

	ReturnVector returns = impl::get_value(node);	
	return std::move(returns[index]);
}

};