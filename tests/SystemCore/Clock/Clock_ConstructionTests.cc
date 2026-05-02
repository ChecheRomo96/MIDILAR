#include "ClockTestFixture.h"

using namespace MIDILAR::SystemCore;
using namespace MIDILAR::Tests::SystemCore;

TEST_F(ClockTest, DefaultConstructor_InitializesTimeToZero) {
    Clock ClockInstance;

    EXPECT_EQ(ClockInstance.getTime(), 0u);
    EXPECT_EQ(ClockInstance.getFrequency(), Clock::Seconds);
    EXPECT_FALSE(ClockInstance.clockStatus());
    EXPECT_FALSE(ClockInstance.setupStatus());
}

TEST_F(ClockTest, CallbackConstructor_BindsClockCallbackAndFrequency) {
    Clock ClockInstance(ClockMockCallback, Clock::Milliseconds);

    EXPECT_EQ(ClockInstance.getTime(), 0u);
    EXPECT_EQ(ClockInstance.getFrequency(), Clock::Milliseconds);
    EXPECT_TRUE(ClockInstance.clockStatus());
}
