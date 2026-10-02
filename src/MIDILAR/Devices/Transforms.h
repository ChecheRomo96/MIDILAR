#ifndef MIDILAR_DEVICES_TRANSFORMS_H
#define MIDILAR_DEVICES_TRANSFORMS_H

#include <stdint.h>

#include <Foundation/Functional/Callback.h>
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
 * Notes that would leave `[0, 127]` are dropped. Changing the amount while
 * notes are held can leave their Note Off on another note.
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
    void Process(const Protocol::Packet& packet) const {
        const Protocol::NoteNumber note = packet.Note();
        if (!note.IsValid() || _semitones == 0) {
            Emit(packet);
            return;
        }
        const Protocol::NoteNumber shifted = Protocol::NoteNumber::FromValue(note.Value() + _semitones);
        if (shifted.IsValid()) {
            Emit(packet.WithNote(shifted));
        }
    }

private:
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

} // namespace MIDILAR::Devices

#endif // MIDILAR_DEVICES_TRANSFORMS_H
