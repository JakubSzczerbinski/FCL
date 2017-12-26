
#include <vector>
#include <memory>

#include "AddFunction.hpp"
#include "SubtractFunction.hpp"
#include "MultiplyFunction.hpp"
#include "DivideFunction.hpp"
#include "NWDandNWWFunction.hpp"
#include "IntToStringFunction.hpp"
#include "ConcatFunction.hpp"
#include "UppercaseFunction.hpp"
#include "LowercaseFunction.hpp"


namespace fcl
{

inline Functions functions()
{
	std::vector<std::shared_ptr<IFunction>> funcs{};
	funcs.emplace_back(std::make_shared<AddFunction>());
	funcs.emplace_back(std::make_shared<SubtractFunction>());
	funcs.emplace_back(std::make_shared<MultiplyFunction>());
	funcs.emplace_back(std::make_shared<DivideFunction>());
	funcs.emplace_back(std::make_shared<NWDandNWWFunction>());
	funcs.emplace_back(std::make_shared<IntToStringFunction>());
	funcs.emplace_back(std::make_shared<ConcatFunction>());
	funcs.emplace_back(std::make_shared<UppercaseFunction>());
	funcs.emplace_back(std::make_shared<LowercaseFunction>());
	return funcs;
}
	
} // namespace fcl