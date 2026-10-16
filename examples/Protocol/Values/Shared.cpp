#include "Shared.h"

namespace MIDILARExamples::Protocol::Values {
using namespace MIDILAR::Protocol;
void Run(PrintFunction print) {
    MIDILAR_EXAMPLE_PRINT(print, "\n=== Values ===\n");
    MIDILAR_EXAMPLE_PRINT(print, "Valores: canal humano 1-16 = direccion de cable 0-15.\nVelocity: escala 7 -> 16 bits y vuelve a 100.\nLos indicadores de invalidez deben ser 1 (verdadero).\n\n");
    const Channel channel = Channel::FromNumber(1);
    const Velocity velocity = Velocity::FromMidi1(100);
    PrintNumber(print, "Canal 1: direccion en el cable (esperado 0)", channel.Wire());
    PrintNumber(print, "Velocity 100 escalada a MIDI 2.0", velocity.Midi2());
    PrintNumber(print, "Velocity de regreso a MIDI 1.0 (esperado 100)", velocity.Midi1());
    PrintNumber(print, "Pitch bend centrado (esperado 8192)", PitchBend::Center().Midi1());
    PrintNumber(print, "Canal 17 rechazado (esperado 1)", !Channel::FromNumber(17).IsValid());
    PrintNumber(print, "Nota 128 rechazada (esperado 1)", !NoteNumber::FromValue(128).IsValid());
}
}
