
#include <NodeLink.hpp>
#include <Node.hpp>

namespace fcl
{

nonType* NodeLink::get()
{
	return node_.getValue(idx_);
}

} // namespace fcl