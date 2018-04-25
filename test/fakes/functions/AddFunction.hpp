#pragma once

#include <cassert>

#include <libFCL/Functions/IFunction.hpp>
#include <libFCL/Functions/FunctionHelpers.hpp>

namespace fcl
{

class AddFunction : public IFunction
{
	ReturnVector call(std::vector<nonType*> args) override
	{
		assert(args.size() == 2);
		const int& lhs = get_argument<int>(args, 0);
		const int& rhs = get_argument<int>(args, 1);

		int* result = new int(lhs + rhs);

		return make_return_vector(result);
	}
	TypeVector argTypes() override
	{
		return make_type_vector<int, int>();
	}
	TypeVector returnTypes() override
	{
		return make_type_vector<int>();
	}
	std::string name() override
	{
		return "int_add";
	}
};

}  // namespace fcl