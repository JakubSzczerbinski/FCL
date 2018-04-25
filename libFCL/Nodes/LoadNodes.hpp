#pragma once

#include <map>
#include <stdexcept>

#include "Node.hpp"
#include "../Serialization/ISerializer.hpp"
#include "../Parser/Parser.hpp"


namespace fcl
{

std::pair<Nodes, ReturnNodes> loadNodes(
	const Program& program,
	const Functions& functions,
	const Serializers& serializers);

std::pair<Nodes, ReturnNodes> readNodes(
	const std::string& program_text,
	const Functions& functions,
	const Serializers& serializers);

}