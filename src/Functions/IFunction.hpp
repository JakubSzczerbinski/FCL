#pragma once 

#include <vector>
#include <memory>

#include <boost/type_index.hpp>

#include <Common/Constants.hpp>

namespace fcl
{

using Return = std::unique_ptr<nonType, std::function<void(nonType*)>>;
using ReturnVector = std::vector<Return>;
using ArgVector = std::vector<nonType*>;

class IFunction
{
public:
	virtual ReturnVector call(ArgVector) = 0;
	virtual TypeVector inputArgs() = 0;
	virtual TypeVector outputArgs() = 0;
	virtual std::string name() = 0;
	virtual ~IFunction() = default;
};

} // namespace fcl