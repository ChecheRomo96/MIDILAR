#include <MIDILAR_MidiCore.h>

using namespace MIDILAR::MidiCore;

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

  Serial.println("Running Standalone Callback Test...");

  MessageParser parser(32);
  parser.BindDefaultCallback(DefaultCallback);
  parser.BindChannelVoiceCallback(ChannelVoiceCallback);
  parser.BindControlChangeCallback(ControlChangeCallback);
  parser.BindRealTimeCallback(RealTimeCallback);
  parser.BindSystemCommonCallback(SystemCommonCallback);
  parser.BindSysExCallback(SysExCallback);
  parser.BindMTCCallback(MTCCallback);
  parser.BindMSCCallback(MSCCallback);

  Message msg;

  // Test messages
  uint8_t noteOn[] = { 0x90, 0x3C, 100 };    // Note On
  uint8_t cc[]     = { 0xB0, 1, 64 };        // Control Change
  uint8_t clock[]  = { 0xF8 };               // Real-Time Clock
  uint8_t sysex[]  = { 0xF0, 0x7D, 0x01, 0x02, 0xF7 }; // SysEx

  parser.ProcessData(noteOn, sizeof(noteOn));
  parser.ProcessData(cc, sizeof(cc));
  parser.ProcessData(clock, sizeof(clock));
  parser.ProcessData(sysex, sizeof(sysex));

  Serial.println("Standalone Callback Test complete.");
}

void loop() {
  // Nothing here
}
