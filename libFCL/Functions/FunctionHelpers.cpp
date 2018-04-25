
#include "FunctionHelpers.hpp"

namespace fcl
{

ArgVector to_arg_vector(const ReturnVector& rets)
{
	ArgVector args;
	for (auto&& ret : rets)
	{
		args.push_back(ret.get());
	}
	return args;
}

nonType* to_arg(const ReturnVector& rets, size_t index)
{
	return reinterpret_cast<nonType*>(rets[index].get());
}

}