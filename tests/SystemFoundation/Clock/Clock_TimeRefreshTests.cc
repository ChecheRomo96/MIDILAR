#include "ClockTestFixture.h"

using namespace MIDILAR::SystemFoundation;
using namespace MIDILAR::Tests::SystemFoundation;

TEST_F(ClockTest, Refresh_ReturnsCurrentMockTime) {
    Clock ClockInstance(ClockMockCallback, Clock::Milliseconds);

    GClockMock.SetCurrentTime(42);

    EXPECT_EQ(ClockInstance.now(), 42u);
}
