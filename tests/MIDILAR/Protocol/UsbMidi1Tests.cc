#include <gtest/gtest.h>

#include <array>
#include <vector>

#include <MIDILAR.h>

using namespace MIDILAR::Protocol;

namespace {

using Event = std::array<uint8_t, 4>;

std::vector<Packet> Parse(const std::vector<uint8_t>& bytes, Group group = Group::FromWire(0)) {
    Midi1Parser parser(group);
    std::vector<Packet> packets;
    Packet out[Midi1Parser::MaxPackets];
    for (uint8_t byte : bytes) {
        const uint8_t count = parser.Parse(byte, out);
        packets.insert(packets.end(), out, out + count);
    }
    return packets;
}

std::vector<Event> ToUsb(UsbMidi1Encoder& encoder, const std::vector<Packet>& packets) {
    std::vector<Event> events;
    uint8_t out[UsbMidi1Encoder::MaxEvents][4];
    for (const Packet& packet : packets) {
        const uint8_t count = encoder.Encode(packet, out);
        for (uint8_t i = 0; i < count; ++i) {
            events.push_back({out[i][0], out[i][1], out[i][2], out[i][3]});
        }
    }
    return events;
}

std::vector<Event> ToUsb(const std::vector<Packet>& packets) {
    UsbMidi1Encoder encoder;
    return ToUsb(encoder, packets);
}

std::vector<Packet> FromUsb(const std::vector<Event>& events) {
    std::vector<Packet> packets;
    for (const Event& event : events) {
        const uint8_t raw[4] = {event[0], event[1], event[2], event[3]};
        Packet packet;
        if (DecodeUsbMidi1(raw, packet)) {
            packets.push_back(packet);
        }
    }
    return packets;
}

std::vector<uint8_t> ToBytes(const std::vector<Packet>& packets) {
    Midi1Encoder encoder;
    std::vector<uint8_t> bytes;
    uint8_t out[Midi1Encoder::MaxBytes];
    for (const Packet& packet : packets) {
        const uint8_t count = encoder.Encode(packet, out);
        bytes.insert(bytes.end(), out, out + count);
    }
    return bytes;
}

} // namespace

// SPEC-USB-1, SPEC-USB-2
TEST(MIDILARProtocolUsbMidi1Tests, EncodesSpecificationEvents) {
    EXPECT_EQ(ToUsb(Parse({0x90, 0x3C, 0x64})), (std::vector<Event>{{0x09, 0x90, 0x3C, 0x64}}));
    EXPECT_EQ(ToUsb(Parse({0xC5, 0x07}, Group::FromWire(3))), (std::vector<Event>{{0x3C, 0xC5, 0x07, 0x00}}));
    EXPECT_EQ(ToUsb(Parse({0xF8, 0xF2, 0x10, 0x20, 0xF1, 0x05, 0xF6})),
              (std::vector<Event>{{0x0F, 0xF8, 0, 0}, {0x03, 0xF2, 0x10, 0x20}, {0x02, 0xF1, 0x05, 0}, {0x05, 0xF6, 0, 0}}));
    EXPECT_EQ(ToUsb(Parse({0xF0, 0x7E, 0x7F, 0x06, 0x01, 0xF7})),
              (std::vector<Event>{{0x04, 0xF0, 0x7E, 0x7F}, {0x07, 0x06, 0x01, 0xF7}}));
    EXPECT_EQ(ToUsb(Parse({0xF0, 0xF7})), (std::vector<Event>{{0x06, 0xF0, 0xF7, 0}}));
    EXPECT_EQ(ToUsb(Parse({0xF0, 0x01, 0x02, 0xF7})),
              (std::vector<Event>{{0x04, 0xF0, 0x01, 0x02}, {0x05, 0xF7, 0, 0}}));
}

TEST(MIDILARProtocolUsbMidi1Tests, StreamsRoundTripThroughUsb) {
    const std::vector<std::vector<uint8_t>> streams = {
        {0x80, 1, 2, 0x91, 3, 4, 0xA2, 5, 6, 0xB3, 7, 8, 0xC4, 9, 0xD5, 10, 0xE6, 11, 12},
        {0xF1, 0x23, 0xF2, 0x01, 0x02, 0xF3, 0x04, 0xF6, 0xF8, 0xFA, 0xFB, 0xFC, 0xFE, 0xFF},
    };
    for (const auto& stream : streams) {
        EXPECT_EQ(ToBytes(FromUsb(ToUsb(Parse(stream)))), stream);
    }
    for (uint8_t size = 0; size <= 40; ++size) {
        std::vector<uint8_t> sysex{0xF0};
        for (uint8_t i = 0; i < size; ++i) {
            sysex.push_back(static_cast<uint8_t>((i * 29u) & 0x7Fu));
        }
        sysex.push_back(0xF7);
        const std::vector<Event> events = ToUsb(Parse(sysex));
        EXPECT_EQ(events.size(), (size + 2u + 2u) / 3u) << int(size);
        EXPECT_EQ(ToBytes(FromUsb(events)), sysex) << int(size);

        uint8_t buffer[64] = {};
        SysExAssembler assembler(buffer, sizeof buffer);
        bool complete = false;
        for (const Packet& packet : FromUsb(events)) {
            complete = assembler.Push(packet);
        }
        EXPECT_TRUE(complete);
        EXPECT_EQ(assembler.Size(), size);
    }
}

TEST(MIDILARProtocolUsbMidi1Tests, CablesAreGroups) {
    for (uint8_t cable = 0; cable < 16; ++cable) {
        const uint8_t event[4] = {static_cast<uint8_t>((cable << 4) | 0x9), 0x92, 60, 1};
        Packet packet;
        ASSERT_TRUE(DecodeUsbMidi1(event, packet));
        EXPECT_EQ(packet.Group(), Group::FromWire(cable));
        EXPECT_EQ(packet.Channel(), Channel::FromWire(2));
        EXPECT_EQ(ToUsb({packet}), (std::vector<Event>{{event[0], event[1], event[2], event[3]}}));
    }
}

TEST(MIDILARProtocolUsbMidi1Tests, InterleavedSysExKeepsStatePerCable) {
    UsbMidi1Encoder encoder;
    const std::vector<Packet> a = Parse({0xF0, 1, 2, 3, 4, 5, 6, 7, 0xF7}, Group::FromWire(0));
    const std::vector<Packet> b = Parse({0xF0, 9, 0xF7}, Group::FromWire(1));
    std::vector<Event> events = ToUsb(encoder, {a[0]});
    const std::vector<Event> other = ToUsb(encoder, b);
    const std::vector<Event> rest = ToUsb(encoder, {a[1]});
    EXPECT_EQ(other, (std::vector<Event>{{0x17, 0xF0, 9, 0xF7}}));
    events.insert(events.end(), rest.begin(), rest.end());
    EXPECT_EQ(ToBytes(FromUsb(events)), (std::vector<uint8_t>{0xF0, 1, 2, 3, 4, 5, 6, 7, 0xF7}));
}

TEST(MIDILARProtocolUsbMidi1Tests, DecodesNoteOnVelocityZeroAsNoteOff) {
    const uint8_t event[4] = {0x09, 0x90, 60, 0};
    Packet packet;
    ASSERT_TRUE(DecodeUsbMidi1(event, packet));
    EXPECT_EQ(packet.VoiceStatus(), VoiceStatus::NoteOff);
}

TEST(MIDILARProtocolUsbMidi1Tests, RejectsMalformedEvents) {
    const uint8_t events[][4] = {
        {0x00, 0x90, 1, 2}, {0x01, 0x90, 1, 2}, // reserved CINs
        {0x09, 0x80, 1, 2},                     // CIN does not match the status
        {0x09, 0x90, 0x80, 2},                  // 8-bit data
        {0x0F, 0x40, 0, 0}, {0x0F, 0xF9, 0, 0}, // not real-time, undefined
        {0x02, 0xF2, 1, 0},                     // wrong length
        {0x02, 0xF1, 0x80, 0},                  // 8-bit time code
        {0x06, 0x01, 0x02, 0},                  // end without 0xF7
        {0x04, 0x01, 0x82, 0x03},               // 8-bit SysEx data
    };
    for (const auto& event : events) {
        Packet packet;
        EXPECT_FALSE(DecodeUsbMidi1(event, packet)) << std::hex << int(event[0]) << " " << int(event[1]);
    }
}

TEST(MIDILARProtocolUsbMidi1Tests, EncoderSkipsPacketsWithoutMidi1Form) {
    UsbMidi1Encoder encoder;
    uint8_t out[UsbMidi1Encoder::MaxEvents][4];
    EXPECT_EQ(encoder.Encode(Packet::Noop(), out), 0);
    EXPECT_EQ(encoder.Encode(Packet(), out), 0);
    const uint8_t data[3] = {1, 2, 3};
    EXPECT_EQ(encoder.Encode(Packet::SysEx7(Group::FromWire(0), SysExStatus::Continue, data, 3), out), 0);
}

TEST(MIDILARProtocolUsbMidi1Tests, EncodesMidi2ChannelVoice) {
    const Group g = Group::FromWire(2);
    const Channel c = Channel::FromWire(0);
    const std::vector<Event> events =
        ToUsb({Packet::Midi2ProgramChange(g, c, ProgramNumber::FromValue(5), (1 << 7) | 2)});
    EXPECT_EQ(events, (std::vector<Event>{{0x2B, 0xB0, 0, 1}, {0x2B, 0xB0, 32, 2}, {0x2C, 0xC0, 5, 0}}));
}
