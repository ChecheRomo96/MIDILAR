#include <gtest/gtest.h>
#include <SystemCore/CallbackHandler/CallbackHandler.h>

using MIDILAR::SystemCore::CallbackHandler;

namespace {

int ReturnTen() {
    return 10;
}

int ReturnTwenty() {
    return 20;
}

class RefreshMock {
public:
    int value = 0;
    int touchCount = 0;

    int ReadValue() {
        ++touchCount;
        return value;
    }

    void Touch() {
        ++touchCount;
    }
};

class CallbackRefreshFixture : public ::testing::Test {
protected:
    CallbackHandler<int, void> noArgIntHandler;
    CallbackHandler<void, void> voidNoArgHandler;

    RefreshMock first;
    RefreshMock second;
};

} // namespace

TEST_F(CallbackRefreshFixture, RebindingNoArgFreeFunctionRefreshesReturnedValue) {
    noArgIntHandler.bind(&ReturnTen);
    EXPECT_EQ(noArgIntHandler.invoke(), 10);

    noArgIntHandler.bind(&ReturnTwenty);
    EXPECT_EQ(noArgIntHandler.invoke(), 20);
}

TEST_F(CallbackRefreshFixture, UnbindAfterMethodBindingClearsHandlerState) {
    first.value = 77;
    noArgIntHandler.bind<RefreshMock, &RefreshMock::ReadValue>(&first);

    ASSERT_TRUE(noArgIntHandler.status());
    EXPECT_EQ(noArgIntHandler.invoke(), 77);

    noArgIntHandler.unbind();
    EXPECT_FALSE(noArgIntHandler.status());
}

TEST_F(CallbackRefreshFixture, RebindingFromOneInstanceToAnotherRefreshesContext) {
    first.value = 11;
    second.value = 99;

    noArgIntHandler.bind<RefreshMock, &RefreshMock::ReadValue>(&first);
    EXPECT_EQ(noArgIntHandler.invoke(), 11);
    EXPECT_EQ(first.touchCount, 1);

    noArgIntHandler.bind<RefreshMock, &RefreshMock::ReadValue>(&second);
    EXPECT_EQ(noArgIntHandler.invoke(), 99);
    EXPECT_EQ(second.touchCount, 1);
    EXPECT_EQ(first.touchCount, 1);
}

TEST_F(CallbackRefreshFixture, BindingVoidMethodAfterUnbindWorksCorrectly) {
    voidNoArgHandler.bind<RefreshMock, &RefreshMock::Touch>(&first);
    voidNoArgHandler.invoke();
    EXPECT_EQ(first.touchCount, 1);

    voidNoArgHandler.unbind();
    EXPECT_FALSE(voidNoArgHandler.status());

    voidNoArgHandler.bind<RefreshMock, &RefreshMock::Touch>(&second);
    ASSERT_TRUE(voidNoArgHandler.status());

    voidNoArgHandler.invoke();
    EXPECT_EQ(second.touchCount, 1);
    EXPECT_EQ(first.touchCount, 1);
}

TEST_F(CallbackRefreshFixture, MultipleRefreshCyclesMaintainCorrectBehavior) {
    first.value = 5;
    second.value = 15;

    noArgIntHandler.bind<RefreshMock, &RefreshMock::ReadValue>(&first);
    EXPECT_EQ(noArgIntHandler.invoke(), 5);

    noArgIntHandler.unbind();
    EXPECT_FALSE(noArgIntHandler.status());

    noArgIntHandler.bind(&ReturnTwenty);
    EXPECT_EQ(noArgIntHandler.invoke(), 20);

    noArgIntHandler.bind<RefreshMock, &RefreshMock::ReadValue>(&second);
    EXPECT_EQ(noArgIntHandler.invoke(), 15);

    EXPECT_EQ(first.touchCount, 1);
    EXPECT_EQ(second.touchCount, 1);
}