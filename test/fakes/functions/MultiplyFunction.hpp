#pragma once

#include <cassert>

#include <Functions/IFunction.hpp>
#include <Functions/FunctionHelpers.hpp>

namespace fcl
{

class MultiplyFunction : public IFunction
{
	ReturnVector call(std::vector<nonType*> args) override
	{
		assert(args.size() == 2);
		const int& lhs = get_argument<int>(args, 0);
		const int& rhs = get_argument<int>(args, 1);

		int* result = new int(lhs * rhs);

		return make_return_vector(result);
	}
	TypeVector inputArgs() override
	{
		return make_type_vector<int, int>();
	}
	TypeVector outputArgs() override
	{
		return make_type_vector<int>();
	}
	std::string name() override
	{
		return "int_multiply";
	}
};

}  // namespace fcl