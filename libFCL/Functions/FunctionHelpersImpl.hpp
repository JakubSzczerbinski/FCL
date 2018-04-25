#pragma once

#include "IFunction.hpp"

namespace fcl
{

namespace detail
{

inline void make_return_vector_impl(ReturnVector& vector){}

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

template <typename Arg, typename ... Args>
void make_return_vector_impl(ReturnVector& vector, Arg* arg, Args ... args)
{
	vector.emplace_back(reinterpret_cast<nonType*>(arg), nonTypeDelete<Arg>());
	make_return_vector_impl(vector, args ...);
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

template<unsigned i, unsigned ... is>
struct ReturnVectorConverterImpl : public ReturnVectorConverterImpl<i - 1, i, is ... >
{};

template<unsigned ... is>
struct ReturnVectorConverterImpl<0, is ...>
{
	template<typename T>
	static ReturnVector convert(T* ret)
	{
		ReturnVector result;
		detail::make_return_vector_impl(result, ret);
		return result;
	}
	template<typename ... Ts>
	static ReturnVector convert(std::tuple<Ts* ... > returns)
	{
		ReturnVector result;
		detail::make_return_vector_impl(result, std::get<0>(returns), std::get<is>(returns) ...); 
		return result;
	}
};

template<unsigned n>
struct ReturnVectorConverter : public detail::ReturnVectorConverterImpl<n>
{};

}

}
