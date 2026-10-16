#ifndef MIDILAR_PROTOCOL_MIDI2_MESSAGE_H
#define MIDILAR_PROTOCOL_MIDI2_MESSAGE_H
#include <MIDILAR_BuildSettings.h>
#include "../Channel.h"
#include "../Values.h"
#include "../Status.h"
#include "Values.h"

namespace MIDILAR::Protocol::Midi2 {
/**
 * @brief Native MIDI 2.0 channel-voice message, without UMP routing (SPEC-NS-2..3).
 * @ingroup MIDILAR_Midi2
 *
 * Two inline field words store the seven supported channel-voice families:
 * notes, poly pressure, CC, program/bank, channel pressure and pitch bend.
 * No UMP message-type or group is stored. SysEx, system, utility, flex and
 * stream messages belong to their own families, not to this class.
 * Invalid factories return one canonical invalid message; no allocation.
 */
class Message {
    uint32_t _header;
    uint32_t _data;

    constexpr Message(uint32_t header, uint32_t data) noexcept : _header(header), _data(data) {}
    static constexpr Message Make(Protocol::Channel channel, Protocol::VoiceStatus status,
                                  uint32_t index, uint32_t data) noexcept {
        return channel.IsValid()
            ? Message((static_cast<uint32_t>(status) << 20) |
                      (static_cast<uint32_t>(channel.Wire()) << 16) | index, data)
            : Message();
    }
    constexpr bool Is(Protocol::VoiceStatus status) const noexcept {
        return IsValid() && Status() == status;
    }
public:
    /** @brief Creates the canonical invalid message. */
    constexpr Message() noexcept : _header(0xFFFFFFFFu), _data(0) {}
    /** @brief True for a supported, validated channel-voice message. */
    constexpr bool IsValid() const noexcept { return _header != 0xFFFFFFFFu; }
    /** @brief Field word 0 or 1; zero for other indices. No UMP group/type. */
    constexpr uint32_t DataWord(uint8_t index) const noexcept {
        return index == 0 ? _header : index == 1 ? _data : 0;
    }
    /** @brief Channel-voice status; meaningful only when valid. */
    constexpr Protocol::VoiceStatus Status() const noexcept {
        return static_cast<Protocol::VoiceStatus>((_header >> 20) & 15u);
    }
    /** @brief Channel, or invalid for an invalid message. */
    constexpr Protocol::Channel Channel() const noexcept {
        return IsValid() ? Protocol::Channel::FromWire(static_cast<int32_t>((_header >> 16) & 15u))
                         : Protocol::Channel::Invalid();
    }

    /** @brief Note On. Velocity zero remains Note On in MIDI 2.0. */
    static constexpr Message NoteOn(Protocol::Channel channel, NoteNumber note, Midi2::Velocity velocity,
                                    uint8_t attributeType = 0, uint16_t attributeData = 0) noexcept {
        return note.IsValid() && (attributeType != 0 || attributeData == 0)
            ? Make(channel, Protocol::VoiceStatus::NoteOn,
                   (static_cast<uint32_t>(note.Value()) << 8) | attributeType,
                   (static_cast<uint32_t>(velocity.Midi2()) << 16) | attributeData) : Message();
    }
    /** @brief Note Off with release velocity and optional attribute. */
    static constexpr Message NoteOff(Protocol::Channel channel, NoteNumber note, Midi2::Velocity velocity,
                                     uint8_t attributeType = 0, uint16_t attributeData = 0) noexcept {
        return note.IsValid() && (attributeType != 0 || attributeData == 0)
            ? Make(channel, Protocol::VoiceStatus::NoteOff,
                   (static_cast<uint32_t>(note.Value()) << 8) | attributeType,
                   (static_cast<uint32_t>(velocity.Midi2()) << 16) | attributeData) : Message();
    }
    /** @brief Polyphonic key pressure at full 32-bit resolution. */
    static constexpr Message PolyPressure(Protocol::Channel channel, NoteNumber note, PressureValue pressure) noexcept {
        return note.IsValid() ? Make(channel, Protocol::VoiceStatus::PolyPressure,
            static_cast<uint32_t>(note.Value()) << 8, pressure.Midi2()) : Message();
    }
    /** @brief Control Change with a 7-bit index and 32-bit value. */
    static constexpr Message ControlChange(Protocol::Channel channel, ControllerNumber controller,
                                           Midi2::ControllerValue value) noexcept {
        return controller.IsValid() ? Make(channel, Protocol::VoiceStatus::ControlChange,
            static_cast<uint32_t>(controller.Value()) << 8, value.Midi2()) : Message();
    }
    /** @brief Program index 0-127; optional bank 0-16383, -1 means absent. */
    static constexpr Message ProgramChange(Protocol::Channel channel, ProgramNumber program,
                                           int32_t bank = -1) noexcept {
        return program.IsValid() && bank >= -1 && bank <= 16383
            ? Make(channel, Protocol::VoiceStatus::ProgramChange, bank >= 0 ? 1u : 0u,
                   (static_cast<uint32_t>(program.Value()) << 24) |
                   (bank >= 0 ? ((static_cast<uint32_t>(bank) >> 7) << 8) |
                                (static_cast<uint32_t>(bank) & 127u) : 0u)) : Message();
    }
    /** @brief Channel-wide pressure at full 32-bit resolution. */
    static constexpr Message ChannelPressure(Protocol::Channel channel, PressureValue pressure) noexcept {
        return Make(channel, Protocol::VoiceStatus::ChannelPressure, 0, pressure.Midi2());
    }
    /** @brief Pitch bend at full 32-bit resolution, center 0x80000000. */
    static constexpr Message PitchBend(Protocol::Channel channel, Midi2::PitchBend bend) noexcept {
        return Make(channel, Protocol::VoiceStatus::PitchBendChange, 0, bend.Midi2());
    }

    /** @brief Note number for note/poly-pressure messages; invalid otherwise. */
    constexpr NoteNumber Note() const noexcept {
        return Is(Protocol::VoiceStatus::NoteOn) || Is(Protocol::VoiceStatus::NoteOff) ||
               Is(Protocol::VoiceStatus::PolyPressure)
            ? NoteNumber::FromValue(static_cast<int32_t>((_header >> 8) & 127u)) : NoteNumber::Invalid();
    }
    /** @brief Note velocity, or Min() on other families. */
    constexpr Midi2::Velocity Velocity() const noexcept {
        return Is(Protocol::VoiceStatus::NoteOn) || Is(Protocol::VoiceStatus::NoteOff)
            ? Midi2::Velocity::FromMidi2(static_cast<uint16_t>(_data >> 16)) : Midi2::Velocity::Min();
    }
    /** @brief Note attribute type, or zero on other families. */
    constexpr uint8_t AttributeType() const noexcept {
        return Is(Protocol::VoiceStatus::NoteOn) || Is(Protocol::VoiceStatus::NoteOff)
            ? static_cast<uint8_t>(_header) : 0;
    }
    /** @brief Note attribute data, or zero when no attribute exists. */
    constexpr uint16_t AttributeData() const noexcept {
        return AttributeType() != 0 ? static_cast<uint16_t>(_data) : 0;
    }
    /** @brief Control Change index, or invalid on other families. */
    constexpr ControllerNumber Controller() const noexcept {
        return Is(Protocol::VoiceStatus::ControlChange)
            ? ControllerNumber::FromValue(static_cast<int32_t>((_header >> 8) & 127u)) : ControllerNumber::Invalid();
    }
    /** @brief Control Change value, or Min() on other families. */
    constexpr Midi2::ControllerValue ControllerValue() const noexcept {
        return Is(Protocol::VoiceStatus::ControlChange) ? Midi2::ControllerValue::FromMidi2(_data)
                                                       : Midi2::ControllerValue::Min();
    }
    /** @brief Poly/channel pressure, or Min() on other families. */
    constexpr PressureValue Pressure() const noexcept {
        return Is(Protocol::VoiceStatus::PolyPressure) || Is(Protocol::VoiceStatus::ChannelPressure)
            ? PressureValue::FromMidi2(_data) : PressureValue::Min();
    }
    /** @brief Pitch bend, or Min() on other families. */
    constexpr Midi2::PitchBend PitchBend() const noexcept {
        return Is(Protocol::VoiceStatus::PitchBendChange) ? Midi2::PitchBend::FromMidi2(_data) : Midi2::PitchBend::Min();
    }
    /** @brief Program index, or invalid on other families. */
    constexpr ProgramNumber Program() const noexcept {
        return Is(Protocol::VoiceStatus::ProgramChange)
            ? ProgramNumber::FromValue(static_cast<int32_t>(_data >> 24)) : ProgramNumber::Invalid();
    }
    /** @brief True when Program Change includes bank selection. */
    constexpr bool HasBank() const noexcept {
        return Is(Protocol::VoiceStatus::ProgramChange) && (_header & 1u) != 0;
    }
    /** @brief 14-bit bank, or zero when absent. */
    constexpr uint16_t Bank() const noexcept {
        return HasBank() ? static_cast<uint16_t>(((_data >> 8) & 127u) << 7 | (_data & 127u)) : 0;
    }

    /**
     * @brief Validates native field words (not a UMP packet).
     * The first word's top byte must be zero. Unknown message families,
     * out-of-range indices and nonzero reserved fields are rejected.
     */
    static FOUNDATION_CONSTEXPR14 Message FromDataWords(uint32_t header, uint32_t data) noexcept {
        if ((header & 0xFF000000u) != 0) return Message();
        const Protocol::VoiceStatus status = static_cast<Protocol::VoiceStatus>((header >> 20) & 15u);
        switch (status) {
        case Protocol::VoiceStatus::NoteOn:
        case Protocol::VoiceStatus::NoteOff:
            if ((header & 0x8000u) != 0 || ((header & 255u) == 0 && (data & 0xFFFFu) != 0)) return Message();
            break;
        case Protocol::VoiceStatus::PolyPressure:
        case Protocol::VoiceStatus::ControlChange:
            if ((header & 0x80FFu) != 0) return Message();
            break;
        case Protocol::VoiceStatus::ProgramChange:
            if ((header & 0xFFFEu) != 0 || (data & 0x80FF8080u) != 0 ||
                ((header & 1u) == 0 && (data & 0xFFFFu) != 0)) return Message();
            break;
        case Protocol::VoiceStatus::ChannelPressure:
        case Protocol::VoiceStatus::PitchBendChange:
            if ((header & 0xFFFFu) != 0) return Message();
            break;
        default: return Message();
        }
        return Message(header, data);
    }
    /** @brief Compares exact native fields, including optional note attributes. */
    friend constexpr bool operator==(const Message& a, const Message& b) noexcept {
        return a._header == b._header && a._data == b._data;
    }
    /** @brief Negation of equality. */
    friend constexpr bool operator!=(const Message& a, const Message& b) noexcept { return !(a == b); }
};
}
#endif
