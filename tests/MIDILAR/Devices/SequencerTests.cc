#include <gtest/gtest.h>

#include <vector>

#include <MIDILAR.h>

using namespace MIDILAR::Devices;
using namespace MIDILAR::Protocol;

namespace {

int Onsets(uint64_t pattern) {
    int count = 0;
    for (; pattern != 0; pattern >>= 1) {
        count += static_cast<int>(pattern & 1u);
    }
    return count;
}

struct Recorder {
    std::vector<std::pair<uint32_t, Packet>> notes;
    uint32_t clock = 0;
    void Process(const Packet& packet) {
        if (packet.IsSystem(SystemStatus::TimingClock)) {
            ++clock;
        } else if (packet.IsChannelVoice()) {
            notes.emplace_back(clock, packet);
        }
    }
};

const Group G = Group::FromWire(0);

void Send(StepSequencer& sequencer, SystemStatus status, int times = 1) {
    for (int i = 0; i < times; ++i) {
        sequencer.Process(Packet::System(G, status));
    }
}

} // namespace

// SPEC-SEQ-1
TEST(MIDILARDevicesSequencerTests, EuclideanPatternsSpreadOnsetsEvenly) {
    EXPECT_EQ(EuclideanPattern(3, 8), 0b01001001u);
    EXPECT_EQ(EuclideanPattern(4, 16), 0x1111u);
    EXPECT_EQ(EuclideanPattern(0, 8), 0u);
    EXPECT_EQ(EuclideanPattern(8, 8), 0xFFu);
    EXPECT_EQ(EuclideanPattern(9, 8), 0xFFu);
    EXPECT_EQ(EuclideanPattern(1, 0), 0u);
    EXPECT_EQ(EuclideanPattern(1, 65), 0u);
    EXPECT_EQ(EuclideanPattern(64, 64), ~static_cast<uint64_t>(0));
    EXPECT_EQ(EuclideanPattern(3, 8, 1), 0b10010010u);
    EXPECT_EQ(EuclideanPattern(3, 8, 8), EuclideanPattern(3, 8));

    for (uint8_t steps = 1; steps <= MaxSteps; ++steps) {
        for (uint8_t pulses = 0; pulses <= steps; ++pulses) {
            const uint64_t pattern = EuclideanPattern(pulses, steps);
            ASSERT_EQ(Onsets(pattern), pulses) << int(pulses) << "/" << int(steps);
            if (pulses == 0) {
                continue;
            }
            EXPECT_EQ(pattern & 1u, 1u);
            // Gaps between onsets differ by at most one step.
            int shortest = steps;
            int longest = 0;
            int previous = -1;
            int first = -1;
            for (int i = 0; i < steps; ++i) {
                if (((pattern >> i) & 1u) != 0) {
                    if (previous >= 0) {
                        shortest = std::min(shortest, i - previous);
                        longest = std::max(longest, i - previous);
                    } else {
                        first = i;
                    }
                    previous = i;
                }
            }
            const int wrap = steps - previous + first;
            shortest = std::min(shortest, wrap);
            longest = std::max(longest, wrap);
            EXPECT_LE(longest - shortest, 1) << int(pulses) << "/" << int(steps);
        }
    }
}

// SPEC-SEQ-2
TEST(MIDILARDevicesSequencerTests, PlaysThePatternOnTheClock) {
    StepSequencer sequencer;
    Recorder recorder;
    sequencer.Output().Bind<Recorder, &Recorder::Process>(&recorder);
    sequencer.SetPattern(EuclideanPattern(3, 8), 8);
    sequencer.SetNote(NoteNumber::FromValue(36));

    Send(sequencer, SystemStatus::TimingClock, 10);
    EXPECT_TRUE(recorder.notes.empty());

    recorder.clock = 0;
    Send(sequencer, SystemStatus::Start);
    Send(sequencer, SystemStatus::TimingClock, 48);
    ASSERT_EQ(recorder.notes.size(), 6u);
    // Each timing clock is forwarded before the notes it triggers, so the
    // note on clock k (0-based) is recorded after k + 1 clocks.
    const uint32_t onClocks[] = {1, 19, 37};
    for (size_t i = 0; i < 3; ++i) {
        const Packet& on = recorder.notes[2 * i].second;
        const Packet& off = recorder.notes[2 * i + 1].second;
        EXPECT_EQ(recorder.notes[2 * i].first, onClocks[i]);
        EXPECT_EQ(on.VoiceStatus(), VoiceStatus::NoteOn);
        EXPECT_EQ(on.Note(), NoteNumber::FromValue(36));
        EXPECT_EQ(recorder.notes[2 * i + 1].first, onClocks[i] + 3);
        EXPECT_EQ(off.VoiceStatus(), VoiceStatus::NoteOff);
    }
}

TEST(MIDILARDevicesSequencerTests, StopReleasesAndSongPositionMoves) {
    StepSequencer sequencer;
    Recorder recorder;
    sequencer.Output().Bind<Recorder, &Recorder::Process>(&recorder);
    sequencer.SetPattern(0xFFFF, 16);
    sequencer.SetGate(100);
    Send(sequencer, SystemStatus::Start);
    Send(sequencer, SystemStatus::TimingClock, 7);
    ASSERT_EQ(recorder.notes.size(), 3u); // on, off at the next onset, on
    Send(sequencer, SystemStatus::Stop);
    ASSERT_EQ(recorder.notes.size(), 4u);
    EXPECT_EQ(recorder.notes.back().second.VoiceStatus(), VoiceStatus::NoteOff);
    EXPECT_FALSE(sequencer.IsRunning());

    sequencer.Process(Packet::SongPosition(G, 5));
    EXPECT_EQ(sequencer.CurrentStep(), 5);
    sequencer.SetPattern(1u << 5, 16);
    Send(sequencer, SystemStatus::Continue);
    Send(sequencer, SystemStatus::TimingClock);
    ASSERT_EQ(recorder.notes.size(), 5u);
    EXPECT_EQ(recorder.notes.back().second.VoiceStatus(), VoiceStatus::NoteOn);
}

TEST(MIDILARDevicesSequencerTests, SettingsAreClamped) {
    StepSequencer sequencer;
    sequencer.SetPattern(~static_cast<uint64_t>(0), 200);
    EXPECT_EQ(sequencer.Steps(), MaxSteps);
    sequencer.SetPattern(0xFF, 4);
    EXPECT_EQ(sequencer.Pattern(), 0xFu);
    sequencer.SetPattern(1, 0);
    EXPECT_EQ(sequencer.Steps(), 1);
    sequencer.SetClocksPerStep(0);
    EXPECT_EQ(sequencer.ClocksPerStep(), 1);
    sequencer.SetClocksPerStep(200);
    EXPECT_EQ(sequencer.ClocksPerStep(), 96);
    sequencer.SetGate(0);
    EXPECT_EQ(sequencer.Gate(), 1);
}

TEST(MIDILARDevicesSequencerTests, FollowsAClockGenerator) {
    static uint32_t milliseconds = 0;
    Foundation::Time::Clock clock([]() { return milliseconds; }, Foundation::Time::Frequency(1000, 1));
    ClockGenerator generator(clock);
    StepSequencer sequencer;
    Recorder recorder;
    generator.Output().Bind<StepSequencer, &StepSequencer::Process>(&sequencer);
    sequencer.Output().Bind<Recorder, &Recorder::Process>(&recorder);
    sequencer.SetPattern(EuclideanPattern(4, 16), 16);
    generator.Start();
    // One bar of 4/4 at 120 BPM is two seconds: four onsets.
    for (milliseconds = 0; milliseconds < 2000; ++milliseconds) {
        generator.Update();
    }
    int onsets = 0;
    for (const auto& note : recorder.notes) {
        onsets += note.second.VoiceStatus() == VoiceStatus::NoteOn ? 1 : 0;
    }
    EXPECT_EQ(onsets, 4);
}
