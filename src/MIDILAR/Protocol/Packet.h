#ifndef MIDILAR_PROTOCOL_PACKET_H
#define MIDILAR_PROTOCOL_PACKET_H

#include <stdint.h>

#include "Channel.h"
#include "Scaling.h"
#include "Values.h"

namespace MIDILAR::Protocol {

/** @brief UMP message type, bits 31-28 of the first word. @ingroup MIDILAR_Protocol */
enum class MessageType : uint8_t {
    Utility = 0x0,
    System = 0x1,
    Midi1ChannelVoice = 0x2,
    Data64 = 0x3,
    Midi2ChannelVoice = 0x4,
    Data128 = 0x5,
    FlexData = 0xD,
    Stream = 0xF
};

/** @brief Utility message status. @ingroup MIDILAR_Protocol */
enum class UtilityStatus : uint8_t {
    Noop = 0x0,
    JrClock = 0x1,
    JrTimestamp = 0x2,
    TicksPerQuarterNote = 0x3,
    DeltaClockstamp = 0x4
};

/** @brief System common and real-time status bytes. @ingroup MIDILAR_Protocol */
enum class SystemStatus : uint8_t {
    TimeCode = 0xF1,
    SongPosition = 0xF2,
    SongSelect = 0xF3,
    TuneRequest = 0xF6,
    TimingClock = 0xF8,
    Start = 0xFA,
    Continue = 0xFB,
    Stop = 0xFC,
    ActiveSensing = 0xFE,
    Reset = 0xFF
};

/**
 * @brief Channel voice status, bits 23-20 of the first word.
 * @ingroup MIDILAR_Protocol
 *
 * `NoteOff` to `PitchBend` exist in both protocols. The per-note and
 * registered/assignable controller statuses are MIDI 2.0 only and are
 * classified here; their typed constructors come in a later phase.
 */
enum class VoiceStatus : uint8_t {
    RegisteredPerNoteController = 0x0,
    AssignablePerNoteController = 0x1,
    RegisteredController = 0x2,
    AssignableController = 0x3,
    RelativeRegisteredController = 0x4,
    RelativeAssignableController = 0x5,
    PerNotePitchBend = 0x6,
    NoteOff = 0x8,
    NoteOn = 0x9,
    PolyPressure = 0xA,
    ControlChange = 0xB,
    ProgramChange = 0xC,
    ChannelPressure = 0xD,
    PitchBendChange = 0xE,
    PerNoteManagement = 0xF
};

/** @brief 7-bit System Exclusive packet position. @ingroup MIDILAR_Protocol */
enum class SysExStatus : uint8_t {
    Complete = 0x0,
    Start = 0x1,
    Continue = 0x2,
    End = 0x3
};

/**
 * @brief One Universal MIDI Packet of one to four 32-bit words (SPEC-MIDI-1,
 * SPEC-MIDI-16..19).
 * @ingroup MIDILAR_Protocol
 *
 * Named constructors build valid packets from validated values and return the
 * invalid packet when any argument is invalid. Queries read the fields of the
 * message type they belong to; on other packets they return the invalid
 * value, or zero/minimum for types without one, so check `Type()` and the
 * status first.
 *
 * MIDI 1.0 channel voice packets carry 7- and 14-bit values; their
 * constructors scale the MIDI 2.0 resolution values down and their queries
 * scale back up (SPEC-MIDI-3).
 */
class Packet {
    static constexpr uint32_t InvalidWord = 0xFFFFFFFFu;

    uint32_t _words[4];

    constexpr Packet(uint32_t w0, uint32_t w1, uint32_t w2, uint32_t w3) noexcept
        : _words{w0, w1, w2, w3} {}

    static constexpr uint32_t Header(MessageType type, uint32_t group, uint32_t status) noexcept {
        return (static_cast<uint32_t>(type) << 28) | (group << 24) | (status << 20);
    }

    static constexpr bool IsVoiceAddress(Protocol::Group group, Protocol::Channel channel) noexcept {
        return group.IsValid() && channel.IsValid();
    }

    static constexpr Packet Midi1Voice(Protocol::Group group, Protocol::Channel channel,
                                       Protocol::VoiceStatus status, uint32_t data1,
                                       uint32_t data2) noexcept {
        return IsVoiceAddress(group, channel)
            ? Packet(Header(MessageType::Midi1ChannelVoice, group.Wire(), static_cast<uint32_t>(status)) |
                         (static_cast<uint32_t>(channel.Wire()) << 16) | (data1 << 8) | data2,
                     0, 0, 0)
            : Packet();
    }

    static constexpr Packet Midi2Voice(Protocol::Group group, Protocol::Channel channel,
                                       Protocol::VoiceStatus status, uint32_t index,
                                       uint32_t data) noexcept {
        return IsVoiceAddress(group, channel)
            ? Packet(Header(MessageType::Midi2ChannelVoice, group.Wire(), static_cast<uint32_t>(status)) |
                         (static_cast<uint32_t>(channel.Wire()) << 16) | index,
                     data, 0, 0)
            : Packet();
    }

    static constexpr Packet SystemPacket(Protocol::Group group, Protocol::SystemStatus status,
                                         uint32_t data1, uint32_t data2) noexcept {
        return group.IsValid()
            ? Packet((static_cast<uint32_t>(MessageType::System) << 28) |
                         (static_cast<uint32_t>(group.Wire()) << 24) |
                         (static_cast<uint32_t>(status) << 16) | (data1 << 8) | data2,
                     0, 0, 0)
            : Packet();
    }

    static constexpr Packet UtilityPacket(Protocol::UtilityStatus status, uint32_t data) noexcept {
        return Packet(Header(MessageType::Utility, 0, static_cast<uint32_t>(status)) | data, 0, 0, 0);
    }

    constexpr uint32_t Field(uint32_t word, uint32_t shift, uint32_t mask) const noexcept {
        return (_words[word] >> shift) & mask;
    }

    constexpr bool IsVoice(Protocol::VoiceStatus status) const noexcept {
        return IsChannelVoice() && VoiceStatus() == status;
    }

    constexpr bool IsMidi1() const noexcept { return Type() == MessageType::Midi1ChannelVoice; }

public:
    /** @brief Creates the invalid packet (SPEC-RT-3). */
    constexpr Packet() noexcept : _words{InvalidWord, InvalidWord, InvalidWord, InvalidWord} {}

    /** @brief Returns the invalid packet. */
    static constexpr Packet Invalid() noexcept { return Packet(); }

    /**
     * @brief Creates a packet from raw words, as read from a transport.
     *
     * Words beyond the message type's `WordCount()` are ignored by
     * comparisons. The words are not validated.
     */
    static constexpr Packet FromWords(uint32_t w0, uint32_t w1 = 0, uint32_t w2 = 0, uint32_t w3 = 0) noexcept {
        return Packet(w0, w1, w2, w3);
    }

    /** @name Utility messages (32-bit, no group) */
    ///@{
    /** @brief No operation. */
    static constexpr Packet Noop() noexcept { return UtilityPacket(Protocol::UtilityStatus::Noop, 0); }

    /** @brief Jitter-reduction clock with the sender's 16-bit clock time. */
    static constexpr Packet JrClock(uint16_t time) noexcept {
        return UtilityPacket(Protocol::UtilityStatus::JrClock, time);
    }

    /** @brief Jitter-reduction timestamp of the following message. */
    static constexpr Packet JrTimestamp(uint16_t time) noexcept {
        return UtilityPacket(Protocol::UtilityStatus::JrTimestamp, time);
    }

    /** @brief Delta-clockstamp resolution in ticks per quarter note. */
    static constexpr Packet TicksPerQuarterNote(uint16_t ticks) noexcept {
        return UtilityPacket(Protocol::UtilityStatus::TicksPerQuarterNote, ticks);
    }

    /** @brief Ticks since the previous event, `0-0xFFFFF`. */
    static constexpr Packet DeltaClockstamp(uint32_t ticks) noexcept {
        return ticks <= 0xFFFFFu ? UtilityPacket(Protocol::UtilityStatus::DeltaClockstamp, ticks) : Packet();
    }
    ///@}

    /** @name System common and real-time messages (32-bit) */
    ///@{
    /**
     * @brief A system message without data: tune request or any real-time
     * status. Statuses that carry data return the invalid packet.
     */
    static constexpr Packet System(Protocol::Group group, Protocol::SystemStatus status) noexcept {
        return status == Protocol::SystemStatus::TuneRequest ||
                       static_cast<uint8_t>(status) >= static_cast<uint8_t>(Protocol::SystemStatus::TimingClock)
            ? SystemPacket(group, status, 0, 0)
            : Packet();
    }

    /** @brief MIDI Time Code quarter frame with its 7-bit data byte. */
    static constexpr Packet TimeCode(Protocol::Group group, uint8_t value) noexcept {
        return value <= 127 ? SystemPacket(group, Protocol::SystemStatus::TimeCode, value, 0) : Packet();
    }

    /** @brief Song position pointer in MIDI beats, `0-16383`. */
    static constexpr Packet SongPosition(Protocol::Group group, uint16_t beats) noexcept {
        return beats <= 16383
            ? SystemPacket(group, Protocol::SystemStatus::SongPosition, beats & 0x7Fu,
                           static_cast<uint32_t>(beats) >> 7)
            : Packet();
    }

    /** @brief Song select, `0-127`. */
    static constexpr Packet SongSelect(Protocol::Group group, uint8_t song) noexcept {
        return song <= 127 ? SystemPacket(group, Protocol::SystemStatus::SongSelect, song, 0) : Packet();
    }
    ///@}

    /** @name MIDI 1.0 channel voice messages (32-bit) */
    ///@{
    /**
     * @brief Note On. A velocity that scales down to 0 is sent as 1, so the
     * message stays a Note On (MIDI 2.0 translation rule).
     */
    static constexpr Packet Midi1NoteOn(Protocol::Group group, Protocol::Channel channel, NoteNumber note,
                                        Protocol::Velocity velocity) noexcept {
        return note.IsValid()
            ? Midi1Voice(group, channel, Protocol::VoiceStatus::NoteOn, note.Value(),
                         velocity.Midi1() == 0 ? 1u : velocity.Midi1())
            : Packet();
    }

    /** @brief Note Off. */
    static constexpr Packet Midi1NoteOff(Protocol::Group group, Protocol::Channel channel, NoteNumber note,
                                         Protocol::Velocity velocity) noexcept {
        return note.IsValid()
            ? Midi1Voice(group, channel, Protocol::VoiceStatus::NoteOff, note.Value(), velocity.Midi1())
            : Packet();
    }

    /** @brief Polyphonic key pressure. */
    static constexpr Packet Midi1PolyPressure(Protocol::Group group, Protocol::Channel channel,
                                              NoteNumber note, PressureValue pressure) noexcept {
        return note.IsValid()
            ? Midi1Voice(group, channel, Protocol::VoiceStatus::PolyPressure, note.Value(), pressure.Midi1())
            : Packet();
    }

    /** @brief Control change. */
    static constexpr Packet Midi1ControlChange(Protocol::Group group, Protocol::Channel channel,
                                               ControllerNumber controller,
                                               Protocol::ControllerValue value) noexcept {
        return controller.IsValid()
            ? Midi1Voice(group, channel, Protocol::VoiceStatus::ControlChange, controller.Value(), value.Midi1())
            : Packet();
    }

    /** @brief Program change. */
    static constexpr Packet Midi1ProgramChange(Protocol::Group group, Protocol::Channel channel,
                                               ProgramNumber program) noexcept {
        return program.IsValid()
            ? Midi1Voice(group, channel, Protocol::VoiceStatus::ProgramChange, program.Value(), 0)
            : Packet();
    }

    /** @brief Channel pressure. */
    static constexpr Packet Midi1ChannelPressure(Protocol::Group group, Protocol::Channel channel,
                                                 PressureValue pressure) noexcept {
        return Midi1Voice(group, channel, Protocol::VoiceStatus::ChannelPressure, pressure.Midi1(), 0);
    }

    /** @brief Pitch bend, 14-bit as LSB then MSB. */
    static constexpr Packet Midi1PitchBend(Protocol::Group group, Protocol::Channel channel,
                                           Protocol::PitchBend bend) noexcept {
        return Midi1Voice(group, channel, Protocol::VoiceStatus::PitchBendChange, bend.Midi1() & 0x7Fu,
                          bend.Midi1() >> 7);
    }
    ///@}

    /** @name MIDI 2.0 channel voice messages (64-bit) */
    ///@{
    /** @brief Note On with an optional attribute (type `0` is none). */
    static constexpr Packet Midi2NoteOn(Protocol::Group group, Protocol::Channel channel, NoteNumber note,
                                        Protocol::Velocity velocity, uint8_t attributeType = 0,
                                        uint16_t attributeData = 0) noexcept {
        return note.IsValid()
            ? Midi2Voice(group, channel, Protocol::VoiceStatus::NoteOn,
                         (static_cast<uint32_t>(note.Value()) << 8) | attributeType,
                         (static_cast<uint32_t>(velocity.Midi2()) << 16) | attributeData)
            : Packet();
    }

    /** @brief Note Off with an optional attribute (type `0` is none). */
    static constexpr Packet Midi2NoteOff(Protocol::Group group, Protocol::Channel channel, NoteNumber note,
                                         Protocol::Velocity velocity, uint8_t attributeType = 0,
                                         uint16_t attributeData = 0) noexcept {
        return note.IsValid()
            ? Midi2Voice(group, channel, Protocol::VoiceStatus::NoteOff,
                         (static_cast<uint32_t>(note.Value()) << 8) | attributeType,
                         (static_cast<uint32_t>(velocity.Midi2()) << 16) | attributeData)
            : Packet();
    }

    /** @brief Polyphonic key pressure. */
    static constexpr Packet Midi2PolyPressure(Protocol::Group group, Protocol::Channel channel,
                                              NoteNumber note, PressureValue pressure) noexcept {
        return note.IsValid()
            ? Midi2Voice(group, channel, Protocol::VoiceStatus::PolyPressure,
                         static_cast<uint32_t>(note.Value()) << 8, pressure.Midi2())
            : Packet();
    }

    /** @brief Control change. */
    static constexpr Packet Midi2ControlChange(Protocol::Group group, Protocol::Channel channel,
                                               ControllerNumber controller,
                                               Protocol::ControllerValue value) noexcept {
        return controller.IsValid()
            ? Midi2Voice(group, channel, Protocol::VoiceStatus::ControlChange,
                         static_cast<uint32_t>(controller.Value()) << 8, value.Midi2())
            : Packet();
    }

    /**
     * @brief Program change, optionally with a 14-bit bank (`0-16383`);
     * `bank = -1` sends no bank. Any other bank is invalid.
     */
    static constexpr Packet Midi2ProgramChange(Protocol::Group group, Protocol::Channel channel,
                                               ProgramNumber program, int32_t bank = -1) noexcept {
        return program.IsValid() && bank >= -1 && bank <= 16383
            ? Midi2Voice(group, channel, Protocol::VoiceStatus::ProgramChange, bank >= 0 ? 1u : 0u,
                         (static_cast<uint32_t>(program.Value()) << 24) |
                             (bank >= 0 ? ((static_cast<uint32_t>(bank) >> 7) << 8) |
                                              (static_cast<uint32_t>(bank) & 0x7Fu)
                                        : 0u))
            : Packet();
    }

    /** @brief Channel pressure. */
    static constexpr Packet Midi2ChannelPressure(Protocol::Group group, Protocol::Channel channel,
                                                 PressureValue pressure) noexcept {
        return Midi2Voice(group, channel, Protocol::VoiceStatus::ChannelPressure, 0, pressure.Midi2());
    }

    /** @brief Pitch bend. */
    static constexpr Packet Midi2PitchBend(Protocol::Group group, Protocol::Channel channel,
                                           Protocol::PitchBend bend) noexcept {
        return Midi2Voice(group, channel, Protocol::VoiceStatus::PitchBendChange, 0, bend.Midi2());
    }
    ///@}

    /**
     * @brief One 7-bit System Exclusive packet with `size` (`0-6`) data
     * bytes, without `0xF0`/`0xF7` (SPEC-MIDI-13). Larger sizes, a null
     * `data` with bytes, or a byte above 127 return the invalid packet.
     */
    static FOUNDATION_CONSTEXPR14 Packet SysEx7(Protocol::Group group, Protocol::SysExStatus status, const uint8_t* data,
                                   uint8_t size) noexcept {
        if (!group.IsValid() || size > 6 || (data == nullptr && size > 0)) {
            return Packet();
        }
        uint32_t bytes[6] = {0, 0, 0, 0, 0, 0};
        for (uint8_t i = 0; i < size; ++i) {
            if (data[i] > 127) {
                return Packet();
            }
            bytes[i] = data[i];
        }
        return Packet(Header(MessageType::Data64, group.Wire(), static_cast<uint32_t>(status)) |
                          (static_cast<uint32_t>(size) << 16) | (bytes[0] << 8) | bytes[1],
                      (bytes[2] << 24) | (bytes[3] << 16) | (bytes[4] << 8) | bytes[5], 0, 0);
    }

    /** @brief Returns `true` unless this is the invalid packet. */
    constexpr bool IsValid() const noexcept { return _words[0] != InvalidWord; }

    /** @brief Returns word `index` (`0-3`), or `0` for other indexes. */
    constexpr uint32_t Word(uint8_t index) const noexcept { return index < 4 ? _words[index] : 0; }

    /** @brief Returns the message type. */
    constexpr MessageType Type() const noexcept { return static_cast<MessageType>(Field(0, 28, 0xFu)); }

    /** @brief Returns the packet size in 32-bit words, from the message type. */
    constexpr uint8_t WordCount() const noexcept {
        return Field(0, 28, 0xFu) <= 0x2u || Field(0, 28, 0xFu) == 0x6u || Field(0, 28, 0xFu) == 0x7u ? 1
            : Field(0, 28, 0xFu) <= 0x4u || (Field(0, 28, 0xFu) >= 0x8u && Field(0, 28, 0xFu) <= 0xAu) ? 2
            : Field(0, 28, 0xFu) == 0xBu || Field(0, 28, 0xFu) == 0xCu ? 3
            : 4;
    }

    /**
     * @brief Returns the group; invalid for utility and stream messages,
     * which have none, and for the invalid packet.
     */
    constexpr Protocol::Group Group() const noexcept {
        return Type() == MessageType::Utility || Type() == MessageType::Stream
            ? Protocol::Group::Invalid()
            : Protocol::Group::FromWire(static_cast<int32_t>(Field(0, 24, 0xFu)));
    }

    /** @brief Returns `true` for MIDI 1.0 and MIDI 2.0 channel voice packets. */
    constexpr bool IsChannelVoice() const noexcept {
        return Type() == MessageType::Midi1ChannelVoice || Type() == MessageType::Midi2ChannelVoice;
    }

    /** @brief Returns the channel voice status (channel voice packets). */
    constexpr Protocol::VoiceStatus VoiceStatus() const noexcept {
        return static_cast<Protocol::VoiceStatus>(Field(0, 20, 0xFu));
    }

    /** @brief Returns the channel; invalid unless a channel voice packet. */
    constexpr Protocol::Channel Channel() const noexcept {
        return IsChannelVoice() ? Protocol::Channel::FromWire(static_cast<int32_t>(Field(0, 16, 0xFu)))
                                : Protocol::Channel::Invalid();
    }

    /**
     * @brief Returns `true` for packets addressed to one note: Note On, Note
     * Off and poly pressure, and the MIDI 2.0 per-note controller, per-note
     * pitch bend and per-note management packets.
     */
    constexpr bool HasNote() const noexcept {
        return IsVoice(Protocol::VoiceStatus::NoteOn) || IsVoice(Protocol::VoiceStatus::NoteOff) ||
            IsVoice(Protocol::VoiceStatus::PolyPressure) ||
            (Type() == MessageType::Midi2ChannelVoice &&
             (VoiceStatus() == Protocol::VoiceStatus::RegisteredPerNoteController ||
              VoiceStatus() == Protocol::VoiceStatus::AssignablePerNoteController ||
              VoiceStatus() == Protocol::VoiceStatus::PerNotePitchBend ||
              VoiceStatus() == Protocol::VoiceStatus::PerNoteManagement));
    }

    /** @brief Returns the note of a packet for which `HasNote()` is `true`. */
    constexpr NoteNumber Note() const noexcept {
        return HasNote() ? NoteNumber::FromValue(static_cast<int32_t>(Field(0, 8, 0xFFu))) : NoteNumber::Invalid();
    }

    /**
     * @brief Returns a copy on `group`; packets without a group are returned
     * unchanged and an invalid group gives the invalid packet.
     */
    constexpr Packet WithGroup(Protocol::Group group) const noexcept {
        return !Group().IsValid() ? *this
            : !group.IsValid()    ? Packet()
                                  : Packet((_words[0] & 0xF0FFFFFFu) | (static_cast<uint32_t>(group.Wire()) << 24),
                                           _words[1], _words[2], _words[3]);
    }

    /**
     * @brief Returns a copy on `channel`; packets without a channel are
     * returned unchanged and an invalid channel gives the invalid packet.
     */
    constexpr Packet WithChannel(Protocol::Channel channel) const noexcept {
        return !IsChannelVoice()   ? *this
            : !channel.IsValid()   ? Packet()
                                   : Packet((_words[0] & 0xFFF0FFFFu) | (static_cast<uint32_t>(channel.Wire()) << 16),
                                            _words[1], _words[2], _words[3]);
    }

    /**
     * @brief Returns a copy addressed to `note`; packets without a note are
     * returned unchanged and an invalid note gives the invalid packet.
     */
    constexpr Packet WithNote(NoteNumber note) const noexcept {
        return !HasNote()       ? *this
            : !note.IsValid()   ? Packet()
                                : Packet((_words[0] & 0xFFFF00FFu) | (static_cast<uint32_t>(note.Value()) << 8),
                                         _words[1], _words[2], _words[3]);
    }

    /** @brief Returns the velocity of a Note On or Note Off packet. */
    constexpr Protocol::Velocity Velocity() const noexcept {
        return !(IsVoice(Protocol::VoiceStatus::NoteOn) || IsVoice(Protocol::VoiceStatus::NoteOff))
            ? Protocol::Velocity::Min()
            : IsMidi1() ? Protocol::Velocity::FromMidi1(static_cast<int32_t>(Field(0, 0, 0xFFu)))
                        : Protocol::Velocity::FromMidi2(static_cast<uint16_t>(Field(1, 16, 0xFFFFu)));
    }

    /** @brief Returns the attribute type of a MIDI 2.0 note packet (`0` is none). */
    constexpr uint8_t AttributeType() const noexcept {
        return Type() == MessageType::Midi2ChannelVoice &&
                       (VoiceStatus() == Protocol::VoiceStatus::NoteOn ||
                        VoiceStatus() == Protocol::VoiceStatus::NoteOff)
            ? static_cast<uint8_t>(Field(0, 0, 0xFFu))
            : 0;
    }

    /** @brief Returns the attribute data of a MIDI 2.0 note packet. */
    constexpr uint16_t AttributeData() const noexcept {
        return AttributeType() != 0 ? static_cast<uint16_t>(Field(1, 0, 0xFFFFu)) : 0;
    }

    /** @brief Returns the controller of a control change packet. */
    constexpr ControllerNumber Controller() const noexcept {
        return IsVoice(Protocol::VoiceStatus::ControlChange)
            ? ControllerNumber::FromValue(static_cast<int32_t>(Field(0, 8, 0xFFu)))
            : ControllerNumber::Invalid();
    }

    /** @brief Returns the value of a control change packet. */
    constexpr Protocol::ControllerValue ControllerValue() const noexcept {
        return !IsVoice(Protocol::VoiceStatus::ControlChange) ? Protocol::ControllerValue::Min()
            : IsMidi1() ? Protocol::ControllerValue::FromMidi1(static_cast<int32_t>(Field(0, 0, 0xFFu)))
                        : Protocol::ControllerValue::FromMidi2(_words[1]);
    }

    /** @brief Returns the pressure of a poly or channel pressure packet. */
    constexpr PressureValue Pressure() const noexcept {
        return IsVoice(Protocol::VoiceStatus::PolyPressure)
            ? (IsMidi1() ? PressureValue::FromMidi1(static_cast<int32_t>(Field(0, 0, 0xFFu)))
                         : PressureValue::FromMidi2(_words[1]))
            : IsVoice(Protocol::VoiceStatus::ChannelPressure)
            ? (IsMidi1() ? PressureValue::FromMidi1(static_cast<int32_t>(Field(0, 8, 0xFFu)))
                         : PressureValue::FromMidi2(_words[1]))
            : PressureValue::Min();
    }

    /** @brief Returns the bend of a pitch bend packet; `Min()` otherwise. */
    constexpr Protocol::PitchBend PitchBend() const noexcept {
        return !IsVoice(Protocol::VoiceStatus::PitchBendChange) ? Protocol::PitchBend::Min()
            : IsMidi1() ? Protocol::PitchBend::FromMidi1(
                              static_cast<int32_t>((Field(0, 0, 0x7Fu) << 7) | Field(0, 8, 0x7Fu)))
                        : Protocol::PitchBend::FromMidi2(_words[1]);
    }

    /** @brief Returns the program of a program change packet. */
    constexpr ProgramNumber Program() const noexcept {
        return !IsVoice(Protocol::VoiceStatus::ProgramChange) ? ProgramNumber::Invalid()
            : IsMidi1() ? ProgramNumber::FromValue(static_cast<int32_t>(Field(0, 8, 0xFFu)))
                        : ProgramNumber::FromValue(static_cast<int32_t>(Field(1, 24, 0xFFu)));
    }

    /** @brief Returns `true` for a MIDI 2.0 program change that selects a bank. */
    constexpr bool HasBank() const noexcept {
        return Type() == MessageType::Midi2ChannelVoice &&
            VoiceStatus() == Protocol::VoiceStatus::ProgramChange && Field(0, 0, 1u) != 0;
    }

    /** @brief Returns the 14-bit bank of a MIDI 2.0 program change, or `0`. */
    constexpr uint16_t Bank() const noexcept {
        return HasBank() ? static_cast<uint16_t>((Field(1, 8, 0x7Fu) << 7) | Field(1, 0, 0x7Fu)) : 0;
    }

    /** @brief Returns the status byte of a system packet, `0xF1-0xFF`. */
    constexpr Protocol::SystemStatus SystemStatus() const noexcept {
        return static_cast<Protocol::SystemStatus>(Field(0, 16, 0xFFu));
    }

    /** @brief Returns the quarter-frame byte of a time code packet, or `0`. */
    constexpr uint8_t TimeCode() const noexcept {
        return IsSystem(Protocol::SystemStatus::TimeCode) ? static_cast<uint8_t>(Field(0, 8, 0x7Fu)) : 0;
    }

    /** @brief Returns the beats of a song position packet, or `0`. */
    constexpr uint16_t SongPosition() const noexcept {
        return IsSystem(Protocol::SystemStatus::SongPosition)
            ? static_cast<uint16_t>((Field(0, 0, 0x7Fu) << 7) | Field(0, 8, 0x7Fu))
            : 0;
    }

    /** @brief Returns the song of a song select packet, or `0`. */
    constexpr uint8_t SongSelect() const noexcept {
        return IsSystem(Protocol::SystemStatus::SongSelect) ? static_cast<uint8_t>(Field(0, 8, 0x7Fu)) : 0;
    }

    /** @brief Returns `true` for a system packet with `status`. */
    constexpr bool IsSystem(Protocol::SystemStatus status) const noexcept {
        return Type() == MessageType::System && SystemStatus() == status;
    }

    /** @brief Returns the utility status (utility packets). */
    constexpr Protocol::UtilityStatus UtilityStatus() const noexcept {
        return static_cast<Protocol::UtilityStatus>(Field(0, 20, 0xFu));
    }

    /** @brief Returns the 20-bit payload of a utility packet, or `0`. */
    constexpr uint32_t UtilityData() const noexcept {
        return Type() == MessageType::Utility ? Field(0, 0, 0xFFFFFu) : 0;
    }

    /** @brief Returns the position of a 7-bit System Exclusive packet. */
    constexpr Protocol::SysExStatus SysExStatus() const noexcept {
        return static_cast<Protocol::SysExStatus>(Field(0, 20, 0xFu));
    }

    /** @brief Returns the data byte count of a 7-bit System Exclusive packet. */
    constexpr uint8_t SysExSize() const noexcept {
        return Type() == MessageType::Data64 && Field(0, 16, 0xFu) <= 6
            ? static_cast<uint8_t>(Field(0, 16, 0xFu))
            : 0;
    }

    /** @brief Returns data byte `index` of a 7-bit System Exclusive packet, or `0`. */
    constexpr uint8_t SysExByte(uint8_t index) const noexcept {
        return index >= SysExSize() ? 0
            : index < 2 ? static_cast<uint8_t>(Field(0, 8u - 8u * index, 0xFFu))
                        : static_cast<uint8_t>(Field(1, 24u - 8u * (index - 2u), 0xFFu));
    }

    /** @brief Compares the words that belong to the message; all invalid packets are equal. */
    friend constexpr bool operator==(const Packet& a, const Packet& b) noexcept {
        return a._words[0] == b._words[0] && (a.WordCount() < 2 || a._words[1] == b._words[1]) &&
            (a.WordCount() < 3 || a._words[2] == b._words[2]) &&
            (a.WordCount() < 4 || a._words[3] == b._words[3]) && (a.IsValid() || !b.IsValid());
    }

    /** @brief Negation of `operator==`. */
    friend constexpr bool operator!=(const Packet& a, const Packet& b) noexcept { return !(a == b); }
};

} // namespace MIDILAR::Protocol

#endif // MIDILAR_PROTOCOL_PACKET_H
