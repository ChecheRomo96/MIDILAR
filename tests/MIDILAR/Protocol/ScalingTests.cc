#include <gtest/gtest.h>

#include <MIDILAR.h>

#ifndef MIDILAR_PROTOCOL
    #error "MIDILAR.h must expose MIDILAR_PROTOCOL when the Protocol facade is available"
#endif

using MIDILAR::Protocol::ScaleDown;
using MIDILAR::Protocol::ScaleUp;

// SPEC-MIDI-3: reference vectors of the MIDI 2.0 min-center-max rule.
static_assert(ScaleUp(0, 7, 16) == 0x0000u, "7->16 minimum");
static_assert(ScaleUp(64, 7, 16) == 0x8000u, "7->16 center");
static_assert(ScaleUp(127, 7, 16) == 0xFFFFu, "7->16 maximum");
static_assert(ScaleUp(127, 7, 32) == 0xFFFFFFFFu, "7->32 maximum");
static_assert(ScaleUp(8192, 14, 32) == 0x80000000u, "14->32 center");
static_assert(ScaleUp(16383, 14, 32) == 0xFFFFFFFFu, "14->32 maximum");

TEST(MIDILARProtocolScalingTests, MapsMinimumCenterAndMaximum) {
    const uint32_t widths[][2] = {{7, 16}, {7, 32}, {14, 32}, {16, 32}};
    for (const auto& w : widths) {
        const uint32_t max = (1u << w[0]) - 1;
        const uint32_t center = 1u << (w[0] - 1);
        const uint32_t targetMax = w[1] == 32 ? 0xFFFFFFFFu : (1u << w[1]) - 1;
        EXPECT_EQ(ScaleUp(0, w[0], w[1]), 0u);
        EXPECT_EQ(ScaleUp(center, w[0], w[1]), 1u << (w[1] - 1));
        EXPECT_EQ(ScaleUp(max, w[0], w[1]), targetMax);
    }
}

TEST(MIDILARProtocolScalingTests, SevenAndFourteenBitValuesSurviveRoundTrips) {
    const uint32_t widths[][2] = {{7, 16}, {7, 32}, {14, 32}};
    for (const auto& w : widths) {
        uint32_t previous = 0;
        for (uint32_t value = 0; value < (1u << w[0]); ++value) {
            const uint32_t scaled = ScaleUp(value, w[0], w[1]);
            EXPECT_EQ(ScaleDown(scaled, w[1], w[0]), value);
            if (value > 0) {
                EXPECT_GT(scaled, previous);
            }
            previous = scaled;
        }
    }
}

TEST(MIDILARProtocolScalingTests, ValuesBelowCenterOnlyShift) {
    for (uint32_t value = 0; value <= 64; ++value) {
        EXPECT_EQ(ScaleUp(value, 7, 16), value << 9);
    }
}

TEST(MIDILARProtocolScalingTests, InvalidArgumentsReturnZero) {
    EXPECT_EQ(ScaleUp(128, 7, 16), 0u);
    EXPECT_EQ(ScaleUp(1, 16, 16), 0u);
    EXPECT_EQ(ScaleUp(1, 0, 16), 0u);
    EXPECT_EQ(ScaleUp(1, 7, 33), 0u);
    EXPECT_EQ(ScaleDown(1, 7, 7), 0u);
    EXPECT_EQ(ScaleDown(1, 33, 7), 0u);
    EXPECT_EQ(ScaleDown(0xFFFFu, 16, 0), 0u);
}

TEST(MIDILARProtocolScalingTests, ScaleDownIgnoresBitsAboveTheSourceWidth) {
    EXPECT_EQ(ScaleDown(0x1FFFFu, 16, 7), 127u);
}
