#include <gtest/gtest.h>

#include <vector>

#include <MIDILAR.h>

using namespace MIDILAR::Protocol;

namespace {

std::vector<Packet> ParseAll(Midi1Parser& parser, const std::vector<uint8_t>& bytes) {
    std::vector<Packet> packets;
    Packet out[Midi1Parser::MaxPackets];
    for (uint8_t byte : bytes) {
        const uint8_t count = parser.Parse(byte, out);
        for (uint8_t i = 0; i < count; ++i) {
            packets.push_back(out[i]);
        }
    }
    return packets;
}

std::vector<Packet> ParseAll(const std::vector<uint8_t>& bytes) {
    Midi1Parser parser;
    return ParseAll(parser, bytes);
}

std::vector<uint8_t> EncodeAll(Midi1Encoder& encoder, const std::vector<Packet>& packets) {
    std::vector<uint8_t> bytes;
    uint8_t out[Midi1Encoder::MaxBytes];
    for (const Packet& packet : packets) {
        const uint8_t count = encoder.Encode(packet, out);
        bytes.insert(bytes.end(), out, out + count);
    }
    return bytes;
}

std::vector<uint8_t> EncodeAll(const std::vector<Packet>& packets) {
    Midi1Encoder encoder;
    return EncodeAll(encoder, packets);
}

Packet Voice(uint32_t status, uint32_t data1, uint32_t data2) {
    return Packet::FromWords((0x20u << 24) | (status << 16) | (data1 << 8) | data2);
}

Packet System(uint32_t status, uint32_t data1 = 0, uint32_t data2 = 0) {
    return Packet::FromWords((0x10u << 24) | (status << 16) | (data1 << 8) | data2);
}

} // namespace

TEST(MIDILARProtocolMidi1StreamTests, ParsesEveryChannelMessage) {
    for (uint32_t status = 0x80; status < 0xF0; ++status) {
        const bool oneByte = (status & 0xF0) == 0xC0 || (status & 0xF0) == 0xD0;
        const std::vector<uint8_t> bytes = oneByte
            ? std::vector<uint8_t>{static_cast<uint8_t>(status), 0x45}
            : std::vector<uint8_t>{static_cast<uint8_t>(status), 0x45, 0x67};
        const std::vector<Packet> packets = ParseAll(bytes);
        ASSERT_EQ(packets.size(), 1u) << status;
        EXPECT_EQ(packets[0], Voice(status, 0x45, oneByte ? 0 : 0x67));
        EXPECT_EQ(EncodeAll(packets), bytes);
    }
}

TEST(MIDILARProtocolMidi1StreamTests, StampsTheParserGroup) {
    Midi1Parser parser(Group::FromNumber(5));
    const std::vector<Packet> packets = ParseAll(parser, {0x90, 60, 100, 0xF8});
    ASSERT_EQ(packets.size(), 2u);
    EXPECT_EQ(packets[0].Group(), Group::FromNumber(5));
    EXPECT_EQ(packets[1].Group(), Group::FromNumber(5));
}

// SPEC-MIDI-8
TEST(MIDILARProtocolMidi1StreamTests, NoteOnWithVelocityZeroBecomesNoteOff) {
    const std::vector<Packet> packets = ParseAll({0x93, 60, 0});
    ASSERT_EQ(packets.size(), 1u);
    EXPECT_EQ(packets[0].VoiceStatus(), VoiceStatus::NoteOff);
    EXPECT_EQ(packets[0].Channel(), Channel::FromWire(3));
    EXPECT_EQ(packets[0].Velocity(), Velocity());
}

// SPEC-MIDI-10
TEST(MIDILARProtocolMidi1StreamTests, AcceptsRunningStatus) {
    const std::vector<Packet> packets = ParseAll({0x90, 60, 100, 62, 101, 0xF8, 64, 0, 0xC1, 5, 6});
    ASSERT_EQ(packets.size(), 6u);
    EXPECT_EQ(packets[0], Voice(0x90, 60, 100));
    EXPECT_EQ(packets[1], Voice(0x90, 62, 101));
    EXPECT_EQ(packets[2], System(0xF8));
    EXPECT_EQ(packets[3], Voice(0x80, 64, 0));
    EXPECT_EQ(packets[4], Voice(0xC1, 5, 0));
    EXPECT_EQ(packets[5], Voice(0xC1, 6, 0));
}

TEST(MIDILARProtocolMidi1StreamTests, SystemCommonAndSysExClearRunningStatus) {
    EXPECT_EQ(ParseAll({0x90, 60, 100, 0xF6, 62, 101}).size(), 2u);
    EXPECT_EQ(ParseAll({0x90, 60, 100, 0xF3, 1, 62, 101}).size(), 2u);
    EXPECT_EQ(ParseAll({0x90, 60, 100, 0xF0, 1, 0xF7, 62, 101}).size(), 2u);
}

TEST(MIDILARProtocolMidi1StreamTests, ParsesSystemCommon) {
    const std::vector<Packet> packets = ParseAll({0xF1, 0x23, 0xF2, 0x00, 0x40, 0xF3, 7, 0xF6});
    ASSERT_EQ(packets.size(), 4u);
    EXPECT_EQ(packets[0].TimeCode(), 0x23);
    EXPECT_EQ(packets[1].SongPosition(), 8192);
    EXPECT_EQ(packets[2].SongSelect(), 7);
    EXPECT_TRUE(packets[3].IsSystem(SystemStatus::TuneRequest));
    EXPECT_EQ(EncodeAll(packets), (std::vector<uint8_t>{0xF1, 0x23, 0xF2, 0x00, 0x40, 0xF3, 7, 0xF6}));
}

// SPEC-MIDI-11
TEST(MIDILARProtocolMidi1StreamTests, RealTimeBytesInterruptWithoutBreakingMessages) {
    const std::vector<Packet> packets = ParseAll({0xB2, 0xF8, 7, 0xFE, 100, 0xF0, 1, 0xFA, 2, 0xF7});
    ASSERT_EQ(packets.size(), 5u);
    EXPECT_EQ(packets[0], System(0xF8));
    EXPECT_EQ(packets[1], System(0xFE));
    EXPECT_EQ(packets[2], Voice(0xB2, 7, 100));
    EXPECT_EQ(packets[3], System(0xFA));
    EXPECT_EQ(packets[4].SysExStatus(), SysExStatus::Complete);
    EXPECT_EQ(packets[4].SysExSize(), 2);
}

// SPEC-MIDI-12
TEST(MIDILARProtocolMidi1StreamTests, StatusWhereDataWasExpectedAbortsTheMessage) {
    const std::vector<Packet> packets = ParseAll({0x90, 60, 0x80, 61, 0});
    ASSERT_EQ(packets.size(), 1u);
    EXPECT_EQ(packets[0], Voice(0x80, 61, 0));
}

TEST(MIDILARProtocolMidi1StreamTests, IgnoresUndefinedStatusAndStrayBytes) {
    EXPECT_TRUE(ParseAll({0xF4, 0xF5, 0xF9, 0xFD, 0xF7, 1, 2, 3}).empty());
    const std::vector<Packet> packets = ParseAll({0x90, 60, 0xF4, 0xFD, 100});
    ASSERT_EQ(packets.size(), 1u);
    EXPECT_EQ(packets[0], Voice(0x90, 60, 100));
}

#if defined(MIDILAR_PARSER_DIAGNOSTICS)
// SPEC-MIDI-15
TEST(MIDILARProtocolMidi1StreamTests, CountsAbortedMessagesAndIgnoredBytes) {
    Midi1Parser parser;
    ParseAll(parser, {0x90, 60, 0x80, 61, 0, 0xF4, 0xF7, 0xFD});
    EXPECT_EQ(parser.AbortedMessages(), 1u);
    EXPECT_EQ(parser.IgnoredBytes(), 3u);
}
#endif

// SPEC-MIDI-13, SPEC-MIDI-14
TEST(MIDILARProtocolMidi1StreamTests, SysExOfEverySizeRoundTrips) {
    for (uint8_t size = 0; size <= 40; ++size) {
        std::vector<uint8_t> bytes{0xF0};
        for (uint8_t i = 0; i < size; ++i) {
            bytes.push_back(static_cast<uint8_t>((i * 37u) & 0x7Fu));
        }
        bytes.push_back(0xF7);

        const std::vector<Packet> packets = ParseAll(bytes);
        ASSERT_EQ(packets.size(), size == 0 ? 1u : (size + 5u) / 6u) << int(size);
        if (packets.size() == 1) {
            EXPECT_EQ(packets[0].SysExStatus(), SysExStatus::Complete);
        } else {
            EXPECT_EQ(packets.front().SysExStatus(), SysExStatus::Start);
            EXPECT_EQ(packets.back().SysExStatus(), SysExStatus::End);
            for (size_t i = 1; i + 1 < packets.size(); ++i) {
                EXPECT_EQ(packets[i].SysExStatus(), SysExStatus::Continue);
            }
        }
        EXPECT_EQ(EncodeAll(packets), bytes);

        uint8_t buffer[64] = {};
        SysExAssembler assembler(buffer, sizeof buffer);
        bool complete = false;
        for (const Packet& packet : packets) {
            complete = assembler.Push(packet);
        }
        EXPECT_TRUE(complete);
        ASSERT_EQ(assembler.Size(), size);
        EXPECT_FALSE(assembler.IsTruncated());
        EXPECT_TRUE(std::equal(buffer, buffer + size, bytes.begin() + 1));
    }
}

TEST(MIDILARProtocolMidi1StreamTests, StatusByteEndsSysEx) {
    const std::vector<Packet> packets = ParseAll({0xF0, 1, 2, 3, 4, 5, 6, 7, 0x90, 60, 100});
    ASSERT_EQ(packets.size(), 3u);
    EXPECT_EQ(packets[0].SysExStatus(), SysExStatus::Start);
    EXPECT_EQ(packets[1].SysExStatus(), SysExStatus::End);
    EXPECT_EQ(packets[1].SysExSize(), 1);
    EXPECT_EQ(packets[2], Voice(0x90, 60, 100));

    const std::vector<Packet> tune = ParseAll({0xF0, 1, 0xF6});
    ASSERT_EQ(tune.size(), 2u);
    EXPECT_EQ(tune[0].SysExStatus(), SysExStatus::Complete);
    EXPECT_TRUE(tune[1].IsSystem(SystemStatus::TuneRequest));
}

TEST(MIDILARProtocolMidi1StreamTests, AssemblerTruncatesAndKeepsSynchronized) {
    const std::vector<Packet> packets = ParseAll({0xF0, 1, 2, 3, 4, 5, 6, 7, 8, 0xF7, 0xF0, 9, 0xF7});
    uint8_t buffer[4] = {};
    SysExAssembler assembler(buffer, sizeof buffer);
    ASSERT_EQ(packets.size(), 3u);
    EXPECT_FALSE(assembler.Push(packets[0]));
    EXPECT_TRUE(assembler.Push(packets[1]));
    EXPECT_EQ(assembler.Size(), 4u);
    EXPECT_TRUE(assembler.IsTruncated());
    EXPECT_EQ(buffer[3], 4);
    EXPECT_TRUE(assembler.Push(packets[2]));
    EXPECT_EQ(assembler.Size(), 1u);
    EXPECT_FALSE(assembler.IsTruncated());
    EXPECT_EQ(buffer[0], 9);

    SysExAssembler empty(nullptr, 10);
    EXPECT_TRUE(empty.Push(packets[2]));
    EXPECT_EQ(empty.Size(), 0u);
    EXPECT_TRUE(empty.IsTruncated());
    EXPECT_FALSE(empty.Push(Packet::System(Group::FromWire(0), SystemStatus::Start)));
}

TEST(MIDILARProtocolMidi1StreamTests, AssemblerIgnoresContinuationWithoutStart) {
    const uint8_t data[2] = {1, 2};
    uint8_t buffer[8] = {};
    SysExAssembler assembler(buffer, sizeof buffer);
    EXPECT_FALSE(assembler.Push(Packet::SysEx7(Group::FromWire(0), SysExStatus::End, data, 2)));
    EXPECT_EQ(assembler.Size(), 0u);
}

TEST(MIDILARProtocolMidi1StreamTests, EncoderWritesRunningStatusOnlyWhenEnabled) {
    const std::vector<Packet> packets = {Voice(0x90, 60, 100), Voice(0x90, 62, 0), System(0xF8),
                                         Voice(0x90, 64, 1), System(0xF6), Voice(0x90, 65, 2)};
    EXPECT_EQ(EncodeAll(packets),
              (std::vector<uint8_t>{0x90, 60, 100, 0x90, 62, 0, 0xF8, 0x90, 64, 1, 0xF6, 0x90, 65, 2}));

    Midi1Encoder encoder;
    encoder.SetRunningStatus(true);
    const std::vector<uint8_t> bytes = EncodeAll(encoder, packets);
    EXPECT_EQ(bytes, (std::vector<uint8_t>{0x90, 60, 100, 62, 0, 0xF8, 64, 1, 0xF6, 0x90, 65, 2}));

    const std::vector<Packet> parsed = ParseAll(bytes);
    ASSERT_EQ(parsed.size(), packets.size());
    EXPECT_EQ(parsed[1], Voice(0x80, 62, 0));
}

TEST(MIDILARProtocolMidi1StreamTests, EncodesMidi2ChannelVoiceAtMidi1Resolution) {
    const Group g = Group::FromWire(0);
    const Channel c = Channel::FromWire(1);
    const NoteNumber n = NoteNumber::FromValue(60);
    const std::vector<Packet> packets = {
        Packet::Midi2NoteOn(g, c, n, Velocity::FromMidi2(0x0001)),
        Packet::Midi2NoteOff(g, c, n, Velocity::FromMidi1(64)),
        Packet::Midi2PolyPressure(g, c, n, PressureValue::FromMidi1(3)),
        Packet::Midi2ControlChange(g, c, ControllerNumber::FromValue(7), ControllerValue::Max()),
        Packet::Midi2ChannelPressure(g, c, PressureValue::FromMidi1(9)),
        Packet::Midi2PitchBend(g, c, PitchBend::Center()),
        Packet::Midi2ProgramChange(g, c, ProgramNumber::FromValue(5), (2 << 7) | 3),
        Packet::Midi2ProgramChange(g, c, ProgramNumber::FromValue(6)),
    };
    EXPECT_EQ(EncodeAll(packets), (std::vector<uint8_t>{0x91, 60, 1, 0x81, 60, 64, 0xA1, 60, 3, 0xB1, 7, 127,
                                                        0xD1, 9, 0xE1, 0x00, 0x40, 0xB1, 0, 2, 0xB1, 32, 3,
                                                        0xC1, 5, 0xC1, 6}));
}

TEST(MIDILARProtocolMidi1StreamTests, EncoderSkipsPacketsWithoutMidi1Form) {
    uint8_t out[Midi1Encoder::MaxBytes];
    Midi1Encoder encoder;
    EXPECT_EQ(encoder.Encode(Packet(), out), 0);
    EXPECT_EQ(encoder.Encode(Packet::Noop(), out), 0);
    EXPECT_EQ(encoder.Encode(Packet::JrTimestamp(5), out), 0);
    EXPECT_EQ(encoder.Encode(Packet::FromWords(0x40000000u, 0), out), 0);
    EXPECT_EQ(encoder.Encode(Packet::FromWords(0xF0000000u), out), 0);
    EXPECT_EQ(encoder.Encode(System(0xF4), out), 0);
    EXPECT_EQ(encoder.Encode(Packet::FromWords(0x30700000u), out), 0);
}
