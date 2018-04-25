
#include <fstream>
#include <vector>

#include <CLI/CLI.hpp>
#include <libFCL/FCL.hpp>
#include <libFCL/Parser/Lexer.hpp>


struct IntSerializerImpl
{
	std::string toString(int i)
	{
		return std::to_string(i);
	}
	int fromString(const std::string& string)
	{
		return std::stoi(string);
	}
	std::string typeName()
	{
		return "int";
	}
};

using IntSerializer = fcl::Serializer<int, IntSerializerImpl>;

int main(int argc, char** argv)
{
	CLI::App app{"Function composition language interpreter"};
	std::string input_file_path = "";
	app.add_option(
		"--input-file, -f", input_file_path, 
		"Path to file with function composition in fcl", false);

	CLI11_PARSE(app, argc, argv);

	std::ifstream input_file(input_file_path);
	if (not input_file.good() || not input_file.is_open())
	{
		std::cerr << "Failed to open file '" << input_file_path << "'" << std::endl;
		return 1;
	}

	auto program_text = std::string(
		std::istreambuf_iterator<char>(input_file),
        std::istreambuf_iterator<char>());

	fcl::Serializers serializers =
	{
		std::make_shared<IntSerializer>()
	};

	fcl::Functions functions = 
	{
		fcl::Input<int, int>::Output<int>::makeFunction(
			[&](int lhs, int rhs)
			{
				return new int(lhs + rhs);
			}, "int_add"),
		fcl::Input<int, int>::Output<int>::makeFunction(
			[&](int lhs, int rhs)
			{
				return new int(lhs - rhs);
			}, "int_sub")
	};

	std::cout << "Serializers info" << std::endl;
	for (auto& serializer : serializers)
	{
		std::cout << serializer->typeName() << std::endl;
	}


	std::cout << "Functions info" << std::endl;
	for (auto& function : functions)
	{
		std::cout << function->name() << " : "; 
		for (auto& input : function->argTypes())
		{
			std::cout << input.pretty_name() << " "; 
		}

		std::cout << "-> ";
		for (auto& output : function->returnTypes())
		{
			std::cout << output.pretty_name() << " "; 
		}

		std::cout << std::endl;
	}

	auto nodes = fcl::readNodes(program_text, functions, serializers);

	for (auto& returnNode : nodes.second)
	{
		auto endpoint = returnNode.sourceEndpoint;
		auto retType = endpoint.node()->function->returnTypes()[endpoint.index()];
		auto ret = fcl::evaluate(endpoint);
		auto it = std::find_if(serializers.begin(), serializers.end(),
			[&](std::shared_ptr<fcl::ISerializer> serializer)
			{
				return serializer->type().name() == retType.name();
			});

		if (it == serializers.end())
			throw std::runtime_error("Unable to deserialize return");

		auto serializer = *it;

		std::cout << endpoint.node()->name << "." << endpoint.index() << ": '";
		std::cout << serializer->serialize(ret.get()) << "'" << std::endl;
	}
}
