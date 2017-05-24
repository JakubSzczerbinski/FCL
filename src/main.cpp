#include <vector> 
#include <memory>
#include <iostream>
#include <string>
#include <cassert>

#include <boost/type_index.hpp>

#include <IFunction.hpp>
#include <Constants.hpp>
#include <Node.hpp>

namespace bti = boost::typeindex;

class String : public fcl::IFunction
{
public:
	String (std::string&& input) : str_(input) {}
	std::vector<std::unique_ptr<fcl::nonType>> call(std::vector<fcl::nonType*>) override
	{
		std::vector<std::unique_ptr<fcl::nonType>> result;
		result.emplace_back(reinterpret_cast<fcl::nonType*>(new std::string(str_)));
		return result;
	}
	fcl::TypeVector inputArgs() override
	{
		return {};
	}
	fcl::TypeVector outputArgs() override
	{
		return {bti::type_id<std::string>()};
	}
	std::string name() override
	{
		return "String";
	}
private:
	std::string str_;
};

class Lowercase : public fcl::IFunction
{
public:
	std::vector<std::unique_ptr<fcl::nonType>> call(std::vector<fcl::nonType*> args) override
	{
		std::vector<std::unique_ptr<fcl::nonType>> result;
		std::string& str = *reinterpret_cast<std::string*>(args[0]);
		auto outStr = new std::string();

		for (auto& ch : str)
		{
			outStr->push_back(std::tolower(ch));
		}

		result.emplace_back(reinterpret_cast<fcl::nonType*>(outStr));
		return result;
	}
	fcl::TypeVector inputArgs() override
	{
		return {};
	}
	fcl::TypeVector outputArgs() override
	{
		return {bti::type_id<std::string>()};
	}
	std::string name() override
	{
		return "String";
	}
};

void testStringFunction()
{
	String str("xd");
	auto ret = str.call({});
	std::string outputString = *reinterpret_cast<std::string*>(ret[0].get());
	assert(outputString == "xd");
}

void testStringFunctionNode()
{
	fcl::Node node(std::make_unique<String>("XD"), {});
	std::string& outputString = *reinterpret_cast<std::string*>(node.getValue(0));
	assert(outputString == "XD");
}

void testStringFunctionNodeWithNodeLink()
{
	fcl::Node node(std::make_unique<String>("XD"), {});
	auto link = node.getLink(0);
	std::string& outputString = *reinterpret_cast<std::string*>(link->get());
	assert(outputString == "XD");
}

void testOneArgFunctionComposition()
{
	fcl::Node xdString(std::make_unique<String>("XD"), {});
	fcl::Node toLowercase(std::make_unique<Lowercase>(), {xdString.getLink(0)});
	auto link = toLowercase.getLink(0);
	std::string& outputString = *reinterpret_cast<std::string*>(link->get());
	assert(outputString == "xd");
}

int main()
{
	testStringFunction();
	testStringFunctionNode();
	testStringFunctionNodeWithNodeLink();
	return 0;
}