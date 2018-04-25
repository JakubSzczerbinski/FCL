#pragma once

#include <cassert>

#include <libFCL/Functions/IFunction.hpp>
#include <libFCL/Functions/FunctionHelpers.hpp>

namespace fcl
{

class IntToStringFunction : public IFunction
{
	ReturnVector call(std::vector<nonType*> args) override
	{
		assert(args.size() == 1);
		const int& arg = get_argument<int>(args, 0);
		return make_return_vector(new std::string(std::to_string(arg)));
	}
	TypeVector argTypes() override
	{
		return make_type_vector<int>();
	}
	TypeVector returnTypes() override
	{
		return make_type_vector<std::string>();
	}
	std::string name() override
	{
		return "int_to_string";
	}
};

}  // namespace fcl