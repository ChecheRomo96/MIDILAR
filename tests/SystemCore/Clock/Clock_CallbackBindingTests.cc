#include "ClockTestFixture.h"

using namespace MIDILAR::SystemCore;
using namespace MIDILAR::Tests::SystemCore;

TEST_F(ClockTest, BindClock_EnablesClockStatus) {
    Clock ClockInstance;

    ClockInstance.bindClock(ClockMockCallback);

    EXPECT_TRUE(ClockInstance.clockStatus());
}

TEST_F(ClockTest, UnbindClock_DisablesClockStatus) {
    Clock ClockInstance;

    ClockInstance.bindClock(ClockMockCallback);
    ClockInstance.unbindClock();

    EXPECT_FALSE(ClockInstance.clockStatus());
}
