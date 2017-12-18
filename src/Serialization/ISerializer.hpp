
#pragma once

#include <memory>

#include <Functions/IFunction.hpp>

namespace fcl
{

class ISerializer
{
public:
	virtual std::shared_ptr<IFunction> deserialize(std::string serialized) = 0;
	virtual std::string serialize(nonType* deserialized) = 0;
	virtual bti::type_index type() = 0;
	virtual std::string typeName() = 0;
};

using Serializers = std::vector<std::shared_ptr<ISerializer>>;
	
}