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
	}
} // namespace detail

	using error_code = detail::error_code;
	using FunctionHandle = std::string;
	using TypeHandle = std::string;
	using NodeHandle = std::string;
	using ConnectionHandle = std::string;

	bool load(const std::string&, error_code&);
	bool write(std::string&, error_code&);

	std::vector<FunctionHandle> get_functions();
	std::string get_function_pretty_name(const FunctionHandle&, error_code&);
	std::vector<TypeHandle> get_return_types(const FunctionHandle&, error_code&);
	std::vector<TypeHandle> get_arg_types(const FunctionHandle&, error_code&);

	std::vector<NodeHandle> get_nodes();
	std::vector<ConnectionHandle> get_links(const NodeHandle&, error_code&);
	std::string get_node_pretty_name(const NodeHandle&, error_code&);
	FunctionHandle get_node_function(const NodeHandle&, error_code&);

	NodeHandle new_node(const FunctionHandle&, error_code&);

	template<typename T> T get_return(const NodeHandle&, unsigned);

	std::string get_type_pretty_name(const TypeHandle&);

	unsigned get_argument_index(const ConnectionHandle&);
	NodeHandle get_argument_node(const ConnectionHandle&);

	ConnectionHandle new_link(const NodeHandle&, unsigned);

} // namespace fcl

#endif // FCL_API_HPP