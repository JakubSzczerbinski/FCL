#ifndef FCL_API_HPP
#define FCL_API_HPP

#include <string>
#include <vector>

namespace fcl
{

namespace detail
{
	enum error_code
	{
		success,
		not_implemented
	};
} // namespace detail

	using error_code = detail::error_code;
	using FunctionHandle = std::string;
	using TypeHandle = std::string;
	using NodeHandle = std::string;
	using ConnectionHandle = std::string;

	/*
	** 	FILES READ/WRITE
	*/

	/* 	
	** 	Loads program from file.
	*/
	bool load(std::string path, error_code& ec);

	/* 
	** 	Writes current program to file.
	*/
	bool write(std::string path, error_code& ec);


	/*
	**	FUNCTIONS
	*/

	/*
	**  Returns handle to all availble functions.
	*/
	std::vector<FunctionHandle> get_functions();

	/*
	**	Returns human readable name of function.
	**  Throws error on invalid function handle.
	*/
	std::string get_function_pretty_name(FunctionHandle hdl, error_code& ec);

	/*
	**  Returns handles to return types of function.
	**  Throws error on invalid function handle.
	*/ 
	std::vector<TypeHandle> get_return_types(FunctionHandle hdl, error_code& ec);

	/*
	**  Returns handles to argument types of function.
	**  Throws error on invalid function handle.
	*/
	std::vector<TypeHandle> get_arg_types(FunctionHandle hdl, error_code& ec);


	/*
	**	TYPES
	*/

	/*
	**	Returns human readable name of type.
	**	Throws error on invalid handle.
	*/
	std::string get_type_pretty_name(TypeHandle hdl, error_code& ec);


	/*
	**	NODES
	*/

	/*
	**  Returns handles to all availble nodes.
	*/
	std::vector<NodeHandle> get_nodes();

	/*
	**	Returns handles to all connections that end as args in @param hdl.
	**	Throws error on invalid handle.
	** 	Note: Calling this function for all nodes gives you unique list of all connections.
	*/
	std::vector<ConnectionHandle> get_arg_connections(NodeHandle hdl, error_code& ec);

	/*
	**	Returns human readable name of the node.
	**  Throws error on invalid handle.
	*/
	std::string get_node_pretty_name(NodeHandle hdl, error_code& ec);

	/*
	**  Returns handle to node's function.
	**	Throws error on invalid handle.
	*/
	FunctionHandle get_node_function(NodeHandle hdl, error_code& ec);

	/*
	**	Creates function with @param hdl as node's function, then
	** 	Returns handle to newly created node.
	**	Throws error on invalid handle.
	*/
	NodeHandle create_node(FunctionHandle hdl, error_code& ec);

	/*
	** 	Deleted node.
	**	Returns true on succesfull delete, returns false on unsuccesfull delete.
	**	Throws error on invalid handle.
	*/
	bool delete_node(NodeHandle hdl, error_code& ec);


	/*
	**  CONNECTIONS
	*/

	/*
	**  Returns index of argument at connection arg endpoint or -1 on error.
	**  Throws error on invalid hdl.
	*/
	int get_argument_index(ConnectionHandle hdl);

	/*
	**	Returns handle to node at connection arg endpoint.
	**	Throws error on invalid hdl.
	*/
	NodeHandle get_argument_node(ConnectionHandle hdl);

	/*
	**  Returns index of return at connection return endpoint or -1 on error.
	**  Throws error on invalid hdl.
	*/
	int  get_return_index(ConnectionHandle hdl);

	/*
	**	Returns handle to node at connection return endpoint.
	**	Throws error on invalid hdl.
	*/
	NodeHandle get_return_node(ConnectionHandle hdl);

	/*
	**	Creates connection with @param return_hdl, @param return_index as return endpoint
	** 	and @param arg_hdl, @param arg_index as argument endpoint
	**  Returns handle to this connection.
	**	Throws error on invalid handle or index
	*/
	ConnectionHandle create_connection(NodeHandle return_hdl, int return_index,
									   NodeHandle arg_hdl, int arg_index,
									   error_code& ec);

	/*
	** 	Deleted connection.
	**	Returns true on succesfull delete, returns false on unsuccesfull delete.
	**	Throws error on invalid handle.
	*/
	bool delete_connection(ConnectionHandle hdl, error_code& ec);


	/*
	**	OUTPUT
	*/

	/*
	** 	Returns return of node @param hdl at @param index, or nullptr if error occured.
	** 	Throws error on:
	** 		- invalid handle 
	** 		- invalid index
	**		- unable to cast to type T
	*/
	template<typename T> T* get_return(NodeHandle hdl, int index, error_code& ec);

} // namespace fcl

#endif // FCL_API_HPP