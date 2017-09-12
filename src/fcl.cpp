
#include <fcl.hpp>


namespace fcl
{

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
        return {};
    }

    std::string get_function_pretty_name(FunctionHandle hdl, error_code& ec)
    {
        ec = error_code::not_implemented;
        return "";
    }

    std::vector<TypeHandle> get_return_types(FunctionHandle hdl, error_code& ec)
    {
        ec = error_code::not_implemented;
        return {};
    }

    std::vector<TypeHandle> get_arg_types(FunctionHandle hdl, error_code& ec)
    {
        ec = error_code::not_implemented;
        return {};
    }

    template <typename ValueType>
    FunctionHandle create_value_function(ValueType val, error_code& ec)
    {
        ec = error_code::not_implemented;
        return "";
    }

    bool delete_value_function(FunctionHandle hdl, error_code& ec)
    {
        ec = error_code::not_implemented;
        return false;
    }

    std::string get_type_pretty_name(TypeHandle hdl, error_code& ec)
    {
        ec = error_code::not_implemented;
        return "";
    }

    std::vector<NodeHandle> get_nodes()
    {
        return {};
    }

    std::vector<LinkHandle> get_arg_connections(NodeHandle hdl, error_code& ec)
    {
        ec = error_code::not_implemented;
        return {};
    }

    std::string get_node_pretty_name(NodeHandle hdl, error_code& ec)
    {
        ec = error_code::not_implemented;
        return {};
    }

    FunctionHandle get_node_function(NodeHandle hdl, error_code& ec)
    {
        ec = error_code::not_implemented;
        return "";
    }

    NodeHandle create_node(FunctionHandle hdl, error_code& ec)
    {
        ec = error_code::not_implemented;
        return "";
    }

    bool delete_node(NodeHandle hdl, error_code& ec)
    {
        ec = error_code::not_implemented;
        return false;
    }

    int get_argument_index(LinkHandle hdl)
    {
        return -1;
    }

    NodeHandle get_argument_node(LinkHandle hdl)
    {
        return "";
    }

    int get_return_index(LinkHandle hdl)
    {
        return -1;
    }

    NodeHandle get_return_node(LinkHandle hdl)
    {
        return "";
    }

    LinkHandle create_connection(NodeHandle return_hdl, int return_index,
                                       NodeHandle arg_hdl, int arg_index,
                                       error_code& ec)
    {
        ec = error_code::not_implemented;
        return "";
    }

    bool delete_connection(LinkHandle hdl, error_code& ec)
    {
        ec = error_code::not_implemented;
        return false;
    }

    template<typename T> 
    T* get_return(NodeHandle hdl, int index, error_code& ec)
    {
        ec = error_code::not_implemented;
        return nullptr;
    }

} // namespace fcl