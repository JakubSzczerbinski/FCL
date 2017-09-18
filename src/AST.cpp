
#include <AST.hpp>

namespace fcl
{
    AST::AST(std::vector<IFunction*> functions)
    {
        for (auto func : functions)
        {
            boost::uuids::uuid name = generator();
            std::string nhdl = boost::uuids::to_string(name);
            functionMap.emplace(std::make_pair(nhdl, Function{func}));
        }
    }

	std::vector<FunctionHandle> AST::get_functions() const
	{
		std::vector<FunctionHandle> result;

		for(auto fhdl : functionMap)
			result.push_back(fhdl.first);

		return result;
	}

	std::string AST::get_function_pretty_name(FunctionHandle hdl, error_code& ec) const
	{
		auto it = functionMap.find(hdl);

		if(it == functionMap.end())
		{
			ec = error_code::not_implemented;
			return "";
		}
		
		return it->second.function->name();
	}

	std::vector<TypeHandle> AST::get_return_types(FunctionHandle hdl, error_code& ec) const
	{
		std::vector<TypeHandle> result;

		auto it = functionMap.find(hdl);

		if(it == functionMap.end())
		{
			ec = error_code::invalid_handle;
			return result;
		}

		TypeVector typeVector = it->second.function->outputArgs();

		for(auto t : typeVector)
			result.push_back(reverseTypeMap.at(t));

		return result;
	}

	std::vector<TypeHandle> AST::get_arg_types(FunctionHandle hdl, error_code& ec) const
	{
		std::vector<TypeHandle> result;

		auto it = functionMap.find(hdl);

		if(it == functionMap.end())
		{
			ec = error_code::invalid_handle;
			return result;
		}

		TypeVector typeVector = it->second.function->inputArgs();

		for(auto t : typeVector)
			result.push_back(reverseTypeMap.at(t));

		return result;
	}

	template <typename ValueType>
	FunctionHandle AST::create_value_function(ValueType val, error_code& ec)
	{
		// TODO
	}

	bool AST::delete_value_function(FunctionHandle hdl, error_code& ec)
	{
		// TODO
	}



	std::string AST::get_type_pretty_name(TypeHandle hdl, error_code& ec) const
	{
		auto it = typeMap.find(hdl);

		if(it == typeMap.end())
		{
			ec = error_code::type_not_found;
			return "";
		}

		return it->second.pretty_name();
	}



	std::vector<NodeHandle> AST::get_nodes() const
	{
		std::vector<NodeHandle> result;

		for(auto nhdl : nodeMap)
			result.push_back(nhdl.first);

		return result;
	}

    std::vector<LinkHandle> AST::get_arg_links(NodeHandle hdl, error_code& ec) const
    {
    	std::vector<LinkHandle> result;

    	if(nodeMap.find(hdl) == nodeMap.end())
    	{
    		ec = error_code::invalid_handle;
    		return result;
    	}

    	for(auto lhdl : linkMap)
    		if(lhdl.second.outNode == hdl)
    			result.push_back(lhdl.first);

    	return result;
    }

    std::string AST::get_node_pretty_name(NodeHandle hdl, error_code& ec) const
    {
    	auto it = nodeMap.find(hdl);

		if(it == nodeMap.end())
		{
			ec = error_code::node_not_found;
			return "";
		}
		
		return it->second.prettyName;
    }

    FunctionHandle AST::get_node_function(NodeHandle hdl, error_code& ec) const
    {
    	auto it = nodeMap.find(hdl);

    	if(it == nodeMap.end())
    	{
    		ec = error_code::invalid_handle;
    		return "";
    	}

    	return it->second.nodeFunction;
    }

    NodeHandle AST::create_node(FunctionHandle hdl, error_code& ec)
    {
    	if(functionMap.find(hdl) == functionMap.end())
    	{
    		ec = error_code::invalid_handle;
    		return "";
    	}

    	boost::uuids::uuid name = generator();
    	std::string nhdl = boost::uuids::to_string(name);

    	nodeMap.insert(std::pair<NodeHandle, Node>(nhdl, Node()));

    	return nhdl;
    }

    bool AST::delete_node(NodeHandle hdl, error_code& ec)
    {
    	auto it = nodeMap.find(hdl);
    	auto itNext = std::next(it, 1);

    	if(it == nodeMap.end())
    	{
    		ec = error_code::node_not_found;
    		return false;
    	}

    	if(nodeMap.erase(it) != itNext)
    		return false;

    	return true;
    }


    int AST::get_argument_index(LinkHandle hdl, error_code& ec) const
    {
    	// TODO
    }

    NodeHandle AST::get_argument_node(LinkHandle hdl, error_code& ec) const
    {
    	auto it = linkMap.find(hdl);

    	if(it == linkMap.end())
    	{
    		ec = error_code::invalid_handle;
    		return "";
    	}

    	return it->second.inNode;
    }

    int  AST::get_return_index(LinkHandle hdl, error_code& ec) const
    {
    	// TODO
    }

    NodeHandle AST::get_return_node(LinkHandle hdl, error_code& ec) const
    {
    	auto it = linkMap.find(hdl);

    	if(it == linkMap.end())
    	{
    		ec = error_code::invalid_handle;
    		return "";
    	}

    	return it->second.outNode;
    }

    LinkHandle AST::create_link(NodeHandle return_hdl, int return_index,
                                       NodeHandle arg_hdl, int arg_index,
                                       error_code& ec)
    {
    	//TODO
    }

    bool AST::delete_link(LinkHandle hdl, error_code& ec)
    {
    	auto it = linkMap.find(hdl);
    	auto itNext = std::next(it, 1);

    	if(it == linkMap.end())
    	{
    		ec = error_code::link_not_found;
    		return false;
    	}

    	if(linkMap.erase(it) != itNext)
    		return false;

    	return true;
    }
}