#include "Shared.h"

namespace MIDILARExamples::Protocol::ControlChanges {
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
    MIDILAR_EXAMPLE_PRINT(print, "\n=== ControlChanges ===\n");
    MIDILAR_EXAMPLE_PRINT(print, "Controladores: cada constructor reemplaza un solo mensaje.\n");
    MIDILAR_EXAMPLE_PRINT(print, "Canal de cable 0 = canal humano 1. Solo se muestran los datos.\n");
    Message message;
    message.CC_Modulation(64);
    Show(print, "Modulation CC 1 = 64 -> B0 01 40", message);
    message.CC_Volume(100);
    Show(print, "Volume CC 7 = 100", message);
    message.CC_Pan(64);
    Show(print, "Pan CC 10 = 64: centro", message);
    message.CC_LocalControlOff();
    Show(print, "Local Control Off CC 122 = 0", message);
    message.CC_AllNotesOff();
    Show(print, "All Notes Off CC 123 = 0", message);
    MIDILAR_EXAMPLE_PRINT(print, "\nNRPN 266 (MSB 2, LSB 10), valor 1570 (MSB 12, LSB 34).\n");
    MIDILAR_EXAMPLE_PRINT(print, "Enviar cada CC antes de construir el siguiente. Son 4 mensajes.\n");
    const int32_t controllers[] = {99, 98, 6, 38};
    const int32_t values[] = {2, 10, 12, 34};
    for (uint8_t i = 0; i < 4; ++i) {
        message.ControlChange(controllers[i], values[i]);
        Show(print, "Componente de la secuencia NRPN", message);
    }
}
}
