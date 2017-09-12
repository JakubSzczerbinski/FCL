#pragma once

#include <map>
#include <string>
#include <vector>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <IFunction.hpp>
#include <fcl.hpp>

namespace fcl
{

class AST
{
    std::vector<FunctionHandle> get_functions();

    std::string get_function_pretty_name(FunctionHandle hdl, error_code& ec);

    std::vector<TypeHandle> get_return_types(FunctionHandle hdl, error_code& ec);

    std::vector<TypeHandle> get_arg_types(FunctionHandle hdl, error_code& ec);

    template <typename ValueType>
    FunctionHandle create_value_function(ValueType val, error_code& ec);

    bool delete_value_function(FunctionHandle hdl, error_code& ec);



    std::string get_type_pretty_name(TypeHandle hdl, error_code& ec);



    std::vector<NodeHandle> get_nodes();

    std::vector<LinkHandle> get_arg_links(NodeHandle hdl, error_code& ec);

    std::string get_node_pretty_name(NodeHandle hdl, error_code& ec);

    FunctionHandle get_node_function(NodeHandle hdl, error_code& ec);

    NodeHandle create_node(FunctionHandle hdl, error_code& ec);

    bool delete_node(NodeHandle hdl, error_code& ec);

    

    int get_argument_index(LinkHandle hdl, error_code& ec);

    NodeHandle get_argument_node(LinkHandle hdl, error_code& ec);

    int  get_return_index(LinkHandle hdl, error_code& ec);

    NodeHandle get_return_node(LinkHandle hdl, error_code& ec);

    LinkHandle create_link(NodeHandle return_hdl, int return_index,
                                       NodeHandle arg_hdl, int arg_index,
                                       error_code& ec);

    bool delete_link(LinkHandle hdl, error_code& ec);


private:

    boost::uuids::random_generator generator;

    struct Function
    {
        IFunction* function;
    };

    struct Link
    {
        NodeHandle inNode;
        NodeHandle outNode;
    };

    struct Node
    {
        std::string prettyName;
        FunctionHandle nodeFunction;
    };

    std::map<FunctionHandle, Function> functionMap;

    std::map<LinkHandle, Link> linkMap;
    
    std::map<NodeHandle, Node> nodeMap;
    
    std::map<TypeHandle, boost::typeindex::type_index> typeMap;
    std::map<boost::typeindex::type_index, TypeHandle> reverseTypeMap;
};

} // namespace fcl