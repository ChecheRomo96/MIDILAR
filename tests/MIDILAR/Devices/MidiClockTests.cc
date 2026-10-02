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
    size_t Count(SystemStatus status) const {
        size_t count = 0;
        for (const Packet& packet : packets) {
            count += packet.IsSystem(status) ? 1u : 0u;
        }
        return count;
    }
};

struct ClockFixture : ::testing::Test {
    Foundation::Time::Clock clock{Milliseconds, Foundation::Time::Frequency(1000, 1)};
    Sink sink;

    void SetUp() override { g_milliseconds = 0; }

    void RunFor(ClockGenerator& generator, uint32_t milliseconds) {
        for (uint32_t i = 0; i < milliseconds; ++i) {
            ++g_milliseconds;
            generator.Update();
        }
    }
};

} // namespace

// SPEC-CLK-1
TEST_F(ClockFixture, GeneratorSendsTwentyFourClocksPerBeatWithoutDrift) {
    ClockGenerator generator(clock);
    generator.Output().Bind<Sink, &Sink::Process>(&sink);
    generator.Update();
    RunFor(generator, 60000);
    // 120 BPM for one minute: 120 beats x 24 clocks, plus the clock at t = 0.
    EXPECT_EQ(sink.Count(SystemStatus::TimingClock), 2881u);

    sink.packets.clear();
    generator.SetTempo(9000);
    RunFor(generator, 60000);
    EXPECT_EQ(sink.Count(SystemStatus::TimingClock), 2160u);
    EXPECT_EQ(generator.Tempo(), 9000u);
}

// SPEC-CLK-2
TEST_F(ClockFixture, GeneratorTransportControlsPosition) {
    ClockGenerator generator(clock, Group::FromWire(2));
    generator.Output().Bind<Sink, &Sink::Process>(&sink);
    generator.Update();
    RunFor(generator, 500);
    EXPECT_EQ(generator.Position(), 0u);
    generator.Start();
    RunFor(generator, 500);
    const uint32_t played = generator.Position();
    EXPECT_EQ(played, 24u);
    generator.Stop();
    RunFor(generator, 500);
    EXPECT_EQ(generator.Position(), played);
    generator.SetSongPosition(4);
    EXPECT_EQ(generator.Position(), 24u);
    generator.Continue();
    generator.SetSongPosition(8);
    EXPECT_EQ(generator.Position(), 24u);
    EXPECT_EQ(sink.Count(SystemStatus::Start), 1u);
    EXPECT_EQ(sink.Count(SystemStatus::Stop), 1u);
    EXPECT_EQ(sink.Count(SystemStatus::Continue), 1u);
    EXPECT_EQ(sink.Count(SystemStatus::SongPosition), 1u);
    for (const Packet& packet : sink.packets) {
        EXPECT_EQ(packet.Group(), Group::FromWire(2));
    }
}

TEST_F(ClockFixture, GeneratorDropsTheBacklogAfterAStall) {
    ClockGenerator generator(clock);
    generator.Output().Bind<Sink, &Sink::Process>(&sink);
    generator.Update();
    g_milliseconds = 10000;
    generator.Update();
    EXPECT_EQ(sink.Count(SystemStatus::TimingClock), 1u + ClockGenerator::MaxCatchUp);
    sink.packets.clear();
    RunFor(generator, 1000);
    EXPECT_EQ(sink.Count(SystemStatus::TimingClock), 48u);
}

TEST_F(ClockFixture, GeneratorClampsTempo) {
    ClockGenerator generator(clock);
    generator.SetTempo(0);
    EXPECT_EQ(generator.Tempo(), 100u);
    generator.SetTempo(1000000);
    EXPECT_EQ(generator.Tempo(), 99999u);
}

// SPEC-CLK-3
TEST_F(ClockFixture, ReceiverFollowsTheGenerator) {
    ClockGenerator generator(clock);
    ClockReceiver receiver(clock);
    generator.Output().Bind<ClockReceiver, &ClockReceiver::Process>(&receiver);
    receiver.Output().Bind<Sink, &Sink::Process>(&sink);
    EXPECT_EQ(receiver.Tempo(), 0u);

    generator.SetTempo(14000);
    generator.Update();
    RunFor(generator, 2000);
    EXPECT_NEAR(static_cast<double>(receiver.Tempo()), 14000.0, 150.0);
    EXPECT_FALSE(receiver.IsRunning());

    generator.Start();
    RunFor(generator, 1000);
    EXPECT_TRUE(receiver.IsRunning());
    EXPECT_EQ(receiver.Position(), generator.Position());
    generator.Stop();
    generator.SetSongPosition(16);
    EXPECT_FALSE(receiver.IsRunning());
    EXPECT_EQ(receiver.Position(), 96u);
    generator.Continue();
    RunFor(generator, 1000);
    EXPECT_EQ(receiver.Position(), generator.Position());
    EXPECT_EQ(sink.packets.size(), sink.Count(SystemStatus::TimingClock) + 4u);
}

TEST_F(ClockFixture, ReceiverIgnoresSongPositionWhileRunning) {
    ClockReceiver receiver(clock);
    receiver.Process(Packet::System(Group::FromWire(0), SystemStatus::Start));
    receiver.Process(Packet::SongPosition(Group::FromWire(0), 10));
    EXPECT_EQ(receiver.Position(), 0u);
    receiver.Process(Packet::Noop());
}
