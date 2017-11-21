#pragma once

#include <cassert>
#include <vector>
#include <iostream>

#include "IFunction.hpp"

namespace fcl
{

inline ArgVector to_arg_vector(const ReturnVector& rets)
{
	ArgVector args;
	for (auto&& ret : rets)
	{
		args.push_back(ret.get());
	}
	return args;
}

template <typename T>
nonType* make_arg(T* arg)
{
	return reinterpret_cast<nonType*>(arg);
}

inline nonType* make_arg(const ReturnVector& rets, size_t index)
{
	return reinterpret_cast<nonType*>(rets[index].get());
}

template <typename T>
const T& get_argument(const std::vector<nonType*>& args, size_t index)
{
	assert(args.size() > index);
	return *reinterpret_cast<T*>(args[index]);
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


template <typename T>
struct nonTypeDelete
{
	void operator() (nonType* ptr)
	{
		std::default_delete<T> deleter;
		T* real_ptr = reinterpret_cast<T*>(ptr);
		deleter(real_ptr);
	}
};

inline void make_return_vector_impl(ReturnVector& vector){}

template <typename Arg, typename ... Args>
void make_return_vector_impl(ReturnVector& vector, Arg* arg, Args ... args)
{
	vector.emplace_back(reinterpret_cast<nonType*>(arg), nonTypeDelete<Arg>());
	make_return_vector_impl(vector, args ...);
}

template <typename ... Args>
ReturnVector make_return_vector(Args ... args)
{
	ReturnVector result;
	make_return_vector_impl(result, args ...);
	return result;
}

template <typename ... Args>
TypeVector make_type_vector()
{
	return {bti::type_id<Args>() ... };
}

template <typename T>
class ValueFunction : public IFunction
{
public:
	ValueFunction(T&& t) : t_(std::move(t)) {}
	ReturnVector call(std::vector<nonType*> args) override
	{
		assert(args.size() == 0);
		return make_return_vector(new T(t_));
	}
	TypeVector inputArgs() override
	{
		return make_type_vector<>();
	}
	TypeVector outputArgs() override
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
std::unique_ptr<IFunction> make_value_function(T&& t)
{
	return std::unique_ptr<IFunction>(new ValueFunction<T>(std::move(t)));
}

template<typename Iterator, typename ... Ts>
struct Invoker;

template<typename Iterator, typename T, typename ... Ts>
struct Invoker<Iterator, T, Ts ... >
{
	template<typename Functor>
	static auto invoke(Functor func, Iterator it) 
	-> decltype(func(*reinterpret_cast<T*>(*it), std::declval<Ts>() ... ))
	{
		return Invoker<Iterator, Ts ... >::invoke
		(
			[&it, &func](Ts ... ts)
			{
				return func(*reinterpret_cast<T*>(*it), ts ...);
			},
			it + 1
		);
	}

	template<typename Functor>
	static auto invoke(Functor func, Iterator it) 
	-> decltype(func(reinterpret_cast<T*>(*it), std::declval<Ts>() ... ))
	{
		return Invoker<Iterator, Ts ... >::invoke
		(
			[&it, &func](Ts ... ts)
			{
				auto arg = reinterpret_cast<T*>(*it);
				return func(arg, ts ...);
			},
			it + 1
		);
	}
};

template<typename Iterator>
struct Invoker<Iterator>
{
	template<typename Functor>
	static auto invoke(Functor func, Iterator it)
	{
		return func();
	}
};

namespace detail
{


template<unsigned i, unsigned ... is>
struct ReturnVectorConverterImpl : public ReturnVectorConverterImpl<i - 1, i, is ... >
{};

template<unsigned ... is>
struct ReturnVectorConverterImpl<0, is ...>
{
	template<typename T>
	static ReturnVector convert(T* ret)
	{
		return make_return_vector(ret);
	}
	template<typename ... Ts>
	static ReturnVector convert(std::tuple<Ts* ... > returns)
	{
		return make_return_vector(std::get<0>(returns), std::get<is>(returns) ... );
	}
};

}

template<unsigned n>
struct ReturnVectorConverter : public detail::ReturnVectorConverterImpl<n>
{};

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
			Invoker<std::vector<nonType*>::iterator, Inputs ...>::invoke(function_, types.begin());
		return ReturnVectorConverter<sizeof...(Outputs)>::convert(tuple);
	}

	TypeVector inputArgs() override
	{
		return make_type_vector<Inputs ... >();
	}
	TypeVector outputArgs() override
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
