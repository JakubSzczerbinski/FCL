#include <vector> 
#include <memory>
#include <boost/type_index.hpp>

namespace bti = boost::typeindex;

using TypeVector = std::vector<bti::type_index>;

class IFunction
{
public:
	virtual void call(std::vector<void*>&, std::vector<std::unique_ptr<void>>&) = 0;
	virtual TypeVector inputArgs() = 0;
	virtual TypeVector outputArgs() = 0;
	virtual std::string name() = 0;
};

class Node;

class NodeLink
{
public:
	NodeLink (Node& node, unsigned idx, bti::type_index t) : node_(node), idx_(idx), type_(t) {}
	unsigned idx_;
	Node& node_;
	bti::type_index type_;
	void* get();
	bti::type_index type();
};

class Node
{
public:
	std::vector<std::shared_ptr<NodeLink>> inNodeLinks;
	std::vector<std::shared_ptr<NodeLink>> outNodeLinks;
	std::vector<std::unique_ptr<void>> outputData;
	std::unique_ptr<IFunction> function_;
	Node(std::unique_ptr<IFunction> f, std::vector<std::shared_ptr<NodeLink>> nodeLinks) : inNodeLinks(nodeLinks), function_(move(f)) 
	{
		auto types = f->outputArgs();
		for (size_t i = 0; i < types.size(); i++)
		{
			outNodeLinks.emplace_back(*this, i, types[i]);
		}
	}
	std::shared_ptr<NodeLink> getLink(unsigned int idx)
	{
		return outNodeLinks[idx];
	}
	void* getValue(unsigned int)
	{
		update();
		return function_.get();
	}
	void update()
	{
		std::vector<void*> inArgs;
		for (auto& link : inNodeLinks)
		{
			inArgs.push_back(link->get());
		}
		function_->call(inArgs, outputData);
	}
};

void* NodeLink::get()
{
	node_.update();
	return node_.getValue(idx_);
}

// template <typename ... IArgs> 
// class InputArgs
// {
// template <typename ... OArgs>
// class OutputArgs
// {

// class Function : IFunction
// {
// 	virtual void call(std::vector<std::unique_ptr<void>> iArgs, std::vector<std::unique_ptr<void>> oArgs) override;
// 	virtual TypeVector inputArgs() = 0;
// 	virtual TypeVector outputArgs() = 0;
// 	virtual std::string name() = 0;
// };

// };
	
// };

int main()
{
	return 0;
}