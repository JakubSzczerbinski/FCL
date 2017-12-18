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


struct Node
{
	Node (std::shared_ptr<IFunction> function_, std::string name_)
	: name(name_), function(function_)
	{
		endpoints.resize(function->inputArgs().size());
	}

	SourceEndpoint sourceEndpoint(unsigned index)
	{
		if (index >= function->outputArgs().size())
		{
			throw std::runtime_error(
				"Requesting invalid endpoint, index " + std::to_string(index) + " exceeding number of returns.");
		}
		return SourceEndpoint{this, index};
	}

	void setTargetEndpoint(unsigned index, SourceEndpoint src)
	{
		if (index >= function->inputArgs().size())
		{
			throw std::runtime_error(
				"Setting invalid enpoint, index " + std::to_string(index) + " exceeding number of returns.");
		}
		
		endpoints[index] = src;
	}

	std::vector<SourceEndpoint> endpoints;
	const std::string name;
	const std::shared_ptr<IFunction> function;
};

using Nodes = std::vector<Node*>;

}
