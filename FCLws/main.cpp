
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <algorithm>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <json/json.h>
#include <CLI/CLI.hpp>
#include <libFCL/FCL.hpp>

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

std::string get_path(const std::string& resource_path)
{
    static std::map<std::string, std::string> resource_map =
        {{"/", "html/index.html"}};

    auto it = resource_map.find(resource_path);
    if (it != resource_map.end())
    {
        return it->second;
    }

    return "html" + resource_path;
}

std::string get_mime(const std::string& path)
{
    static std::vector<std::pair<std::string, std::string>> ext_to_mime =
        { {"(html|htm)", "text/html; charset=utf-8"}
        , {"js", "application/javascript"}
        , {"txt", "text/plain"} 
        , {"css", "text/css"} };
    for (auto&& etm : ext_to_mime)
    {
        auto regex = std::regex("^.*\." + etm.first + "$");
        auto& mime_type = etm.second;
        if (std::regex_search(path, regex))
            return mime_type;
    }
    return "text/plain";
}

using server = websocketpp::server<websocketpp::config::asio>;

int main(int argc, char** argv) {
    CLI::App app{"Function composition language interpreter"};
	int port = 8000;
	app.add_option(
		"--parse, -p", port, 
		"server port", true);
	CLI11_PARSE(app, argc, argv);

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

    server server_instance;
    server_instance.set_message_handler(
        [&](websocketpp::connection_hdl hdl, server::message_ptr msg) {
            server::connection_ptr con = server_instance.get_con_from_hdl(hdl);
            std::cout << msg->get_payload() << std::endl;

            Json::Value root;
            std::stringstream ss(msg->get_payload());
            ss >> root;
            assert(root.isObject());

            std::string type = root.get("type", "invalid").asString();
            if (type == "invalid")
                return;
            
            if (type == "hello")
            {
                std::cerr << "Client connected. Sending available function, serializers, converters..." << std::endl;
                Json::Value response;

                Json::Value json_serializers(Json::arrayValue);
                for (auto&& serializer : serializers)
                {
                    Json::Value json_serializer;
                    json_serializer["type"] = serializer->type().pretty_name();
                    json_serializers.append(json_serializer);
                }

                Json::Value json_functions(Json::arrayValue);
                for (auto&& function : functions)
                {
                    Json::Value json_function;
                    json_function["name"] = function->name();
                    Json::Value inputs;
                    for (auto&& type : function->argTypes())
                    {
                        Json::Value input;
                        input["type"] = type.pretty_name();
                        inputs.append(input);
                    }
                    Json::Value outputs;
                    for (auto&& type: function->returnTypes())
                    {
                        Json::Value output;
                        output["type"] = type.pretty_name();
                        outputs.append(output);
                    }
                    json_function["inputs"] = inputs;
                    json_function["outputs"] = outputs;
                    json_functions.append(json_function);
                }
                response["type"] = "hi";
                response["content"]["serializers"] = json_serializers;
                response["content"]["functions"] = json_functions;
                Json::FastWriter fastWriter;
                std::string response_data = fastWriter.write(response);
                std::cerr << response_data << std::endl;
                server_instance.send(hdl, response_data, websocketpp::frame::opcode::text);
                return;
            }

            if (type == "evaluate")
            {
                std::cerr << "Client requests program execution." << std::endl;
                std::string program_text = root.get("program", "").asString();
                if (program_text == "")
                {
                    std::cerr << "Invalid message" << std::endl;
                    return;
                }
                std::cerr << program_text << std::endl;

                Json::Value response;
                try{

                    auto nodes = fcl::readNodes(program_text, functions, serializers);

                    Json::Value returns(Json::arrayValue);
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

                        Json::Value json_ret;
                        json_ret["node_name"] = endpoint.node()->name;
                        json_ret["node_index"] = endpoint.index();
                        json_ret["result"] = serializer->serialize(ret.get());
                        returns.append(json_ret);
                        // std::cerr << endpoint.node()->name << "." << endpoint.index() << ": '";
                        // std::cerr << serializer->serialize(ret.get()) << "'" << std::endl;
                    }
                    response["content"] = returns;
                    response["type"] = "result";
                } catch(...)
                {
                    response["content"] = "Failed to evaluate";
                    response["type"] = "error";
                }
                Json::FastWriter fastWriter;
                std::string response_data = fastWriter.write(response);
                std::cerr << response_data << std::endl;
                server_instance.send(hdl, response_data, websocketpp::frame::opcode::text);
                return;
            }
        });

    server_instance.set_http_handler(
    	[&](websocketpp::connection_hdl hdl)
    	{
            server::connection_ptr con = server_instance.get_con_from_hdl(hdl);
            std::string path = get_path(con->get_resource());
            std::cerr << "Request for " << con->get_resource() << std::endl;

            std::ifstream file(path);

            if (not file.is_open() || not file.good())
            {
                con->set_status(websocketpp::http::status_code::not_found);
                std::cerr << "Unable to serve " << path << " Reason: failed to open file." << std::endl;
                return;
            }

            std::string file_data = 
                { std::istreambuf_iterator<char>(file)
                , std::istreambuf_iterator<char>() };

            std::cerr << "Responding with file " << path << std::endl;
            con->append_header("Content-Type", get_mime(path));
            con->set_body(file_data);
            con->set_status(websocketpp::http::status_code::ok);
    	});
    server_instance.init_asio();
    server_instance.listen(port);
    server_instance.start_accept();

    server_instance.run();
};
