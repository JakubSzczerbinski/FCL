#pragma once

#include <map>
#include <stdexcept>

#include "Node.hpp"
#include <Serialization/ISerializer.hpp>
#include <Parser/Parser.hpp>


namespace fcl
{

Nodes loadNodes(
	const Program& program,
	const Functions& functions,
	const Serializers& serializers);

Nodes readNodes(
	const std::string& program_text,
	const Functions& functions,
	const Serializers& serializers);

}