#include <gtest/gtest.h>
#include <SystemFoundation/CallbackHandler/CallbackHandler.h>

using MIDILAR::SystemFoundation::CallbackHandler;

namespace {

int Multiply(int a, int b) {
    return a * b;
}

int Subtract(int a, int b) {
    return a - b;
}

class ArithmeticMock {
public:
    int bias = 0;
    int callCount = 0;
    int lastA = 0;
    int lastB = 0;

    int SumWithBias(int a, int b) {
        ++callCount;
        lastA = a;
        lastB = b;
        return a + b + bias;
    }

    int DifferenceWithBias(int a, int b) {
        ++callCount;
        lastA = a;
        lastB = b;
        return a - b + bias;
    }
};

class VoidMock {
public:
    int callCount = 0;
    int lastValue = 0;

    void Capture(int value) {
        ++callCount;
        lastValue = value;
    }
};

void StoreDoubleToReference(int input, int& output) {
    output = input * 2;
}

class CallbackBindingFixture : public ::testing::Test {
protected:
    CallbackHandler<int, int, int> intHandler;
    CallbackHandler<void, int> voidHandler;
    CallbackHandler<void, int, int&> refHandler;

    ArithmeticMock arithmeticA;
    ArithmeticMock arithmeticB;
    VoidMock voidMock;
};

} // namespace

TEST_F(CallbackBindingFixture, CanBindToFreeFunction) {
    intHandler.bind(&Multiply);

    ASSERT_TRUE(intHandler.status());
    EXPECT_EQ(intHandler.invoke(3, 4), 12);
}

TEST_F(CallbackBindingFixture, CanBindToInstanceMethod) {
    arithmeticA.bias = 5;

    intHandler.bind<ArithmeticMock, &ArithmeticMock::SumWithBias>(&arithmeticA);

    ASSERT_TRUE(intHandler.status());
    EXPECT_EQ(intHandler.invoke(2, 3), 10);
    EXPECT_EQ(arithmeticA.callCount, 1);
    EXPECT_EQ(arithmeticA.lastA, 2);
    EXPECT_EQ(arithmeticA.lastB, 3);
}

TEST_F(CallbackBindingFixture, RebindingFreeFunctionReplacesPreviousFreeFunction) {
    intHandler.bind(&Multiply);
    EXPECT_EQ(intHandler.invoke(3, 5), 15);

    intHandler.bind(&Subtract);
    EXPECT_EQ(intHandler.invoke(9, 4), 5);
}

TEST_F(CallbackBindingFixture, RebindingMethodReplacesPreviousMethod) {
    arithmeticA.bias = 1;
    arithmeticB.bias = 20;

    intHandler.bind<ArithmeticMock, &ArithmeticMock::SumWithBias>(&arithmeticA);
    EXPECT_EQ(intHandler.invoke(2, 2), 5);

    intHandler.bind<ArithmeticMock, &ArithmeticMock::SumWithBias>(&arithmeticB);
    EXPECT_EQ(intHandler.invoke(2, 2), 24);

    EXPECT_EQ(arithmeticA.callCount, 1);
    EXPECT_EQ(arithmeticB.callCount, 1);
}

TEST_F(CallbackBindingFixture, BindingMethodAfterFreeFunctionSwitchesInvocationTarget) {
    arithmeticA.bias = 7;

    intHandler.bind(&Multiply);
    EXPECT_EQ(intHandler.invoke(2, 4), 8);

    intHandler.bind<ArithmeticMock, &ArithmeticMock::SumWithBias>(&arithmeticA);
    EXPECT_EQ(intHandler.invoke(2, 4), 13);
    EXPECT_EQ(arithmeticA.callCount, 1);
}

TEST_F(CallbackBindingFixture, BindingFreeFunctionAfterMethodSwitchesInvocationTarget) {
    arithmeticA.bias = 10;

    intHandler.bind<ArithmeticMock, &ArithmeticMock::DifferenceWithBias>(&arithmeticA);
    EXPECT_EQ(intHandler.invoke(9, 4), 15);

    intHandler.bind(&Subtract);
    EXPECT_EQ(intHandler.invoke(9, 4), 5);
    EXPECT_EQ(arithmeticA.callCount, 1);
}

TEST_F(CallbackBindingFixture, CanBindVoidInstanceMethod) {
    voidHandler.bind<VoidMock, &VoidMock::Capture>(&voidMock);

    ASSERT_TRUE(voidHandler.status());
    voidHandler.invoke(123);

    EXPECT_EQ(voidMock.callCount, 1);
    EXPECT_EQ(voidMock.lastValue, 123);
}

TEST_F(CallbackBindingFixture, CanBindFunctionWithReferenceArgument) {
    int output = 0;
    refHandler.bind(&StoreDoubleToReference);

    refHandler.invoke(21, output);
    EXPECT_EQ(output, 42);
}