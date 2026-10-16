#include <MIDILAR_Protocol.h>
#include <gtest/gtest.h>
using namespace MIDILAR::Protocol;

// SPEC-NS-2..3: routing changes only the group, never native MIDI 2.0 fields.
TEST(MIDILARUmpConversionTests, ExactRoundTripsAcrossGroupsChannelsAndNotes) {
    for (int32_t group = 0; group < 16; ++group)
        for (int32_t channel = 0; channel < 16; ++channel)
            for (int32_t note = 0; note < 128; ++note) {
                const auto message = Midi2::Message::NoteOn(Channel::FromWire(channel),
                    NoteNumber::FromValue(note), Midi2::Velocity::FromMidi2(
                        static_cast<uint16_t>(note * 513)), 3, 0xBEEF);
                const auto packet = Ump::Encode(Ump::Group::FromWire(group), message);
                ASSERT_TRUE(packet.IsValid());
                EXPECT_EQ(packet.Type(), Ump::MessageType::Midi2ChannelVoice);
                EXPECT_EQ(packet.Group(), Ump::Group::FromWire(group));
                EXPECT_EQ(packet.Word(0) & 0x00FFFFFFu, message.DataWord(0));
                EXPECT_EQ(packet.Word(1), message.DataWord(1));
                EXPECT_EQ(Ump::DecodeMidi2(packet), message);
                EXPECT_FALSE(Ump::DecodeMidi1(packet).IsValid());
            }
}

TEST(MIDILARUmpConversionTests, AllSupportedMidi2Families) {
    const auto channel = Channel::FromNumber(1);
    const auto group = Ump::Group::FromNumber(1);
    const Midi2::Message messages[] = {
        Midi2::Message::NoteOn(channel, NoteNumber::FromValue(60), Midi2::Velocity::Min()),
        Midi2::Message::NoteOff(channel, NoteNumber::FromValue(60), Midi2::Velocity::Max()),
        Midi2::Message::PolyPressure(channel, NoteNumber::FromValue(60), Midi2::PressureValue::Max()),
        Midi2::Message::ControlChange(channel, ControllerNumber::FromValue(74), Midi2::ControllerValue::Max()),
        Midi2::Message::ProgramChange(channel, ProgramNumber::FromValue(127), 16383),
        Midi2::Message::ChannelPressure(channel, Midi2::PressureValue::Center()),
        Midi2::Message::PitchBend(channel, Midi2::PitchBend::Center())
    };
    for (const auto& message : messages) {
        const auto packet = Ump::Encode(group, message);
        EXPECT_EQ(Ump::DecodeMidi2(packet), message);
    }
    EXPECT_EQ(Ump::Encode(group, messages[0]).VoiceStatus(), VoiceStatus::NoteOn);
    EXPECT_FALSE(Ump::Encode(Ump::Group::Invalid(), messages[0]).IsValid());
    EXPECT_FALSE(Ump::Encode(group, Midi2::Message()).IsValid());
}

TEST(MIDILARUmpConversionTests, NoImplicitProtocolConversionOrWrongFamilyDecoding) {
    Midi::Message message;
    message.NoteOn(60, 100, 0);
    const auto packet1 = Ump::Encode(Ump::Group::FromNumber(1), message);
    EXPECT_FALSE(Ump::DecodeMidi2(packet1).IsValid());
    EXPECT_FALSE(Ump::DecodeMidi2(Ump::Packet::Noop()).IsValid());
    EXPECT_FALSE(Ump::DecodeMidi2(Ump::Packet()).IsValid());
    EXPECT_FALSE(Ump::DecodeMidi2(Ump::Packet::FromWords(0x40908000u, 0)).IsValid());
    EXPECT_FALSE(Ump::DecodeMidi2(Ump::Packet::FromWords(0x40B04A01u, 0)).IsValid());
    const auto decoded = Ump::DecodeMidi1(packet1);
    EXPECT_EQ(decoded.Status(), message.Status());
    EXPECT_EQ(decoded.Data(1), message.Data(1));
    EXPECT_EQ(decoded.Data(2), message.Data(2));
}
