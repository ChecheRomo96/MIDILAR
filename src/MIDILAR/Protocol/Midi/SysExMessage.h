#ifndef MIDILAR_PROTOCOL_MIDI_SYSEX_MESSAGE_H
#define MIDILAR_PROTOCOL_MIDI_SYSEX_MESSAGE_H
#include <MIDILAR_BuildSettings.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../TimeCode.h"

namespace MIDILAR::Protocol::Midi {

/**
 * @brief Non-owning complete F0..F7 message in caller storage (SPEC-M1-3).
 * @ingroup MIDILAR_Midi
 *
 * Copies share the same bytes. Keep the caller buffer alive and unchanged
 * while reading the view. Build validates everything before writing, supports
 * overlapping payloads, and never allocates or truncates.
 */
class SysExMessage {
    const uint8_t* _bytes;
    size_t _size;
    constexpr SysExMessage(const uint8_t* bytes, size_t count) noexcept : _bytes(bytes), _size(count) {}
public:
    /** @brief Creates the canonical invalid view. */
    constexpr SysExMessage() noexcept : _bytes(nullptr), _size(0) {}
    /** @brief True for a successfully constructed view. */
    constexpr bool IsValid() const noexcept { return _bytes != nullptr; }
    /** @brief Complete MIDI bytes, including F0 and F7, or nullptr. */
    constexpr const uint8_t* Buffer() const noexcept { return _bytes; }
    /** @brief Complete message size, or zero when invalid. */
    constexpr size_t size() const noexcept { return _size; }
    /** @brief Byte at index, or zero outside the view. */
    constexpr uint8_t Data(size_t index) const noexcept { return index < _size ? _bytes[index] : 0; }

    /**
     * @brief Writes F0, payload and F7; payload excludes both delimiters.
     * A null payload is allowed only for an empty payload. The output buffer
     * must hold payloadSize + 2 bytes. Invalid input leaves it untouched.
     */
    static SysExMessage Build(uint8_t* buffer, size_t capacity,
                              const uint8_t* payload, size_t payloadSize) noexcept {
        if (buffer == nullptr || capacity < 2 || payloadSize > capacity - 2 ||
            (payload == nullptr && payloadSize != 0)) return SysExMessage();
        for (size_t i = 0; i < payloadSize; ++i)
            if (payload[i] > 127) return SysExMessage();
        if (payloadSize != 0) memmove(buffer + 1, payload, payloadSize);
        buffer[0] = 0xF0;
        buffer[payloadSize + 1] = 0xF7;
        return SysExMessage(buffer, payloadSize + 2);
    }

    /** @brief Universal real-time MTC Full Frame; needs ten output bytes. */
    static SysExMessage MTC_FullFrame(uint8_t* buffer, size_t capacity,
                                     const Protocol::TimeCode& time, uint8_t device = 0x7F) noexcept {
        if (!time.IsValid() || device > 127) return SysExMessage();
        const uint8_t payload[8] = {
            0x7F, device, 0x01, 0x01,
            static_cast<uint8_t>((static_cast<uint8_t>(time.Rate()) << 5) | time.Hours()),
            time.Minutes(), time.Seconds(), time.Frames()
        };
        return Build(buffer, capacity, payload, sizeof(payload));
    }
};
}
#endif
