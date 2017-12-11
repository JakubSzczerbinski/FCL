#pragma once

#include <map>
#include <stdexcept>

#include <boost/optional.hpp>

#include <Parser/Parser.hpp>
#include "AST.hpp"


namespace fcl
{

template <typename ReturnType, typename... Lambdas>
struct lambda_visitor;

template <typename ReturnType, typename Lambda1, typename... Lambdas>
struct lambda_visitor< ReturnType, Lambda1 , Lambdas...> 
  : public lambda_visitor<ReturnType, Lambdas...>, public Lambda1 {

    using Lambda1::operator();
    using lambda_visitor< ReturnType , Lambdas...>::operator();
    lambda_visitor(Lambda1&& l1, Lambdas&&... lambdas) 
      : lambda_visitor< ReturnType , Lambdas...> (std::forward<Lambdas>(lambdas)...), Lambda1(std::forward<Lambda1>(l1))
    {}
};


template <typename ReturnType, typename Lambda1>
struct lambda_visitor<ReturnType, Lambda1> 
  : public boost::static_visitor<ReturnType>, public Lambda1 {

    using Lambda1::operator();
    lambda_visitor(Lambda1&& l1) 
      : boost::static_visitor<ReturnType>(), Lambda1(std::forward<Lambda1>(l1))
    {}
};


template <typename ReturnType>
struct lambda_visitor<ReturnType> 
  : public boost::static_visitor<ReturnType> {

    lambda_visitor() : boost::static_visitor<ReturnType>() {}
};

template <typename ReturnType, typename... Lambdas>
lambda_visitor<ReturnType, Lambdas...> make_lambda_visitor(Lambdas&&... lambdas) {
    return { std::forward<Lambdas>(lambdas)... };
}

class ProgramLoader
{
public:
	ProgramLoader(AST& ast) : ast_(ast) {}
	void load_program(const Program& program)
	{
		auto visitor = make_lambda_visitor<int>(
		[&](const Definition& definition) -> int
		{
			if (definition.type == "int")
			{
				error_code ec;
				auto function_hdl = ast_.create_value_function<int>(
					std::stoi(definition.initialization_string), ec);

				auto node_hdl = ast_.create_node(function_hdl, definition.node_name, ec);
				return 0;
			}

			if (definition.type == "string")
			{
				error_code ec;
				auto function_hdl = ast_.create_value_function<std::string>(
					std::string(definition.initialization_string), ec);

				auto node_hdl = ast_.create_node(function_hdl, definition.node_name, ec);
				return 0;
			}

			throw std::runtime_error("Unable to deserialize type " + definition.type);
		},
		[&](const Declaration& declaration) -> int
		{
			auto functions = ast_.get_functions();
			std::string func_hdl;
			for (auto&& func : functions)
			{
				error_code ec = error_code::success;
				auto func_name = ast_.get_function_pretty_name(func, ec);
				if (func_name == declaration.constructor.function_name)
				{
					func_hdl = func;
					break;
				}
			}

			if (func_hdl == "")
				throw std::runtime_error("No function named " + declaration.constructor.function_name);

			error_code ec = error_code::success;
			auto new_node_hdl = ast_.create_node(func_hdl, declaration.node_name, ec);

			unsigned i = 0;
			for (auto&& link : declaration.constructor.links)
			{
				auto nodes = ast_.get_nodes();
				std::string node_hdl;
				for (auto&& node : nodes)
				{
					error_code ec = error_code::success;
					auto node_name = ast_.get_node_pretty_name(node, ec);
					if (node_name == link.node_name)
					{
						node_hdl = node;
						break;
					}
				}
				error_code ec = error_code::success;
				ast_.create_link(node_hdl, link.index, new_node_hdl, i++, ec);
				if (ec)
				{
					throw std::runtime_error(
						"Unable to create link between " 
						+ node_hdl + "." + std::to_string(link.index) + " and " 
						+ new_node_hdl + "." + std::to_string(i-1));
				}
			}
			return 0;
		});

		for (auto&& statement : program)
		{
			boost::apply_visitor(visitor, statement);
		}
	}

private:
	AST& ast_;
	
};


}