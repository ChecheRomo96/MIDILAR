#include "Shared.h"

namespace MIDILARExamples::Protocol::SysExBuffer {
using namespace MIDILAR::Protocol::Midi1;
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
    MIDILAR_EXAMPLE_PRINT(print, "\n=== SysExBuffer ===\n");
    MIDILAR_EXAMPLE_PRINT(print, "SysEx en buffer del usuario; la vista no es propietaria.\n");
    uint8_t storage[16];
    const uint8_t payload[] = {0x7D, 1, 2, 3};
    auto message = SysExMessage::Build(storage, sizeof(storage), payload, sizeof(payload));
    Show(print, "Esperado F0 7D 01 02 03 F7", message);
    MIDILAR_EXAMPLE_PRINT(print, "Capacidad 5 insuficiente para 6 bytes: no se modifica el buffer.\n");
    auto rejected = SysExMessage::Build(storage, 5, payload, sizeof(payload));
    Show(print, "Resultado invalido, sin truncamiento implicito", rejected);
    Show(print, "El mensaje anterior sigue intacto", message);
    const auto time = MIDILAR::Protocol::TimeCode::From(
        1, 2, 3, 4, MIDILAR::Protocol::TimeCodeRate::Fps25);
    MIDILAR_EXAMPLE_PRINT(print, "MTC Full Frame 01:02:03:04 a 25 fps.\n");
    auto fullFrame = SysExMessage::MTC_FullFrame(storage, sizeof(storage), time);
    Show(print, "Esperado F0 7F 7F 01 01 21 02 03 04 F7", fullFrame);
    MIDILAR_EXAMPLE_PRINT(print, "Se reutilizo storage: no volver a leer la vista anterior.\n");
}
}
