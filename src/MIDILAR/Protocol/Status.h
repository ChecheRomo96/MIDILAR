#pragma once
#include <stdint.h>
namespace MIDILAR::Protocol {
/** @brief System common and real-time status bytes. @ingroup MIDILAR_Protocol */
enum class SystemStatus : uint8_t {
    TimeCode = 0xF1,
    SongPosition = 0xF2,
    SongSelect = 0xF3,
    TuneRequest = 0xF6,
    TimingClock = 0xF8,
    Start = 0xFA,
    Continue = 0xFB,
    Stop = 0xFC,
    ActiveSensing = 0xFE,
    Reset = 0xFF
};

/**
 * @brief Channel voice status, bits 23-20 of the first word.
 * @ingroup MIDILAR_Protocol
 *
 * `NoteOff` to `PitchBend` exist in both protocols. The per-note and
 * registered/assignable controller statuses are MIDI 2.0 only and are
 * classified here; their typed constructors come in a later phase.
 */
enum class VoiceStatus : uint8_t {
    RegisteredPerNoteController = 0x0,
    AssignablePerNoteController = 0x1,
    RegisteredController = 0x2,
    AssignableController = 0x3,
    RelativeRegisteredController = 0x4,
    RelativeAssignableController = 0x5,
    PerNotePitchBend = 0x6,
    NoteOff = 0x8,
    NoteOn = 0x9,
    PolyPressure = 0xA,
    ControlChange = 0xB,
    ProgramChange = 0xC,
    ChannelPressure = 0xD,
    PitchBendChange = 0xE,
    PerNoteManagement = 0xF
};

}
