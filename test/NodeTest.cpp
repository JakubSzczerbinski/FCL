#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <vector>
#include <string>

#include <boost/type_index.hpp>

#include <Node.hpp>

#include <mocks/IFunctionMock.hpp>

namespace fcl
{

using namespace testing;
namespace bti = boost::typeindex;

class NodeTest : public ::testing::Test
{
public:
	NodeTest () 
		: returnXDMock(new IFunctionMock())
		, verifyinputMock(new IFunctionMock())
		, lowercaseMock(new IFunctionMock())
		, uppercaseMock(new IFunctionMock())
		, concatMock(new IFunctionMock())
	{
		EXPECT_CALL(*lowercaseMock, outputArgs())
			.WillRepeatedly(Return(std::vector<bti::type_index>{bti::type_id<std::string>()}));
		EXPECT_CALL(*lowercaseMock, inputArgs())
			.WillRepeatedly(Return(std::vector<bti::type_index>{bti::type_id<std::string>()}));

		EXPECT_CALL(*lowercaseMock, call(_))
			.Times(AtLeast(0))
			.WillRepeatedly(Invoke([this](std::vector<nonType*> args) -> std::vector<std::unique_ptr<nonType>>
			{
				auto value = extractValue<std::string>(args[0]);
				std::string result;
				for (auto& c : value)
				{
					result.push_back(std::tolower(c));
				}
				std::vector<std::unique_ptr<nonType>> retvals;
				retvals.push_back(makeNonTypeUniquePtr<std::string>(result));
				return retvals;
			}));

		EXPECT_CALL(*uppercaseMock, outputArgs())
			.WillRepeatedly(Return(std::vector<bti::type_index>{bti::type_id<std::string>()}));
		EXPECT_CALL(*uppercaseMock, inputArgs())
			.WillRepeatedly(Return(std::vector<bti::type_index>{bti::type_id<std::string>()}));

		EXPECT_CALL(*uppercaseMock, call(_))
			.Times(AtLeast(0))
			.WillRepeatedly(Invoke([this](std::vector<nonType*> args) -> std::vector<std::unique_ptr<nonType>>
			{
				auto value = extractValue<std::string>(args[0]);
				std::string result;
				for (auto& c : value)
				{
					result.push_back(std::toupper(c));
				}
				std::vector<std::unique_ptr<nonType>> retvals;
				retvals.push_back(makeNonTypeUniquePtr<std::string>(result));
				return retvals;
			}));

		EXPECT_CALL(*concatMock, outputArgs())
			.WillRepeatedly(Return(std::vector<bti::type_index>{bti::type_id<std::string>()}));
		EXPECT_CALL(*concatMock, inputArgs())
			.WillRepeatedly(Return(std::vector<bti::type_index>{bti::type_id<std::string>(), bti::type_id<std::string>()}));

		EXPECT_CALL(*concatMock, call(_))
			.Times(AtLeast(0))
			.WillRepeatedly(Invoke([this](std::vector<nonType*> args) -> std::vector<std::unique_ptr<nonType>>
			{
				auto str1 = extractValue<std::string>(args[0]);
				auto str2 = extractValue<std::string>(args[1]);
				std::string result = str1 + str2;
				std::vector<std::unique_ptr<nonType>> retvals;
				retvals.push_back(makeNonTypeUniquePtr<std::string>(result));
				return retvals;
			}));
	}
	std::unique_ptr<IFunctionMock> returnXDMock;
	std::unique_ptr<IFunctionMock> verifyinputMock;
	std::unique_ptr<IFunctionMock> lowercaseMock;
	std::unique_ptr<IFunctionMock> uppercaseMock;
	std::unique_ptr<IFunctionMock> concatMock;
	template <typename T, typename ... Args>
	std::unique_ptr<nonType> makeNonTypeUniquePtr(const Args& ... args)
	{
		return std::unique_ptr<nonType>(reinterpret_cast<nonType*>(new T(args ...)));
	}
	template <typename T>
	T& extractValue(nonType* ptr)
	{
		return *reinterpret_cast<T*>(ptr);
	}
};

TEST_F(NodeTest, getValuesFromNodesShouldReturnValidValue)
{
	EXPECT_CALL(*returnXDMock, outputArgs())
		.WillRepeatedly(Return(std::vector<bti::type_index>{bti::type_id<std::string>()}));

	auto testNode = Node(returnXDMock.get(), {});

	EXPECT_CALL(*returnXDMock, call(_))
		.WillOnce(Invoke([this](std::vector<nonType*>)
		{
			std::vector<std::unique_ptr<nonType>> retvals;
			retvals.push_back(makeNonTypeUniquePtr<std::string>("XD"));
			return move(retvals);
		}));

	auto value = extractValue<std::string>(testNode.getValue(0));
	EXPECT_EQ("XD", value);
}

TEST_F(NodeTest, getValuesFromNodesTwiceShouldReturnValidValue)
{
	EXPECT_CALL(*returnXDMock, outputArgs())
		.WillRepeatedly(Return(std::vector<bti::type_index>{bti::type_id<std::string>()}));

	auto testNode = Node(returnXDMock.get(), {});

	EXPECT_CALL(*returnXDMock, call(_))
		.Times(AtLeast(1))
		.WillRepeatedly(Invoke([this](std::vector<nonType*>)
		{
			std::vector<std::unique_ptr<nonType>> retvals;
			retvals.push_back(makeNonTypeUniquePtr<std::string>("XD"));
			return move(retvals);
		}));

	auto value = extractValue<std::string>(testNode.getValue(0));
	EXPECT_EQ("XD", value);
	auto value2 = extractValue<std::string>(testNode.getValue(0));
	EXPECT_EQ("XD", value2);
}

TEST_F(NodeTest, getValuesFromNodesShouldCallWithValidArgs)
{
	EXPECT_CALL(*returnXDMock, outputArgs())
		.WillRepeatedly(Return(std::vector<bti::type_index>{bti::type_id<std::string>()}));
	EXPECT_CALL(*returnXDMock, inputArgs())
		.WillRepeatedly(Return(std::vector<bti::type_index>{}));

	auto xdNode = Node(returnXDMock.get(), {});

	EXPECT_CALL(*returnXDMock, call(_))
		.Times(AtLeast(1))
		.WillRepeatedly(Invoke([this](std::vector<nonType*>)
		{
			std::vector<std::unique_ptr<nonType>> retvals;
			retvals.push_back(makeNonTypeUniquePtr<std::string>("XD"));
			return retvals;
		}));

	EXPECT_CALL(*verifyinputMock, outputArgs())
		.WillRepeatedly(Return(std::vector<bti::type_index>{bti::type_id<std::string>()}));
	EXPECT_CALL(*verifyinputMock, inputArgs())
		.WillRepeatedly(Return(std::vector<bti::type_index>{bti::type_id<std::string>()}));

	auto verifyinputNode = Node(verifyinputMock.get(), {xdNode.getLink(0)});
	bool inputEqualsXD = false;

	EXPECT_CALL(*verifyinputMock, call(_))
		.Times(AtLeast(1))
		.WillRepeatedly(Invoke([this, &inputEqualsXD](std::vector<nonType*> args) -> std::vector<std::unique_ptr<nonType>>
		{
			auto value = extractValue<std::string>(args[0]);
			inputEqualsXD = ("XD" == value);
			std::vector<std::unique_ptr<nonType>> result;
			result.push_back(makeNonTypeUniquePtr<std::string>(value));
			return result;
		}));

	auto value = extractValue<std::string>(verifyinputNode.getValue(0));

	EXPECT_TRUE(inputEqualsXD);
	EXPECT_EQ(value, "XD");
}

TEST_F(NodeTest, lowercaseFunctionShouldLowercase)
{
	EXPECT_CALL(*returnXDMock, outputArgs())
		.WillRepeatedly(Return(std::vector<bti::type_index>{bti::type_id<std::string>()}));
	EXPECT_CALL(*returnXDMock, inputArgs())
		.WillRepeatedly(Return(std::vector<bti::type_index>{}));

	auto xdNode = Node(returnXDMock.get(), {});

	EXPECT_CALL(*returnXDMock, call(_))
		.Times(AtLeast(1))
		.WillRepeatedly(Invoke([this](std::vector<nonType*>)
		{
			std::vector<std::unique_ptr<nonType>> retvals;
			retvals.push_back(makeNonTypeUniquePtr<std::string>("XD"));
			return retvals;
		}));

	auto lowercaseNode = Node(lowercaseMock.get(), {xdNode.getLink(0)});

	auto value = extractValue<std::string>(lowercaseNode.getValue(0));
	EXPECT_EQ(value, "xd");
}

TEST_F(NodeTest, concatUppercaseAndLowercasexDShouldReturnXDxd)
{
	EXPECT_CALL(*returnXDMock, outputArgs())
		.WillRepeatedly(Return(std::vector<bti::type_index>{bti::type_id<std::string>()}));
	EXPECT_CALL(*returnXDMock, inputArgs())
		.WillRepeatedly(Return(std::vector<bti::type_index>{}));

	auto xdNode = Node(returnXDMock.get(), {});

	EXPECT_CALL(*returnXDMock, call(_))
		.Times(AtLeast(1))
		.WillRepeatedly(Invoke([this](std::vector<nonType*>)
		{
			std::vector<std::unique_ptr<nonType>> retvals;
			retvals.push_back(makeNonTypeUniquePtr<std::string>("xD"));
			return retvals;
		}));

	auto lowercaseNode = Node(lowercaseMock.get(), {xdNode.getLink(0)});
	auto uppercaseNode = Node(uppercaseMock.get(), {xdNode.getLink(0)});
	auto concatNode = Node(concatMock.get(), {uppercaseNode.getLink(0), lowercaseNode.getLink(0)});

	auto value = extractValue<std::string>(concatNode.getValue(0));

	ASSERT_EQ(value, "XDxd");
}

} // namespace fcl