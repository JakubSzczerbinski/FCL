#pragma once

#include "../Nodes/Node.hpp"
#include "../Serialization/ISerializer.hpp"

namespace fcl
{

std::string writeNodes(const Nodes& nodes, Serializers serializers);

}
