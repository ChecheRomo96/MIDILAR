#ifndef MIDILAR_PROTOCOL_SCALING_H
#define MIDILAR_PROTOCOL_SCALING_H

#include <stdint.h>

namespace MIDILAR::Protocol {

/**
 * @brief Scales an unsigned value from `sourceBits` to `targetBits` with the
 * MIDI 2.0 min-center-max rule (SPEC-MIDI-3).
 * @ingroup MIDILAR_Protocol
 *
 * Zero stays zero, the source center (`1 << (sourceBits - 1)`) maps to the
 * target center, and the source maximum maps to the target maximum; values
 * above the center repeat their low bits to fill the extra resolution. Valid
 * for `1 <= sourceBits < targetBits <= 32` and `value < (1 << sourceBits)`;
 * other inputs return 0.
 */
constexpr uint32_t ScaleUp(uint32_t value, uint32_t sourceBits, uint32_t targetBits) noexcept {
    if (sourceBits < 1 || sourceBits >= targetBits || targetBits > 32 ||
        value >= (static_cast<uint64_t>(1) << sourceBits)) {
        return 0;
    }
    const uint32_t scaleBits = targetBits - sourceBits;
    uint32_t shifted = value << scaleBits;
    const uint32_t center = static_cast<uint32_t>(1) << (sourceBits - 1);
    if (value <= center) {
        return shifted;
    }
    const uint32_t repeatBits = sourceBits - 1;
    const uint32_t repeatMask = (static_cast<uint32_t>(1) << repeatBits) - 1;
    uint32_t repeat = value & repeatMask;
    if (scaleBits > repeatBits) {
        repeat <<= scaleBits - repeatBits;
    } else {
        repeat >>= repeatBits - scaleBits;
    }
    while (repeat != 0) {
        shifted |= repeat;
        repeat >>= repeatBits;
    }
    return shifted;
}

/**
 * @brief Scales an unsigned value down from `sourceBits` to `targetBits` by
 * keeping its most significant bits, the inverse of `ScaleUp()`.
 * @ingroup MIDILAR_Protocol
 *
 * Valid for `1 <= targetBits < sourceBits <= 32`; other inputs return 0.
 */
constexpr uint32_t ScaleDown(uint32_t value, uint32_t sourceBits, uint32_t targetBits) noexcept {
    if (targetBits < 1 || targetBits >= sourceBits || sourceBits > 32) {
        return 0;
    }
    const uint64_t limited = sourceBits == 32 ? value
        : (value & ((static_cast<uint64_t>(1) << sourceBits) - 1));
    return static_cast<uint32_t>(limited >> (sourceBits - targetBits));
}

} // namespace MIDILAR::Protocol

#endif // MIDILAR_PROTOCOL_SCALING_H
