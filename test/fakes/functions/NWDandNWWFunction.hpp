#pragma once

#include <cassert>
#include <utility>

#include <Functions/IFunction.hpp>
#include <Functions/FunctionHelpers.hpp>

namespace fcl
{

class NWDandNWWFunction : public IFunction
{
	ReturnVector call(std::vector<nonType*> args) override
	{
		assert(args.size() == 2);
		int arg1 = get_argument<int>(args, 0);
		int arg2 = get_argument<int>(args, 1);

		int a = arg1;
		int b = arg2;
		if (a > b) { std::swap(a, b); }
		if (a == 1) { return make_return_vector(new int(1), new int(arg1 * arg2)); }

		while (a != 0)
		{
			b = b % a;
			std::swap(a, b);
		}

		return make_return_vector(new int(b), new int((arg1/b) * arg2));
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
		return "nwd_nww";
	}
};

}  // namespace fcl