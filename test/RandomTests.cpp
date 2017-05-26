#include "gtest/gtest.h"
#include "gmock/gmock.h"
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

class RandomTests : public ::testing::Test
{
public:
	RandomTests() : str(new String("XD")), lower(new Lowercase()) {} 
	std::string& extractString(void* string)
	{
		return *reinterpret_cast<std::string*>(string);
	}
	std::unique_ptr<String> str;
	std::unique_ptr<Lowercase> lower;
};


TEST_F(RandomTests, functionCallShouldReturnString)
{
	auto ret = str->call({});
	std::string outputString = extractString(ret[0].get());
	ASSERT_EQ(outputString, "XD");
}

TEST_F(RandomTests, nodeGetShouldReturnString)
{
	fcl::Node node(str.get(), {});
	std::string& outputString = extractString(node.getValue(0));
	ASSERT_EQ(outputString, "XD");
}

TEST_F(RandomTests, nodeLinkGetShouldReturnString)
{
	fcl::Node node(str.get(), {});
	auto link = node.getLink(0);
	std::string& outputString = extractString(link->get());
	ASSERT_EQ(outputString, "XD");
}

TEST_F(RandomTests, functionCompositionShouldLowercase)
{
	fcl::Node xdString(str.get(), {});
	fcl::Node toLowercase(lower.get(), {xdString.getLink(0)});
	auto link = toLowercase.getLink(0);
	std::string& outputString = extractString(link->get());
	ASSERT_EQ(outputString, "xd");
}