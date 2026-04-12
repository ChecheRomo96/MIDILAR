#include <MIDILAR_MidiFoundation.h>

using namespace MIDILAR::MidiFoundation;

#define BUFFER_SIZE 128
uint8_t buffer[BUFFER_SIZE];
size_t bufferLen = 0;

Message msg;

// ===============================
// Noise Injection
// ===============================
void InjectNoise(uint8_t* buf, size_t& len, size_t count) {
    for (size_t i = 0; i < count && len < BUFFER_SIZE; ++i) {
        buf[len++] = static_cast<uint8_t>(rand() & 0xFF);
    }
}

// ===============================
// Print Helper
// ===============================
void PrintHex(const char* label, const uint8_t* data, size_t size) {
    Serial.print(label);
    Serial.print(" ");
    for (size_t i = 0; i < size; ++i) {
        if (data[i] < 0x10) Serial.print("0");
        Serial.print(data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
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

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Running ParsingSim Test...");

    // Build buffer
    msg.NoteOn(60, 120, 1);
    memcpy(buffer + bufferLen, msg.Buffer(), msg.size()); bufferLen += msg.size();
    InjectNoise(buffer, bufferLen, 3);

    msg.NoteOff(60, 120, 1);
    memcpy(buffer + bufferLen, msg.Buffer(), msg.size()); bufferLen += msg.size();
    InjectNoise(buffer, bufferLen, 3);

    msg.ControlChange(1, 64, 1);
    memcpy(buffer + bufferLen, msg.Buffer(), msg.size()); bufferLen += msg.size();
    InjectNoise(buffer, bufferLen, 3);
    
    msg.TimingTick();
    memcpy(buffer + bufferLen, msg.Buffer(), msg.size()); bufferLen += msg.size();
    InjectNoise(buffer, bufferLen, 3);
    
    msg.SongSelect(3);
    memcpy(buffer + bufferLen, msg.Buffer(), msg.size()); bufferLen += msg.size();
    InjectNoise(buffer, bufferLen, 3);
    
    msg.MTC_QuarterFrame(2, 0x3F);
    memcpy(buffer + bufferLen, msg.Buffer(), msg.size()); bufferLen += msg.size();
    InjectNoise(buffer, bufferLen, 3);
    
    uint8_t sysExData[] = { 0x7D, 0x01, 0x02, 0x03 };
    msg.SystemExclusive(sysExData, 4);
    memcpy(buffer + bufferLen, msg.Buffer(), msg.size()); bufferLen += msg.size();
    InjectNoise(buffer, bufferLen, 3);
    
    MessageParser parser(4);  // Minimal buffer size is fine

    // ==== TEST 1 ====
    Serial.println("\n---- TEST 1: Default Callback Only ----");
    parser.BindDefaultCallback(DefaultCallback);
    parser.ProcessData(buffer, bufferLen);
    parser.UnbindAll();

    // ==== TEST 2 ====
    Serial.println("\n---- TEST 2: All Specific Callbacks ----");
    parser.BindDefaultCallback(DefaultCallback);
    parser.BindChannelVoiceCallback(ChannelVoiceCallback);
    parser.BindControlChangeCallback(ControlChangeCallback);
    parser.BindRealTimeCallback(RealTimeCallback);
    parser.BindSystemCommonCallback(SystemCommonCallback);
    parser.BindSysExCallback(SysExCallback);
    parser.BindMTCCallback(MTCCallback);
    parser.BindMSCCallback(MSCCallback);
    parser.ProcessData(buffer, bufferLen);
    parser.UnbindAll();

    // ==== TEST 3 ====
    Serial.println("\n---- TEST 3: Only Channel Voice Callback ----");
    parser.BindChannelVoiceCallback(ChannelVoiceCallback);
    parser.ProcessData(buffer, bufferLen);
    parser.UnbindAll();

    // ==== TEST 4 ====
    Serial.println("\n---- TEST 4: Only Control Change Callback ----");
    parser.BindControlChangeCallback(ControlChangeCallback);
    parser.ProcessData(buffer, bufferLen);

    // ==== TEST 5 ====
    Serial.println("\n---- TEST 5: Control Change + Default Callback ----");
    parser.BindControlChangeCallback(ControlChangeCallback);
    parser.BindDefaultCallback(DefaultCallback);
    parser.ProcessData(buffer, bufferLen);

    Serial.println("\nParsingSim Test complete.");
}

void loop() {
    // Nothing here
}
