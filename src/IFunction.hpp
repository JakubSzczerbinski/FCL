#pragma once 

#include <vector>
#include <memory>

#include <boost/type_index.hpp>

#include <Constants.hpp>

namespace fcl
{

class IFunction
{
public:
	virtual std::vector<std::unique_ptr<nonType>> call(std::vector<nonType*>) = 0;
	virtual TypeVector inputArgs() = 0;
	virtual TypeVector outputArgs() = 0;
	virtual std::string name() = 0;
};

} // namespace fcl