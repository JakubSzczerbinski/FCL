
#include <fcl.hpp>
#include <AST/AST.hpp>


namespace fcl
{

    AST& ast()
    {
        static AST ast{};
        return ast;
    }

    bool load(std::string path, error_code& ec)
    {
        ec = error_code::not_implemented;
        return false;
    }

    bool write(std::string path, error_code& ec)
    {
        ec = error_code::not_implemented;
        return false;
    }

    std::vector<FunctionHandle> get_functions()
    {
        return ast().get_functions();
    }

    std::string get_function_pretty_name(FunctionHandle hdl, error_code& ec)
    {
        return ast().get_function_pretty_name(hdl, ec);
    }

    std::vector<TypeHandle> get_return_types(FunctionHandle hdl, error_code& ec)
    {
        return ast().get_return_types(hdl, ec);
    }

    std::vector<TypeHandle> get_arg_types(FunctionHandle hdl, error_code& ec)
    {
        return ast().get_arg_types(hdl, ec);
    }

    template <typename ValueType>
    FunctionHandle create_value_function(ValueType val, error_code& ec)
    {
        return ast().create_value_function(val, ec);
    }

    bool delete_value_function(FunctionHandle hdl, error_code& ec)
    {
        return ast().delete_value_function(hdl, ec);
    }

    std::string get_type_pretty_name(TypeHandle hdl, error_code& ec)
    {
        return ast().get_type_pretty_name(hdl, ec);
    }

    std::vector<NodeHandle> get_nodes()
    {
        return ast().get_nodes();
    }

    std::vector<LinkHandle> get_arg_links(NodeHandle hdl, error_code& ec)
    {
        return ast().get_arg_links(hdl, ec);
    }

    std::string get_node_pretty_name(NodeHandle hdl, error_code& ec)
    {
        return ast().get_node_pretty_name(hdl, ec);
    }

    FunctionHandle get_node_function(NodeHandle hdl, error_code& ec)
    {
        return ast().get_node_function(hdl, ec);
    }

    NodeHandle create_node(FunctionHandle hdl, error_code& ec)
    {
        return ast().create_node(hdl, ec);
    }

    bool delete_node(NodeHandle hdl, error_code& ec)
    {
        return ast().delete_node(hdl, ec);
    }

    int get_argument_index(LinkHandle hdl, error_code& ec)
    {
        return ast().get_argument_index(hdl, ec);
    }

    NodeHandle get_argument_node(LinkHandle hdl, error_code& ec)
    {
        return ast().get_argument_node(hdl, ec);
    }

    int get_return_index(LinkHandle hdl, error_code& ec)
    {
        return ast().get_return_index(hdl, ec);
    }

    NodeHandle get_return_node(LinkHandle hdl, error_code& ec)
    {
        return ast().get_return_node(hdl, ec);
    }

    LinkHandle create_link(NodeHandle return_hdl, int return_index,
                           NodeHandle arg_hdl, int arg_index,
                           error_code& ec)
    {
        return ast().create_link(return_hdl, return_index, arg_hdl, arg_index, ec);
    }

    bool delete_connection(LinkHandle hdl, error_code& ec)
    {
        return ast().delete_link(hdl, ec);
    }

    template<typename T> 
    T* get_return(NodeHandle hdl, int index, error_code& ec)
    {
        ec = error_code::not_implemented;
        return nullptr;
    }

} // namespace fcl