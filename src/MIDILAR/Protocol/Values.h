#pragma once
#include "Detail/Values.h"
namespace MIDILAR::Protocol {
/** @brief A note number; 60 is C4 (SPEC-MIDI-7). @ingroup MIDILAR_Protocol */
using NoteNumber = Detail::DataValue<Detail::NoteNumberTag>;

/** @brief A control change index. @ingroup MIDILAR_Protocol */
using ControllerNumber = Detail::DataValue<Detail::ControllerNumberTag>;

/** @brief A program change number, wire value `0-127`. @ingroup MIDILAR_Protocol */
using ProgramNumber = Detail::DataValue<Detail::ProgramNumberTag>;

}
