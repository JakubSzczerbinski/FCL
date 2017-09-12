#pragma once

#include <vector>

#include <boost/type_index.hpp>

#include <Constants.hpp>

namespace fcl
{

template <typename ... Args>
TypeVector make_type_vector()
{
	return std::vector<bti::type_index>(bti::type_id<Args>() ... );
}

}  // namespace fcl