#pragma once 

#include <vector>
#include <memory>

#include <boost/type_index.hpp>

#include <Common/Constants.hpp>

namespace fcl
{

using Return = std::unique_ptr<nonType, std::function<void(nonType*)>>;
using ReturnVector = std::vector<Return>;
using Arg = nonType*;
using ArgVector = std::vector<Arg>;

class IFunction
{
public:
	virtual ReturnVector call(ArgVector) = 0;
	virtual TypeVector argTypes() = 0;
	virtual TypeVector returnTypes() = 0;
	virtual std::string name() = 0;
	virtual ~IFunction() = default;
};

using Functions = std::vector<std::shared_ptr<IFunction>>;

} // namespace fcl