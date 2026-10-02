#ifndef MIDILAR_PROTOCOL_MIDI1_ENCODER_H
#define MIDILAR_PROTOCOL_MIDI1_ENCODER_H

#include <stdint.h>

#include "Packet.h"

namespace MIDILAR::Protocol {

/**
 * @brief Translates UMP packets into a MIDI 1.0 byte stream (SPEC-MIDI-2,
 * SPEC-MIDI-10).
 * @ingroup MIDILAR_Protocol
 *
 * Encodes system, MIDI 1.0 channel voice and 7-bit System Exclusive packets,
 * and MIDI 2.0 channel voice packets whose status exists in MIDI 1.0 (values
 * scaled down; a banked program change becomes bank select CC 0 and CC 32
 * followed by the program change). The group is dropped. Other packets
 * (utility, per-note and registered/assignable controllers, 8-bit System
 * Exclusive, flex data, stream) produce no bytes.
 *
 * Full status bytes are written by default; `SetRunningStatus(true)` omits
 * a channel status equal to the previous one. System common and System
 * Exclusive clear running status; real-time messages do not.
 */
class Midi1Encoder {
public:
    /** @brief Most bytes one packet can produce. */
    static constexpr uint8_t MaxBytes = 8;

    /** @brief Enables or disables running status on output (default: off). */
    void SetRunningStatus(bool enabled) noexcept {
        _useRunningStatus = enabled;
        _runningStatus = 0;
    }

    /** @brief Forgets the running status, e.g. after the link was reset. */
    void Reset() noexcept { _runningStatus = 0; }

    /**
     * @brief Writes the bytes of `packet` to `out`.
     * @return The number of bytes written, `0-8`.
     */
    uint8_t Encode(const Packet& packet, uint8_t (&out)[MaxBytes]) noexcept {
        if (!packet.IsValid()) {
            return 0;
        }
        switch (packet.Type()) {
        case MessageType::System:
            return EncodeSystem(packet, out);
        case MessageType::Midi1ChannelVoice:
            return EncodeVoice(packet.Word(0), out, 0);
        case MessageType::Midi2ChannelVoice:
            return EncodeMidi2(packet, out);
        case MessageType::Data64:
            return EncodeSysEx(packet, out);
        default:
            return 0;
        }
    }

private:
    uint8_t EncodeSystem(const Packet& packet, uint8_t (&out)[MaxBytes]) noexcept {
        const uint8_t status = static_cast<uint8_t>(packet.SystemStatus());
        if (status < 0xF1 || status == 0xF4 || status == 0xF5 || status == 0xF7 || status == 0xF9 ||
            status == 0xFD) {
            return 0;
        }
        out[0] = status;
        if (status >= 0xF8) {
            return 1;
        }
        _runningStatus = 0;
        if (status == 0xF6) {
            return 1;
        }
        out[1] = static_cast<uint8_t>((packet.Word(0) >> 8) & 0x7Fu);
        if (status != 0xF2) {
            return 2;
        }
        out[2] = static_cast<uint8_t>(packet.Word(0) & 0x7Fu);
        return 3;
    }

    uint8_t EncodeVoice(uint32_t word, uint8_t (&out)[MaxBytes], uint8_t count) noexcept {
        const uint8_t status = static_cast<uint8_t>((word >> 16) & 0xFFu);
        if (status < 0x80 || status >= 0xF0) {
            return count;
        }
        if (!_useRunningStatus || status != _runningStatus) {
            out[count++] = status;
        }
        _runningStatus = status;
        out[count++] = static_cast<uint8_t>((word >> 8) & 0x7Fu);
        if ((status & 0xF0) != 0xC0 && (status & 0xF0) != 0xD0) {
            out[count++] = static_cast<uint8_t>(word & 0x7Fu);
        }
        return count;
    }

    uint8_t EncodeMidi2(const Packet& packet, uint8_t (&out)[MaxBytes]) noexcept {
        const Group group = packet.Group();
        const Channel channel = packet.Channel();
        switch (packet.VoiceStatus()) {
        case VoiceStatus::NoteOn:
            return EncodeVoice(Packet::Midi1NoteOn(group, channel, packet.Note(), packet.Velocity()).Word(0), out, 0);
        case VoiceStatus::NoteOff:
            return EncodeVoice(Packet::Midi1NoteOff(group, channel, packet.Note(), packet.Velocity()).Word(0), out,
                               0);
        case VoiceStatus::PolyPressure:
            return EncodeVoice(Packet::Midi1PolyPressure(group, channel, packet.Note(), packet.Pressure()).Word(0),
                               out, 0);
        case VoiceStatus::ControlChange:
            return EncodeVoice(
                Packet::Midi1ControlChange(group, channel, packet.Controller(), packet.ControllerValue()).Word(0),
                out, 0);
        case VoiceStatus::ChannelPressure:
            return EncodeVoice(Packet::Midi1ChannelPressure(group, channel, packet.Pressure()).Word(0), out, 0);
        case VoiceStatus::PitchBendChange:
            return EncodeVoice(Packet::Midi1PitchBend(group, channel, packet.PitchBend()).Word(0), out, 0);
        case VoiceStatus::ProgramChange: {
            uint8_t count = 0;
            if (packet.HasBank()) {
                count = EncodeVoice(Packet::Midi1ControlChange(group, channel, ControllerNumber::FromValue(0),
                                                               ControllerValue::FromMidi1(packet.Bank() >> 7))
                                        .Word(0),
                                    out, count);
                count = EncodeVoice(Packet::Midi1ControlChange(group, channel, ControllerNumber::FromValue(32),
                                                               ControllerValue::FromMidi1(packet.Bank() & 0x7F))
                                        .Word(0),
                                    out, count);
            }
            return EncodeVoice(Packet::Midi1ProgramChange(group, channel, packet.Program()).Word(0), out, count);
        }
        default:
            return 0;
        }
    }

    uint8_t EncodeSysEx(const Packet& packet, uint8_t (&out)[MaxBytes]) noexcept {
        const SysExStatus status = packet.SysExStatus();
        if (static_cast<uint8_t>(status) > static_cast<uint8_t>(SysExStatus::End) ||
            ((packet.Word(0) >> 16) & 0xFu) > 6) {
            return 0;
        }
        _runningStatus = 0;
        uint8_t count = 0;
        if (status == SysExStatus::Complete || status == SysExStatus::Start) {
            out[count++] = 0xF0;
        }
        for (uint8_t i = 0; i < packet.SysExSize(); ++i) {
            out[count++] = static_cast<uint8_t>(packet.SysExByte(i) & 0x7Fu);
        }
        if (status == SysExStatus::Complete || status == SysExStatus::End) {
            out[count++] = 0xF7;
        }
        return count;
    }

    bool _useRunningStatus = false;
    uint8_t _runningStatus = 0;
};

} // namespace MIDILAR::Protocol

#endif // MIDILAR_PROTOCOL_MIDI1_ENCODER_H
