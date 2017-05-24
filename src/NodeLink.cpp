
#include <NodeLink.hpp>
#include <Node.hpp>

namespace fcl
{

nonType* NodeLink::get()
{
	node_.update();
	return node_.getValue(idx_);
}

} // namespace fcl