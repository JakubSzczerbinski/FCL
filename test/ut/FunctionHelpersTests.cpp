
#include <gtest/gtest.h>

#include <libFCL/Functions/FunctionHelpers.hpp>
#include <libFCL/Functions/ValueFunction.hpp>

namespace fcl
{

TEST(FunctionHelpersTests, shouldCreateTypeVector)
{
	auto result = make_type_vector<int, float, double>();
	ASSERT_EQ(result.size(), 3);
	ASSERT_EQ(result[0], bti::type_id<int>());
	ASSERT_EQ(result[1], bti::type_id<float>());
	ASSERT_EQ(result[2], bti::type_id<double>());
}

TEST(FunctionHelpersTests, shouldCreateReturnVectorAndGetAsArgs)
{
	auto result = make_return_vector( new unsigned(1u)
									, new float(2.0f)
									, new std::string("3.0"));
	ASSERT_EQ(result.size(), 3);

	std::vector<nonType*> args;
	for (auto& el : result)
	{
		args.push_back(el.get());
	}

	auto arg0 = get_argument<unsigned>(args, 0);
	auto arg1 = get_argument<float>(args, 1);
	auto arg2 = get_argument<std::string>(args, 2);

	ASSERT_EQ(arg0, 1u);
	ASSERT_EQ(arg1, 2.0f);
	ASSERT_EQ(arg2, "3.0");
}

class TestDestruction
{
public:
	TestDestruction(std::function<void()> destuctionCallback)
		: destuctionCallback_(destuctionCallback)
	{}
	~TestDestruction(){ destuctionCallback_(); }
private:
	std::function<void()> destuctionCallback_;
};

TEST(FunctionHelpersTests, shouldDestructObjectProperly)
{
	bool isCalled = false;
	[&isCalled](){
		auto onDestruction = [&isCalled](){ isCalled = true; };
		auto result = make_return_vector(new TestDestruction(onDestruction));
		ASSERT_EQ(result.size(), 1);
	}();
	ASSERT_TRUE(isCalled);
}

TEST(FunctionHelpersTests, createdValueFunctionReturnsGoodValue)
{
	ValueFunction<float> func(10.3f);
	auto result = func.call({});

	std::vector<nonType*> args;
	for (auto& el : result)
	{
		args.push_back(el.get());
	}

	auto arg0 = get_argument<float>(args, 0);
	ASSERT_EQ(arg0, 10.3f);
}

TEST(FunctionHelpersTests, createFunctionFromLambdaAndExecute)
{
	auto func = Input<int>::Output<int>::makeFunction(
		[](const int& xd)-> int* 
		{
			return new int(1); 
		}, 
		"testFun");

	ValueFunction<int> val123(123);
	auto retVector = func->call({to_arg(val123.call({}), 0)});
	auto ret = get_return<int>(retVector, 0);
	ASSERT_EQ(1, ret);
}

TEST(FunctionHelpersTests, createFunctionFromLambdaAndForwardTheArg)
{
	auto func = Input<int>::Output<int>::makeFunction(
		[](const int& xd)-> int* 
		{
			return new int(xd); 
		}, 
		"testFun");

	ValueFunction<int> val123(123);
	auto retVector = func->call({to_arg(val123.call({}), 0)});
	auto ret = get_return<int>(retVector, 0);
	ASSERT_EQ(123, ret);
}

TEST(FunctionHelpersTests, createFunctionFromLambdaAndForwardThePointerArg)
{
	auto func = Input<int>::Output<int>::makeFunction(
		[](int* xd)-> int* 
		{
			return new int(*xd); 
		}, 
		"testFun");

	ValueFunction<int> val123(123);
	auto retVector = func->call({to_arg(val123.call({}), 0)});
	auto ret = get_return<int>(retVector, 0);
	ASSERT_EQ(123, ret);
}

}  // namespace fcl