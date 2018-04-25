#pragma once

#include <cassert>

#include <libFCL/Functions/IFunction.hpp>
#include <libFCL/Functions/FunctionHelpers.hpp>

namespace fcl
{

class UppercaseFunction : public IFunction
{
	ReturnVector call(std::vector<nonType*> args) override
	{
		assert(args.size() == 1);
		std::string arg = get_argument<std::string>(args, 0);
		for (size_t i = 0; i < arg.size(); i++)
		{
			if (arg[i] >= 'a' && arg[i] <= 'z')
			{
				arg[i] = arg[i] - 'a' + 'A';
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
		return "uppercase";
	}
};

}  // namespace fcl