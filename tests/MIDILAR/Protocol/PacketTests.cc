#include <gtest/gtest.h>

#include <MIDILAR.h>

using namespace MIDILAR::Protocol;

namespace {

constexpr Ump::Group G1 = Ump::Group::FromWire(1);
constexpr Channel C2 = Channel::FromWire(2);
constexpr NoteNumber Middle = NoteNumber::FromValue(60);

} // namespace

// SPEC-MIDI-16: packet sizes follow the message type.
TEST(MIDILARProtocolPacketTests, WordCountFollowsMessageType) {
    const uint8_t expected[16] = {1, 1, 1, 2, 2, 4, 1, 1, 2, 2, 2, 3, 3, 4, 4, 4};
    for (uint32_t type = 0; type < 16; ++type) {
        EXPECT_EQ(Ump::Packet::FromWords(type << 28).WordCount(), expected[type]) << type;
    }
}

TEST(MIDILARProtocolPacketTests, DefaultPacketIsInvalid) {
    EXPECT_FALSE(Ump::Packet().IsValid());
    EXPECT_EQ(Ump::Packet(), Ump::Packet::Invalid());
    EXPECT_FALSE(Ump::Packet().Group().IsValid());
    EXPECT_FALSE(Ump::Packet().Channel().IsValid());
    EXPECT_TRUE(Ump::Packet::Noop().IsValid());
    EXPECT_NE(Ump::Packet::Noop(), Ump::Packet::Invalid());
}

// SPEC-MIDI-17: bit layouts of the UMP specification.
TEST(MIDILARProtocolPacketTests, BuildsSpecificationLayouts) {
    EXPECT_EQ(Ump::Packet::Midi1NoteOn(G1, C2, Middle, Midi2::Velocity::FromMidi1(100)).Word(0), 0x21923C64u);
    EXPECT_EQ(Ump::Packet::Midi1PitchBend(G1, C2, Midi2::PitchBend::FromMidi1(0x2001)).Word(0), 0x21E20140u);
    EXPECT_EQ(Ump::Packet::Midi1ProgramChange(G1, C2, ProgramNumber::FromValue(5)).Word(0), 0x21C20500u);
    EXPECT_EQ(Ump::Packet::Midi1ChannelPressure(G1, C2, Midi2::PressureValue::FromMidi1(9)).Word(0), 0x21D20900u);

    const Ump::Packet noteOn = Ump::Packet::Midi2NoteOn(G1, C2, Middle, Midi2::Velocity::FromMidi2(0xABCD), 3, 0x1234);
    EXPECT_EQ(noteOn.Word(0), 0x41923C03u);
    EXPECT_EQ(noteOn.Word(1), 0xABCD1234u);

    const Ump::Packet program = Ump::Packet::Midi2ProgramChange(Ump::Group::FromWire(0), Channel::FromWire(0),
                                                      ProgramNumber::FromValue(5), 0x81);
    EXPECT_EQ(program.Word(0), 0x40C00001u);
    EXPECT_EQ(program.Word(1), 0x05000101u);

    EXPECT_EQ(Ump::Packet::Midi2ControlChange(G1, C2, ControllerNumber::FromValue(7),
                                         Midi2::ControllerValue::FromMidi2(0x12345678u)).Word(1),
              0x12345678u);
    EXPECT_EQ(Ump::Packet::Midi2ControlChange(G1, C2, ControllerNumber::FromValue(7), Midi2::ControllerValue()).Word(0),
              0x41B20700u);

    EXPECT_EQ(Ump::Packet::System(Ump::Group::FromWire(0), SystemStatus::TimingClock).Word(0), 0x10F80000u);
    EXPECT_EQ(Ump::Packet::SongPosition(Ump::Group::FromWire(0), 8192).Word(0), 0x10F20040u);
    EXPECT_EQ(Ump::Packet::JrTimestamp(0x1234).Word(0), 0x00201234u);
    EXPECT_EQ(Ump::Packet::DeltaClockstamp(0xFFFFF).Word(0), 0x004FFFFFu);

    const uint8_t data[4] = {0x7E, 0x7F, 0x06, 0x01};
    const Ump::Packet sysex = Ump::Packet::SysEx7(Ump::Group::FromWire(0), Ump::SysExStatus::Start, data, 4);
    EXPECT_EQ(sysex.Word(0), 0x30147E7Fu);
    EXPECT_EQ(sysex.Word(1), 0x06010000u);
}

TEST(MIDILARProtocolPacketTests, ChannelVoiceRoundTripsEveryAddress) {
    for (int32_t g = 0; g <= 15; ++g) {
        for (int32_t c = 0; c <= 15; ++c) {
            const Ump::Group group = Ump::Group::FromWire(g);
            const Channel channel = Channel::FromWire(c);
            const Ump::Packet packets[2] = {Ump::Packet::Midi1NoteOff(group, channel, Middle, Midi2::Velocity()),
                                       Ump::Packet::Midi2NoteOff(group, channel, Middle, Midi2::Velocity())};
            for (const Ump::Packet& packet : packets) {
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
        const Midi2::Velocity v1 = Midi2::Velocity::FromMidi1(n);
        const Ump::Packet midi1 = Ump::Packet::Midi1NoteOff(G1, C2, note, v1);
        EXPECT_EQ(midi1.Type(), Ump::MessageType::Midi1ChannelVoice);
        EXPECT_EQ(midi1.Note(), note);
        EXPECT_EQ(midi1.Velocity(), v1);

        const Midi2::Velocity v2 = Midi2::Velocity::FromMidi2(static_cast<uint16_t>(n * 513));
        const Ump::Packet midi2 = Ump::Packet::Midi2NoteOn(G1, C2, note, v2);
        EXPECT_EQ(midi2.Type(), Ump::MessageType::Midi2ChannelVoice);
        EXPECT_EQ(midi2.WordCount(), 2);
        EXPECT_EQ(midi2.Note(), note);
        EXPECT_EQ(midi2.Velocity(), v2);
        EXPECT_EQ(midi2.AttributeType(), 0);
    }
}

// SPEC-MIDI-18: a MIDI 1.0 Note On never becomes a Note Off.
TEST(MIDILARProtocolPacketTests, Midi1NoteOnKeepsVelocityAboveZero) {
    EXPECT_EQ(Ump::Packet::Midi1NoteOn(G1, C2, Middle, Midi2::Velocity()).Velocity(), Midi2::Velocity::FromMidi1(1));
    EXPECT_EQ(Ump::Packet::Midi1NoteOn(G1, C2, Middle, Midi2::Velocity::FromMidi2(0x01FF)).Velocity(),
              Midi2::Velocity::FromMidi1(1));
    EXPECT_EQ(Ump::Packet::Midi1NoteOff(G1, C2, Middle, Midi2::Velocity()).Velocity(), Midi2::Velocity());
    EXPECT_EQ(Ump::Packet::Midi2NoteOn(G1, C2, Middle, Midi2::Velocity()).Velocity(), Midi2::Velocity());
}

TEST(MIDILARProtocolPacketTests, NoteAttributesRoundTrip) {
    const Ump::Packet packet = Ump::Packet::Midi2NoteOff(G1, C2, Middle, Midi2::Velocity::Max(), 3, 0xBEEF);
    EXPECT_EQ(packet.AttributeType(), 3);
    EXPECT_EQ(packet.AttributeData(), 0xBEEF);
    EXPECT_EQ(Ump::Packet::Midi1NoteOn(G1, C2, Middle, Midi2::Velocity::Max()).AttributeType(), 0);
}

TEST(MIDILARProtocolPacketTests, ControllersPressureAndPitchBendRoundTrip) {
    for (int32_t value = 0; value <= 127; ++value) {
        const ControllerNumber controller = ControllerNumber::FromValue(value);
        const Ump::Packet cc = Ump::Packet::Midi1ControlChange(G1, C2, controller, Midi2::ControllerValue::FromMidi1(value));
        EXPECT_EQ(cc.Controller(), controller);
        EXPECT_EQ(cc.ControllerValue(), Midi2::ControllerValue::FromMidi1(value));

        EXPECT_EQ(Ump::Packet::Midi1PolyPressure(G1, C2, Middle, Midi2::PressureValue::FromMidi1(value)).Pressure(),
                  Midi2::PressureValue::FromMidi1(value));
        EXPECT_EQ(Ump::Packet::Midi1ChannelPressure(G1, C2, Midi2::PressureValue::FromMidi1(value)).Pressure(),
                  Midi2::PressureValue::FromMidi1(value));
        EXPECT_EQ(Ump::Packet::Midi1ProgramChange(G1, C2, ProgramNumber::FromValue(value)).Program(),
                  ProgramNumber::FromValue(value));
        EXPECT_EQ(Ump::Packet::Midi2ProgramChange(G1, C2, ProgramNumber::FromValue(value)).Program(),
                  ProgramNumber::FromValue(value));
    }
    for (int32_t bend = 0; bend <= 16383; ++bend) {
        EXPECT_EQ(Ump::Packet::Midi1PitchBend(G1, C2, Midi2::PitchBend::FromMidi1(bend)).PitchBend(),
                  Midi2::PitchBend::FromMidi1(bend));
    }
    const uint32_t wide[] = {0u, 1u, 0x80000000u, 0xDEADBEEFu, 0xFFFFFFFFu};
    for (uint32_t value : wide) {
        EXPECT_EQ(Ump::Packet::Midi2ControlChange(G1, C2, ControllerNumber::FromValue(1),
                                             Midi2::ControllerValue::FromMidi2(value)).ControllerValue().Midi2(),
                  value);
        EXPECT_EQ(Ump::Packet::Midi2PolyPressure(G1, C2, Middle, Midi2::PressureValue::FromMidi2(value)).Pressure().Midi2(),
                  value);
        EXPECT_EQ(Ump::Packet::Midi2ChannelPressure(G1, C2, Midi2::PressureValue::FromMidi2(value)).Pressure().Midi2(), value);
        EXPECT_EQ(Ump::Packet::Midi2PitchBend(G1, C2, Midi2::PitchBend::FromMidi2(value)).PitchBend().Midi2(), value);
    }
}

TEST(MIDILARProtocolPacketTests, ProgramChangeBank) {
    for (int32_t bank = 0; bank <= 16383; bank += 127) {
        const Ump::Packet packet = Ump::Packet::Midi2ProgramChange(G1, C2, ProgramNumber::FromValue(1), bank);
        EXPECT_TRUE(packet.HasBank());
        EXPECT_EQ(packet.Bank(), bank);
    }
    EXPECT_FALSE(Ump::Packet::Midi2ProgramChange(G1, C2, ProgramNumber::FromValue(1)).HasBank());
    EXPECT_FALSE(Ump::Packet::Midi2ProgramChange(G1, C2, ProgramNumber::FromValue(1), 16384).IsValid());
    EXPECT_FALSE(Ump::Packet::Midi2ProgramChange(G1, C2, ProgramNumber::FromValue(1), -2).IsValid());
}

TEST(MIDILARProtocolPacketTests, SystemMessagesRoundTrip) {
    const SystemStatus plain[] = {SystemStatus::TuneRequest, SystemStatus::TimingClock, SystemStatus::Start,
                                  SystemStatus::Continue, SystemStatus::Stop, SystemStatus::ActiveSensing,
                                  SystemStatus::Reset};
    for (SystemStatus status : plain) {
        const Ump::Packet packet = Ump::Packet::System(G1, status);
        EXPECT_EQ(packet.Type(), Ump::MessageType::System);
        EXPECT_EQ(packet.Group(), G1);
        EXPECT_TRUE(packet.IsSystem(status));
        EXPECT_FALSE(packet.IsChannelVoice());
        EXPECT_FALSE(packet.Channel().IsValid());
    }
    EXPECT_FALSE(Ump::Packet::System(G1, SystemStatus::SongPosition).IsValid());
    EXPECT_FALSE(Ump::Packet::System(G1, SystemStatus::TimeCode).IsValid());

    for (uint16_t beats = 0; beats <= 16383; ++beats) {
        EXPECT_EQ(Ump::Packet::SongPosition(G1, beats).SongPosition(), beats);
    }
    for (uint8_t value = 0; value <= 127; ++value) {
        EXPECT_EQ(Ump::Packet::TimeCode(G1, value).TimeCode(), value);
        EXPECT_EQ(Ump::Packet::SongSelect(G1, value).SongSelect(), value);
    }
    EXPECT_FALSE(Ump::Packet::SongPosition(G1, 16384).IsValid());
    EXPECT_FALSE(Ump::Packet::TimeCode(G1, 128).IsValid());
    EXPECT_FALSE(Ump::Packet::SongSelect(G1, 128).IsValid());
}

TEST(MIDILARProtocolPacketTests, UtilityMessagesHaveNoGroup) {
    const Ump::Packet packets[] = {Ump::Packet::Noop(), Ump::Packet::JrClock(0xFFFF), Ump::Packet::JrTimestamp(7),
                              Ump::Packet::TicksPerQuarterNote(480), Ump::Packet::DeltaClockstamp(0x12345)};
    const Ump::UtilityStatus statuses[] = {Ump::UtilityStatus::Noop, Ump::UtilityStatus::JrClock, Ump::UtilityStatus::JrTimestamp,
                                      Ump::UtilityStatus::TicksPerQuarterNote, Ump::UtilityStatus::DeltaClockstamp};
    const uint32_t data[] = {0, 0xFFFF, 7, 480, 0x12345};
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(packets[i].Type(), Ump::MessageType::Utility);
        EXPECT_FALSE(packets[i].Group().IsValid());
        EXPECT_EQ(packets[i].UtilityStatus(), statuses[i]);
        EXPECT_EQ(packets[i].UtilityData(), data[i]);
    }
    EXPECT_FALSE(Ump::Packet::DeltaClockstamp(0x100000).IsValid());
}

// SPEC-MIDI-13
TEST(MIDILARProtocolPacketTests, SysEx7CarriesUpToSixDataBytes) {
    const uint8_t data[6] = {1, 2, 3, 4, 5, 127};
    for (uint8_t size = 0; size <= 6; ++size) {
        const Ump::Packet packet = Ump::Packet::SysEx7(G1, Ump::SysExStatus::Continue, data, size);
        ASSERT_TRUE(packet.IsValid());
        EXPECT_EQ(packet.Type(), Ump::MessageType::Data64);
        EXPECT_EQ(packet.SysExStatus(), Ump::SysExStatus::Continue);
        EXPECT_EQ(packet.SysExSize(), size);
        for (uint8_t i = 0; i < 6; ++i) {
            EXPECT_EQ(packet.SysExByte(i), i < size ? data[i] : 0);
        }
    }
    const uint8_t eightBit[1] = {0x80};
    EXPECT_FALSE(Ump::Packet::SysEx7(G1, Ump::SysExStatus::Complete, eightBit, 1).IsValid());
    EXPECT_FALSE(Ump::Packet::SysEx7(G1, Ump::SysExStatus::Complete, data, 7).IsValid());
    EXPECT_FALSE(Ump::Packet::SysEx7(G1, Ump::SysExStatus::Complete, nullptr, 1).IsValid());
    EXPECT_TRUE(Ump::Packet::SysEx7(G1, Ump::SysExStatus::Complete, nullptr, 0).IsValid());
    EXPECT_FALSE(Ump::Packet::SysEx7(Ump::Group::Invalid(), Ump::SysExStatus::Complete, data, 1).IsValid());
}

// SPEC-MIDI-19: invalid arguments produce the invalid packet.
TEST(MIDILARProtocolPacketTests, InvalidArgumentsProduceTheInvalidPacket) {
    const Midi2::Velocity v = Midi2::Velocity::Max();
    EXPECT_FALSE(Ump::Packet::Midi1NoteOn(Ump::Group::Invalid(), C2, Middle, v).IsValid());
    EXPECT_FALSE(Ump::Packet::Midi1NoteOn(G1, Channel::Invalid(), Middle, v).IsValid());
    EXPECT_FALSE(Ump::Packet::Midi1NoteOn(G1, C2, NoteNumber::Invalid(), v).IsValid());
    EXPECT_FALSE(Ump::Packet::Midi2NoteOn(G1, C2, NoteNumber::Invalid(), v).IsValid());
    EXPECT_FALSE(Ump::Packet::Midi1ControlChange(G1, C2, ControllerNumber::Invalid(), Midi2::ControllerValue()).IsValid());
    EXPECT_FALSE(Ump::Packet::Midi2ProgramChange(G1, C2, ProgramNumber::Invalid()).IsValid());
    EXPECT_FALSE(Ump::Packet::Midi2PitchBend(G1, Channel::Invalid(), Midi2::PitchBend::Center()).IsValid());
    EXPECT_FALSE(Ump::Packet::System(Ump::Group::Invalid(), SystemStatus::Start).IsValid());
}

TEST(MIDILARProtocolPacketTests, QueriesOnOtherMessagesReturnNeutralValues) {
    const Ump::Packet clock = Ump::Packet::System(G1, SystemStatus::TimingClock);
    EXPECT_FALSE(clock.Note().IsValid());
    EXPECT_FALSE(clock.Controller().IsValid());
    EXPECT_FALSE(clock.Program().IsValid());
    EXPECT_EQ(clock.Velocity(), Midi2::Velocity::Min());
    EXPECT_EQ(clock.PitchBend(), Midi2::PitchBend::Min());
    EXPECT_EQ(clock.SysExSize(), 0);
    EXPECT_EQ(clock.UtilityData(), 0u);
    EXPECT_FALSE(Ump::Packet::Midi1ProgramChange(G1, C2, ProgramNumber::FromValue(1)).Note().IsValid());
}

TEST(MIDILARProtocolPacketTests, EqualityIgnoresWordsOutsideTheMessage) {
    EXPECT_EQ(Ump::Packet::FromWords(0x10F80000u, 1, 2, 3), Ump::Packet::FromWords(0x10F80000u));
    EXPECT_NE(Ump::Packet::FromWords(0x40000000u, 1), Ump::Packet::FromWords(0x40000000u, 2));
}
