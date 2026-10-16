#ifndef MIDILAR_PROTOCOL_MIDI1_MESSAGE_H
#define MIDILAR_PROTOCOL_MIDI1_MESSAGE_H

#include <MIDILAR_BuildSettings.h>
#include <stddef.h>
#include <stdint.h>
#include "../Packet.h"

namespace MIDILAR::Protocol::Midi1 {

/** @brief MIDI 1.0 short message family or system status. @ingroup MIDILAR_Midi1 */
enum class MessageType : uint8_t {
    Invalid = 0, NoteOff = 0x80, NoteOn = 0x90, AfterTouch = 0xA0,
    ControlChange = 0xB0, ProgramChange = 0xC0, ChannelPressure = 0xD0,
    PitchBend = 0xE0, QuarterFrame = 0xF1, SongPosition = 0xF2,
    SongSelect = 0xF3, TuneRequest = 0xF6, TimingClock = 0xF8,
    Start = 0xFA, Continue = 0xFB, Stop = 0xFC, ActiveSensing = 0xFE, Reset = 0xFF
};

/**
 * @brief Complete short MIDI 1.0 message in three inline bytes (SPEC-M1-1..4).
 * @ingroup MIDILAR_Midi1
 *
 * Reintroduces the useful builder surface of the historical MidiCore::Message
 * without dynamic storage or exceptions. Not a drop-in replacement: SysEx
 * uses SysExMessage, invalid arguments clear the message instead of clamping,
 * and pitch-bend resolutions have distinct names.
 *
 * Builder channels are wire indices 0-15 (0 is human channel 1). For explicit
 * numbering use Channel::FromNumber(n).Wire(). Buffer() always includes status:
 * running status belongs to a transport encoder, not this complete message.
 *
 * @code{.cpp}
 * MIDILAR::Protocol::Midi1::Message msg;
 * msg.NoteOn(60, 100, 0); // 90 3C 64
 * const uint8_t* bytes = msg.Buffer(); // valid until msg changes or is destroyed
 * size_t count = msg.size();          // 3
 * @endcode
 */
class Message {
    uint8_t _bytes[3];

    static constexpr uint8_t Length(uint8_t status) noexcept {
        return status >= 0x80 && status < 0xF0
            ? ((status & 0xF0u) == 0xC0 || (status & 0xF0u) == 0xD0 ? 2 : 3)
            : status == 0xF2 ? 3
            : status == 0xF1 || status == 0xF3 ? 2
            : status == 0xF6 || status == 0xF8 || status == 0xFA ||
              status == 0xFB || status == 0xFC || status == 0xFE || status == 0xFF ? 1 : 0;
    }

    Message& Set(uint8_t status, uint8_t first = 0, uint8_t second = 0) noexcept {
        _bytes[0] = status;
        _bytes[1] = first;
        _bytes[2] = second;
        return *this;
    }

    Message& Voice(uint8_t status, int32_t first, int32_t second, int32_t channel) noexcept {
        if (channel < 0 || channel > 15 || first < 0 || first > 127 || second < 0 || second > 127)
            return Clear();
        return Set(static_cast<uint8_t>(status | channel),
                   static_cast<uint8_t>(first), static_cast<uint8_t>(second));
    }

public:
    /** @brief Creates the canonical invalid/empty message. */
    constexpr Message() noexcept : _bytes{0, 0, 0} {}
    /** @brief Clears all bytes and returns this message. */
    Message& Clear() noexcept { return Set(0); }
    /** @brief True for a complete short message. */
    constexpr bool IsValid() const noexcept { return Length(_bytes[0]) != 0; }
    /** @brief Number of complete MIDI bytes, 0-3, including status. */
    constexpr size_t size() const noexcept { return Length(_bytes[0]); }
    /** @brief Inline bytes or nullptr when invalid; invalidated by mutation. */
    const uint8_t* Buffer() const noexcept { return IsValid() ? _bytes : nullptr; }
    /** @brief Byte at index, or zero outside size(). */
    constexpr uint8_t Data(size_t index) const noexcept { return index < size() ? _bytes[index] : 0; }
    /** @brief Full status byte, or zero when invalid. */
    constexpr uint8_t Status() const noexcept { return _bytes[0]; }
    /** @brief Channel family (without channel bits), or exact system status. */
    constexpr MessageType Type() const noexcept {
        return static_cast<MessageType>(_bytes[0] < 0xF0 ? (_bytes[0] & 0xF0u) : _bytes[0]);
    }
    /** @brief Wire channel wrapped as Channel, or invalid for system/empty messages. */
    constexpr Protocol::Channel Channel() const noexcept {
        return _bytes[0] >= 0x80 && _bytes[0] < 0xF0
            ? Protocol::Channel::FromWire(_bytes[0] & 0x0Fu) : Protocol::Channel::Invalid();
    }

    /**
     * @brief Copies exactly one validated complete message, including status.
     * Rejects undefined status, SysEx, running-status fragments, incorrect
     * lengths and non-7-bit data. Overlapping/self input is supported.
     */
    Message& SetRawData(const uint8_t* bytes, size_t count) noexcept {
        if (bytes == nullptr || count == 0 || count > 3 || Length(bytes[0]) != count)
            return Clear();
        const uint8_t first = count > 1 ? bytes[1] : 0;
        const uint8_t second = count > 2 ? bytes[2] : 0;
        if (first > 127 || second > 127) return Clear();
        return Set(bytes[0], first, second);
    }

    /** @brief Note On, preserving velocity zero as exact MIDI 1.0 bytes. */
    Message& NoteOn(int32_t pitch, int32_t velocity, int32_t channel = 0) noexcept {
        return Voice(0x90, pitch, velocity, channel);
    }
    /** @brief Note Off with optional release velocity. */
    Message& NoteOff(int32_t pitch, int32_t velocity = 0, int32_t channel = 0) noexcept {
        return Voice(0x80, pitch, velocity, channel);
    }
    /** @brief Polyphonic key pressure. */
    Message& AfterTouch(int32_t pitch, int32_t pressure, int32_t channel = 0) noexcept {
        return Voice(0xA0, pitch, pressure, channel);
    }
    /** @brief Control Change, including channel modes and RPN/NRPN components. */
    Message& ControlChange(int32_t controller, int32_t value, int32_t channel = 0) noexcept {
        return Voice(0xB0, controller, value, channel);
    }
    /** @brief Program index 0-127; produces two bytes. */
    Message& ProgramChange(int32_t program, int32_t channel = 0) noexcept {
        return Voice(0xC0, program, 0, channel);
    }
    /** @brief Channel pressure, 0-127; produces two bytes. */
    Message& ChannelPressure(int32_t pressure, int32_t channel = 0) noexcept {
        return Voice(0xD0, pressure, 0, channel);
    }
    /** @brief Unsigned 14-bit bend, 0-16383, with center 8192. */
    Message& PitchBend(int32_t value, int32_t channel = 0) noexcept {
        if (value < 0 || value > 16383) return Clear();
        return Voice(0xE0, value & 127, value >> 7, channel);
    }
    /** @brief Signed bend, -8192..8191, with center 0. */
    Message& PitchBendSigned(int32_t value, int32_t channel = 0) noexcept {
        return value < -8192 || value > 8191 ? Clear() : PitchBend(value + 8192, channel);
    }
    /** @brief 7-bit bend preserving minimum, center 64 and maximum 127. */
    Message& PitchBend7(int32_t value, int32_t channel = 0) noexcept {
        return value < 0 || value > 127 ? Clear()
            : PitchBend(static_cast<int32_t>(ScaleUp(static_cast<uint32_t>(value), 7, 14)), channel);
    }

    /** @brief Bank Select MSB (CC 0); use CC 32 for the LSB. */
    Message& CC_BankSelect(int32_t value, int32_t channel = 0) noexcept { return ControlChange(0, value, channel); }
    /** @brief Modulation wheel (CC 1). */
    Message& CC_Modulation(int32_t value, int32_t channel = 0) noexcept { return ControlChange(1, value, channel); }
    /** @brief Breath control (CC 2). */
    Message& CC_BreathControl(int32_t value, int32_t channel = 0) noexcept { return ControlChange(2, value, channel); }
    /** @brief Foot controller (CC 4). */
    Message& CC_FootPedal(int32_t value, int32_t channel = 0) noexcept { return ControlChange(4, value, channel); }
    /** @brief Portamento time (CC 5), not portamento on/off (CC 65). */
    Message& CC_Portamento(int32_t value, int32_t channel = 0) noexcept { return ControlChange(5, value, channel); }
    /** @brief Channel volume (CC 7). */
    Message& CC_Volume(int32_t value, int32_t channel = 0) noexcept { return ControlChange(7, value, channel); }
    /** @brief Balance (CC 8). */
    Message& CC_Balance(int32_t value, int32_t channel = 0) noexcept { return ControlChange(8, value, channel); }
    /** @brief Pan (CC 10). */
    Message& CC_Pan(int32_t value, int32_t channel = 0) noexcept { return ControlChange(10, value, channel); }
    /** @brief Expression (CC 11). */
    Message& CC_Expression(int32_t value, int32_t channel = 0) noexcept { return ControlChange(11, value, channel); }
    /** @brief All Sound Off (CC 120). */
    Message& CC_AllSoundOff(int32_t channel = 0) noexcept { return ControlChange(120, 0, channel); }
    /** @brief Reset All Controllers (CC 121). */
    Message& CC_ResetAllControllers(int32_t channel = 0) noexcept { return ControlChange(121, 0, channel); }
    /** @brief All Notes Off (CC 123). */
    Message& CC_AllNotesOff(int32_t channel = 0) noexcept { return ControlChange(123, 0, channel); }
    /** @brief Local Control off/on (CC 122, value 0/127). */
    Message& CC_LocalControl(bool enabled, int32_t channel = 0) noexcept {
        return ControlChange(122, enabled ? 127 : 0, channel);
    }
    /** @brief Enables local keyboard control. */
    Message& CC_LocalControlOn(int32_t channel = 0) noexcept { return CC_LocalControl(true, channel); }
    /** @brief Disables local keyboard control. */
    Message& CC_LocalControlOff(int32_t channel = 0) noexcept { return CC_LocalControl(false, channel); }
    /** @brief Omni off/on (CC 124/125). */
    Message& CC_OmniMode(bool enabled, int32_t channel = 0) noexcept { return ControlChange(enabled ? 125 : 124, 0, channel); }
    /** @brief Enables Omni reception. */
    Message& CC_OmniOn(int32_t channel = 0) noexcept { return CC_OmniMode(true, channel); }
    /** @brief Disables Omni reception. */
    Message& CC_OmniOff(int32_t channel = 0) noexcept { return CC_OmniMode(false, channel); }
    /** @brief Mono mode (CC 126); channel count 0-16, zero is receiver-defined. */
    Message& CC_Mono(int32_t channel = 0, int32_t count = 0) noexcept {
        return count < 0 || count > 16 ? Clear() : ControlChange(126, count, channel);
    }
    /** @brief Poly mode (CC 127). */
    Message& CC_Poly(int32_t channel = 0) noexcept { return ControlChange(127, 0, channel); }
    /** @brief Selects polyphonic (true) or monophonic (false) mode. */
    Message& CC_Polyphony(bool polyphonic, int32_t channel = 0) noexcept {
        return polyphonic ? CC_Poly(channel) : CC_Mono(channel);
    }

    /** @brief One timing-clock pulse (24 pulses per quarter note). */
    Message& TimingTick() noexcept { return Set(0xF8); }
    /** @brief Start transport from the beginning. */
    Message& Start() noexcept { return Set(0xFA); }
    /** @brief Resume transport from the current position. */
    Message& Continue() noexcept { return Set(0xFB); }
    /** @brief Stop transport. */
    Message& Stop() noexcept { return Set(0xFC); }
    /** @brief Active Sensing keepalive. */
    Message& ActiveSensing() noexcept { return Set(0xFE); }
    /** @brief System Reset. */
    Message& SystemReset() noexcept { return Set(0xFF); }
    /** @brief Song position in units of six MIDI clocks, 0-16383. */
    Message& SongPositionPointer(int32_t position) noexcept {
        return position < 0 || position > 16383 ? Clear()
            : Set(0xF2, static_cast<uint8_t>(position & 127), static_cast<uint8_t>(position >> 7));
    }
    /** @brief Song index 0-127. */
    Message& SongSelect(int32_t song) noexcept {
        return song < 0 || song > 127 ? Clear() : Set(0xF3, static_cast<uint8_t>(song));
    }
    /** @brief Tune Request. */
    Message& TuningRequest() noexcept { return Set(0xF6); }
    /** @brief MTC piece index 0-7 and data nibble 0-15. */
    Message& MTC_QuarterFrame(int32_t piece, int32_t nibble) noexcept {
        return piece < 0 || piece > 7 || nibble < 0 || nibble > 15 ? Clear()
            : Set(0xF1, static_cast<uint8_t>((piece << 4) | nibble));
    }

    /** @brief Explicit UMP conversion; normalizes velocity-zero Note On. */
    Protocol::Packet ToPacket(Protocol::Group group) const noexcept {
        if (!IsValid() || !group.IsValid()) return Protocol::Packet();
        const uint8_t status = Type() == MessageType::NoteOn && _bytes[2] == 0
            ? static_cast<uint8_t>(0x80u | (_bytes[0] & 15u)) : _bytes[0];
        const uint32_t type = status < 0xF0 ? 2u : 1u;
        return Protocol::Packet::FromWords((type << 28) |
            (static_cast<uint32_t>(group.Wire()) << 24) |
            (static_cast<uint32_t>(status) << 16) |
            (static_cast<uint32_t>(_bytes[1]) << 8) | _bytes[2]);
    }

    /** @brief Converts valid short MIDI 1.0 UMP to bytes, discarding its group. */
    static Message FromPacket(const Protocol::Packet& packet) noexcept {
        Message result;
        if (!packet.IsValid() || (packet.Type() != Protocol::MessageType::Midi1ChannelVoice &&
                                 packet.Type() != Protocol::MessageType::System)) return result;
        const uint8_t bytes[3] = {
            static_cast<uint8_t>(packet.Word(0) >> 16),
            static_cast<uint8_t>(packet.Word(0) >> 8),
            static_cast<uint8_t>(packet.Word(0))
        };
        if ((packet.Type() == Protocol::MessageType::System) != (bytes[0] >= 0xF0)) return result;
        const uint8_t count = Length(bytes[0]);
        if (count == 0 || (count < 3 && bytes[2] != 0) || (count < 2 && bytes[1] != 0)) return result;
        return result.SetRawData(bytes, count);
    }
};

} // namespace MIDILAR::Protocol::Midi1
#endif
