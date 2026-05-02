#include "ClockTestFixture.h"

using namespace MIDILAR::SystemCore;
using namespace MIDILAR::Tests::SystemCore;

TEST_F(ClockTest, SetFrequency_ChangesFrequencyWhenSetupIsBound) {
    Clock ClockInstance;

    ClockInstance.bindSetup(ClockMockSetupCallback);

    ClockInstance.setFrequency(Clock::Milliseconds);
    EXPECT_EQ(ClockInstance.getFrequency(), Clock::Milliseconds);
}
