#include "Shared.h"

namespace MIDILARExamples::Protocol::SysEx {
using namespace MIDILAR::Protocol;
void Run(PrintFunction print) {
    MIDILAR_EXAMPLE_PRINT(print, "\n=== SysEx ===\n");
    MIDILAR_EXAMPLE_PRINT(print, "SysEx: ensamblado con buffer de 8 bytes.\nEntrada: F0 7D 01 02 03 F8 04 05 06 07 08 09 F7.\nF0/F7 delimitan; F8 es Clock y no es payload.\n10 bytes de payload, solo caben 8. Esperado: truncamiento = 1.\n\n");
    uint8_t buffer[8];
    SysExAssembler assembler(buffer, sizeof(buffer));
    Midi1Parser parser;
    // 0x7D is the educational/non-commercial manufacturer ID. Clock is not payload.
    const uint8_t stream[] = {0xF0, 0x7D, 1, 2, 3, 0xF8, 4, 5, 6, 7, 8, 9, 0xF7};
    for (uint8_t byte : stream) {
        Packet packets[Midi1Parser::MaxPackets];
        const uint8_t count = parser.Parse(byte, packets);
        for (uint8_t i = 0; i < count; ++i) {
            PrintPacket(print, packets[i]);
            if (assembler.Push(packets[i])) {
                MIDILAR_EXAMPLE_PRINT(print, "\nSysEx terminado: delimitadores y Clock fuera del payload.\n");
                PrintNumber(print, "Bytes guardados (capacidad 8)", static_cast<uint32_t>(assembler.Size()));
                PrintNumber(print, "Truncado (1 = bytes descartados)", assembler.IsTruncated());
                for (size_t j = 0; j < assembler.Size(); ++j) PrintByte(print, "Payload", assembler.Data()[j]);
            }
        }
    }
}
}
