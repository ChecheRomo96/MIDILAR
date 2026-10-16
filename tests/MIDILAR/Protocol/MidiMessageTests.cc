#include <gtest/gtest.h>
#include <MIDILAR.h>
#include <cstring>
#include <limits>
#include <type_traits>

using MIDILAR::Protocol::Midi::Message;
using MIDILAR::Protocol::Midi::SysExMessage;
using namespace MIDILAR::Protocol;

// SPEC-M1-1: inline ownership and canonical invalid state.
TEST(MIDILARMidiMessageTests, InlineOwnershipAndInvalidState) {
    static_assert(sizeof(Message) == 3, "short messages occupy three bytes");
    static_assert(std::is_trivially_copyable<Message>::value, "independent byte copies");
    Message original;
    EXPECT_FALSE(original.IsValid());
    EXPECT_EQ(original.Buffer(), nullptr);
    original.NoteOn(60, 100, 0);
    Message copy = original;
    original.NoteOff(60);
    EXPECT_EQ(copy.Data(0), 0x90);
    EXPECT_EQ(copy.Data(1), 60);
    EXPECT_EQ(copy.Data(2), 100);
    EXPECT_EQ(copy.size(), 3u);
    EXPECT_EQ(copy.Data(3), 0);
    EXPECT_EQ(copy.Channel(), Channel::FromNumber(1));
}

TEST(MIDILARMidiMessageTests, RejectsInvalidBuildersAndRawData) {
    Message msg;
    msg.NoteOn(128, 100);
    EXPECT_FALSE(msg.IsValid());
    msg.NoteOn(60, -1);
    EXPECT_FALSE(msg.IsValid());
    msg.NoteOn(60, 100, 16);
    EXPECT_FALSE(msg.IsValid());
    msg.NoteOn(60, 100, -1);
    EXPECT_FALSE(msg.IsValid());
    const uint8_t invalid[][3] = {{0x90, 128, 1}, {0xF4, 0, 0}, {0xF0, 1, 0xF7}, {60, 100, 0}};
    for (const auto& bytes : invalid) {
        msg.NoteOn(60, 100).SetRawData(bytes, 3);
        EXPECT_FALSE(msg.IsValid());
        EXPECT_EQ(msg.size(), 0u);
        EXPECT_EQ(msg.Buffer(), nullptr);
        EXPECT_EQ(msg.Data(0), 0);
    }
    const uint8_t raw[] = {0x90, 60, 0};
    msg.SetRawData(raw, 2);
    EXPECT_FALSE(msg.IsValid());
    msg.SetRawData(raw, 3);
    msg.SetRawData(msg.Buffer(), msg.size());
    EXPECT_EQ(msg.Data(0), 0x90);
    msg.SetRawData(nullptr, 3);
    EXPECT_FALSE(msg.IsValid());
}

// SPEC-M1-2: all statuses and 7-bit data survive the byte/UMP bridge.
TEST(MIDILARMidiMessageTests, ExhaustiveChannelVoiceRoundTrip) {
    const Ump::Group group = Ump::Group::FromNumber(16);
    for (int status = 0x80; status < 0xF0; ++status) {
        const size_t length = (status & 0xF0) == 0xC0 || (status & 0xF0) == 0xD0 ? 2u : 3u;
        for (int value = 0; value < 128; ++value) {
            const uint8_t raw[] = {static_cast<uint8_t>(status), static_cast<uint8_t>(value),
                                   static_cast<uint8_t>(127 - value)};
            Message msg;
            msg.SetRawData(raw, length);
            ASSERT_TRUE(msg.IsValid());
            const Ump::Packet packet = Ump::Encode(group, msg);
            EXPECT_EQ(packet.Group(), group);
            const Message restored = Ump::DecodeMidi1(packet);
            ASSERT_TRUE(restored.IsValid());
            EXPECT_EQ(restored.size(), length);
            const bool zeroNoteOn = (status & 0xF0) == 0x90 && raw[2] == 0;
            EXPECT_EQ(restored.Status(), zeroNoteOn ? (0x80 | (status & 15)) : status);
            EXPECT_EQ(restored.Data(1), raw[1]);
            if (length == 3) {
                EXPECT_EQ(restored.Data(2), raw[2]);
            }
        }
    }
}

TEST(MIDILARMidiMessageTests, ShortSystemLayoutsAndRoundTrip) {
    Message messages[11];
    messages[0].TimingTick(); messages[1].Start(); messages[2].Continue();
    messages[3].Stop(); messages[4].ActiveSensing(); messages[5].SystemReset();
    messages[6].TuningRequest(); messages[7].SongSelect(127);
    messages[8].SongPositionPointer(16383); messages[9].MTC_QuarterFrame(7, 15);
    messages[10].SongPositionPointer(0);
    const uint8_t statuses[] = {0xF8, 0xFA, 0xFB, 0xFC, 0xFE, 0xFF, 0xF6, 0xF3, 0xF2, 0xF1, 0xF2};
    const size_t sizes[] = {1, 1, 1, 1, 1, 1, 1, 2, 3, 2, 3};
    for (size_t i = 0; i < 11; ++i) {
        EXPECT_EQ(messages[i].Status(), statuses[i]);
        EXPECT_EQ(messages[i].size(), sizes[i]);
        EXPECT_FALSE(messages[i].Channel().IsValid());
        const Message restored = Ump::DecodeMidi1(Ump::Encode(Ump::Group::FromWire(0), messages[i]));
        ASSERT_EQ(restored.size(), messages[i].size());
        EXPECT_EQ(std::memcmp(restored.Buffer(), messages[i].Buffer(), sizes[i]), 0);
    }
    EXPECT_EQ(messages[8].Data(1), 127);
    EXPECT_EQ(messages[8].Data(2), 127);
    EXPECT_EQ(messages[9].Data(1), 127);
}

TEST(MIDILARMidiMessageTests, ChannelVoiceBuildersAndSystemBounds) {
    Message messages[7];
    messages[0].NoteOff(60, 50, 15);
    messages[1].NoteOn(60, 100, 15);
    messages[2].AfterTouch(60, 70, 15);
    messages[3].ControlChange(10, 64, 15);
    messages[4].ProgramChange(127, 15);
    messages[5].ChannelPressure(90, 15);
    messages[6].PitchBend(8192, 15);
    const uint8_t expected[][3] = {
        {0x8F, 60, 50}, {0x9F, 60, 100}, {0xAF, 60, 70},
        {0xBF, 10, 64}, {0xCF, 127, 0}, {0xDF, 90, 0}, {0xEF, 0, 64}
    };
    for (size_t i = 0; i < 7; ++i) {
        ASSERT_TRUE(messages[i].IsValid());
        const size_t count = i == 4 || i == 5 ? 2u : 3u;
        EXPECT_EQ(messages[i].size(), count);
        EXPECT_EQ(std::memcmp(messages[i].Buffer(), expected[i], count), 0);
    }
    Message msg;
    EXPECT_FALSE(msg.SongPositionPointer(-1).IsValid());
    EXPECT_FALSE(msg.SongPositionPointer(16384).IsValid());
    EXPECT_FALSE(msg.SongSelect(128).IsValid());
    EXPECT_FALSE(msg.MTC_QuarterFrame(8, 0).IsValid());
    EXPECT_FALSE(msg.MTC_QuarterFrame(0, 16).IsValid());
    EXPECT_FALSE(msg.ProgramChange(128).IsValid());
    EXPECT_FALSE(msg.ChannelPressure(128).IsValid());
    EXPECT_FALSE(msg.AfterTouch(60, 128).IsValid());
    EXPECT_FALSE(msg.ControlChange(128, 0).IsValid());
    EXPECT_FALSE(msg.CC_Mono(0, 17).IsValid());
}

TEST(MIDILARMidiMessageTests, BridgeRejectsWrongTypesAndMalformedPackets) {
    EXPECT_FALSE(Ump::DecodeMidi1(Ump::Packet::Noop()).IsValid());
    EXPECT_FALSE(Ump::DecodeMidi1(Ump::Packet::Midi2NoteOn(Ump::Group::FromWire(0),
        Channel::FromWire(0), NoteNumber::FromValue(60), Midi2::Velocity::Max())).IsValid());
    const uint32_t malformed[] = {0x20908001u, 0x20C00101u, 0x10F80100u,
                                  0x10F40000u, 0x20F80000u, 0x10903C64u};
    for (uint32_t word : malformed)
        EXPECT_FALSE(Ump::DecodeMidi1(Ump::Packet::FromWords(word)).IsValid()) << word;
    Message msg;
    EXPECT_FALSE(Ump::Encode(Ump::Group::FromWire(0), msg).IsValid());
    msg.NoteOn(60, 0);
    EXPECT_EQ(msg.Status(), 0x90);
    EXPECT_EQ(Ump::Encode(Ump::Group::FromWire(0), msg).VoiceStatus(), VoiceStatus::NoteOff);
    EXPECT_FALSE(Ump::Encode(Ump::Group::Invalid(), msg).IsValid());
}

// SPEC-M1-4: min/center/max and monotonicity, no overload-dependent resolution.
TEST(MIDILARMidiMessageTests, PitchBendResolutions) {
    Message msg;
    for (int32_t value = 0; value <= 16383; ++value) {
        msg.PitchBend(value);
        EXPECT_EQ(static_cast<int32_t>(msg.Data(1) | (msg.Data(2) << 7)), value);
        msg.PitchBendSigned(value - 8192);
        EXPECT_EQ(static_cast<int32_t>(msg.Data(1) | (msg.Data(2) << 7)), value);
    }
    uint16_t previous = 0;
    for (int32_t value = 0; value < 128; ++value) {
        msg.PitchBend7(value);
        const uint16_t bend = static_cast<uint16_t>(msg.Data(1) | (msg.Data(2) << 7));
        EXPECT_GE(bend, previous);
        previous = bend;
        if (value == 0) {
            EXPECT_EQ(bend, 0);
        }
        if (value == 64) {
            EXPECT_EQ(bend, 8192);
        }
        if (value == 127) {
            EXPECT_EQ(bend, 16383);
        }
    }
    EXPECT_FALSE(msg.PitchBend(16384).IsValid());
    EXPECT_FALSE(msg.PitchBendSigned(-8193).IsValid());
    EXPECT_FALSE(msg.PitchBend7(128).IsValid());
}

TEST(MIDILARMidiMessageTests, NamedControllerAndModeHelpers) {
    using Builder = Message& (Message::*)(int32_t, int32_t) noexcept;
    const Builder builders[] = {&Message::CC_BankSelect, &Message::CC_Modulation,
        &Message::CC_BreathControl, &Message::CC_FootPedal, &Message::CC_Portamento,
        &Message::CC_Volume, &Message::CC_Balance, &Message::CC_Pan, &Message::CC_Expression};
    const uint8_t numbers[] = {0, 1, 2, 4, 5, 7, 8, 10, 11};
    Message msg;
    for (size_t i = 0; i < 9; ++i) {
        (msg.*builders[i])(100, 15);
        EXPECT_EQ(msg.Status(), 0xBF);
        EXPECT_EQ(msg.Data(1), numbers[i]);
        EXPECT_EQ(msg.Data(2), 100);
    }
    msg.CC_LocalControlOff(); EXPECT_EQ(msg.Data(1), 122); EXPECT_EQ(msg.Data(2), 0);
    msg.CC_LocalControlOn(); EXPECT_EQ(msg.Data(2), 127);
    msg.CC_OmniOff(); EXPECT_EQ(msg.Data(1), 124);
    msg.CC_OmniOn(); EXPECT_EQ(msg.Data(1), 125);
    msg.CC_Mono(0, 4); EXPECT_EQ(msg.Data(1), 126); EXPECT_EQ(msg.Data(2), 4);
    msg.CC_Poly(); EXPECT_EQ(msg.Data(1), 127); EXPECT_EQ(msg.Data(2), 0);
    msg.CC_AllSoundOff(); EXPECT_EQ(msg.Data(1), 120);
    msg.CC_ResetAllControllers(); EXPECT_EQ(msg.Data(1), 121);
    msg.CC_AllNotesOff(); EXPECT_EQ(msg.Data(1), 123);
}

// SPEC-M1-3: no partial writes, exact framing, overlap and caller ownership.
TEST(MIDILARMidiMessageTests, SysExCallerStorageAndFailures) {
    uint8_t storage[8] = {0x7D, 1, 2, 3, 0x55, 0x55, 0x55, 0x55};
    const auto view = SysExMessage::Build(storage, sizeof(storage), storage, 4);
    ASSERT_TRUE(view.IsValid());
    const uint8_t expected[] = {0xF0, 0x7D, 1, 2, 3, 0xF7};
    EXPECT_EQ(view.size(), sizeof(expected));
    EXPECT_EQ(std::memcmp(view.Buffer(), expected, sizeof(expected)), 0);
    const auto copy = view;
    EXPECT_EQ(copy.Buffer(), view.Buffer());
    const uint8_t invalidPayload[] = {0x7D, 0x80};
    uint8_t before[8]; std::memcpy(before, storage, 8);
    EXPECT_FALSE(SysExMessage::Build(storage, 5, expected, 6).IsValid());
    EXPECT_FALSE(SysExMessage::Build(storage, 8, invalidPayload, 2).IsValid());
    EXPECT_FALSE(SysExMessage::Build(storage, 8, nullptr, 1).IsValid());
    EXPECT_FALSE(SysExMessage::Build(storage, 8, nullptr, std::numeric_limits<size_t>::max()).IsValid());
    EXPECT_FALSE(SysExMessage::Build(nullptr, 8, nullptr, 0).IsValid());
    EXPECT_EQ(std::memcmp(storage, before, 8), 0);
    const auto empty = SysExMessage::Build(storage, 2, nullptr, 0);
    ASSERT_TRUE(empty.IsValid());
    EXPECT_EQ(empty.Data(0), 0xF0);
    EXPECT_EQ(empty.Data(1), 0xF7);
    EXPECT_EQ(empty.Data(2), 0);
}

TEST(MIDILARMidiMessageTests, FullFrameHasUniversalRealtimeHeader) {
    uint8_t storage[10] = {};
    const auto time = TimeCode::From(1, 2, 3, 4, TimeCodeRate::Fps25);
    const auto view = SysExMessage::MTC_FullFrame(storage, sizeof(storage), time);
    const uint8_t expected[] = {0xF0, 0x7F, 0x7F, 0x01, 0x01, 0x21, 2, 3, 4, 0xF7};
    ASSERT_TRUE(view.IsValid());
    EXPECT_EQ(view.size(), 10u);
    EXPECT_EQ(std::memcmp(view.Buffer(), expected, 10), 0);
    EXPECT_FALSE(SysExMessage::MTC_FullFrame(storage, 9, time).IsValid());
    EXPECT_FALSE(SysExMessage::MTC_FullFrame(storage, 10, TimeCode::Invalid()).IsValid());
    EXPECT_EQ(std::memcmp(storage, expected, 10), 0);
}
