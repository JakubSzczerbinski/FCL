#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <IFunction.hpp>

namespace fcl
{

class IFunctionMock : public IFunction, ::testing::Mock
{
public:
	MOCK_METHOD1(call, std::vector<std::unique_ptr<nonType>>(std::vector<nonType*>));
	MOCK_METHOD0(inputArgs, TypeVector());
	MOCK_METHOD0(outputArgs, TypeVector());
	MOCK_METHOD0(name, std::string());
};

} // namespace fcl
