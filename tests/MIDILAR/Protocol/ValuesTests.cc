#include <gtest/gtest.h>

#include <MIDILAR.h>

using namespace MIDILAR::Protocol;

// SPEC-MIDI-6
TEST(MIDILARProtocolValuesTests, SevenBitValuesAcceptOnlyZeroTo127) {
    for (int32_t value = -2; value <= 257; ++value) {
        const NoteNumber note = NoteNumber::FromValue(value);
        EXPECT_EQ(note.IsValid(), value >= 0 && value <= 127);
        if (note.IsValid()) {
            EXPECT_EQ(note.Value(), value);
        } else {
            EXPECT_EQ(note, NoteNumber::Invalid());
        }
        EXPECT_EQ(ControllerNumber::FromValue(value).IsValid(), note.IsValid());
        EXPECT_EQ(ProgramNumber::FromValue(value).IsValid(), note.IsValid());
    }
    EXPECT_FALSE(NoteNumber().IsValid());
}

// SPEC-MIDI-3
TEST(MIDILARProtocolValuesTests, VelocityKeepsSixteenBits) {
    EXPECT_EQ(Midi2::Velocity::FromMidi1(0).Midi2(), 0u);
    EXPECT_EQ(Midi2::Velocity::FromMidi1(64).Midi2(), 0x8000u);
    EXPECT_EQ(Midi2::Velocity::FromMidi1(127).Midi2(), 0xFFFFu);
    for (int32_t value = 0; value <= 127; ++value) {
        EXPECT_EQ(Midi2::Velocity::FromMidi1(value).Midi(), static_cast<uint32_t>(value));
    }
    EXPECT_EQ(Midi2::Velocity::FromMidi2(0x1234u).Midi2(), 0x1234u);
    EXPECT_EQ(Midi2::Velocity::FromMidi2(0x1234u).Midi(), 0x1234u >> 9);
    EXPECT_EQ(Midi2::Velocity::Max().Midi2(), 0xFFFFu);
}

TEST(MIDILARProtocolValuesTests, MidiOneInputIsClamped) {
    EXPECT_EQ(Midi2::Velocity::FromMidi1(-1), Midi2::Velocity::Min());
    EXPECT_EQ(Midi2::Velocity::FromMidi1(128), Midi2::Velocity::Max());
    EXPECT_EQ(Midi2::PitchBend::FromMidi1(16384), Midi2::PitchBend::Max());
    EXPECT_EQ(Midi2::ControllerValue::FromMidi1(1000), Midi2::ControllerValue::Max());
}

TEST(MIDILARProtocolValuesTests, ThirtyTwoBitValuesRoundTripMidiOne) {
    for (int32_t value = 0; value <= 127; ++value) {
        EXPECT_EQ(Midi2::ControllerValue::FromMidi1(value).Midi(), static_cast<uint32_t>(value));
        EXPECT_EQ(Midi2::PressureValue::FromMidi1(value).Midi(), static_cast<uint32_t>(value));
    }
    for (int32_t value = 0; value <= 16383; ++value) {
        EXPECT_EQ(Midi2::PitchBend::FromMidi1(value).Midi(), static_cast<uint32_t>(value));
    }
    EXPECT_EQ(Midi2::ControllerValue::FromMidi1(127).Midi2(), 0xFFFFFFFFu);
}

TEST(MIDILARProtocolValuesTests, PitchBendIsCentered) {
    EXPECT_EQ(Midi2::PitchBend::FromMidi1(8192), Midi2::PitchBend::Center());
    EXPECT_EQ(Midi2::PitchBend::Center().Midi2(), 0x80000000u);
    EXPECT_EQ(Midi2::PitchBend::Center().Offset(), 0);
    EXPECT_EQ(Midi2::PitchBend::Min().Offset(), INT32_MIN);
    EXPECT_EQ(Midi2::PitchBend::Max().Offset(), INT32_MAX);
    EXPECT_EQ(Midi2::PitchBend().Midi2(), 0u);
    EXPECT_EQ(Midi2::Velocity::Center().Offset(), 0);
    EXPECT_EQ(Midi2::Velocity::Min().Offset(), -32768);
}
