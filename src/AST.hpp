
#include <vector>
#include <string>

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

    std::vector<ConnectionHandle> get_arg_connections(NodeHandle hdl, error_code& ec);

    std::string get_node_pretty_name(NodeHandle hdl, error_code& ec);

    FunctionHandle get_node_function(NodeHandle hdl, error_code& ec);

    NodeHandle create_node(FunctionHandle hdl, error_code& ec);

    bool delete_node(NodeHandle hdl, error_code& ec);

    

    int get_argument_index(ConnectionHandle hdl);

    NodeHandle get_argument_node(ConnectionHandle hdl);

    int  get_return_index(ConnectionHandle hdl);

    NodeHandle get_return_node(ConnectionHandle hdl);

    ConnectionHandle create_connection(NodeHandle return_hdl, int return_index,
                                       NodeHandle arg_hdl, int arg_index,
                                       error_code& ec);

    bool delete_connection(ConnectionHandle hdl, error_code& ec);
};

} // namespace fcl