#include "ClockTestFixture.h"

using namespace MIDILAR::SystemFoundation;
using namespace MIDILAR::Tests::SystemFoundation;

TEST_F(ClockTest, MultipleUnbinds_DoNotCrash) {
    Clock ClockInstance;

    ClockInstance.unbindClock();
    ClockInstance.unbindClock();

    ClockInstance.unbindSetup();
    ClockInstance.unbindSetup();

    EXPECT_FALSE(ClockInstance.clockStatus());
    EXPECT_FALSE(ClockInstance.setupStatus());
}
