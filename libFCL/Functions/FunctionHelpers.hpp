#pragma once

#include <cassert>
#include <vector>
#include <iostream>

#include "IFunction.hpp"
#include "FunctionHelpersImpl.hpp"

namespace fcl
{

ArgVector to_arg_vector(const ReturnVector& rets);

nonType* to_arg(const ReturnVector& rets, size_t index);

template <typename T>
nonType* make_arg(T* arg)
{
	return reinterpret_cast<nonType*>(arg);
}

template <typename T>
const T& get_argument(const std::vector<nonType*>& args, size_t index)
{
	assert(args.size() > index);
	return *reinterpret_cast<T*>(args[index]);
}

template <typename T>
const T& get_argument(nonType* arg)
{
	return *reinterpret_cast<T*>(arg);
}

template <typename T>
const T& get_return(const ReturnVector& args, size_t index)
{
	assert(args.size() > index);
	return *reinterpret_cast<T*>(args[index].get());
}

template <typename T>
const T& get_return(const Return& ret)
{
	return *reinterpret_cast<T*>(ret.get());
}

template <typename ... Args>
ReturnVector make_return_vector(Args ... args)
{
	ReturnVector result;
	detail::make_return_vector_impl(result, args ...);
	return result;
}

template <typename ... Args>
TypeVector make_type_vector()
{
	return {bti::type_id<Args>() ... };
}


template <typename ... Inputs>
struct Input
{

template <typename ... Outputs>
struct Output
{

template<typename Functor>
class Function : public IFunction
{
public:
	Function(Functor function, std::string name)
		: function_(function), name_(name)
	{}
	ReturnVector call(std::vector<nonType*> types) override
	{
		auto tuple = 
			detail::Invoker<std::vector<nonType*>::iterator, Inputs ...>::invoke(function_, types.begin());
		return detail::ReturnVectorConverter<sizeof...(Outputs)>::convert(tuple);
	}

	TypeVector argTypes() override
	{
		return make_type_vector<Inputs ... >();
	}
	TypeVector returnTypes() override
	{
		return make_type_vector<Outputs ... >();
	}
	std::string name() override
	{
		return name_;
	}
private:
	Functor function_;
	const std::string name_;
};

template<typename Functor>
static std::unique_ptr<IFunction> makeFunction(Functor functor, std::string name)
{
	return std::unique_ptr<IFunction>(new Function<Functor>(functor, name));
}

};

};

}  // namespace fcl
