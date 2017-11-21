#ifndef FCL_API_HPP
#define FCL_API_HPP

#include <string>
#include <vector>
#include <map>

namespace fcl
{

namespace detail
{
	enum error_code
	{
		success = 0,
		invalid_handle = 1,
		node_not_found = 2,
		not_implemented = 3,
		type_not_found = 4,
		link_not_found = 5,
		function_not_found = 6,
		invalid_index = 7,
		type_not_supported = 8,
		operation_permitted = 9
	};
} // namespace detail

	using error_code = detail::error_code;
	using FunctionHandle = std::string;
	using TypeHandle = std::string;
	using NodeHandle = std::string;
	using LinkHandle = std::string;

	/*
	**  FILES READ/WRITE
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
    **  Creates function that takes 0 arguments and always returns value
    **  Throws error if ValueType is unregistered.
    */
    template <typename ValueType>
    FunctionHandle create_value_function(ValueType val, error_code& ec);

    /*
    **   Deletes function created with create_value_function. Returns true if delete is sucessfull
    **   Throws error if @param hdl is invalid (is not value function handle).
    */
    bool delete_value_function(FunctionHandle hdl, error_code& ec);


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
	**	Returns handles to all links that end as args in @param hdl.
	**	Throws error on invalid handle.
	** 	Note: Calling this function for all nodes gives you unique list of all links.
	*/
	std::vector<LinkHandle> get_arg_links(NodeHandle hdl, error_code& ec);

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
	** 	Deletes node.
	**	Returns true on succesfull delete, returns false on unsuccesfull delete.
	**	Throws error on invalid handle.
	*/
	bool delete_node(NodeHandle hdl, error_code& ec);


	/*
	**  LINKS
	*/

	/*
	**  Returns index of argument at link arg endpoint or -1 on error.
	**  Throws error on invalid hdl.
	*/
	int get_argument_index(LinkHandle hdl, error_code& ec);

	/*
	**	Returns handle to node at link arg endpoint.
	**	Throws error on invalid hdl.
	*/
	NodeHandle get_argument_node(LinkHandle hdl, error_code& ec);

	/*
	**  Returns index of return at link return endpoint or -1 on error.
	**  Throws error on invalid hdl.
	*/
	int  get_return_index(LinkHandle hdl, error_code& ec);

	/*
	**	Returns handle to node at link return endpoint.
	**	Throws error on invalid hdl.
	*/
	NodeHandle get_return_node(LinkHandle hdl, error_code& ec);

	/*
	**	Creates link with @param return_hdl, @param return_index as return endpoint
	** 	and @param arg_hdl, @param arg_index as argument endpoint
	**  Returns handle to this link.
	**	Throws error on invalid handle or index
	*/
	LinkHandle create_link(NodeHandle return_hdl, int return_index,
									   NodeHandle arg_hdl, int arg_index,
									   error_code& ec);

	/*
	** 	Deleted link.
	**	Returns true on succesfull delete, returns false on unsuccesfull delete.
	**	Throws error on invalid handle.
	*/
	bool delete_link(LinkHandle hdl, error_code& ec);


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
	template<typename T> T get_return(NodeHandle hdl, int index, error_code& ec);                                            

} // namespace fcl

#endif // FCL_API_HPP