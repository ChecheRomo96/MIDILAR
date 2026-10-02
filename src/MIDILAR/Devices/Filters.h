#ifndef MIDILAR_DEVICES_FILTERS_H
#define MIDILAR_DEVICES_FILTERS_H

#include <stdint.h>

#include <MCC/Scale/Scale.h>
#include <MIDILAR/Protocol/Channel.h>
#include <MIDILAR/Protocol/NotePitch.h>
#include <MIDILAR/Protocol/Packet.h>

#include "Device.h"

namespace MIDILAR::Devices {

/**
 * @brief Passes packets on the selected groups and channels.
 * @ingroup MIDILAR_Devices
 *
 * Packets with a group pass only when it is in `Groups()`; channel voice
 * packets also need their channel in `Channels()`. Packets without a group
 * (utility, stream) always pass. Both masks start as `All()`.
 */
class ChannelFilter : public Device {
public:
    /** @brief Selects the groups that pass. */
    void SetGroups(Protocol::GroupMask groups) noexcept { _groups = groups; }

    /** @brief Selects the channels that pass. */
    void SetChannels(Protocol::ChannelMask channels) noexcept { _channels = channels; }

    /** @brief Returns the groups that pass. */
    Protocol::GroupMask Groups() const noexcept { return _groups; }

    /** @brief Returns the channels that pass. */
    Protocol::ChannelMask Channels() const noexcept { return _channels; }

    /** @brief Emits `packet` when it passes the masks. */
    void Process(const Protocol::Packet& packet) const {
        const Protocol::Group group = packet.Group();
        if (group.IsValid() && !_groups.Contains(group)) {
            return;
        }
        if (packet.IsChannelVoice() && !_channels.Contains(packet.Channel())) {
            return;
        }
        Emit(packet);
    }

private:
    Protocol::GroupMask _groups = Protocol::GroupMask::All();
    Protocol::ChannelMask _channels = Protocol::ChannelMask::All();
};

/** @brief What `ScaleFilter` does with a note outside the scale. @ingroup MIDILAR_Devices */
enum class ScaleFilterMode : uint8_t {
    Drop,   ///< Drop the packet.
    Down,   ///< Move to the nearest scale note below.
    Up,     ///< Move to the nearest scale note above.
    Nearest ///< Move to the nearest scale note; ties go down.
};

/**
 * @brief Keeps notes on an `MCC::Scale`, compared by pitch class
 * (`MCC::Scale::ContainsPitchClass`).
 * @ingroup MIDILAR_Devices
 *
 * Applies to every packet with a note (`Packet::HasNote()`); other packets
 * pass unchanged, and so does everything while the scale is invalid. A note
 * that would move outside `[0, 127]` is dropped. Held notes keep the mapping
 * they started with until their Note Off (`HeldNotes`).
 */
class ScaleFilter : public Device {
public:
    /** @brief Sets the scale; an invalid scale disables the filter. */
    void SetScale(const MCC::Scale& scale) noexcept { _scale = scale; }

    /** @brief Sets what happens to notes outside the scale (default: `Nearest`). */
    void SetMode(ScaleFilterMode mode) noexcept { _mode = mode; }

    /** @brief Returns the scale. */
    const MCC::Scale& Scale() const noexcept { return _scale; }

    /** @brief Emits `packet` with its note kept on the scale, or drops it. */
    void Process(const Protocol::Packet& packet) {
        const Protocol::NoteNumber note = packet.Note();
        if (!note.IsValid()) {
            Emit(packet);
            return;
        }
        const Protocol::NoteNumber mapped = _held.Route(packet, Map(note));
        if (mapped.IsValid()) {
            Emit(packet.WithNote(mapped));
        }
    }

    /** @brief Returns the scale note `note` maps to, or the invalid note when dropped. */
    Protocol::NoteNumber Map(Protocol::NoteNumber note) const noexcept {
        if (!note.IsValid() || !_scale.IsValid() || InScale(note.Value())) {
            return note;
        }
        if (_mode == ScaleFilterMode::Drop) {
            return Protocol::NoteNumber::Invalid();
        }
        for (int32_t distance = 1; distance < 12; ++distance) {
            const int32_t below = note.Value() - distance;
            const int32_t above = note.Value() + distance;
            if (_mode != ScaleFilterMode::Up && InScale(below)) {
                return Protocol::NoteNumber::FromValue(below);
            }
            if (_mode != ScaleFilterMode::Down && InScale(above)) {
                return Protocol::NoteNumber::FromValue(above);
            }
        }
        return Protocol::NoteNumber::Invalid();
    }

private:
    bool InScale(int32_t value) const noexcept {
        const Protocol::NoteNumber note = Protocol::NoteNumber::FromValue(value);
        return note.IsValid() && _scale.ContainsPitchClass(Protocol::ToChromaticIndex(note).PitchClass());
    }

    HeldNotes _held;
    MCC::Scale _scale;
    ScaleFilterMode _mode = ScaleFilterMode::Nearest;
};

} // namespace MIDILAR::Devices

#endif // MIDILAR_DEVICES_FILTERS_H
