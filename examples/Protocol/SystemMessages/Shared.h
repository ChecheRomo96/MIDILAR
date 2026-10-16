#pragma once
#include <MIDILAR_Protocol.h>
#include <stdio.h>
#if defined(__AVR__)
#include <avr/pgmspace.h>
#endif
namespace MIDILARExamples {
using PrintFunction = void (*)(const char*);
#if defined(__AVR__)
// Stream explanatory literals from flash, retaining only a small RAM chunk.
inline void PrintFlash(PrintFunction print, const char* source) {
    char chunk[48];
    uint8_t count = 0;
    for (;;) {
        const char next = static_cast<char>(pgm_read_byte(source++));
        if (next == '\0') {
            chunk[count] = '\0';
            if (count != 0) print(chunk);
            return;
        }
        chunk[count++] = next;
        if (count == sizeof(chunk) - 1) {
            chunk[count] = '\0';
            print(chunk);
            count = 0;
        }
    }
}
#define MIDILAR_EXAMPLE_PRINT(printer, text) ::MIDILARExamples::PrintFlash(printer, PSTR(text))
#else
#define MIDILAR_EXAMPLE_PRINT(printer, text) printer(text)
#endif
inline void PrintNumber(PrintFunction print, const char* label, uint32_t value) {
    char text[96];
    snprintf(text, sizeof(text), "%s: %lu\n", label, static_cast<unsigned long>(value));
    print(text);
}
inline void PrintByte(PrintFunction print, const char* label, uint8_t byte) {
    char text[96];
    snprintf(text, sizeof(text), "%s: 0x%02X (%u)\n", label,
             static_cast<unsigned int>(byte), static_cast<unsigned int>(byte));
    print(text);
}
inline void PrintPacket(PrintFunction print, const MIDILAR::Protocol::Packet& packet) {
    using namespace MIDILAR::Protocol;
    if (!packet.IsValid()) { MIDILAR_EXAMPLE_PRINT(print, "Paquete invalido: no se puede enviar.\n"); return; }
    MIDILAR_EXAMPLE_PRINT(print, "\n--- Mensaje ---\n");
    if (packet.IsChannelVoice()) {
        print(packet.Type() == MessageType::Midi1ChannelVoice
            ? "Protocolo: MIDI 1.0 (UMP de 32 bits)\n"
            : "Protocolo: MIDI 2.0 (UMP de 64 bits)\n");
        PrintNumber(print, "Grupo (1-16)", packet.Group().Number());
        PrintNumber(print, "Canal (1-16)", packet.Channel().Number());
        switch (packet.VoiceStatus()) {
        case VoiceStatus::NoteOn: MIDILAR_EXAMPLE_PRINT(print, "Note On: iniciar nota\n"); break;
        case VoiceStatus::NoteOff: MIDILAR_EXAMPLE_PRINT(print, "Note Off: liberar nota\n"); break;
        case VoiceStatus::PolyPressure: MIDILAR_EXAMPLE_PRINT(print, "Poly Pressure: presion de una nota\n"); break;
        case VoiceStatus::ControlChange: MIDILAR_EXAMPLE_PRINT(print, "Control Change: cambiar controlador\n"); break;
        case VoiceStatus::ProgramChange: MIDILAR_EXAMPLE_PRINT(print, "Program Change: seleccionar programa\n"); break;
        case VoiceStatus::ChannelPressure: MIDILAR_EXAMPLE_PRINT(print, "Channel Pressure: presion del canal\n"); break;
        case VoiceStatus::PitchBendChange: MIDILAR_EXAMPLE_PRINT(print, "Pitch Bend: modificar afinacion\n"); break;
        default: MIDILAR_EXAMPLE_PRINT(print, "Otro mensaje de canal\n"); break;
        }
        if (packet.HasNote()) {
            PrintNumber(print, "Numero de nota (60 = C4)", packet.Note().Value());
        }
        switch (packet.VoiceStatus()) {
        case VoiceStatus::NoteOn:
        case VoiceStatus::NoteOff:
            PrintNumber(print, "Velocity MIDI 1.0 (0-127)", packet.Velocity().Midi1());
            PrintNumber(print, "Velocity MIDI 2.0 (0-65535)", packet.Velocity().Midi2());
            if (packet.Type() == MessageType::Midi2ChannelVoice) {
                PrintNumber(print, "Tipo de atributo (0 = ninguno)", packet.AttributeType());
                PrintNumber(print, "Dato del atributo", packet.AttributeData());
            }
            break;
        case VoiceStatus::ControlChange:
            PrintNumber(print, "Controlador (CC)", packet.Controller().Value());
            PrintNumber(print, "Valor MIDI 1.0 (0-127)", packet.ControllerValue().Midi1());
            PrintNumber(print, "Valor MIDI 2.0 (32 bits)", packet.ControllerValue().Midi2());
            break;
        case VoiceStatus::ProgramChange:
            PrintNumber(print, "Programa (indice 0-127)", packet.Program().Value());
            if (packet.HasBank()) PrintNumber(print, "Banco (14 bits)", packet.Bank());
            break;
        case VoiceStatus::PolyPressure:
        case VoiceStatus::ChannelPressure:
            PrintNumber(print, "Presion MIDI 1.0 (0-127)", packet.Pressure().Midi1());
            PrintNumber(print, "Presion MIDI 2.0 (32 bits)", packet.Pressure().Midi2());
            break;
        case VoiceStatus::PitchBendChange:
            PrintNumber(print, "Bend MIDI 1.0 (8192 = centro)", packet.PitchBend().Midi1());
            PrintNumber(print, "Bend MIDI 2.0 (2147483648 = centro)", packet.PitchBend().Midi2());
            break;
        default: break;
        }
    } else if (packet.Type() == MessageType::System) {
        PrintNumber(print, "Grupo (1-16)", packet.Group().Number());
        switch (packet.SystemStatus()) {
        case SystemStatus::Start: MIDILAR_EXAMPLE_PRINT(print, "Start: iniciar desde el principio\n"); break;
        case SystemStatus::Stop: MIDILAR_EXAMPLE_PRINT(print, "Stop: detener reproduccion\n"); break;
        case SystemStatus::Continue: MIDILAR_EXAMPLE_PRINT(print, "Continue: reanudar reproduccion\n"); break;
        case SystemStatus::TimingClock: MIDILAR_EXAMPLE_PRINT(print, "Timing Clock: un pulso de los 24 por negra\n"); break;
        case SystemStatus::SongPosition:
            PrintNumber(print, "Song Position (unidades de 6 clocks)", packet.SongPosition()); break;
        case SystemStatus::SongSelect:
            PrintNumber(print, "Song Select (indice 0-127)", packet.SongSelect()); break;
        case SystemStatus::TuneRequest: MIDILAR_EXAMPLE_PRINT(print, "Tune Request: solicitar afinacion\n"); break;
        case SystemStatus::TimeCode:
            MIDILAR_EXAMPLE_PRINT(print, "MTC Quarter Frame: fragmento del codigo de tiempo\n");
            PrintByte(print, "Tipo y dato del fragmento", packet.TimeCode()); break;
        case SystemStatus::ActiveSensing: MIDILAR_EXAMPLE_PRINT(print, "Active Sensing: conexion activa\n"); break;
        case SystemStatus::Reset: MIDILAR_EXAMPLE_PRINT(print, "Reset: reiniciar receptor\n"); break;
        }
    } else if (packet.Type() == MessageType::Utility) {
        switch (packet.UtilityStatus()) {
        case UtilityStatus::Noop: MIDILAR_EXAMPLE_PRINT(print, "Utility / Noop: sin operacion\n"); break;
        case UtilityStatus::JrClock: MIDILAR_EXAMPLE_PRINT(print, "Utility / JR Clock: referencia del reloj\n"); break;
        case UtilityStatus::JrTimestamp: MIDILAR_EXAMPLE_PRINT(print, "Utility / JR Timestamp: marca de tiempo\n"); break;
        case UtilityStatus::TicksPerQuarterNote: MIDILAR_EXAMPLE_PRINT(print, "Utility / Ticks Per Quarter Note: resolucion temporal\n"); break;
        case UtilityStatus::DeltaClockstamp: MIDILAR_EXAMPLE_PRINT(print, "Utility / Delta Clockstamp: incremento temporal\n"); break;
        }
        PrintNumber(print, "Dato de utilidad", packet.UtilityData());
    } else if (packet.Type() == MessageType::Data64) {
        MIDILAR_EXAMPLE_PRINT(print, "SysEx7: fragmento de System Exclusive\n");
        PrintNumber(print, "Estado (0=completo, 1=inicio, 2=continua, 3=fin)",
                    static_cast<uint32_t>(packet.SysExStatus()));
        PrintNumber(print, "Bytes de payload", packet.SysExSize());
    } else {
        PrintNumber(print, "Tipo UMP", static_cast<uint32_t>(packet.Type()));
    }
    MIDILAR_EXAMPLE_PRINT(print, "Representacion UMP (palabras hex de 32 bits):\n");
    for (uint8_t i = 0; i < packet.WordCount(); ++i) {
        char text[32];
        snprintf(text, sizeof(text), "  palabra %u: %08lX\n",
                 static_cast<unsigned int>(i), static_cast<unsigned long>(packet.Word(i)));
        print(text);
    }
}
}
namespace MIDILARExamples::Protocol::SystemMessages {
void Run(PrintFunction print);
}
