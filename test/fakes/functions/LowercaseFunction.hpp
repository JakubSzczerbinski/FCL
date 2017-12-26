#pragma once

#include <cassert>

#include <Functions/IFunction.hpp>
#include <Functions/FunctionHelpers.hpp>

namespace fcl
{

class LowercaseFunction : public IFunction
{
	ReturnVector call(std::vector<nonType*> args) override
	{
		assert(args.size() == 1);
		std::string arg = get_argument<std::string>(args, 0);
		for (size_t i = 0; i < arg.size(); i++)
		{
			if (arg[i] >= 'A' && arg[i] <= 'Z')
			{
				arg[i] = arg[i] - 'A' + 'a';
			}
		} 
		return make_return_vector(new std::string(arg));
	}
	TypeVector argTypes() override
	{
		return make_type_vector<std::string>();
	}
	TypeVector returnTypes() override
	{
		return make_type_vector<std::string>();
	}
	std::string name() override
	{
		return "lowercase";
	}
};

}  // namespace fcl