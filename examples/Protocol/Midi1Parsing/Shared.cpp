#include "Shared.h"

namespace MIDILARExamples::Protocol::Midi1Parsing {
using namespace MIDILAR::Protocol;
void Run(PrintFunction print) {
    MIDILAR_EXAMPLE_PRINT(print, "\n=== Midi1Parsing ===\n");
    MIDILAR_EXAMPLE_PRINT(print, "Parser: de bytes MIDI 1.0 a mensajes.\nEntrada: 90 3C 64 3E F8 6E 3C 00.\nEsperado: Note On 60, Clock, Note On 62, Note Off 60.\nF8 no borra la nota incompleta; velocity 0 libera la nota.\n\n");
    Midi1Parser parser;
    // Running status, an interleaved clock, then velocity-zero Note Off.
    const uint8_t stream[] = {0x90, 60, 100, 62, 0xF8, 110, 60, 0};
    for (uint8_t byte : stream) {
        PrintByte(print, "\nByte de entrada", byte);
        Packet packets[Midi1Parser::MaxPackets];
        const uint8_t count = parser.Parse(byte, packets);
        if (count == 0) MIDILAR_EXAMPLE_PRINT(print, "Mensaje incompleto: esperando mas bytes.\n");
        for (uint8_t i = 0; i < count; ++i) PrintPacket(print, packets[i]);
    }
}
}
