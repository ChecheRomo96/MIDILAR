#ifndef MIDILAR_PROTOCOL_NOTE_PITCH_H
#define MIDILAR_PROTOCOL_NOTE_PITCH_H

#include <MCC/Key/Key.h>
#include <MCC/Pitch/ChromaticIndex.h>
#include <MCC/Pitch/Pitch.h>

#include "Values.h"

namespace MIDILAR::Protocol {

/**
 * @brief Returns the sounding pitch of `note`: note number `n` is
 * `MCC::ChromaticIndex(n)`, so 60 is C4 (SPEC-MIDI-7).
 * @ingroup MIDILAR_Protocol
 */
constexpr MCC::ChromaticIndex ToChromaticIndex(NoteNumber note) noexcept {
    return note.IsValid() ? MCC::ChromaticIndex(note.Value()) : MCC::ChromaticIndex::Invalid();
}

/**
 * @brief Returns the note number of `index`, or the invalid note outside
 * `[0, 127]` (SPEC-MIDI-7).
 * @ingroup MIDILAR_Protocol
 */
constexpr NoteNumber ToNoteNumber(MCC::ChromaticIndex index) noexcept {
    return index.IsValid() ? NoteNumber::FromValue(index.Value()) : NoteNumber::Invalid();
}

/**
 * @brief Returns the note number that sounds `pitch`, whatever its spelling,
 * or the invalid note outside `[0, 127]` (SPEC-MIDI-7).
 * @ingroup MIDILAR_Protocol
 */
FOUNDATION_CONSTEXPR14 NoteNumber ToNoteNumber(const MCC::Pitch& pitch) noexcept {
    return ToNoteNumber(pitch.ChromaticIndex());
}

/**
 * @brief Writes `note` as a pitch spelled in `key`, by default C major
 * (SPEC-MIDI-7); an invalid note or key yields the invalid pitch.
 * @ingroup MIDILAR_Protocol
 */
FOUNDATION_CONSTEXPR14 MCC::Pitch ToPitch(
    NoteNumber note,
    const MCC::Key& key = MCC::Key(MCC::NoteName(MCC::Letter::C, MCC::Accidental::Natural()),
                                   MCC::KeyMode::Major)) noexcept {
    return key.Spell(ToChromaticIndex(note));
}

} // namespace MIDILAR::Protocol

#endif // MIDILAR_PROTOCOL_NOTE_PITCH_H
