#pragma once

#include <map>
#include <string>
#include <vector>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <Functions/IFunction.hpp>
#include <Functions/FunctionHelpers.hpp>
#include <fcl.hpp>

namespace fcl
{

class AST
{
public:
    AST(std::vector<std::unique_ptr<IFunction>>&& functions);
    AST() : AST(std::vector<std::unique_ptr<IFunction>>{}) {}

    IFunction* get_function_from_handle(FunctionHandle hdl, error_code& ec) const;

    std::vector<FunctionHandle> get_functions() const;

    std::string get_function_pretty_name(FunctionHandle hdl, error_code& ec) const;

    std::vector<TypeHandle> get_return_types(FunctionHandle hdl, error_code& ec) const;

    std::vector<TypeHandle> get_arg_types(FunctionHandle hdl, error_code& ec) const;

    template <typename ValueType>
    FunctionHandle create_value_function(ValueType&& val, error_code& ec)
    {
        auto returnType = bti::type_id<ValueType>();
        add_type(returnType);

        auto it = std::find_if(typeMap.begin(), typeMap.end(), 
            [&returnType](const std::pair<TypeHandle, boost::typeindex::type_index>& el)
            {
                return el.second == returnType;
            });

        if (it == typeMap.end())
        {
            ec = error_code::type_not_supported;
            return "";
        }


        auto functionObject = make_value_function<ValueType>(std::move(val));

        boost::uuids::uuid name = generator();
        std::string nhdl = boost::uuids::to_string(name);
        functionMap.emplace(std::make_pair(nhdl, Function{FunctionType::USER_CREATED, std::move(functionObject)}));
        return nhdl;
    }

    bool delete_value_function(FunctionHandle hdl, error_code& ec);



    std::string get_type_pretty_name(TypeHandle hdl, error_code& ec) const;



    std::vector<NodeHandle> get_nodes() const;

    std::vector<LinkHandle> get_arg_links(NodeHandle hdl, error_code& ec) const;

    std::string get_node_pretty_name(NodeHandle hdl, error_code& ec) const;

    FunctionHandle get_node_function(NodeHandle hdl, error_code& ec) const;

    NodeHandle create_node(FunctionHandle hdl, error_code& ec);

    bool delete_node(NodeHandle hdl, error_code& ec);

    

    int get_argument_index(LinkHandle hdl, error_code& ec) const;

    NodeHandle get_argument_node(LinkHandle hdl, error_code& ec) const;

    int  get_return_index(LinkHandle hdl, error_code& ec) const;

    NodeHandle get_return_node(LinkHandle hdl, error_code& ec) const;

    LinkHandle create_link(NodeHandle return_hdl, unsigned int return_index,
                                       NodeHandle arg_hdl, unsigned int arg_index,
                                       error_code& ec);

    bool delete_link(LinkHandle hdl, error_code& ec);

    template<typename T>
    bool has_type(TypeHandle hdl, error_code& ec)
    {
        auto it = typeMap.find(hdl);
        if (it == typeMap.end())
        {
            ec = error_code::invalid_handle;
            return false;
        }

        auto type_index = it->second;
        if (type_index != bti::type_id<T>())
        {
            return false;
        }
        return true;
    }

private:

    template<typename T>
    void add_type()
    {
        add_type(bti::type_id<T>());
    }

    void add_type(bti::type_index type_index)
    {
        auto it = reverseTypeMap.find(type_index);

        if(it == reverseTypeMap.end())
        {
            boost::uuids::uuid name = generator();
            std::string nhdl = boost::uuids::to_string(name);
            reverseTypeMap[type_index] = nhdl;
            typeMap[nhdl] = type_index;
        }

    }

    boost::uuids::random_generator generator;

    enum FunctionType
    {
        SYSTEM_CREATED,
        USER_CREATED
    };

    struct Function
    {
        FunctionType type;
        std::unique_ptr<IFunction> function_;
    };

    struct Link
    {
        NodeHandle inNode_;
        NodeHandle outNode_;

        unsigned int inIndex_;
        unsigned int outIndex_;

        Link(NodeHandle inNode, NodeHandle outNode, unsigned int inIndex,
            unsigned int outIndex)
        : inNode_(inNode),
            outNode_(outNode),
            inIndex_(inIndex),
            outIndex_(outIndex)
        {
        };
    };

    struct Node
    {
        std::string prettyName_;
        FunctionHandle nodeFunction_;
        unsigned int argNumber_;
        unsigned int retNumber_;

        Node(std::string prettyName, FunctionHandle nodeFunction, unsigned int argNumber,
            unsigned int retNumber)
        : prettyName_(prettyName),
            nodeFunction_(nodeFunction),
            argNumber_(argNumber),
            retNumber_(retNumber)
        {
        };
    };

    std::map<FunctionHandle, Function> functionMap;

    std::map<LinkHandle, Link> linkMap;

    std::map<NodeHandle, Node> nodeMap;

    std::map<TypeHandle, boost::typeindex::type_index> typeMap;
    std::map<boost::typeindex::type_index, TypeHandle> reverseTypeMap;
};

} // namespace fcl