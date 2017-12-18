
#pragma once

#include "IntSerializer.hpp"
#include "StringSerializer.hpp"

namespace fcl
{

inline Serializers serializers()
{
	return 
	{ std::make_shared<IntSerializer>()
	, std::make_shared<StringSerializer>()
	};
}

}