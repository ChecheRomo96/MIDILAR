#include "Shared.h"

namespace MIDILARExamples::Protocol::UsbMidi1 {
using namespace MIDILAR::Protocol;
void Run(PrintFunction print) {
    MIDILAR_EXAMPLE_PRINT(print, "\n=== UsbMidi1 ===\n");
    MIDILAR_EXAMPLE_PRINT(print, "USB MIDI 1.0: evento -> UMP -> evento.\nEntrada: 19 90 3C 64.\n19: cable indice 1 (grupo 2), CIN 9 (Note On).\n90: canal 1; 3C: nota 60; 64: velocity 100.\nEsperado al recodificar: los mismos cuatro bytes.\n\n");
    // Cable 2 (wire index 1), Note On, channel 1.
    const uint8_t event[4] = {0x19, 0x90, 60, 100};
    Packet packet;
    if (!DecodeUsbMidi1(event, packet)) { MIDILAR_EXAMPLE_PRINT(print, "Invalid event\n"); return; }
    PrintPacket(print, packet);
    UsbMidi1Encoder encoder;
    UsbMidi1Encoder::Event events[UsbMidi1Encoder::MaxEvents];
    const uint8_t count = encoder.Encode(packet, events);
    PrintNumber(print, "Eventos USB reconstruidos (esperado 1)", count);
    for (uint8_t i = 0; i < count; ++i)
        for (uint8_t j = 0; j < 4; ++j) PrintByte(print, "Byte USB", events[i][j]);
}
}
