
#pragma once

#include <string>
#include <libFCL/Serialization/Serializer.hpp>

namespace fcl
{

struct StringSerializerImpl
{
	std::string toString(const std::string& string)
	{
		return string;
	}
	std::string fromString(const std::string& string)
	{
		return string;
	}
	std::string typeName()
	{
		return "string";
	}
};

using StringSerializer = Serializer<std::string, StringSerializerImpl>;

}