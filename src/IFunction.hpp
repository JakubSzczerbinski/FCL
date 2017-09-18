#pragma once 

#include <vector>
#include <memory>

#include <boost/type_index.hpp>

#include <Constants.hpp>

namespace fcl
{

using ReturnVector = std::vector<std::unique_ptr<nonType, std::function<void(nonType*)>>>;

class IFunction
{
public:
	virtual ReturnVector call(std::vector<nonType*>) = 0;
	virtual TypeVector inputArgs() = 0;
	virtual TypeVector outputArgs() = 0;
	virtual std::string name() = 0;
	virtual ~IFunction() = default;
};

} // namespace fcl