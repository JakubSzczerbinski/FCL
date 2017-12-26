#pragma once

#include <cassert>

#include <Functions/IFunction.hpp>
#include <Functions/FunctionHelpers.hpp>

namespace fcl
{

class DivideFunction : public IFunction
{
	ReturnVector call(std::vector<nonType*> args) override
	{
		assert(args.size() == 2);
		const int& lhs = get_argument<int>(args, 0);
		const int& rhs = get_argument<int>(args, 1);

		int* quotient = new int(lhs / rhs);
		int* remainder = new int(lhs % rhs);

		return make_return_vector(quotient, remainder);
	}
	TypeVector argTypes() override
	{
		return make_type_vector<int, int>();
	}
	TypeVector returnTypes() override
	{
		return make_type_vector<int, int>();
	}
	std::string name() override
	{
		return "int_divide";
	}
};

}  // namespace fcl