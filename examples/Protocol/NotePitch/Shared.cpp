#include "Shared.h"

namespace MIDILARExamples::Protocol::NotePitch {
using namespace MIDILAR::Protocol;
void Run(PrintFunction print) {
    MIDILAR_EXAMPLE_PRINT(print, "\n=== NotePitch ===\n");
    MIDILAR_EXAMPLE_PRINT(print, "MIDI 60 = C4 (Do central).\nConvertimos nota -> altura MCC -> nota: debe volver a 60.\nIndice 128 invalido: MIDI solo admite notas 0-127.\n\n");
    const NoteNumber middleC = NoteNumber::FromValue(60);
    const MCC::Pitch pitch = ToPitch(middleC);
    PrintNumber(print, "Indice cromatico de C4 (esperado 60)", static_cast<uint32_t>(ToChromaticIndex(middleC).Value()));
    PrintNumber(print, "Altura MCC de regreso a nota MIDI (esperado 60)", ToNoteNumber(pitch).Value());
    PrintNumber(print, "Indice 128 rechazado (esperado 1)", !ToNoteNumber(MCC::ChromaticIndex(128)).IsValid());
}
}
