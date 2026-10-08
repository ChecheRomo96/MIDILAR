#ifndef MIDILAR_DEVICES_DEVICE_H
#define MIDILAR_DEVICES_DEVICE_H

#include <stdint.h>

#include <Foundation/Functional/Callback.h>
#include <MIDILAR/Protocol/Packet.h>

namespace MIDILAR::Devices {

/**
 * @brief Receives one UMP packet; devices are chained by binding an output
 * to the next device's `Process()`.
 * @ingroup MIDILAR_Devices
 *
 * @code
 * transpose.Output().Bind<MIDILAR::Devices::ScaleFilter, &MIDILAR::Devices::ScaleFilter::Process>(&scale);
 * @endcode
 */
using PacketCallback = Foundation::Functional::Callback<void, const Protocol::Packet&>;

/**
 * @brief Base of the devices: one output and no virtual functions
 * (SPEC-DEV-1).
 * @ingroup MIDILAR_Devices
 *
 * Each device has `void Process(const Protocol::Packet&)` and emits zero or
 * more packets to `Output()`. An unbound output drops packets.
 */
class Device {
public:
    /** @brief Returns the output to bind to the next device or sink. */
    PacketCallback& Output() noexcept { return _output; }

protected:
    /** @brief Sends `packet` to the output, if bound and valid. */
    void Emit(const Protocol::Packet& packet) const {
        if (packet.IsValid() && _output.IsBound()) {
            _output.Invoke(packet);
        }
    }

private:
    PacketCallback _output;
};

/**
 * @brief Remembers what a device did with each held note, so a device whose
 * mapping changes while notes sound still releases them (SPEC-DEV-3).
 * @ingroup MIDILAR_Devices
 *
 * `Route()` is called with every packet that has a note and the value the
 * device would use now (an output note, a chord shape...). A Note On (MIDI
 * 1.0 velocity 0 excluded) stores that value; a Note Off returns and forgets
 * the stored one; per-note packets return the stored one. Up to `Capacity`
 * notes are held per device; beyond that, notes use the current value.
 */
template <typename Value>
class HeldValues {
public:
    /** @brief Number of notes remembered at once. */
    static constexpr uint8_t Capacity = 16;

    /** @brief Returns the value to use for `packet`: stored, or `current`. */
    Value Route(const Protocol::Packet& packet, Value current) noexcept {
        const uint8_t address = static_cast<uint8_t>((packet.Group().Wire() << 4) | packet.Channel().Wire());
        const uint8_t note = packet.Note().Value();
        uint8_t index = 0;
        while (index < _count && (_entries[index].address != address || _entries[index].note != note)) {
            ++index;
        }
        const bool found = index < _count;
        const Protocol::VoiceStatus status = packet.VoiceStatus();
        const bool noteOn = status == Protocol::VoiceStatus::NoteOn &&
            !(packet.Type() == Protocol::MessageType::Midi1ChannelVoice && (packet.Word(0) & 0x7Fu) == 0);
        const bool noteOff = status == Protocol::VoiceStatus::NoteOff ||
            (status == Protocol::VoiceStatus::NoteOn && !noteOn);

        if (noteOn) {
            if (!found && _count < Capacity) {
                index = _count++;
            }
            if (index < _count) {
                _entries[index] = Entry{address, note, current};
            }
            return current;
        }
        if (!found) {
            return current;
        }
        const Value stored = _entries[index].value;
        if (noteOff) {
            _entries[index] = _entries[--_count];
        }
        return stored;
    }

    /** @brief Forgets every held note. */
    void Clear() noexcept { _count = 0; }

    /** @brief Returns the number of notes held. */
    uint8_t Count() const noexcept { return _count; }

private:
    struct Entry {
        uint8_t address;
        uint8_t note;
        Value value;
    };

    Entry _entries[Capacity] = {};
    uint8_t _count = 0;
};

/** @brief Output note per held note; `0xFF` is a dropped note. @ingroup MIDILAR_Devices */
using HeldNotes = HeldValues<uint8_t>;

/**
 * @brief Copies every packet to up to `Outputs` sinks, in order.
 * @ingroup MIDILAR_Devices
 */
template <uint8_t Outputs>
class Router {
    static_assert(Outputs > 0, "a router needs at least one output");

public:
    /** @brief Returns output `index`, or `nullptr` when out of range. */
    PacketCallback* Output(uint8_t index) noexcept { return index < Outputs ? &_outputs[index] : nullptr; }

    /** @brief Sends `packet` to every bound output. */
    void Process(const Protocol::Packet& packet) const {
        if (!packet.IsValid()) {
            return;
        }
        for (uint8_t i = 0; i < Outputs; ++i) {
            if (_outputs[i].IsBound()) {
                _outputs[i].Invoke(packet);
            }
        }
    }

private:
    PacketCallback _outputs[Outputs];
};

} // namespace MIDILAR::Devices

#endif // MIDILAR_DEVICES_DEVICE_H
