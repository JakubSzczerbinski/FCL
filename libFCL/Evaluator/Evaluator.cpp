

#include "Evaluator.hpp"

namespace fcl
{

namespace impl
{

ReturnVector get_value(Node* node)
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

}

}
