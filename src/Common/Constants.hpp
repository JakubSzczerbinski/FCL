#pragma once

#include <vector>

#include <boost/type_index.hpp>

namespace fcl
{

namespace bti = boost::typeindex;

using TypeVector = std::vector<bti::type_index>;

using nonType = char;

} // namespace fcl
