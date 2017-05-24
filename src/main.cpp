#include <vector> 
#include <memory>
#include <boost/type_index.hpp>
#include <iostream>
#include <cassert>

namespace bti = boost::typeindex;

using TypeVector = std::vector<bti::type_index>;

using nonType = char;


class IFunction
{
public:
	virtual std::vector<std::unique_ptr<nonType>> call(std::vector<nonType*>) = 0;
	virtual TypeVector inputArgs() = 0;
	virtual TypeVector outputArgs() = 0;
	virtual std::string name() = 0;
};

class String : public IFunction
{
public:
	String (std::string&& input) : str_(input) {}
	std::vector<std::unique_ptr<nonType>> call(std::vector<nonType*>) override
	{
		std::vector<std::unique_ptr<nonType>> result;
		result.emplace_back(reinterpret_cast<char*>(new std::string(str_)));
		return result;
	}
	TypeVector inputArgs() override
	{
		return {};
	}
	TypeVector outputArgs() override
	{
		return {bti::type_id<std::string>()};
	}
	std::string name() override
	{
		return "String";
	}
	std::string str_;
};

class Node;

class NodeLink
{
public:
	NodeLink (Node& node, unsigned idx, bti::type_index t) : node_(node), idx_(idx), type_(t) {}
	unsigned idx_;
	Node& node_;
	bti::type_index type_;
	nonType* get();
	bti::type_index type();
};

class Node
{
public:
	std::vector<std::shared_ptr<NodeLink>> inNodeLinks;
	std::vector<std::shared_ptr<NodeLink>> outNodeLinks;
	std::vector<std::unique_ptr<nonType>> outputData;
	std::unique_ptr<IFunction> function_;
	Node(std::unique_ptr<IFunction> f, std::vector<std::shared_ptr<NodeLink>> nodeLinks) : inNodeLinks(nodeLinks), function_(move(f)) 
	{
		auto types = f->outputArgs();
		for (size_t i = 0; i < types.size(); i++)
		{
			outNodeLinks.emplace_back(std::make_shared<NodeLink>(*this, i, types[i]));
		}
	}
	std::shared_ptr<NodeLink> getLink(unsigned int idx)
	{
		return outNodeLinks[idx];
	}
	nonType* getValue(unsigned int i)
	{
		update();
		return outputData[i].get();
	}
	void update()
	{
		std::vector<nonType*> inArgs;
		for (auto& link : inNodeLinks)
		{
			inArgs.push_back(link->get());
		}
		outputData = function_->call(inArgs);
	}
};

nonType* NodeLink::get()
{
	node_.update();
	return node_.getValue(idx_);
}

void simpleTest()
{
	String str("xd");
	auto ret = str.call({});
	std::string outputString = *reinterpret_cast<std::string*>(ret[0].get());
	assert(outputString == "xd");
}

int main()
{
	simpleTest();
	return 0;
}