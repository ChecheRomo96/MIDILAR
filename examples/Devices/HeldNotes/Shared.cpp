#include "Shared.h"
#include <MIDILAR_Devices.h>

namespace MIDILARExamples::Devices::HeldNotes {
using namespace MIDILAR::Protocol;
using namespace MIDILAR::Devices;
namespace {
struct Sink {
    PrintFunction print;
    const char* label;
    void Process(const Packet& packet) {
        print(label);
        PrintPacket(print, packet);
        PrintNumber(print, "Channel", packet.Channel().Number());
        PrintNumber(print, "Note", packet.Note().Value());
    }
};
}
void Run(PrintFunction print) {
    MIDILAR_EXAMPLE_PRINT(print, "\n=== HeldNotes ===\n");
    MIDILAR_EXAMPLE_PRINT(print, "Transposicion con notas sostenidas, entrada nota 60.\n1. Con +12, Note On debe salir como 72.\n2. Cambiar a -12: Note Off aun debe liberar 72.\n3. Nueva pareja On/Off debe usar 48.\nEsperado: 72, 72, 48, 48; ninguna nota colgada.\n\n");
    Transpose transpose;
    Sink sink{print, "Transposed packet\n"};
    transpose.Output().Bind<Sink, &Sink::Process>(&sink);
    const Group group = Group::FromNumber(1);
    const Channel channel = Channel::FromNumber(1);
    const NoteNumber note = NoteNumber::FromValue(60);
    transpose.SetSemitones(12);
    MIDILAR_EXAMPLE_PRINT(print, "\nPaso 1: Note On 60 con +12 -> 72.\n");
    transpose.Process(Packet::Midi1NoteOn(group, channel, note, Velocity::FromMidi1(100)));
    transpose.SetSemitones(-12);
    MIDILAR_EXAMPLE_PRINT(print, "\nPaso 2: cambio a -12; Note Off debe liberar 72.\n");
    // Release note 72, the note actually started, even though the mapping changed.
    transpose.Process(Packet::Midi1NoteOff(group, channel, note, Velocity::Min()));
    // A fresh note uses the new mapping: note 48.
    MIDILAR_EXAMPLE_PRINT(print, "\nPaso 3: nueva pareja con -12 -> 48.\n");
    transpose.Process(Packet::Midi1NoteOn(group, channel, note, Velocity::FromMidi1(100)));
    transpose.Process(Packet::Midi1NoteOff(group, channel, note, Velocity::Min()));
}
}
