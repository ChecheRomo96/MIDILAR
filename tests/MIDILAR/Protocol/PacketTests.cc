#include <gtest/gtest.h>

#include <MIDILAR.h>

using namespace MIDILAR::Protocol;

namespace {

constexpr Group G1 = Group::FromWire(1);
constexpr Channel C2 = Channel::FromWire(2);
constexpr NoteNumber Middle = NoteNumber::FromValue(60);

} // namespace

// SPEC-MIDI-16: packet sizes follow the message type.
TEST(MIDILARProtocolPacketTests, WordCountFollowsMessageType) {
    const uint8_t expected[16] = {1, 1, 1, 2, 2, 4, 1, 1, 2, 2, 2, 3, 3, 4, 4, 4};
    for (uint32_t type = 0; type < 16; ++type) {
        EXPECT_EQ(Packet::FromWords(type << 28).WordCount(), expected[type]) << type;
    }
}

TEST(MIDILARProtocolPacketTests, DefaultPacketIsInvalid) {
    EXPECT_FALSE(Packet().IsValid());
    EXPECT_EQ(Packet(), Packet::Invalid());
    EXPECT_FALSE(Packet().Group().IsValid());
    EXPECT_FALSE(Packet().Channel().IsValid());
    EXPECT_TRUE(Packet::Noop().IsValid());
    EXPECT_NE(Packet::Noop(), Packet::Invalid());
}

// SPEC-MIDI-17: bit layouts of the UMP specification.
TEST(MIDILARProtocolPacketTests, BuildsSpecificationLayouts) {
    EXPECT_EQ(Packet::Midi1NoteOn(G1, C2, Middle, Velocity::FromMidi1(100)).Word(0), 0x21923C64u);
    EXPECT_EQ(Packet::Midi1PitchBend(G1, C2, PitchBend::FromMidi1(0x2001)).Word(0), 0x21E20140u);
    EXPECT_EQ(Packet::Midi1ProgramChange(G1, C2, ProgramNumber::FromValue(5)).Word(0), 0x21C20500u);
    EXPECT_EQ(Packet::Midi1ChannelPressure(G1, C2, PressureValue::FromMidi1(9)).Word(0), 0x21D20900u);

    const Packet noteOn = Packet::Midi2NoteOn(G1, C2, Middle, Velocity::FromMidi2(0xABCD), 3, 0x1234);
    EXPECT_EQ(noteOn.Word(0), 0x41923C03u);
    EXPECT_EQ(noteOn.Word(1), 0xABCD1234u);

    const Packet program = Packet::Midi2ProgramChange(Group::FromWire(0), Channel::FromWire(0),
                                                      ProgramNumber::FromValue(5), 0x81);
    EXPECT_EQ(program.Word(0), 0x40C00001u);
    EXPECT_EQ(program.Word(1), 0x05000101u);

    EXPECT_EQ(Packet::Midi2ControlChange(G1, C2, ControllerNumber::FromValue(7),
                                         ControllerValue::FromMidi2(0x12345678u)).Word(1),
              0x12345678u);
    EXPECT_EQ(Packet::Midi2ControlChange(G1, C2, ControllerNumber::FromValue(7), ControllerValue()).Word(0),
              0x41B20700u);

    EXPECT_EQ(Packet::System(Group::FromWire(0), SystemStatus::TimingClock).Word(0), 0x10F80000u);
    EXPECT_EQ(Packet::SongPosition(Group::FromWire(0), 8192).Word(0), 0x10F20040u);
    EXPECT_EQ(Packet::JrTimestamp(0x1234).Word(0), 0x00201234u);
    EXPECT_EQ(Packet::DeltaClockstamp(0xFFFFF).Word(0), 0x004FFFFFu);

    const uint8_t data[4] = {0x7E, 0x7F, 0x06, 0x01};
    const Packet sysex = Packet::SysEx7(Group::FromWire(0), SysExStatus::Start, data, 4);
    EXPECT_EQ(sysex.Word(0), 0x30147E7Fu);
    EXPECT_EQ(sysex.Word(1), 0x06010000u);
}

TEST(MIDILARProtocolPacketTests, ChannelVoiceRoundTripsEveryAddress) {
    for (int32_t g = 0; g <= 15; ++g) {
        for (int32_t c = 0; c <= 15; ++c) {
            const Group group = Group::FromWire(g);
            const Channel channel = Channel::FromWire(c);
            const Packet packets[2] = {Packet::Midi1NoteOff(group, channel, Middle, Velocity()),
                                       Packet::Midi2NoteOff(group, channel, Middle, Velocity())};
            for (const Packet& packet : packets) {
                EXPECT_TRUE(packet.IsChannelVoice());
                EXPECT_EQ(packet.Group(), group);
                EXPECT_EQ(packet.Channel(), channel);
                EXPECT_EQ(packet.VoiceStatus(), VoiceStatus::NoteOff);
            }
        }
    }
}

TEST(MIDILARProtocolPacketTests, NotesRoundTripInBothProtocols) {
    for (int32_t n = 0; n <= 127; ++n) {
        const NoteNumber note = NoteNumber::FromValue(n);
        const Velocity v1 = Velocity::FromMidi1(n);
        const Packet midi1 = Packet::Midi1NoteOff(G1, C2, note, v1);
        EXPECT_EQ(midi1.Type(), MessageType::Midi1ChannelVoice);
        EXPECT_EQ(midi1.Note(), note);
        EXPECT_EQ(midi1.Velocity(), v1);

        const Velocity v2 = Velocity::FromMidi2(static_cast<uint16_t>(n * 513));
        const Packet midi2 = Packet::Midi2NoteOn(G1, C2, note, v2);
        EXPECT_EQ(midi2.Type(), MessageType::Midi2ChannelVoice);
        EXPECT_EQ(midi2.WordCount(), 2);
        EXPECT_EQ(midi2.Note(), note);
        EXPECT_EQ(midi2.Velocity(), v2);
        EXPECT_EQ(midi2.AttributeType(), 0);
    }
}

// SPEC-MIDI-18: a MIDI 1.0 Note On never becomes a Note Off.
TEST(MIDILARProtocolPacketTests, Midi1NoteOnKeepsVelocityAboveZero) {
    EXPECT_EQ(Packet::Midi1NoteOn(G1, C2, Middle, Velocity()).Velocity(), Velocity::FromMidi1(1));
    EXPECT_EQ(Packet::Midi1NoteOn(G1, C2, Middle, Velocity::FromMidi2(0x01FF)).Velocity(),
              Velocity::FromMidi1(1));
    EXPECT_EQ(Packet::Midi1NoteOff(G1, C2, Middle, Velocity()).Velocity(), Velocity());
    EXPECT_EQ(Packet::Midi2NoteOn(G1, C2, Middle, Velocity()).Velocity(), Velocity());
}

TEST(MIDILARProtocolPacketTests, NoteAttributesRoundTrip) {
    const Packet packet = Packet::Midi2NoteOff(G1, C2, Middle, Velocity::Max(), 3, 0xBEEF);
    EXPECT_EQ(packet.AttributeType(), 3);
    EXPECT_EQ(packet.AttributeData(), 0xBEEF);
    EXPECT_EQ(Packet::Midi1NoteOn(G1, C2, Middle, Velocity::Max()).AttributeType(), 0);
}

TEST(MIDILARProtocolPacketTests, ControllersPressureAndPitchBendRoundTrip) {
    for (int32_t value = 0; value <= 127; ++value) {
        const ControllerNumber controller = ControllerNumber::FromValue(value);
        const Packet cc = Packet::Midi1ControlChange(G1, C2, controller, ControllerValue::FromMidi1(value));
        EXPECT_EQ(cc.Controller(), controller);
        EXPECT_EQ(cc.ControllerValue(), ControllerValue::FromMidi1(value));

        EXPECT_EQ(Packet::Midi1PolyPressure(G1, C2, Middle, PressureValue::FromMidi1(value)).Pressure(),
                  PressureValue::FromMidi1(value));
        EXPECT_EQ(Packet::Midi1ChannelPressure(G1, C2, PressureValue::FromMidi1(value)).Pressure(),
                  PressureValue::FromMidi1(value));
        EXPECT_EQ(Packet::Midi1ProgramChange(G1, C2, ProgramNumber::FromValue(value)).Program(),
                  ProgramNumber::FromValue(value));
        EXPECT_EQ(Packet::Midi2ProgramChange(G1, C2, ProgramNumber::FromValue(value)).Program(),
                  ProgramNumber::FromValue(value));
    }
    for (int32_t bend = 0; bend <= 16383; ++bend) {
        EXPECT_EQ(Packet::Midi1PitchBend(G1, C2, PitchBend::FromMidi1(bend)).PitchBend(),
                  PitchBend::FromMidi1(bend));
    }
    const uint32_t wide[] = {0u, 1u, 0x80000000u, 0xDEADBEEFu, 0xFFFFFFFFu};
    for (uint32_t value : wide) {
        EXPECT_EQ(Packet::Midi2ControlChange(G1, C2, ControllerNumber::FromValue(1),
                                             ControllerValue::FromMidi2(value)).ControllerValue().Midi2(),
                  value);
        EXPECT_EQ(Packet::Midi2PolyPressure(G1, C2, Middle, PressureValue::FromMidi2(value)).Pressure().Midi2(),
                  value);
        EXPECT_EQ(Packet::Midi2ChannelPressure(G1, C2, PressureValue::FromMidi2(value)).Pressure().Midi2(), value);
        EXPECT_EQ(Packet::Midi2PitchBend(G1, C2, PitchBend::FromMidi2(value)).PitchBend().Midi2(), value);
    }
}

TEST(MIDILARProtocolPacketTests, ProgramChangeBank) {
    for (int32_t bank = 0; bank <= 16383; bank += 127) {
        const Packet packet = Packet::Midi2ProgramChange(G1, C2, ProgramNumber::FromValue(1), bank);
        EXPECT_TRUE(packet.HasBank());
        EXPECT_EQ(packet.Bank(), bank);
    }
    EXPECT_FALSE(Packet::Midi2ProgramChange(G1, C2, ProgramNumber::FromValue(1)).HasBank());
    EXPECT_FALSE(Packet::Midi2ProgramChange(G1, C2, ProgramNumber::FromValue(1), 16384).IsValid());
    EXPECT_FALSE(Packet::Midi2ProgramChange(G1, C2, ProgramNumber::FromValue(1), -2).IsValid());
}

TEST(MIDILARProtocolPacketTests, SystemMessagesRoundTrip) {
    const SystemStatus plain[] = {SystemStatus::TuneRequest, SystemStatus::TimingClock, SystemStatus::Start,
                                  SystemStatus::Continue, SystemStatus::Stop, SystemStatus::ActiveSensing,
                                  SystemStatus::Reset};
    for (SystemStatus status : plain) {
        const Packet packet = Packet::System(G1, status);
        EXPECT_EQ(packet.Type(), MessageType::System);
        EXPECT_EQ(packet.Group(), G1);
        EXPECT_TRUE(packet.IsSystem(status));
        EXPECT_FALSE(packet.IsChannelVoice());
        EXPECT_FALSE(packet.Channel().IsValid());
    }
    EXPECT_FALSE(Packet::System(G1, SystemStatus::SongPosition).IsValid());
    EXPECT_FALSE(Packet::System(G1, SystemStatus::TimeCode).IsValid());

    for (uint16_t beats = 0; beats <= 16383; ++beats) {
        EXPECT_EQ(Packet::SongPosition(G1, beats).SongPosition(), beats);
    }
    for (uint8_t value = 0; value <= 127; ++value) {
        EXPECT_EQ(Packet::TimeCode(G1, value).TimeCode(), value);
        EXPECT_EQ(Packet::SongSelect(G1, value).SongSelect(), value);
    }
    EXPECT_FALSE(Packet::SongPosition(G1, 16384).IsValid());
    EXPECT_FALSE(Packet::TimeCode(G1, 128).IsValid());
    EXPECT_FALSE(Packet::SongSelect(G1, 128).IsValid());
}

TEST(MIDILARProtocolPacketTests, UtilityMessagesHaveNoGroup) {
    const Packet packets[] = {Packet::Noop(), Packet::JrClock(0xFFFF), Packet::JrTimestamp(7),
                              Packet::TicksPerQuarterNote(480), Packet::DeltaClockstamp(0x12345)};
    const UtilityStatus statuses[] = {UtilityStatus::Noop, UtilityStatus::JrClock, UtilityStatus::JrTimestamp,
                                      UtilityStatus::TicksPerQuarterNote, UtilityStatus::DeltaClockstamp};
    const uint32_t data[] = {0, 0xFFFF, 7, 480, 0x12345};
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(packets[i].Type(), MessageType::Utility);
        EXPECT_FALSE(packets[i].Group().IsValid());
        EXPECT_EQ(packets[i].UtilityStatus(), statuses[i]);
        EXPECT_EQ(packets[i].UtilityData(), data[i]);
    }
    EXPECT_FALSE(Packet::DeltaClockstamp(0x100000).IsValid());
}

// SPEC-MIDI-13
TEST(MIDILARProtocolPacketTests, SysEx7CarriesUpToSixDataBytes) {
    const uint8_t data[6] = {1, 2, 3, 4, 5, 127};
    for (uint8_t size = 0; size <= 6; ++size) {
        const Packet packet = Packet::SysEx7(G1, SysExStatus::Continue, data, size);
        ASSERT_TRUE(packet.IsValid());
        EXPECT_EQ(packet.Type(), MessageType::Data64);
        EXPECT_EQ(packet.SysExStatus(), SysExStatus::Continue);
        EXPECT_EQ(packet.SysExSize(), size);
        for (uint8_t i = 0; i < 6; ++i) {
            EXPECT_EQ(packet.SysExByte(i), i < size ? data[i] : 0);
        }
    }
    const uint8_t eightBit[1] = {0x80};
    EXPECT_FALSE(Packet::SysEx7(G1, SysExStatus::Complete, eightBit, 1).IsValid());
    EXPECT_FALSE(Packet::SysEx7(G1, SysExStatus::Complete, data, 7).IsValid());
    EXPECT_FALSE(Packet::SysEx7(G1, SysExStatus::Complete, nullptr, 1).IsValid());
    EXPECT_TRUE(Packet::SysEx7(G1, SysExStatus::Complete, nullptr, 0).IsValid());
    EXPECT_FALSE(Packet::SysEx7(Group::Invalid(), SysExStatus::Complete, data, 1).IsValid());
}

// SPEC-MIDI-19: invalid arguments produce the invalid packet.
TEST(MIDILARProtocolPacketTests, InvalidArgumentsProduceTheInvalidPacket) {
    const Velocity v = Velocity::Max();
    EXPECT_FALSE(Packet::Midi1NoteOn(Group::Invalid(), C2, Middle, v).IsValid());
    EXPECT_FALSE(Packet::Midi1NoteOn(G1, Channel::Invalid(), Middle, v).IsValid());
    EXPECT_FALSE(Packet::Midi1NoteOn(G1, C2, NoteNumber::Invalid(), v).IsValid());
    EXPECT_FALSE(Packet::Midi2NoteOn(G1, C2, NoteNumber::Invalid(), v).IsValid());
    EXPECT_FALSE(Packet::Midi1ControlChange(G1, C2, ControllerNumber::Invalid(), ControllerValue()).IsValid());
    EXPECT_FALSE(Packet::Midi2ProgramChange(G1, C2, ProgramNumber::Invalid()).IsValid());
    EXPECT_FALSE(Packet::Midi2PitchBend(G1, Channel::Invalid(), PitchBend::Center()).IsValid());
    EXPECT_FALSE(Packet::System(Group::Invalid(), SystemStatus::Start).IsValid());
}

TEST(MIDILARProtocolPacketTests, QueriesOnOtherMessagesReturnNeutralValues) {
    const Packet clock = Packet::System(G1, SystemStatus::TimingClock);
    EXPECT_FALSE(clock.Note().IsValid());
    EXPECT_FALSE(clock.Controller().IsValid());
    EXPECT_FALSE(clock.Program().IsValid());
    EXPECT_EQ(clock.Velocity(), Velocity::Min());
    EXPECT_EQ(clock.PitchBend(), PitchBend::Min());
    EXPECT_EQ(clock.SysExSize(), 0);
    EXPECT_EQ(clock.UtilityData(), 0u);
    EXPECT_FALSE(Packet::Midi1ProgramChange(G1, C2, ProgramNumber::FromValue(1)).Note().IsValid());
}

TEST(MIDILARProtocolPacketTests, EqualityIgnoresWordsOutsideTheMessage) {
    EXPECT_EQ(Packet::FromWords(0x10F80000u, 1, 2, 3), Packet::FromWords(0x10F80000u));
    EXPECT_NE(Packet::FromWords(0x40000000u, 1), Packet::FromWords(0x40000000u, 2));
}
