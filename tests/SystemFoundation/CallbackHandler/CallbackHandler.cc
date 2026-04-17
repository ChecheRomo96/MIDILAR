#include <gtest/gtest.h>
#include <SystemFoundation/CallbackHandler/CallbackHandler.h>

using MIDILAR::SystemFoundation::CallbackHandler;

namespace {

int FreeAdd(int a, int b) {
    return a + b;
}

void FreeVoidIncrement(int& value) {
    ++value;
}

int FreeNoArgs() {
    return 42;
}

void FreeVoidNoArgs() {
    // intentionally empty
}

class CallbackHandlerFixture : public ::testing::Test {
protected:
    CallbackHandler<int, int, int> intHandler;
    CallbackHandler<void, int&> voidHandler;
    CallbackHandler<int, void> noArgHandler;
    CallbackHandler<void, void> voidNoArgHandler;
};

} // namespace

TEST_F(CallbackHandlerFixture, StartsUnboundForTypedHandler) {
    EXPECT_FALSE(intHandler.status());
}

TEST_F(CallbackHandlerFixture, StartsUnboundForVoidReturnHandler) {
    EXPECT_FALSE(voidHandler.status());
}

TEST_F(CallbackHandlerFixture, StartsUnboundForNoArgumentHandler) {
    EXPECT_FALSE(noArgHandler.status());
}

TEST_F(CallbackHandlerFixture, StartsUnboundForVoidNoArgumentHandler) {
    EXPECT_FALSE(voidNoArgHandler.status());
}

TEST_F(CallbackHandlerFixture, BindFreeFunctionUpdatesStatus) {
    intHandler.bind(&FreeAdd);
    EXPECT_TRUE(intHandler.status());
}

TEST_F(CallbackHandlerFixture, UnbindClearsStatusAfterFreeFunctionBinding) {
    intHandler.bind(&FreeAdd);
    ASSERT_TRUE(intHandler.status());

    intHandler.unbind();
    EXPECT_FALSE(intHandler.status());
}

TEST_F(CallbackHandlerFixture, InvokeBoundFreeFunctionReturnsExpectedValue) {
    intHandler.bind(&FreeAdd);
    EXPECT_EQ(intHandler.invoke(10, 15), 25);
}

TEST_F(CallbackHandlerFixture, InvokeBoundVoidFunctionUpdatesArgument) {
    int value = 7;
    voidHandler.bind(&FreeVoidIncrement);

    voidHandler.invoke(value);
    EXPECT_EQ(value, 8);
}

TEST_F(CallbackHandlerFixture, InvokeBoundNoArgsFunctionReturnsExpectedValue) {
    noArgHandler.bind(&FreeNoArgs);
    EXPECT_EQ(noArgHandler.invoke(), 42);
}

TEST_F(CallbackHandlerFixture, InvokeBoundVoidNoArgsFunctionDoesNotThrow) {
    voidNoArgHandler.bind(&FreeVoidNoArgs);
    EXPECT_NO_THROW(voidNoArgHandler.invoke());
}

#ifndef MIDILAR_EXCEPTIONS

TEST_F(CallbackHandlerFixture, UnboundTypedInvokeReturnsDefaultConstructedValue) {
    EXPECT_EQ(intHandler.invoke(1, 2), int());
}

TEST_F(CallbackHandlerFixture, UnboundNoArgsInvokeReturnsDefaultConstructedValue) {
    EXPECT_EQ(noArgHandler.invoke(), int());
}

TEST_F(CallbackHandlerFixture, UnboundVoidInvokeDoesNotThrow) {
    int value = 12;
    EXPECT_NO_THROW(voidHandler.invoke(value));
    EXPECT_EQ(value, 12);
}

TEST_F(CallbackHandlerFixture, UnboundVoidNoArgsInvokeDoesNotThrow) {
    EXPECT_NO_THROW(voidNoArgHandler.invoke());
}

#else

TEST_F(CallbackHandlerFixture, UnboundTypedInvokeThrowsRuntimeError) {
    EXPECT_THROW(intHandler.invoke(1, 2), std::runtime_error);
}

TEST_F(CallbackHandlerFixture, UnboundNoArgsInvokeThrowsRuntimeError) {
    EXPECT_THROW(noArgHandler.invoke(), std::runtime_error);
}

TEST_F(CallbackHandlerFixture, UnboundVoidInvokeThrowsRuntimeError) {
    int value = 12;
    EXPECT_THROW(voidHandler.invoke(value), std::runtime_error);
}

TEST_F(CallbackHandlerFixture, UnboundVoidNoArgsInvokeThrowsRuntimeError) {
    EXPECT_THROW(voidNoArgHandler.invoke(), std::runtime_error);
}

#endif