#include <MIDILAR/Protocol/Midi/Message.h>
#include <MIDILAR/Protocol/Midi2/Message.h>
#include <gtest/gtest.h>
#include <type_traits>

// SPEC-NS-2: these headers must compile without pulling in UMP.
#ifdef MIDILAR_PROTOCOL_PACKET_H
#error "Native MIDI message headers must not include UMP"
#endif

using namespace MIDILAR::Protocol;

TEST(MIDILARMidi2MessageTests, NativeStorageAndDefaults) {
    static_assert(sizeof(Midi2::Message) == 8, "eight native field bytes");
    static_assert(std::is_trivially_copyable<Midi2::Message>::value, "plain value");
    static_assert(sizeof(Midi::Message) == 3, "three native MIDI 1.0 bytes");
    constexpr Midi2::Message invalid;
    EXPECT_FALSE(invalid.IsValid());
    EXPECT_FALSE(invalid.Channel().IsValid());
    EXPECT_FALSE(invalid.Note().IsValid());
    EXPECT_FALSE(invalid.Controller().IsValid());
    EXPECT_FALSE(invalid.Program().IsValid());
    EXPECT_EQ(invalid.DataWord(2), 0u);
}

TEST(MIDILARMidi2MessageTests, NotesPreserveResolutionAttributesAndVelocityZero) {
    constexpr auto note = Midi2::Message::NoteOn(Channel::FromNumber(3),
        NoteNumber::FromValue(60), Midi2::Velocity::FromMidi2(50000), 3, 0x1234);
    static_assert(note.DataWord(0) == 0x00923C03u, "no UMP type or group");
    static_assert(note.DataWord(1) == 0xC3501234u, "full velocity/attribute");
    EXPECT_EQ(note.Channel(), Channel::FromNumber(3));
    EXPECT_EQ(note.Note(), NoteNumber::FromValue(60));
    EXPECT_EQ(note.Velocity().Midi2(), 50000u);
    EXPECT_EQ(note.AttributeType(), 3);
    EXPECT_EQ(note.AttributeData(), 0x1234);
    const auto zero = Midi2::Message::NoteOn(Channel::FromWire(0),
        NoteNumber::FromValue(60), Midi2::Velocity::Min());
    EXPECT_EQ(zero.Status(), VoiceStatus::NoteOn);
    EXPECT_EQ(zero.Velocity().Midi2(), 0);
    EXPECT_EQ(Midi2::Message::FromDataWords(note.DataWord(0), note.DataWord(1)), note);
    EXPECT_FALSE(Midi2::Message::NoteOff(Channel::Invalid(), NoteNumber::FromValue(60),
        Midi2::Velocity::Min()).IsValid());
    EXPECT_FALSE(Midi2::Message::NoteOn(Channel::FromWire(0), NoteNumber::Invalid(),
        Midi2::Velocity::Min()).IsValid());
    EXPECT_FALSE(Midi2::Message::NoteOn(Channel::FromWire(0), NoteNumber::FromValue(60),
        Midi2::Velocity::Min(), 0, 1).IsValid());
}

TEST(MIDILARMidi2MessageTests, ControllerPressureAndBendFields) {
    const auto channel = Channel::FromNumber(16);
    const auto cc = Midi2::Message::ControlChange(channel, ControllerNumber::FromValue(74),
        Midi2::ControllerValue::FromMidi2(0xFEDCBA98u));
    EXPECT_EQ(cc.DataWord(0), 0x00BF4A00u);
    EXPECT_EQ(cc.Controller().Value(), 74);
    EXPECT_EQ(cc.ControllerValue().Midi2(), 0xFEDCBA98u);
    EXPECT_FALSE(cc.Note().IsValid());
    EXPECT_EQ(cc.Velocity(), Midi2::Velocity::Min());
    const auto poly = Midi2::Message::PolyPressure(channel, NoteNumber::FromValue(127),
        Midi2::PressureValue::Max());
    EXPECT_EQ(poly.Note().Value(), 127);
    EXPECT_EQ(poly.Pressure().Midi2(), 0xFFFFFFFFu);
    const auto pressure = Midi2::Message::ChannelPressure(channel, Midi2::PressureValue::Center());
    EXPECT_FALSE(pressure.Note().IsValid());
    EXPECT_EQ(pressure.Pressure().Midi2(), 0x80000000u);
    const auto bend = Midi2::Message::PitchBend(channel, Midi2::PitchBend::Center());
    EXPECT_EQ(bend.PitchBend().Midi2(), 0x80000000u);
    EXPECT_FALSE(bend.Controller().IsValid());
    EXPECT_EQ(bend.ControllerValue(), Midi2::ControllerValue::Min());
}

TEST(MIDILARMidi2MessageTests, ProgramBankBoundaries) {
    for (int32_t bank : {-1, 0, 127, 128, 16383}) {
        const auto message = Midi2::Message::ProgramChange(Channel::FromWire(0),
            ProgramNumber::FromValue(127), bank);
        ASSERT_TRUE(message.IsValid());
        EXPECT_EQ(message.Program().Value(), 127);
        EXPECT_EQ(message.HasBank(), bank >= 0);
        EXPECT_EQ(message.Bank(), bank >= 0 ? bank : 0);
        EXPECT_EQ(Midi2::Message::FromDataWords(message.DataWord(0), message.DataWord(1)), message);
    }
    EXPECT_FALSE(Midi2::Message::ProgramChange(Channel::FromWire(0), ProgramNumber::FromValue(0), -2).IsValid());
    EXPECT_FALSE(Midi2::Message::ProgramChange(Channel::FromWire(0), ProgramNumber::FromValue(0), 16384).IsValid());
}

// SPEC-NS-3: malformed and unsupported raw native field combinations fail.
TEST(MIDILARMidi2MessageTests, RejectsReservedBitsAndUnsupportedFamilies) {
    const uint32_t malformed[][2] = {
        {0x40903C00u, 0}, // UMP header is not a native header
        {0x00908000u, 0}, {0x00903C00u, 1}, // bad note or absent attribute
        {0x00B04A01u, 0}, {0x00B08000u, 0}, // CC reserved bits/index
        {0x00C00002u, 0}, {0x00C00001u, 0x80000000u},
        {0x00C00001u, 0x00010000u}, {0x00C00001u, 0x00000080u},
        {0x00C00000u, 1}, // no bank flag
        {0x00D00001u, 0}, {0x00E00100u, 0}, {0x00600000u, 0}
    };
    for (const auto& words : malformed) {
        const auto message = Midi2::Message::FromDataWords(words[0], words[1]);
        EXPECT_FALSE(message.IsValid()) << words[0];
        EXPECT_EQ(message, Midi2::Message());
    }
}
