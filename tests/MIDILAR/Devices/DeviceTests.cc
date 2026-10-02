#include <gtest/gtest.h>

#include <vector>

#include <MCC/Chord/Chords.h>
#include <MCC/Scale/Scales.h>
#include <MIDILAR.h>

#ifndef MIDILAR_DEVICES
    #error "MIDILAR.h must expose MIDILAR_DEVICES when the Devices facade is available"
#endif

using namespace MIDILAR::Devices;
using namespace MIDILAR::Protocol;

namespace {

struct Sink {
    std::vector<Packet> packets;
    void Process(const Packet& packet) { packets.push_back(packet); }
};

template <typename T>
void Connect(T& device, Sink& sink) {
    device.Output().template Bind<Sink, &Sink::Process>(&sink);
}

constexpr Group G1 = Group::FromWire(0);

Packet NoteOn(int32_t note, int32_t channel = 0, int32_t velocity = 100) {
    return Packet::Midi1NoteOn(G1, Channel::FromWire(channel), NoteNumber::FromValue(note),
                               Velocity::FromMidi1(velocity));
}

Packet NoteOff2(int32_t note, int32_t channel = 0) {
    return Packet::Midi2NoteOff(G1, Channel::FromWire(channel), NoteNumber::FromValue(note), Velocity());
}

const Packet Clock = Packet::System(G1, SystemStatus::TimingClock);

} // namespace

// SPEC-DEV-1
TEST(MIDILARDevicesTests, UnboundOutputDropsPackets) {
    Transpose transpose;
    transpose.Process(NoteOn(60));
    Router<2> router;
    router.Process(NoteOn(60));
    EXPECT_EQ(router.Output(2), nullptr);
}

TEST(MIDILARDevicesTests, ChannelFilterPassesSelectedGroupsAndChannels) {
    ChannelFilter filter;
    Sink sink;
    Connect(filter, sink);
    filter.SetChannels(ChannelMask::Only(Channel::FromNumber(10)));
    filter.Process(NoteOn(60, 9));
    filter.Process(NoteOn(60, 0));
    filter.Process(Clock);
    filter.Process(Packet::JrTimestamp(1));
    filter.SetGroups(GroupMask::Only(Group::FromWire(3)));
    filter.Process(NoteOn(60, 9));
    filter.Process(Clock);
    filter.Process(Packet::Noop());
    ASSERT_EQ(sink.packets.size(), 4u);
    EXPECT_EQ(sink.packets[0], NoteOn(60, 9));
    EXPECT_EQ(sink.packets[1], Clock);
    EXPECT_EQ(sink.packets[2], Packet::JrTimestamp(1));
    EXPECT_EQ(sink.packets[3], Packet::Noop());
}

TEST(MIDILARDevicesTests, ChannelReassignMovesSourceChannels) {
    ChannelReassign reassign;
    Sink sink;
    Connect(reassign, sink);
    reassign.Process(NoteOn(60, 1));
    reassign.SetTarget(Channel::FromWire(5));
    reassign.SetSources(ChannelMask::Only(Channel::FromWire(1)));
    reassign.Process(NoteOn(60, 1));
    reassign.Process(NoteOn(60, 2));
    reassign.Process(Clock);
    reassign.SetTargetGroup(Group::FromWire(7));
    reassign.Process(NoteOff2(61, 1));
    ASSERT_EQ(sink.packets.size(), 5u);
    EXPECT_EQ(sink.packets[0], NoteOn(60, 1));
    EXPECT_EQ(sink.packets[1], NoteOn(60, 5));
    EXPECT_EQ(sink.packets[2], NoteOn(60, 2));
    EXPECT_EQ(sink.packets[3], Clock);
    EXPECT_EQ(sink.packets[4].Channel(), Channel::FromWire(5));
    EXPECT_EQ(sink.packets[4].Group(), Group::FromWire(7));
    EXPECT_EQ(sink.packets[4].Note(), NoteNumber::FromValue(61));
    EXPECT_EQ(sink.packets[4].VoiceStatus(), VoiceStatus::NoteOff);
}

TEST(MIDILARDevicesTests, TransposeShiftsNotesAndDropsOutOfRange) {
    Transpose transpose;
    Sink sink;
    Connect(transpose, sink);
    transpose.SetSemitones(12);
    transpose.Process(NoteOn(60));
    transpose.Process(NoteOn(120));
    transpose.Process(NoteOff2(60));
    transpose.Process(Packet::Midi1PolyPressure(G1, Channel::FromWire(0), NoteNumber::FromValue(1),
                                                PressureValue::Max()));
    transpose.Process(Packet::Midi1ControlChange(G1, Channel::FromWire(0), ControllerNumber::FromValue(60),
                                                 ControllerValue::Max()));
    ASSERT_EQ(sink.packets.size(), 4u);
    EXPECT_EQ(sink.packets[0], NoteOn(72));
    EXPECT_EQ(sink.packets[1], NoteOff2(72));
    EXPECT_EQ(sink.packets[2].Note(), NoteNumber::FromValue(13));
    EXPECT_EQ(sink.packets[3].Controller(), ControllerNumber::FromValue(60));
    transpose.SetSemitones(-500);
    EXPECT_EQ(transpose.Semitones(), -127);
}

TEST(MIDILARDevicesTests, TransposeMovesMidi2PerNotePackets) {
    Transpose transpose;
    Sink sink;
    Connect(transpose, sink);
    transpose.SetSemitones(-1);
    const Packet perNoteBend = Packet::FromWords(0x40603C00u, 0x80000000u);
    ASSERT_TRUE(perNoteBend.HasNote());
    transpose.Process(perNoteBend);
    ASSERT_EQ(sink.packets.size(), 1u);
    EXPECT_EQ(sink.packets[0].Note(), NoteNumber::FromValue(59));
    EXPECT_EQ(sink.packets[0].Word(1), 0x80000000u);
}

TEST(MIDILARDevicesTests, VelocityCurveMapsNoteOnOnly) {
    VelocityCurve curve;
    Sink sink;
    Connect(curve, sink);
    curve.Process(NoteOn(60, 0, 100));
    curve.Map().Bind([](Velocity) { return Velocity(); });
    curve.Process(NoteOn(60, 0, 100));
    curve.Process(Packet::Midi2NoteOn(G1, Channel::FromWire(0), NoteNumber::FromValue(60), Velocity::Max(), 3, 7));
    curve.Process(NoteOff2(60));
    ASSERT_EQ(sink.packets.size(), 4u);
    EXPECT_EQ(sink.packets[0], NoteOn(60, 0, 100));
    EXPECT_EQ(sink.packets[1], NoteOn(60, 0, 1));
    EXPECT_EQ(sink.packets[2].Velocity(), Velocity());
    EXPECT_EQ(sink.packets[2].VoiceStatus(), VoiceStatus::NoteOn);
    EXPECT_EQ(sink.packets[2].AttributeData(), 7);
    EXPECT_EQ(sink.packets[3], NoteOff2(60));
}

TEST(MIDILARDevicesTests, ScaleFilterKeepsNotesOnTheScale) {
    ScaleFilter filter;
    filter.SetScale(MCC::Scales::Make(MCC::NoteName(MCC::Letter::C, MCC::Accidental::Natural()),
                                      MCC::Scales::Id::Major));
    const NoteNumber cSharp = NoteNumber::FromValue(61);
    EXPECT_EQ(filter.Map(NoteNumber::FromValue(60)), NoteNumber::FromValue(60));
    EXPECT_EQ(filter.Map(cSharp), NoteNumber::FromValue(60));
    filter.SetMode(ScaleFilterMode::Up);
    EXPECT_EQ(filter.Map(cSharp), NoteNumber::FromValue(62));
    filter.SetMode(ScaleFilterMode::Down);
    EXPECT_EQ(filter.Map(cSharp), NoteNumber::FromValue(60));
    filter.SetMode(ScaleFilterMode::Drop);
    EXPECT_FALSE(filter.Map(cSharp).IsValid());

    // C major has no gap wider than a whole tone, so every note moves at
    // most one semitone and lands on the scale.
    filter.SetMode(ScaleFilterMode::Nearest);
    for (int32_t n = 0; n <= 127; ++n) {
        const NoteNumber mapped = filter.Map(NoteNumber::FromValue(n));
        ASSERT_TRUE(mapped.IsValid()) << n;
        EXPECT_LE(mapped.Value() > n ? mapped.Value() - n : n - mapped.Value(), 1) << n;
        EXPECT_EQ(filter.Map(mapped), mapped);
    }

    Sink sink;
    Connect(filter, sink);
    filter.SetMode(ScaleFilterMode::Drop);
    filter.Process(NoteOn(61));
    filter.Process(NoteOn(62));
    filter.Process(Clock);
    ASSERT_EQ(sink.packets.size(), 2u);
    EXPECT_EQ(sink.packets[0], NoteOn(62));
    EXPECT_EQ(sink.packets[1], Clock);
}

TEST(MIDILARDevicesTests, ScaleFilterWithoutScalePassesEverything) {
    ScaleFilter filter;
    filter.SetMode(ScaleFilterMode::Drop);
    EXPECT_EQ(filter.Map(NoteNumber::FromValue(61)), NoteNumber::FromValue(61));
}

TEST(MIDILARDevicesTests, DevicesChainAndRouterFansOut) {
    Transpose transpose;
    ChannelFilter drums;
    Router<2> router;
    Sink all;
    Sink onlyDrums;
    transpose.SetSemitones(1);
    drums.SetChannels(ChannelMask::Only(Channel::FromNumber(10)));
    transpose.Output().Bind<Router<2>, &Router<2>::Process>(&router);
    router.Output(0)->Bind<Sink, &Sink::Process>(&all);
    router.Output(1)->Bind<ChannelFilter, &ChannelFilter::Process>(&drums);
    Connect(drums, onlyDrums);

    transpose.Process(NoteOn(60, 9));
    transpose.Process(NoteOn(60, 0));
    ASSERT_EQ(all.packets.size(), 2u);
    ASSERT_EQ(onlyDrums.packets.size(), 1u);
    EXPECT_EQ(onlyDrums.packets[0], NoteOn(61, 9));
}

TEST(MIDILARDevicesTests, PacketCopiesChangeOneField) {
    const Packet note = NoteOn(60, 1);
    EXPECT_EQ(note.WithChannel(Channel::FromWire(4)).Channel(), Channel::FromWire(4));
    EXPECT_EQ(note.WithGroup(Group::FromWire(4)).Group(), Group::FromWire(4));
    EXPECT_EQ(note.WithNote(NoteNumber::FromValue(1)).Note(), NoteNumber::FromValue(1));
    EXPECT_EQ(note.WithNote(NoteNumber::FromValue(1)).Velocity(), note.Velocity());
    EXPECT_FALSE(note.WithChannel(Channel::Invalid()).IsValid());
    EXPECT_FALSE(note.WithGroup(Group::Invalid()).IsValid());
    EXPECT_FALSE(note.WithNote(NoteNumber::Invalid()).IsValid());
    EXPECT_EQ(Clock.WithChannel(Channel::FromWire(4)), Clock);
    EXPECT_EQ(Clock.WithNote(NoteNumber::FromValue(4)), Clock);
    EXPECT_EQ(Packet::Noop().WithGroup(Group::FromWire(4)), Packet::Noop());
    EXPECT_FALSE(Packet().WithGroup(Group::FromWire(4)).IsValid());
}

// SPEC-DEV-3
TEST(MIDILARDevicesTests, HeldNotesReleaseWhereTheyStarted) {
    Transpose transpose;
    Sink sink;
    Connect(transpose, sink);
    transpose.SetSemitones(2);
    transpose.Process(NoteOn(60));
    transpose.Process(Packet::Midi2PolyPressure(G1, Channel::FromWire(0), NoteNumber::FromValue(60),
                                                PressureValue::Max()));
    transpose.SetSemitones(5);
    transpose.Process(NoteOff2(60));
    transpose.Process(NoteOff2(60));
    ASSERT_EQ(sink.packets.size(), 4u);
    EXPECT_EQ(sink.packets[1].Note(), NoteNumber::FromValue(62));
    EXPECT_EQ(sink.packets[2], NoteOff2(62));
    EXPECT_EQ(sink.packets[3], NoteOff2(65));
}

TEST(MIDILARDevicesTests, HeldNotesAreTrackedPerChannelAndGroup) {
    Transpose transpose;
    Sink sink;
    Connect(transpose, sink);
    transpose.SetSemitones(1);
    transpose.Process(NoteOn(60, 0));
    transpose.SetSemitones(3);
    transpose.Process(NoteOn(60, 1));
    transpose.Process(NoteOff2(60, 1));
    transpose.Process(NoteOff2(60, 0));
    ASSERT_EQ(sink.packets.size(), 4u);
    EXPECT_EQ(sink.packets[2], NoteOff2(63, 1));
    EXPECT_EQ(sink.packets[3], NoteOff2(61, 0));
}

TEST(MIDILARDevicesTests, Midi1NoteOnWithVelocityZeroReleases) {
    Transpose transpose;
    Sink sink;
    Connect(transpose, sink);
    transpose.SetSemitones(1);
    transpose.Process(NoteOn(60));
    transpose.SetSemitones(0);
    transpose.Process(Packet::FromWords(0x20903C00u));
    ASSERT_EQ(sink.packets.size(), 2u);
    EXPECT_EQ(sink.packets[1].Note(), NoteNumber::FromValue(61));
}

TEST(MIDILARDevicesTests, DroppedNoteOnDropsItsNoteOff) {
    ScaleFilter filter;
    Sink sink;
    Connect(filter, sink);
    filter.SetScale(MCC::Scales::Make(MCC::NoteName(MCC::Letter::C, MCC::Accidental::Natural()),
                                      MCC::Scales::Id::Major));
    filter.SetMode(ScaleFilterMode::Drop);
    filter.Process(NoteOn(61));
    filter.SetScale(MCC::Scale());
    filter.Process(NoteOff2(61));
    filter.Process(NoteOff2(61));
    ASSERT_EQ(sink.packets.size(), 1u);
    EXPECT_EQ(sink.packets[0], NoteOff2(61));
}

TEST(MIDILARDevicesTests, HeldNotesBeyondCapacityUseTheCurrentMapping) {
    Transpose transpose;
    Sink sink;
    Connect(transpose, sink);
    transpose.SetSemitones(1);
    for (int32_t n = 0; n <= HeldNotes::Capacity; ++n) {
        transpose.Process(NoteOn(n));
    }
    transpose.SetSemitones(2);
    transpose.Process(NoteOff2(0));
    transpose.Process(NoteOff2(HeldNotes::Capacity));
    ASSERT_EQ(sink.packets.size(), HeldNotes::Capacity + 3u);
    EXPECT_EQ(sink.packets[HeldNotes::Capacity + 1u], NoteOff2(1));
    EXPECT_EQ(sink.packets[HeldNotes::Capacity + 2u], NoteOff2(HeldNotes::Capacity + 2));
}

TEST(MIDILARDevicesTests, ChordGeneratorPlaysEveryTone) {
    ChordGenerator chords;
    Sink sink;
    Connect(chords, sink);
    chords.Process(NoteOn(60));
    chords.SetChord(MCC::Chords::Pattern(MCC::Chords::Id::Major));
    EXPECT_EQ(chords.Shape(), (1u << 0) | (1u << 4) | (1u << 7));
    chords.Process(NoteOn(60));
    chords.Process(Clock);
    chords.Process(NoteOn(125)); // 129 and 132 are out of range
    ASSERT_EQ(sink.packets.size(), 6u);
    EXPECT_EQ(sink.packets[0], NoteOn(60));
    EXPECT_EQ(sink.packets[1], NoteOn(60));
    EXPECT_EQ(sink.packets[2], NoteOn(64));
    EXPECT_EQ(sink.packets[3], NoteOn(67));
    EXPECT_EQ(sink.packets[4], Clock);
    EXPECT_EQ(sink.packets[5], NoteOn(125));
}

TEST(MIDILARDevicesTests, ChordGeneratorReleasesTheChordANoteStarted) {
    ChordGenerator chords;
    Sink sink;
    Connect(chords, sink);
    chords.SetChord(MCC::Chords::Pattern(MCC::Chords::Id::DominantSeventh));
    chords.Process(NoteOn(48));
    chords.SetChord(MCC::Chords::Pattern(MCC::Chords::Id::Minor));
    chords.Process(NoteOff2(48));
    ASSERT_EQ(sink.packets.size(), 8u);
    EXPECT_EQ(sink.packets[4], NoteOff2(48));
    EXPECT_EQ(sink.packets[5], NoteOff2(52));
    EXPECT_EQ(sink.packets[6], NoteOff2(55));
    EXPECT_EQ(sink.packets[7], NoteOff2(58));
    chords.SetChord(MCC::ChordPattern());
    EXPECT_EQ(chords.Shape(), 1u);
}
