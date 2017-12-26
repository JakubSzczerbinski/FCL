
#pragma once

#include <memory>

#include "ISerializer.hpp"

#include <Functions/IFunction.hpp>
#include <Functions/FunctionHelpers.hpp>
#include <Functions/ValueFunction.hpp>

namespace fcl
{

template <typename Type, typename SerializerImpl>
class Serializer : SerializerImpl, public ISerializer
{
public:
	std::shared_ptr<IFunction> deserialize(std::string serialized) override
	{
		return make_value_function<Type>(SerializerImpl::fromString(serialized));
	}
	std::string serialize(nonType* deserialized) override
	{
		return SerializerImpl::toString(get_argument<Type>(deserialized));
	}
	bti::type_index type() override
	{
		return bti::type_id<Type>();
	}
	std::string typeName() override
	{
		return SerializerImpl::typeName();
	}
};
	
}