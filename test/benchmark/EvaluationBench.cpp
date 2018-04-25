
#include <benchmark/benchmark.h>

#include <libFCL/Nodes/LoadNodes.hpp>
#include <libFCL/Functions/FunctionHelpers.hpp>

#include <libFCL/Parser/Parser.hpp>
#include <libFCL/Parser/Lexer.hpp>
#include <libFCL/Evaluator/Evaluator.hpp>

#include <fakes/functions/Functions.hpp>
#include <fakes/serializers/Serializers.hpp>

namespace fcl
{

static void EvaluationBench(benchmark::State& state) {
  for (auto _ : state) {
	state.PauseTiming();
	std::string program_text = 
R"program(
	string hello = 'Hello'
	string math = 'Math'
	string space = ' '
	string equals = ' = '
	string plus = ' + '
	int a = '2'
	int b = '3'
	concat1 = concat(hello.0, space.0)
	helloMath = concat(concat1.0, math.0)
	add = int_add(a.0, b.0)
	stringResult = int_to_string(add.0)
	aString = int_to_string(a.0)
	bString = int_to_string(b.0)
	aAndAdd = concat(aString.0, plus.0)
	operation = concat(aAndAdd.0, bString.0)
	aPlusbEq = concat(operation.0, equals.0)
	equation = concat(aPlusbEq.0, stringResult.0)
	helloMathSpace = concat(helloMath.0, space.0)
	result = concat(helloMathSpace.0, equation.0)
	resultLowercase = lowercase(result.0)
	resultUppercase = uppercase(result.0)
)program";
	auto program = parse(program_text);
	assert(program.size() != 0);

	auto nodes = loadNodes(program, functions(), serializers());

	auto it = std::find_if(nodes.first.begin(), nodes.first.end(),
		[&](auto&& node){ return node->name == "resultLowercase";});
	assert(it != nodes.first.end());

    state.ResumeTiming();
	auto result = evaluate((*it)->sourceEndpoint(0));
    auto val = get_return<std::string>(result);
    assert(val == "hello math 2 + 3 = 5");
  }
}

BENCHMARK(EvaluationBench);

}
