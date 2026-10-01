#ifndef MIDILAR_PROTOCOL_VALUES_H
#define MIDILAR_PROTOCOL_VALUES_H

#include <stdint.h>

#include "Scaling.h"

namespace MIDILAR::Protocol {

/**
 * @brief A 7-bit data value in `[0, 127]` (SPEC-MIDI-6).
 * @ingroup MIDILAR_Protocol
 *
 * Any other input is the single invalid value. `Tag` keeps note, controller
 * and program numbers from being mixed.
 */
template <typename Tag>
class DataValue {
    static constexpr uint8_t InvalidValue = 0xFF;

    uint8_t _value;

    constexpr explicit DataValue(uint8_t value) noexcept : _value(value) {}

public:
    /** @brief Creates the invalid value (SPEC-RT-3). */
    constexpr DataValue() noexcept : _value(InvalidValue) {}

    /** @brief Returns the invalid value. */
    static constexpr DataValue Invalid() noexcept { return DataValue(); }

    /** @brief Creates the value `value` in `[0, 127]`. */
    static constexpr DataValue FromValue(int32_t value) noexcept {
        return value >= 0 && value <= 127 ? DataValue(static_cast<uint8_t>(value)) : DataValue();
    }

    /** @brief Returns `true` unless this is the invalid value. */
    constexpr bool IsValid() const noexcept { return _value != InvalidValue; }

    /** @brief Returns the value `0-127`, or `0xFF` when invalid. */
    constexpr uint8_t Value() const noexcept { return _value; }

    /** @brief Compares values; all invalid values are equal. */
    friend constexpr bool operator==(DataValue a, DataValue b) noexcept { return a._value == b._value; }

    /** @brief Negation of `operator==`. */
    friend constexpr bool operator!=(DataValue a, DataValue b) noexcept { return !(a == b); }
};

/** @brief Tag of `NoteNumber`. @ingroup MIDILAR_Protocol */
struct NoteNumberTag {};

/** @brief Tag of `ControllerNumber`. @ingroup MIDILAR_Protocol */
struct ControllerNumberTag {};

/** @brief Tag of `ProgramNumber`. @ingroup MIDILAR_Protocol */
struct ProgramNumberTag {};

/** @brief A note number; 60 is C4 (SPEC-MIDI-7). @ingroup MIDILAR_Protocol */
using NoteNumber = DataValue<NoteNumberTag>;

/** @brief A control change index. @ingroup MIDILAR_Protocol */
using ControllerNumber = DataValue<ControllerNumberTag>;

/** @brief A program change number, wire value `0-127`. @ingroup MIDILAR_Protocol */
using ProgramNumber = DataValue<ProgramNumberTag>;

/**
 * @brief A value at MIDI 2.0 resolution (`Bits` wide) with a MIDI 1.0 view
 * (`Midi1Bits` wide), converted with min-center-max scaling (SPEC-MIDI-3).
 * @ingroup MIDILAR_Protocol
 *
 * Every bit pattern is a valid MIDI 2.0 value, so these types have no invalid
 * value. MIDI 1.0 input outside `[0, 2^Midi1Bits - 1]` is clamped.
 */
template <typename Storage, uint32_t Bits, uint32_t Midi1Bits, typename Tag>
class ScaledValue {
    Storage _value;

    constexpr explicit ScaledValue(Storage value) noexcept : _value(value) {}

public:
    /** @brief Largest MIDI 1.0 value. */
    static constexpr uint32_t Midi1Max = (static_cast<uint32_t>(1) << Midi1Bits) - 1;

    /** @brief Creates the minimum value. */
    constexpr ScaledValue() noexcept : _value(0) {}

    /** @brief Creates the value from its full MIDI 2.0 resolution. */
    static constexpr ScaledValue FromMidi2(Storage value) noexcept { return ScaledValue(value); }

    /** @brief Creates the value from MIDI 1.0 resolution, clamped and scaled up. */
    static constexpr ScaledValue FromMidi1(int32_t value) noexcept {
        return ScaledValue(static_cast<Storage>(ScaleUp(
            value < 0 ? 0u : (static_cast<uint32_t>(value) > Midi1Max ? Midi1Max : static_cast<uint32_t>(value)),
            Midi1Bits, Bits)));
    }

    /** @brief Returns the minimum value. */
    static constexpr ScaledValue Min() noexcept { return ScaledValue(); }

    /** @brief Returns the center value, `1 << (Bits - 1)`. */
    static constexpr ScaledValue Center() noexcept {
        return ScaledValue(static_cast<Storage>(static_cast<uint32_t>(1) << (Bits - 1)));
    }

    /** @brief Returns the maximum value. */
    static constexpr ScaledValue Max() noexcept { return ScaledValue(static_cast<Storage>(~static_cast<Storage>(0))); }

    /** @brief Returns the full MIDI 2.0 value. */
    constexpr Storage Midi2() const noexcept { return _value; }

    /** @brief Returns the MIDI 1.0 value, keeping the most significant bits. */
    constexpr uint32_t Midi1() const noexcept { return ScaleDown(_value, Bits, Midi1Bits); }

    /**
     * @brief Returns the signed distance from `Center()`; for pitch bend,
     * negative bends down and positive bends up.
     */
    constexpr int32_t Offset() const noexcept {
        return static_cast<int32_t>(static_cast<int64_t>(_value) - (static_cast<int64_t>(1) << (Bits - 1)));
    }

    /** @brief Compares values. */
    friend constexpr bool operator==(ScaledValue a, ScaledValue b) noexcept { return a._value == b._value; }

    /** @brief Negation of `operator==`. */
    friend constexpr bool operator!=(ScaledValue a, ScaledValue b) noexcept { return !(a == b); }
};

/** @brief Tag of `Velocity`. @ingroup MIDILAR_Protocol */
struct VelocityTag {};

/** @brief Tag of `ControllerValue`. @ingroup MIDILAR_Protocol */
struct ControllerValueTag {};

/** @brief Tag of `PressureValue`. @ingroup MIDILAR_Protocol */
struct PressureValueTag {};

/** @brief Tag of `PitchBend`. @ingroup MIDILAR_Protocol */
struct PitchBendTag {};

/** @brief Note velocity: 16-bit, MIDI 1.0 view 7-bit. @ingroup MIDILAR_Protocol */
using Velocity = ScaledValue<uint16_t, 16, 7, VelocityTag>;

/** @brief Control change value: 32-bit, MIDI 1.0 view 7-bit. @ingroup MIDILAR_Protocol */
using ControllerValue = ScaledValue<uint32_t, 32, 7, ControllerValueTag>;

/** @brief Channel or poly pressure: 32-bit, MIDI 1.0 view 7-bit. @ingroup MIDILAR_Protocol */
using PressureValue = ScaledValue<uint32_t, 32, 7, PressureValueTag>;

/** @brief Pitch bend: 32-bit, MIDI 1.0 view 14-bit, centered. @ingroup MIDILAR_Protocol */
using PitchBend = ScaledValue<uint32_t, 32, 14, PitchBendTag>;

} // namespace MIDILAR::Protocol

#endif // MIDILAR_PROTOCOL_VALUES_H
