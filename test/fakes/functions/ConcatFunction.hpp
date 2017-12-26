#pragma once

#include <cassert>

#include <Functions/IFunction.hpp>
#include <Functions/FunctionHelpers.hpp>

namespace fcl
{

class ConcatFunction : public IFunction
{
	ReturnVector call(std::vector<nonType*> args) override
	{
		assert(args.size() == 2);
		const std::string& arg1 = get_argument<std::string>(args, 0);
		const std::string& arg2 = get_argument<std::string>(args, 1);
		return make_return_vector(new std::string(arg1 + arg2));
	}
	TypeVector argTypes() override
	{
		return make_type_vector<std::string, std::string>();
	}
	TypeVector returnTypes() override
	{
		return make_type_vector<std::string>();
	}
	std::string name() override
	{
		return "concat";
	}
};

}  // namespace fcl