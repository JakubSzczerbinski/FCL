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
public:
    AST(std::vector<IFunction*> functions);


    std::vector<FunctionHandle> get_functions() const;

    std::string get_function_pretty_name(FunctionHandle hdl, error_code& ec) const;

    std::vector<TypeHandle> get_return_types(FunctionHandle hdl, error_code& ec) const;

    std::vector<TypeHandle> get_arg_types(FunctionHandle hdl, error_code& ec) const;

    template <typename ValueType>
    FunctionHandle create_value_function(ValueType val, error_code& ec);

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


private:

    boost::uuids::random_generator generator;

    struct Function
    {
        IFunction* function_;
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