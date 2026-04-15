#include "ClockTestFixture.h"

using namespace MIDILAR::SystemFoundation;
using namespace MIDILAR::Tests::SystemFoundation;

TEST_F(ClockTest, SetFrequency_ChangesFrequencyWhenSetupIsBound) {
    Clock ClockInstance;

    ClockInstance.bindSetup(ClockMockSetupCallback);

    ClockInstance.setFrequency(Clock::Milliseconds);
    EXPECT_EQ(ClockInstance.getFrequency(), Clock::Milliseconds);
}
