#pragma once 

#include <vector>
#include <string>

#include <boost/variant.hpp>

#include "Token.hpp"


namespace fcl
{

struct Link
{
	std::string node_name;
	unsigned index;
};

struct NodeConstructor
{
	std::string function_name;
	std::vector<Link> links;
};

struct Declaration
{
	std::string node_name;
	NodeConstructor constructor;
};

struct Definition
{
	std::string node_name;
	std::string type;
	std::string initialization_string;
};

using Statement = boost::variant<Declaration, Definition>;
using Program = std::vector<Statement>;

Program parse(const std::string& text);

}
