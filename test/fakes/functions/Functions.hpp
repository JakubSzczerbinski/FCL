
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

using Funcs = std::vector<std::unique_ptr<IFunction>>;

Funcs functions()
{
	std::vector<std::unique_ptr<IFunction>> funcs{};
	funcs.emplace_back(std::make_unique<AddFunction>());
	funcs.emplace_back(std::make_unique<SubtractFunction>());
	funcs.emplace_back(std::make_unique<MultiplyFunction>());
	funcs.emplace_back(std::make_unique<DivideFunction>());
	funcs.emplace_back(std::make_unique<NWDandNWWFunction>());
	funcs.emplace_back(std::make_unique<IntToStringFunction>());
	funcs.emplace_back(std::make_unique<ConcatFunction>());
	funcs.emplace_back(std::make_unique<UppercaseFunction>());
	funcs.emplace_back(std::make_unique<LowercaseFunction>());
	return funcs;
}
	
} // namespace fcl