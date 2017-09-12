#pragma once

#include <cassert>

#include <IFunction.hpp>
#include <TypeIndexHelpers.hpp>

namespace fcl
{

class AddFunction : IFunction
{
	std::vector<std::unique_ptr<nonType>> call(std::vector<nonType*> args) override
	{
		assert(args.size() == 2);
		int& lhs = *reinterpret_cast<int*>(args[0]);
		int& rhs = *reinterpret_cast<int*>(args[1]);

		int* result = new int(lhs + rhs);

		return {std::unique_ptr<nonType>(reinterpret_cast<nonType*>(result))};
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
		return "Integer add";
	}
};

}  // namespace fcl