
#pragma once

#include <string>
#include <libFCL/Serialization/Serializer.hpp>

namespace fcl
{

struct IntSerializerImpl
{
	std::string toString(int i)
	{
		return std::to_string(i);
	}
	int fromString(const std::string& string)
	{
		return std::stoi(string);
	}
	std::string typeName()
	{
		return "int";
	}
};

using IntSerializer = Serializer<int, IntSerializerImpl>;

}