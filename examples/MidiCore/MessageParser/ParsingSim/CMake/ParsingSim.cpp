#include <MIDILAR_MidiCore.h>
#include <stdint.h>
#include <iostream>
#include <vector>

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


int main(int argc, char *argv[]) {

    std::vector<uint8_t> buffer;
    Message msg;

    // Construct MIDI messages
    msg.NoteOn(60, 120, 1);             buffer.insert(buffer.end(), msg.Buffer(), msg.Buffer() + msg.size());   InjectNoise(buffer, 3);
    msg.NoteOff(60, 120, 1);            buffer.insert(buffer.end(), msg.Buffer(), msg.Buffer() + msg.size());   InjectNoise(buffer, 3);
    msg.ControlChange(1, 64, 1);        buffer.insert(buffer.end(), msg.Buffer(), msg.Buffer() + msg.size());   InjectNoise(buffer, 3);
    msg.TimingTick();                   buffer.insert(buffer.end(), msg.Buffer(), msg.Buffer() + msg.size());   InjectNoise(buffer, 3);
    msg.SongSelect(3);                  buffer.insert(buffer.end(), msg.Buffer(), msg.Buffer() + msg.size());   InjectNoise(buffer, 3);
    msg.MTC_QuarterFrame(2, 0x3F);      buffer.insert(buffer.end(), msg.Buffer(), msg.Buffer() + msg.size());   InjectNoise(buffer, 3);
    
    uint8_t sysExData[] = { 0x7D, 0x01, 0x02, 0x03 };
    msg.SystemExclusive(sysExData, 4);  buffer.insert(buffer.end(), msg.Buffer(), msg.Buffer() + msg.size());   InjectNoise(buffer, 3);

    // Initialize parser
    MessageParser parser(4); // For this case buffer size of 4 is enough, since it is not sending real SysEx

    // ==== TEST 1 ====
    std::cout << "\n---- TEST 1: Default Callback Only ----\n";
    parser.BindDefaultCallback(DefaultCallback);
    parser.ProcessData(buffer.data(), buffer.size());
    parser.UnbindAll();

    // ==== TEST 2 ====
    std::cout << "\n---- TEST 2: All Specific Callbacks ----\n";
    parser.BindDefaultCallback(DefaultCallback);
    parser.BindChannelVoiceCallback(ChannelVoiceCallback);
    parser.BindControlChangeCallback(ControlChangeCallback);
    parser.BindRealTimeCallback(RealTimeCallback);
    parser.BindSystemCommonCallback(SystemCommonCallback);
    parser.BindSysExCallback(SysExCallback);
    parser.BindMTCCallback(MTCCallback);
    parser.BindMSCCallback(MSCCallback);
    parser.ProcessData(buffer.data(), buffer.size());
    parser.UnbindAll();

    // ==== TEST 3 ====
    std::cout << "\n---- TEST 3: Only Channel Voice Callback ----\n";
    parser.BindChannelVoiceCallback(ChannelVoiceCallback);
    parser.ProcessData(buffer.data(), buffer.size());
    parser.UnbindAll();

    // ==== TEST 4 ====
    std::cout << "\n---- TEST 4: Only Control Change Callback ----\n";
    parser.BindControlChangeCallback(ControlChangeCallback);
    parser.ProcessData(buffer.data(), buffer.size());

    // ==== TEST 5 ====
    std::cout << "\n---- TEST 5: Control Change + Default Callback ----\n";
    parser.BindControlChangeCallback(ControlChangeCallback);
    parser.BindDefaultCallback(DefaultCallback);
    parser.ProcessData(buffer.data(), buffer.size());

    std::cout << "\nAll tests complete.\n";

    return 0;
}
