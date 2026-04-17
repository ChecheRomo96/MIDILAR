#include <gtest/gtest.h>
#include <SystemFoundation/CallbackHandler/CallbackHandler.h>

using MIDILAR::SystemFoundation::CallbackHandler;

namespace {

int IncrementByOne(int value) {
    return value + 1;
}

class CounterMock {
public:
    int callCount = 0;
    int accumulated = 0;

    void Consume(int value) {
        ++callCount;
        accumulated += value;
    }

    int EchoAndCount(int value) {
        ++callCount;
        accumulated += value;
        return value;
    }
};

class CallbackFrequencyFixture : public ::testing::Test {
protected:
    CallbackHandler<int, int> intHandler;
    CallbackHandler<void, int> voidHandler;
    CounterMock counter;
};

} // namespace

TEST_F(CallbackFrequencyFixture, FreeFunctionCanBeInvokedRepeatedlyWithStableResults) {
    intHandler.bind(&IncrementByOne);

    for (int i = 0; i < 1000; ++i) {
        EXPECT_EQ(intHandler.invoke(i), i + 1);
    }
}

TEST_F(CallbackFrequencyFixture, BoundMethodCanBeInvokedRepeatedlyAndPreserveState) {
    voidHandler.bind<CounterMock, &CounterMock::Consume>(&counter);

    for (int i = 1; i <= 100; ++i) {
        voidHandler.invoke(i);
    }

    EXPECT_EQ(counter.callCount, 100);
    EXPECT_EQ(counter.accumulated, 5050);
}

TEST_F(CallbackFrequencyFixture, TypedMethodCanBeInvokedRepeatedlyAndReturnConsistentValue) {
    intHandler.bind<CounterMock, &CounterMock::EchoAndCount>(&counter);

    for (int i = 0; i < 250; ++i) {
        EXPECT_EQ(intHandler.invoke(i), i);
    }

    EXPECT_EQ(counter.callCount, 250);
    EXPECT_EQ(counter.accumulated, 31125);
}

TEST_F(CallbackFrequencyFixture, StatusRemainsTrueAfterManyInvocations) {
    intHandler.bind(&IncrementByOne);

    for (int i = 0; i < 500; ++i) {
        (void)intHandler.invoke(i);
    }

    EXPECT_TRUE(intHandler.status());
}

TEST_F(CallbackFrequencyFixture, ConsecutiveInvocationsDoNotRequireRebinding) {
    voidHandler.bind<CounterMock, &CounterMock::Consume>(&counter);

    voidHandler.invoke(5);
    voidHandler.invoke(10);
    voidHandler.invoke(15);

    EXPECT_EQ(counter.callCount, 3);
    EXPECT_EQ(counter.accumulated, 30);
    EXPECT_TRUE(voidHandler.status());
}