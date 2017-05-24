#pragma once

#include <vector>
#include <memory>

#include <NodeLink.hpp>
#include <IFunction.hpp>

namespace fcl
{

class Node
{
public:
	std::vector<std::shared_ptr<NodeLink>> inNodeLinks;
	std::vector<std::shared_ptr<NodeLink>> outNodeLinks;
	std::vector<std::unique_ptr<nonType>> outputData;
	std::unique_ptr<IFunction> function_;
	Node(std::unique_ptr<IFunction> f, std::vector<std::shared_ptr<NodeLink>> nodeLinks) : inNodeLinks(nodeLinks), function_(move(f)) 
	{
		auto types = function_->outputArgs();
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

} // namespace fcl