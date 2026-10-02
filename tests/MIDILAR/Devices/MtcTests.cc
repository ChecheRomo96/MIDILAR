#include <gtest/gtest.h>

#include <vector>

#include <MIDILAR.h>

using namespace MIDILAR::Devices;
using namespace MIDILAR::Protocol;

namespace {

uint32_t g_milliseconds = 0;
uint32_t Milliseconds() { return g_milliseconds; }

struct Sink {
    std::vector<Packet> packets;
    void Process(const Packet& packet) { packets.push_back(packet); }
};

TimeCode At(int h, int m, int s, int f, TimeCodeRate rate = TimeCodeRate::Fps30) {
    return TimeCode::From(h, m, s, f, rate);
}

struct MtcFixture : ::testing::Test {
    Foundation::Time::Clock clock{Milliseconds, Foundation::Time::Frequency(1000, 1)};
    Sink sink;
    void SetUp() override { g_milliseconds = 0; }
    void RunFor(MtcGenerator& generator, uint32_t milliseconds) {
        for (uint32_t i = 0; i < milliseconds; ++i) {
            ++g_milliseconds;
            generator.Update();
        }
    }
};

} // namespace

// SPEC-MTC-1
TEST(MIDILARProtocolTimeCodeTests, ValidatesFields) {
    EXPECT_TRUE(At(23, 59, 59, 29).IsValid());
    EXPECT_FALSE(At(24, 0, 0, 0).IsValid());
    EXPECT_FALSE(At(0, 60, 0, 0).IsValid());
    EXPECT_FALSE(At(0, 0, 60, 0).IsValid());
    EXPECT_FALSE(At(0, 0, 0, 24, TimeCodeRate::Fps24).IsValid());
    EXPECT_TRUE(At(0, 0, 0, 24, TimeCodeRate::Fps25).IsValid());
    EXPECT_FALSE(At(0, 0, 0, 25, TimeCodeRate::Fps25).IsValid());
    EXPECT_FALSE(At(0, 1, 0, 1, TimeCodeRate::Fps29_97Drop).IsValid());
    EXPECT_TRUE(At(0, 10, 0, 0, TimeCodeRate::Fps29_97Drop).IsValid());
    EXPECT_FALSE(TimeCode::From(0, 0, 0, 0, static_cast<TimeCodeRate>(4)).IsValid());
    EXPECT_FALSE(TimeCode().IsValid());
    EXPECT_FALSE(TimeCode().Next().IsValid());
}

TEST(MIDILARProtocolTimeCodeTests, NextCarriesAndDropsFrames) {
    EXPECT_EQ(At(0, 0, 0, 29).Next(), At(0, 0, 1, 0));
    EXPECT_EQ(At(23, 59, 59, 29).Next(), At(0, 0, 0, 0));
    EXPECT_EQ(At(1, 2, 3, 23, TimeCodeRate::Fps24).Next(), At(1, 2, 4, 0, TimeCodeRate::Fps24));
    const TimeCodeRate df = TimeCodeRate::Fps29_97Drop;
    EXPECT_EQ(At(0, 0, 59, 29, df).Next(), At(0, 1, 0, 2, df));
    EXPECT_EQ(At(0, 9, 59, 29, df).Next(), At(0, 10, 0, 0, df));

    // Ten minutes of 29.97 drop-frame is exactly 17982 frames.
    TimeCode time = At(0, 0, 0, 0, df);
    for (int i = 0; i < 17982; ++i) {
        time = time.Next();
    }
    EXPECT_EQ(time, At(0, 10, 0, 0, df));
}

TEST(MIDILARProtocolTimeCodeTests, QuarterFramesRoundTrip) {
    const TimeCode times[] = {At(23, 59, 59, 29), At(12, 34, 56, 7, TimeCodeRate::Fps25),
                              At(1, 2, 3, 4, TimeCodeRate::Fps29_97Drop), At(0, 0, 0, 0, TimeCodeRate::Fps24)};
    for (const TimeCode& time : times) {
        uint8_t nibbles[8] = {};
        for (uint8_t piece = 0; piece < 8; ++piece) {
            const uint8_t data = time.QuarterFrame(piece);
            EXPECT_EQ(data >> 4, piece);
            nibbles[piece] = data & 0x0Fu;
        }
        EXPECT_EQ(TimeCode::FromQuarterFrames(nibbles), time);
    }
    EXPECT_EQ(At(1, 0, 0, 0, TimeCodeRate::Fps25).QuarterFrame(7), 0x72);
}

// SPEC-MTC-2
TEST_F(MtcFixture, GeneratorSendsFourQuarterFramesPerFrameWithoutDrift) {
    MtcGenerator generator(clock);
    generator.Output().Bind<Sink, &Sink::Process>(&sink);
    generator.Locate(At(0, 0, 0, 0, TimeCodeRate::Fps29_97Drop), false);
    generator.Start();
    RunFor(generator, 600600);
    // 600.6 s at 30000/1001 fps is 18000 frames, 72000 quarter frames.
    EXPECT_EQ(sink.packets.size(), 72001u);
    EXPECT_EQ(generator.Time(), At(0, 10, 0, 18, TimeCodeRate::Fps29_97Drop));
    generator.Stop();
    RunFor(generator, 1000);
    EXPECT_EQ(sink.packets.size(), 72001u);
}

TEST_F(MtcFixture, GeneratorSendsFullFrames) {
    MtcGenerator generator(clock, Group::FromWire(3));
    generator.Output().Bind<Sink, &Sink::Process>(&sink);
    generator.Locate(At(1, 2, 3, 4, TimeCodeRate::Fps25));
    ASSERT_EQ(sink.packets.size(), 2u);
    EXPECT_EQ(sink.packets[0].SysExStatus(), SysExStatus::Start);
    EXPECT_EQ(sink.packets[0].SysExByte(4), (1 << 5) | 1);
    EXPECT_EQ(sink.packets[1].SysExStatus(), SysExStatus::End);
    EXPECT_EQ(sink.packets[1].SysExByte(1), 4);
    EXPECT_EQ(sink.packets[1].Group(), Group::FromWire(3));

    uint8_t bytes[2][Midi1Encoder::MaxBytes];
    Midi1Encoder encoder;
    const uint8_t first = encoder.Encode(sink.packets[0], bytes[0]);
    const uint8_t second = encoder.Encode(sink.packets[1], bytes[1]);
    const std::vector<uint8_t> wire(bytes[0], bytes[0] + first);
    std::vector<uint8_t> all = wire;
    all.insert(all.end(), bytes[1], bytes[1] + second);
    EXPECT_EQ(all, (std::vector<uint8_t>{0xF0, 0x7F, 0x7F, 0x01, 0x01, 0x21, 0x02, 0x03, 0x04, 0xF7}));

    generator.Locate(TimeCode::Invalid());
    EXPECT_EQ(sink.packets.size(), 2u);
}

// SPEC-MTC-3
TEST_F(MtcFixture, ReceiverFollowsTheGenerator) {
    MtcGenerator generator(clock);
    MtcReceiver receiver;
    generator.Output().Bind<MtcReceiver, &MtcReceiver::Process>(&receiver);
    receiver.Output().Bind<Sink, &Sink::Process>(&sink);
    EXPECT_FALSE(receiver.Time().IsValid());

    generator.Locate(At(10, 20, 30, 10, TimeCodeRate::Fps25));
    EXPECT_EQ(receiver.Time(), At(10, 20, 30, 10, TimeCodeRate::Fps25));
    generator.Start();
    RunFor(generator, 2000);
    // Quarter frames lag the generator by up to two frames.
    const TimeCode expected = At(10, 20, 32, 10, TimeCodeRate::Fps25);
    EXPECT_TRUE(receiver.Time() == expected || receiver.Time().Next() == expected ||
                receiver.Time().Next().Next() == expected);
}

TEST(MIDILARDevicesMtcTests, ReceiverRestartsOnOutOfOrderPieces) {
    MtcReceiver receiver;
    const TimeCode time = At(5, 6, 7, 8);
    const Group g = Group::FromWire(0);
    receiver.Process(Packet::TimeCode(g, time.QuarterFrame(3)));
    for (uint8_t piece = 0; piece < 7; ++piece) {
        receiver.Process(Packet::TimeCode(g, time.QuarterFrame(piece)));
    }
    EXPECT_FALSE(receiver.Time().IsValid());
    receiver.Process(Packet::TimeCode(g, time.QuarterFrame(7)));
    EXPECT_EQ(receiver.Time(), At(5, 6, 7, 10));

    const uint8_t other[8] = {0x7F, 0x7F, 0x06, 0x01, 0, 0, 0, 0};
    receiver.Process(Packet::SysEx7(g, SysExStatus::Start, other, 6));
    receiver.Process(Packet::SysEx7(g, SysExStatus::End, other + 6, 2));
    EXPECT_EQ(receiver.Time(), At(5, 6, 7, 10));
}
