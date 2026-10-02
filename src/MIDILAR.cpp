#include <MIDILAR.h>

// SPEC-EMB-1..2: size budgets and trivial copyability of MIDILAR value types
// are checked here, when the library is built for every target, AVR and Arm
// included.
#define MIDILAR_CHECK_VALUE_TYPE(Type, MaximumBytes)                                  \
    static_assert(sizeof(Type) <= (MaximumBytes), #Type " exceeds its size budget"); \
    static_assert(__is_trivially_copyable(Type), #Type " must be trivially copyable")

#if defined(MIDILAR_PROTOCOL)
MIDILAR_CHECK_VALUE_TYPE(MIDILAR::Protocol::Channel, 1);
MIDILAR_CHECK_VALUE_TYPE(MIDILAR::Protocol::Group, 1);
MIDILAR_CHECK_VALUE_TYPE(MIDILAR::Protocol::ChannelMask, 2);
MIDILAR_CHECK_VALUE_TYPE(MIDILAR::Protocol::GroupMask, 2);
MIDILAR_CHECK_VALUE_TYPE(MIDILAR::Protocol::NoteNumber, 1);
MIDILAR_CHECK_VALUE_TYPE(MIDILAR::Protocol::ControllerNumber, 1);
MIDILAR_CHECK_VALUE_TYPE(MIDILAR::Protocol::ProgramNumber, 1);
MIDILAR_CHECK_VALUE_TYPE(MIDILAR::Protocol::Velocity, 2);
MIDILAR_CHECK_VALUE_TYPE(MIDILAR::Protocol::ControllerValue, 4);
MIDILAR_CHECK_VALUE_TYPE(MIDILAR::Protocol::PressureValue, 4);
MIDILAR_CHECK_VALUE_TYPE(MIDILAR::Protocol::PitchBend, 4);
MIDILAR_CHECK_VALUE_TYPE(MIDILAR::Protocol::Packet, 16);
MIDILAR_CHECK_VALUE_TYPE(MIDILAR::Protocol::TimeCode, 5);
#if !defined(MIDILAR_PARSER_DIAGNOSTICS)
MIDILAR_CHECK_VALUE_TYPE(MIDILAR::Protocol::Midi1Parser, 16);
#endif
MIDILAR_CHECK_VALUE_TYPE(MIDILAR::Protocol::Midi1Encoder, 2);

// SPEC-MIDI-3: scaling stays constexpr on every compiler, GCC 7 (Arduino AVR)
// included.
static_assert(MIDILAR::Protocol::ScaleUp(127, 7, 32) == 0xFFFFFFFFu, "7->32 maximum");
static_assert(MIDILAR::Protocol::ScaleUp(8192, 14, 32) == 0x80000000u, "14->32 center");
static_assert(MIDILAR::Protocol::ChannelMask::All().Count() == 16, "sixteen channels");
static_assert(MIDILAR::Protocol::Packet::Midi2NoteOn(MIDILAR::Protocol::Group::FromWire(0),
                  MIDILAR::Protocol::Channel::FromWire(0), MIDILAR::Protocol::NoteNumber::FromValue(60),
                  MIDILAR::Protocol::Velocity::Max()).Word(1) == 0xFFFF0000u,
              "MIDI 2.0 Note On layout");
static_assert(MIDILAR::Protocol::ToNoteNumber(MIDILAR::Protocol::ToPitch(
                  MIDILAR::Protocol::NoteNumber::FromValue(61))) == MIDILAR::Protocol::NoteNumber::FromValue(61),
              "note number to MCC pitch and back");
#endif

#if defined(MIDILAR_DEVICES)
static_assert(MIDILAR::Devices::EuclideanPattern(3, 8) == 0x49u, "E(3, 8) is x..x..x.");
static_assert(MIDILAR::Protocol::TimeCode::From(0, 0, 59, 29, MIDILAR::Protocol::TimeCodeRate::Fps29_97Drop).Next() ==
                  MIDILAR::Protocol::TimeCode::From(0, 1, 0, 2, MIDILAR::Protocol::TimeCodeRate::Fps29_97Drop),
              "29.97 drop-frame skips frames 0 and 1");
#endif

#undef MIDILAR_CHECK_VALUE_TYPE
