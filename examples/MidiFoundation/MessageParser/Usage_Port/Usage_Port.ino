#include <MIDILAR_MidiFoundation.h>
#include <stdint.h>
#include <Arduino.h>

// ===============================
// SERIAL CONFIGURATION
// ===============================
#define MIDI_BAUD_RATE 31250  // Standard MIDI baud rate for Serial1

// ===============================
// CALLBACK FUNCTIONS
// ===============================
/* Handles Channel Voice messages such as Note On, Note Off, Aftertouch, etc. */
void ChannelVoiceCallback(uint8_t* Data, size_t Size) {
    Serial.print("[Channel Voice] ");
    for (size_t i = 0; i < Size; i++) {
        Serial.print(Data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

/* Handles Control Change messages (e.g., Mod Wheel, Volume). */
void ControlChangeCallback(uint8_t* Data, size_t Size) {
    Serial.print("[Control Change] ");
    for (size_t i = 0; i < Size; i++) {
        Serial.print(Data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

/* Handles Real-Time messages such as Timing Clock and Active Sensing. */
void RealTimeCallback(uint8_t* Data, size_t Size) {
    Serial.print("[Real-Time] ");
    for (size_t i = 0; i < Size; i++) {
        Serial.print(Data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

/* Handles System Common messages such as Song Select and Tuning Request. */
void SystemCommonCallback(uint8_t* Data, size_t Size) {
    Serial.print("[System Common] ");
    for (size_t i = 0; i < Size; i++) {
        Serial.print(Data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

/* Handles System Exclusive (SysEx) messages. */
void SysExCallback(uint8_t* Data, size_t Size) {
    Serial.print("[System Exclusive] ");
    for (size_t i = 0; i < Size; i++) {
        Serial.print(Data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

/* Handles MIDI Time Code (MTC) messages. */
void MTCCallback(uint8_t* Data, size_t Size) {
    Serial.print("[MIDI Time Code] ");
    for (size_t i = 0; i < Size; i++) {
        Serial.print(Data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

/* Handles MIDI Show Control (MSC) messages. */
void MSCCallback(uint8_t* Data, size_t Size) {
    Serial.print("[MIDI Show Control] ");
    for (size_t i = 0; i < Size; i++) {
        Serial.print(Data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

/* Default callback for unhandled messages. */
void DefaultCallback(uint8_t* Data, size_t Size) {
    Serial.print("[Default Callback] ");
    for (size_t i = 0; i < Size; i++) {
        Serial.print(Data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

// ===============================
// MIDI PARSER INITIALIZATION
// ===============================
MIDILAR::MidiFoundation::MessageParser myDecoder(256);

void setup() {
    Serial.begin(115200);  // Debug output to Serial Monitor
    Serial1.begin(MIDI_BAUD_RATE); // MIDI IN/OUT communication

    // Bind MIDI Callbacks
    myDecoder.BindDefaultCallback(DefaultCallback);
    myDecoder.BindChannelVoiceCallback(ChannelVoiceCallback);
    myDecoder.BindControlChangeCallback(ControlChangeCallback);
    myDecoder.BindRealTimeCallback(RealTimeCallback);
    myDecoder.BindSystemCommonCallback(SystemCommonCallback);
    myDecoder.BindSysExCallback(SysExCallback);
    myDecoder.BindMTCCallback(MTCCallback);
    myDecoder.BindMSCCallback(MSCCallback);

    Serial.println("MIDI Parser Initialized");
}

void loop() {
    // Process incoming MIDI data from Serial1
    while (Serial1.available()) {
        uint8_t midiByte = Serial1.read();  // Read incoming MIDI byte
        myDecoder.ProcessData(&midiByte, 1);  // Process the MIDI byte
    }
}
