#pragma once 

#include <boost/type_index.hpp>

#include <Constants.hpp>

namespace fcl
{

namespace bti = boost::typeindex;

class Node;

class NodeLink
{
public:
	NodeLink (Node& node, unsigned idx, bti::type_index t) : node_(node), idx_(idx), type_(t) {}
	unsigned idx_;
	Node& node_;
	bti::type_index type_;
	nonType* get();
	bti::type_index type();
};

} // namespace fcl
