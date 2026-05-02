#include <gtest/gtest.h>
#include <SystemCore/CallbackHandler/CallbackHandler.h>

using MIDILAR::SystemCore::CallbackHandler;

namespace {

int Identity(int value) {
    return value;
}

void SetFlagTrue(bool& flag) {
    flag = true;
}

class EdgeMock {
public:
    int value = 0;

    int Read() {
        return value;
    }

    void SetTo(int newValue) {
        value = newValue;
    }
};

class CallbackEdgeCaseFixture : public ::testing::Test {
protected:
    CallbackHandler<int, int> intHandler;
    CallbackHandler<void, bool&> refVoidHandler;
    CallbackHandler<int, void> noArgIntHandler;
    CallbackHandler<void, int> voidIntHandler;

    EdgeMock mock;
};

} // namespace

TEST_F(CallbackEdgeCaseFixture, BindingNullFreeFunctionLeavesHandlerUnbound) {
    using Fn = int (*)(int);
    Fn nullFn = nullptr;

    intHandler.bind(nullFn);
    EXPECT_FALSE(intHandler.status());
}

TEST_F(CallbackEdgeCaseFixture, UnbindCanBeCalledRepeatedly) {
    intHandler.unbind();
    EXPECT_FALSE(intHandler.status());

    intHandler.unbind();
    EXPECT_FALSE(intHandler.status());
}

TEST_F(CallbackEdgeCaseFixture, ReferenceArgumentIsForwardedCorrectly) {
    bool flag = false;

    refVoidHandler.bind(&SetFlagTrue);
    refVoidHandler.invoke(flag);

    EXPECT_TRUE(flag);
}

TEST_F(CallbackEdgeCaseFixture, ZeroValueArgumentsAreHandledCorrectly) {
    intHandler.bind(&Identity);
    EXPECT_EQ(intHandler.invoke(0), 0);
}

TEST_F(CallbackEdgeCaseFixture, MethodBindingWorksForNoArgumentIntSpecialization) {
    mock.value = 321;

    noArgIntHandler.bind<EdgeMock, &EdgeMock::Read>(&mock);
    ASSERT_TRUE(noArgIntHandler.status());
    EXPECT_EQ(noArgIntHandler.invoke(), 321);
}

TEST_F(CallbackEdgeCaseFixture, MethodBindingWorksForVoidReturnSpecialization) {
    voidIntHandler.bind<EdgeMock, &EdgeMock::SetTo>(&mock);

    voidIntHandler.invoke(888);
    EXPECT_EQ(mock.value, 888);
}

#ifndef MIDILAR_EXCEPTIONS

TEST_F(CallbackEdgeCaseFixture, UnboundTypedInvokeReturnsDefaultValueAfterUnbind) {
    intHandler.bind(&Identity);
    EXPECT_EQ(intHandler.invoke(7), 7);

    intHandler.unbind();
    EXPECT_EQ(intHandler.invoke(7), 0);
}

TEST_F(CallbackEdgeCaseFixture, UnboundNoArgTypedInvokeReturnsDefaultValue) {
    EXPECT_EQ(noArgIntHandler.invoke(), 0);
}

#else

TEST_F(CallbackEdgeCaseFixture, UnboundTypedInvokeThrowsAfterUnbind) {
    intHandler.bind(&Identity);
    EXPECT_EQ(intHandler.invoke(9), 9);

    intHandler.unbind();
    EXPECT_THROW(intHandler.invoke(9), std::runtime_error);
}

TEST_F(CallbackEdgeCaseFixture, UnboundNoArgTypedInvokeThrows) {
    EXPECT_THROW(noArgIntHandler.invoke(), std::runtime_error);
}

#endif