#ifndef MIDILAR_PROTOCOL_SYSEX_ASSEMBLER_H
#define MIDILAR_PROTOCOL_SYSEX_ASSEMBLER_H

#include <stddef.h>
#include <stdint.h>

#include "Packet.h"

namespace MIDILAR::Protocol {

/**
 * @brief Collects the payload of 7-bit System Exclusive packets into a
 * caller-provided buffer (SPEC-RT-4, SPEC-MIDI-14).
 * @ingroup MIDILAR_Protocol
 *
 * The payload excludes `0xF0` and `0xF7`. When it does not fit, the first
 * bytes are kept and the message is flagged as truncated. A `Start` or
 * `Complete` packet always begins a new message; `Continue` or `End` without
 * a start is ignored. The assembler keeps one message per group-less stream,
 * so filter by group before pushing when several groups are interleaved.
 */
class SysExAssembler {
public:
    /** @brief Uses `capacity` bytes at `buffer`; a null buffer keeps nothing. */
    SysExAssembler(uint8_t* buffer, size_t capacity) noexcept
        : _buffer(buffer), _capacity(buffer != nullptr ? capacity : 0) {}

    /**
     * @brief Adds one packet.
     * @return `true` when it completed a message; read it with `Data()`,
     * `Size()` and `IsTruncated()` before the next push.
     */
    bool Push(const Packet& packet) noexcept {
        if (packet.Type() != MessageType::Data64 || !packet.IsValid()) {
            return false;
        }
        const SysExStatus status = packet.SysExStatus();
        if (status == SysExStatus::Start || status == SysExStatus::Complete) {
            _size = 0;
            _truncated = false;
            _active = true;
        } else if (!_active || (status != SysExStatus::Continue && status != SysExStatus::End)) {
            return false;
        }
        for (uint8_t i = 0; i < packet.SysExSize(); ++i) {
            if (_size < _capacity) {
                _buffer[_size++] = packet.SysExByte(i);
            } else {
                _truncated = true;
            }
        }
        if (status == SysExStatus::Complete || status == SysExStatus::End) {
            _active = false;
            return true;
        }
        return false;
    }

    /** @brief Returns the caller's buffer holding the payload. */
    const uint8_t* Data() const noexcept { return _buffer; }

    /** @brief Returns the number of payload bytes kept. */
    size_t Size() const noexcept { return _size; }

    /** @brief Returns `true` when payload bytes did not fit and were dropped. */
    bool IsTruncated() const noexcept { return _truncated; }

private:
    uint8_t* _buffer;
    size_t _capacity;
    size_t _size = 0;
    bool _truncated = false;
    bool _active = false;
};

} // namespace MIDILAR::Protocol

#endif // MIDILAR_PROTOCOL_SYSEX_ASSEMBLER_H
