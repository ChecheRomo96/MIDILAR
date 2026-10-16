#include "Shared.h"

namespace MIDILARExamples::Protocol::CreatingMessages {
using namespace MIDILAR::Protocol::Midi;
namespace {
template <typename T>
void Show(PrintFunction print, const char* label, const T& message) {
    MIDILAR_EXAMPLE_PRINT(print, "\n"); print(label); MIDILAR_EXAMPLE_PRINT(print, "\n");
    if (!message.IsValid()) { MIDILAR_EXAMPLE_PRINT(print, "Invalido: size=0, Buffer=null.\n"); return; }
    PrintNumber(print, "Bytes MIDI 1.0", static_cast<uint32_t>(message.size()));
    for (size_t i = 0; i < message.size(); ++i) PrintByte(print, "Byte", message.Data(i));
}
}
void Run(PrintFunction print) {
    MIDILAR_EXAMPLE_PRINT(print, "\n=== CreatingMessages ===\n");
    MIDILAR_EXAMPLE_PRINT(print, "MIDI 1.0 nativo: Message ocupa 3 bytes, sin memoria dinamica.\n");
    PrintNumber(print, "sizeof(Message)", static_cast<uint32_t>(sizeof(Message)));
    Message message;
    message.NoteOn(60, 100, 0);
    Show(print, "Note On: Do central, velocity 100, canal humano 1", message);
    message.NoteOff(60, 0, 0);
    Show(print, "Note Off: liberar la misma nota", message);
    message.ProgramChange(10, 0);
    Show(print, "Program Change: indice 10, solo 2 bytes", message);
    message.PitchBend(8192, 0);
    Show(print, "Pitch Bend: centro 8192 -> E0 00 40", message);
    message.PitchBend7(127, 0);
    Show(print, "Pitch Bend7: maximo 127 -> E0 7F 7F", message);
    message.NoteOn(128, 100, 0);
    Show(print, "Nota 128 invalida: el objeto se vacia", message);
    message.NoteOn(60, 0, 0);
    Show(print, "Bytes originales: Note On con velocity cero", message);
    MIDILAR_EXAMPLE_PRINT(print, "Conversion explicita a UMP: se normaliza a Note Off.\n");
    const auto packet = MIDILAR::Protocol::Ump::Encode(MIDILAR::Protocol::Ump::Group::FromNumber(1), message);
    PrintPacket(print, packet);
    Show(print, "De regreso a bytes MIDI 1.0", MIDILAR::Protocol::Ump::DecodeMidi1(packet));
}
}
