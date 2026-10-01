#include <gtest/gtest.h>

#include <Foundation/Math/Arithmetic.h>
#include <MIDILAR.h>

#ifndef MIDILAR_CORE
    #error "MIDILAR.h must expose MIDILAR_CORE when the Core facade is available"
#endif

TEST(MIDILARCoreTests, ReportsMIDILARVersion) {
    EXPECT_STREQ(MIDILAR::Core::Version(), MIDILAR_VERSION);
}

TEST(MIDILARCoreTests, ReportsFoundationVersion) {
    EXPECT_STREQ(MIDILAR::Core::FoundationVersion(), FOUNDATION_VERSION);
}

TEST(MIDILARCoreTests, ReportsMCCVersion) {
    EXPECT_STREQ(MIDILAR::Core::MCCVersion(), MCC_VERSION);
}

TEST(MIDILARCoreTests, PropagatesFoundationDependency) {
    EXPECT_EQ(Foundation::Math::GCD(12, 8), 4U);
}
