#include <MIDILAR_MidiCore.h>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <string>

using namespace MIDILAR::MidiCore;

void InjectNoise(std::vector<uint8_t>& buf, size_t count) {
    for (size_t i = 0; i < count; ++i)
        buf.push_back(static_cast<uint8_t>(rand()));
}

// ===============================
// Print Helper
// ===============================
void PrintHex(const char* label, const uint8_t* data, size_t size) {
    std::cout << label << " ";
    for (size_t i = 0; i < size; ++i) {
        std::cout << std::to_string(static_cast<int>(data[i])) << (i < size - 1 ? " " : "");
    }
    std::cout << std::endl;
}

// ===============================
// CALLBACK FUNCTIONS
// ===============================
void ChannelVoiceCallback(const uint8_t* data, size_t size) { PrintHex("[Channel Voice]", data, size); }
void ControlChangeCallback(const uint8_t* data, size_t size) { PrintHex("[Control Change]", data, size); }
void RealTimeCallback(const uint8_t* data, size_t size) { PrintHex("[Real-Time]", data, size); }
void SystemCommonCallback(const uint8_t* data, size_t size) { PrintHex("[System Common]", data, size); }
void SysExCallback(const uint8_t* data, size_t size) { PrintHex("[System Exclusive]", data, size); }
void MTCCallback(const uint8_t* data, size_t size) { PrintHex("[MIDI Time Code]", data, size); }
void MSCCallback(const uint8_t* data, size_t size) { PrintHex("[MIDI Show Control]", data, size); }
void DefaultCallback(const uint8_t* data, size_t size) { PrintHex("[Default Callback]", data, size); }

int main() {
    MessageParser parser(32);
    parser.BindDefaultCallback(DefaultCallback);
    parser.BindChannelVoiceCallback(ChannelVoiceCallback);
    parser.BindControlChangeCallback(ControlChangeCallback);
    parser.BindRealTimeCallback(RealTimeCallback);
    parser.BindSystemCommonCallback(SystemCommonCallback);
    parser.BindSysExCallback(SysExCallback);
    parser.BindMTCCallback(MTCCallback);
    parser.BindMSCCallback(MSCCallback);

    // Example: Note On (0x90), Note 60, Velocity 100
    uint8_t noteOn[] = { 0x90, 0x3C, 100 };
    parser.ProcessData(noteOn, sizeof(noteOn));

    // Example: Control Change (0xB0), Mod Wheel (1), Value 64
    uint8_t cc[] = { 0xB0, 1, 64 };
    parser.ProcessData(cc, sizeof(cc));

    // Example: Real-Time Clock
    uint8_t clock[] = { 0xF8 };
    parser.ProcessData(clock, sizeof(clock));

    // Example: SysEx (0xF0 ... 0xF7)
    uint8_t sysex[] = { 0xF0, 0x7D, 0x01, 0x02, 0xF7 };
    parser.ProcessData(sysex, sizeof(sysex));

    return 0;
}
