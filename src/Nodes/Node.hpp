#pragma once

#include <string>
#include <memory>
#include <vector>

#include <Functions/IFunction.hpp>

namespace fcl
{

class Node;

class Endpoint
{
public:
	Endpoint() : Endpoint(nullptr, 0) {}
	Endpoint(Node* node, unsigned index)
	: node_(node), index_(index) {}
	Node* node() const { return node_; }
	unsigned index() const { return index_; }
private:
	Node* node_;
	unsigned index_;
};

class SourceEndpoint : public Endpoint
{
public:
	SourceEndpoint() : Endpoint() {}
	SourceEndpoint(Node* node_, unsigned index_)
	: Endpoint(node_, index_) {}
};

inline bool operator== (const Endpoint& lhs, const Endpoint& rhs)
{
	return lhs.node() == rhs.node() and lhs.index() == rhs.index();
}

inline bool operator!= (const Endpoint& lhs, const Endpoint& rhs)
{
	return not (lhs == rhs);
}

static const Endpoint null_endpoint(nullptr, 0);


class Node
{
public:
	Node (std::shared_ptr<IFunction> function_, std::string name_)
	: name(name_), function(function_)
	{
		endpoints.resize(function->argTypes().size());
	}

	SourceEndpoint sourceEndpoint(unsigned index)
	{
		if (index >= function->returnTypes().size())
		{
			throw std::runtime_error(
				"Requesting invalid endpoint, index " + std::to_string(index) + " exceeding number of returns.");
		}
		return SourceEndpoint{this, index};
	}

	void setTargetEndpoint(unsigned index, SourceEndpoint src)
	{
		if (index >= function->argTypes().size())
		{
			throw std::runtime_error(
				"Setting invalid enpoint, index " + std::to_string(index) + " exceeding number of returns.");
		}

		auto sourceType = src.node()->function->returnTypes()[src.index()];
		auto targetType = function->argTypes()[index];
		if (sourceType != targetType)
		{
			throw std::runtime_error("Types mismatch, when assigning endpoints");
		}
		
		endpoints[index] = src;
	}

	std::vector<SourceEndpoint> endpoints;
	const std::string name;
	const std::shared_ptr<IFunction> function;
};

using Nodes = std::vector<std::unique_ptr<Node>>;

namespace detail
{

inline void make_nodes(Nodes&){}

template<typename T, typename ... Ts>
void make_nodes(Nodes& nodes, T&& t, Ts ... ts)
{
	detail::make_nodes(nodes, ts ... );
	nodes.emplace_back(std::move(t));
}

}

template<typename ... Ts>
Nodes make_nodes(Ts&& ... ts)
{
	Nodes result;
	result.reserve(sizeof...(Ts));
	detail::make_nodes(result, std::forward<Ts>(ts) ...);
	return result;
}

}
