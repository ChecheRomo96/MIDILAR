#include <MIDILAR_MidiCore.h>
#include <iostream>
#include <cstdint>

using namespace MIDILAR::MidiCore;

void PrintHex(const char* label, const uint8_t* data, size_t size) {
    std::cout << label << " ";
    for (size_t i = 0; i < size; ++i) {
        std::cout << std::hex << std::uppercase << (int)data[i] << " ";
    }
    std::cout << std::dec << std::endl;
}

int main() {
    Message msg;

    // 1. Note On
    msg.NoteOn(60, 127, 0);
    PrintHex("[Note On]", msg.Buffer(), msg.size());

    // 2. Note Off
    msg.NoteOff(60, 64, 0);
    PrintHex("[Note Off]", msg.Buffer(), msg.size());

    // 3. Polyphonic AfterTouch
    msg.AfterTouch(60, 100, 0);
    PrintHex("[AfterTouch (Polyphonic)]", msg.Buffer(), msg.size());

    // 4. Control Change (Modulation)
    msg.ControlChange(1, 64, 0); // Modulation wheel
    PrintHex("[Control Change - Modulation]", msg.Buffer(), msg.size());

    // 5. Program Change
    msg.ProgramChange(10, 0);
    PrintHex("[Program Change]", msg.Buffer(), msg.size());

    // 6. Channel Pressure (Mono AfterTouch)
    msg.ChannelPressure(90, 0);
    PrintHex("[Channel Pressure]", msg.Buffer(), msg.size());

    // 7. Pitch Bend - Center (no bend)
    msg.PitchBend((int16_t)0, 0);
    PrintHex("[Pitch Bend Center]", msg.Buffer(), msg.size());

    // 8. Pitch Bend - Full Up
    msg.PitchBend((int16_t)8191, 0);
    PrintHex("[Pitch Bend Max]", msg.Buffer(), msg.size());

    // 9. Pitch Bend - Full Down
    msg.PitchBend((int16_t)-8192, 0);
    PrintHex("[Pitch Bend Min]", msg.Buffer(), msg.size());

    return 0;
}