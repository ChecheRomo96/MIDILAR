#include <MIDILAR_MidiFoundation.h>
#include <stdint.h>
#include <Arduino.h>

// ===============================
// Print Helper
// ===============================
void PrintHex(const char* label, const uint8_t* data, size_t size) {
    Serial.print(label);
    Serial.print(" ");
    for (size_t i = 0; i < size; ++i) {
        if (data[i] < 16) Serial.print("0");
        Serial.print(data[i], HEX);
        Serial.print(i < size - 1 ? " " : "\n");
    }
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

// ===============================
// MIDI PARSER
// ===============================
MIDILAR::MidiFoundation::MessageParser parser(64);

void setup() {
    Serial.begin(115200); // For debugging output in Serial Monitor
    Serial1.begin(31250); // For debugging output in Serial Monitor
    Serial.println("MIDI Parser Initialized");
    
    parser.BindDefaultCallback(DefaultCallback);
    parser.BindChannelVoiceCallback(ChannelVoiceCallback);
    parser.BindControlChangeCallback(ControlChangeCallback);
    parser.BindRealTimeCallback(RealTimeCallback);
    parser.BindSystemCommonCallback(SystemCommonCallback);
    parser.BindSysExCallback(SysExCallback);
    parser.BindMTCCallback(MTCCallback);
    parser.BindMSCCallback(MSCCallback);
}

void loop() {
    while (Serial1.available()) {
        uint8_t byte = Serial1.read();
        parser.ProcessData(&byte, 1);
    }
}
