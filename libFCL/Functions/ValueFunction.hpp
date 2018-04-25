
#pragma once

#include <cassert>

#include "IFunction.hpp"
#include "FunctionHelpers.hpp"

namespace fcl
{

template <typename T>
class ValueFunction : public IFunction
{
public:
	ValueFunction(T&& t) : t_(std::move(t)) {}
	ReturnVector call(std::vector<nonType*> args) override
	{
		if (args.size() != 0)
			throw std::logic_error("Expecting 0 arguments.");

		return make_return_vector(new T(t_));
	}
	TypeVector argTypes() override
	{
		return make_type_vector<>();
	}
	TypeVector returnTypes() override
	{
		return make_type_vector<T>();
	}
	std::string name() override
	{
		return bti::type_id<T>().pretty_name();
	}
private:
	T t_;
};

template <typename T>
std::shared_ptr<IFunction> make_value_function(T t)
{
	return std::shared_ptr<IFunction>(new ValueFunction<T>(std::forward<T>(t)));
}

}
