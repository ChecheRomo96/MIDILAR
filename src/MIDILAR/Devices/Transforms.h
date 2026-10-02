#ifndef MIDILAR_DEVICES_TRANSFORMS_H
#define MIDILAR_DEVICES_TRANSFORMS_H

#include <stdint.h>

#include <Foundation/Functional/Callback.h>
#include <MCC/Chord/ChordPattern.h>
#include <MIDILAR/Protocol/Channel.h>
#include <MIDILAR/Protocol/Packet.h>
#include <MIDILAR/Protocol/Values.h>

#include "Device.h"

namespace MIDILAR::Devices {

/**
 * @brief Moves channel voice packets from the source channels to one target
 * channel, and optionally to one group.
 * @ingroup MIDILAR_Devices
 *
 * Packets outside `Sources()` (default: all) and packets without a channel
 * pass unchanged. Until a target is set, everything passes unchanged.
 */
class ChannelReassign : public Device {
public:
    /** @brief Selects the channels that are moved. */
    void SetSources(Protocol::ChannelMask sources) noexcept { _sources = sources; }

    /** @brief Sets the target channel; the invalid channel disables the move. */
    void SetTarget(Protocol::Channel channel) noexcept { _channel = channel; }

    /** @brief Sets the target group; the invalid group keeps each packet's group. */
    void SetTargetGroup(Protocol::Group group) noexcept { _group = group; }

    /** @brief Emits `packet`, moved when its channel is a source. */
    void Process(const Protocol::Packet& packet) const {
        if (!_channel.IsValid() || !packet.IsChannelVoice() || !_sources.Contains(packet.Channel())) {
            Emit(packet);
            return;
        }
        const Protocol::Packet moved = packet.WithChannel(_channel);
        Emit(_group.IsValid() ? moved.WithGroup(_group) : moved);
    }

private:
    Protocol::ChannelMask _sources = Protocol::ChannelMask::All();
    Protocol::Channel _channel;
    Protocol::Group _group;
};

/**
 * @brief Shifts every packet with a note by a number of semitones.
 * @ingroup MIDILAR_Devices
 *
 * Notes that would leave `[0, 127]` are dropped. Held notes keep the shift
 * they started with until their Note Off (`HeldNotes`).
 */
class Transpose : public Device {
public:
    /** @brief Sets the shift, clamped to `[-127, 127]` semitones. */
    void SetSemitones(int32_t semitones) noexcept {
        _semitones = static_cast<int8_t>(semitones < -127 ? -127 : (semitones > 127 ? 127 : semitones));
    }

    /** @brief Returns the shift in semitones. */
    int32_t Semitones() const noexcept { return _semitones; }

    /** @brief Emits `packet` transposed, or drops it when out of range. */
    void Process(const Protocol::Packet& packet) {
        const Protocol::NoteNumber note = packet.Note();
        if (!note.IsValid()) {
            Emit(packet);
            return;
        }
        const Protocol::NoteNumber shifted = Protocol::NoteNumber::FromValue(
            _held.Route(packet, Protocol::NoteNumber::FromValue(note.Value() + _semitones).Value()));
        if (shifted.IsValid()) {
            Emit(packet.WithNote(shifted));
        }
    }

private:
    HeldNotes _held;
    int8_t _semitones = 0;
};

/** @brief Maps one velocity to another. @ingroup MIDILAR_Devices */
using VelocityMap = Foundation::Functional::Callback<Protocol::Velocity, Protocol::Velocity>;

/**
 * @brief Reshapes Note On velocities with a caller-provided map.
 * @ingroup MIDILAR_Devices
 *
 * Works at 16-bit resolution in both protocols. A MIDI 1.0 Note On never
 * ends with velocity 0 (SPEC-MIDI-18). Note Off and other packets pass
 * unchanged, and so does everything while the map is unbound.
 */
class VelocityCurve : public Device {
public:
    /** @brief Returns the map to bind. */
    VelocityMap& Map() noexcept { return _map; }

    /** @brief Emits `packet` with its Note On velocity mapped. */
    void Process(const Protocol::Packet& packet) const {
        if (!_map.IsBound() || !packet.IsChannelVoice() ||
            packet.VoiceStatus() != Protocol::VoiceStatus::NoteOn) {
            Emit(packet);
            return;
        }
        const Protocol::Velocity velocity = _map.Invoke(packet.Velocity());
        if (packet.Type() == Protocol::MessageType::Midi1ChannelVoice) {
            Emit(Protocol::Packet::Midi1NoteOn(packet.Group(), packet.Channel(), packet.Note(), velocity));
        } else {
            Emit(Protocol::Packet::Midi2NoteOn(packet.Group(), packet.Channel(), packet.Note(), velocity,
                                               packet.AttributeType(), packet.AttributeData()));
        }
    }

private:
    VelocityMap _map;
};

/**
 * @brief Turns every note into a chord built on it from an
 * `MCC::ChordPattern`.
 * @ingroup MIDILAR_Devices
 *
 * Each packet with a note is emitted once per chord tone, with the same
 * velocity and data; tones outside `[0, 127]` are skipped. Held notes keep
 * the chord they started with until their Note Off (SPEC-DEV-3). Two held
 * chords that share a tone share its output note, so the first Note Off ends
 * it. Until a chord is set, packets pass unchanged.
 */
class ChordGenerator : public Device {
public:
    /** @brief Sets the chord; an invalid pattern plays only the note itself. */
    void SetChord(const MCC::ChordPattern& chord) noexcept {
        _shape = 1;
        if (!chord.IsValid()) {
            return;
        }
        _shape = 0;
        for (int32_t tone = 1; tone <= chord.ToneCount(); ++tone) {
            const int32_t semitones = chord.ToneInterval(tone).Semitones();
            if (semitones >= 0 && semitones < 32) {
                _shape |= static_cast<uint32_t>(1) << semitones;
            }
        }
    }

    /** @brief Returns the chord as a mask of semitones above the note (bit 0: the note). */
    uint32_t Shape() const noexcept { return _shape; }

    /** @brief Emits one packet per chord tone of `packet`'s note. */
    void Process(const Protocol::Packet& packet) {
        const Protocol::NoteNumber note = packet.Note();
        if (!note.IsValid()) {
            Emit(packet);
            return;
        }
        const uint32_t shape = _held.Route(packet, _shape);
        for (uint8_t semitones = 0; semitones < 32; ++semitones) {
            if (((shape >> semitones) & 1u) != 0) {
                const Protocol::NoteNumber tone = Protocol::NoteNumber::FromValue(note.Value() + semitones);
                if (tone.IsValid()) {
                    Emit(packet.WithNote(tone));
                }
            }
        }
    }

private:
    HeldValues<uint32_t> _held;
    uint32_t _shape = 1;
};

} // namespace MIDILAR::Devices

#endif // MIDILAR_DEVICES_TRANSFORMS_H
