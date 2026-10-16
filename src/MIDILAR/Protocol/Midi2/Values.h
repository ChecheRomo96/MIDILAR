#pragma once
#include "../Detail/Values.h"
namespace MIDILAR::Protocol::Midi2 {
/** @brief Note velocity: 16-bit, MIDI 1.0 view 7-bit. @ingroup MIDILAR_Midi2 */
using Velocity = Detail::ScaledValue<uint16_t, 16, 7, Detail::VelocityTag>;

/** @brief Control change value: 32-bit, MIDI 1.0 view 7-bit. @ingroup MIDILAR_Midi2 */
using ControllerValue = Detail::ScaledValue<uint32_t, 32, 7, Detail::ControllerValueTag>;

/** @brief Channel or poly pressure: 32-bit, MIDI 1.0 view 7-bit. @ingroup MIDILAR_Midi2 */
using PressureValue = Detail::ScaledValue<uint32_t, 32, 7, Detail::PressureValueTag>;

/** @brief Pitch bend: 32-bit, MIDI 1.0 view 14-bit, centered. @ingroup MIDILAR_Midi2 */
using PitchBend = Detail::ScaledValue<uint32_t, 32, 14, Detail::PitchBendTag>;

}
