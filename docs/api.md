# FCL API
- Able to load code from file 
~~~
bool load(std::string, error_code&);
~~~
- Write code to file 
~~~
bool write(std::string&, error_code&);
~~~
- info about availble functions availble 
~~~
std::vector<FunctionHandle> get_functions();
std::string get_function_pretty_name(FunctionHandler);
std::vector<TypeHandle> get_return_types(FunctionHandler);
std::vector<TypeHandle> get_arg_types(FunctionHandler);
~~~
- info about availble nodes
~~~
std::vector<NodeHandle> get_nodes();
std::vector<LinkHandle> get_links(NodeHandle);
std::string get_node_pretty_name(NodeHandle);
FunctionHandle get_node_function(NodeHandle);
~~~
- creating and editing nodes
~~~
NodeHandle new_node(FunctionHandle);
~~~
- getting results
~~~
JSON get_return(Node, unsigned);
template<typename T> T get_return(Node, unsigned);
~~~
- info about types
~~~
std::string get_type_pretty_name(TypeHandle);
~~~
- info about links
~~~
unsigned get_argument_index(LinkHandle);
NodeHandle get_argument_node(LinkHandle);
~~~
- creating link
~~~
LinkHandle new_link(NodeHandle, unsigned);
~~~