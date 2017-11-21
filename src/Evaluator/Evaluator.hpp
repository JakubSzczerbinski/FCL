
#pragma once

#include <AST/AST.hpp>
#include <Common/Logger.hpp>
#include <fcl.hpp>
#include <cassert>
#include <Functions/FunctionHelpers.hpp>

namespace fcl
{

class Evaluator
{
public:
	Evaluator(AST& ast) 
		: ast_(ast), logger_("fcl::Evaluator")
	{}
	template <typename T>
	Return evaluate(NodeHandle hdl, int index, error_code& ec)
	{
		auto function_hdl = ast_.get_node_function(hdl, ec);
		if (ec)
		{
			logger_.error_log("Exiting due to error while getting node's function for node", hdl);
			return nullptr;
		}

		auto type_hdls = ast_.get_return_types(function_hdl, ec);
		if (index >= type_hdls.size())
		{
			logger_.error_log("Index of evaulated return out of bounds.",
				"index:", std::to_string(index),
				"function:", function_hdl);
			ec = error_code::invalid_index;
			return nullptr;
		}

		auto type_hdl = type_hdls[index];
		if (!ast_.has_type<T>(type_hdl, ec))
		{
			logger_.error_log("Types dont match");
			return nullptr;
		}

		ReturnVector rets = get_value(hdl, ec);
		if (ec)
		{
			logger_.error_log("Failed to evaluate node", hdl);
			return nullptr;
		}
		assert(index < rets.size());
		return std::move(rets[index]);
	}
private:
	ReturnVector get_value(NodeHandle hdl, error_code& ec)
	{
		std::vector<LinkHandle> links = ast_.get_arg_links(hdl, ec);
		if (ec)
		{
			logger_.error_log("Exiting due to error while getting links for", hdl);
			return {};
		}

		ReturnVector args;
		for (auto&& link : links)
		{
			auto node_hdl = ast_.get_argument_node(link, ec);
			if (ec)
			{
				logger_.error_log("Exiting due to error while getting argument node for link", link);
				return {};
			}

			auto node_index = ast_.get_argument_index(link, ec);
			if (ec)
			{
				logger_.error_log("Exiting due to error while getting argument index for link", link);
				return {};
			}

			ReturnVector rets = get_value(node_hdl, ec);
			if (ec)
			{
				logger_.error_log("Exiting due to error while evaluating node", node_hdl);
				return {};
			}

			assert(args.size() >= node_index);
			args.push_back(std::move(rets[node_index]));
		}

		auto function_hdl = ast_.get_node_function(hdl, ec);
		if (ec)
		{
			logger_.error_log("Exiting due to error while getting node's function for node", hdl);
			return {};
		}
		auto function = ast_.get_function_from_handle(function_hdl, ec);
		if (ec)
		{
			logger_.error_log("Exiting due to error while getting function callable for function", function_hdl);
			return {};
		}

		return function->call(to_arg_vector(args));
	}
	AST& ast_;
	Logger logger_;
};

};