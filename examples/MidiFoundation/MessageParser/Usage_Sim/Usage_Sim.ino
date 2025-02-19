#include <MIDILAR_MidiFoundation.h>
#include <stdint.h>
#include <Arduino.h>

// ===============================
// CALLBACK FUNCTIONS
// ===============================
// Each function below is a callback that handles a specific type of MIDI message.
// These functions print the message type along with its data when called.

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

// **MIDI Message Buffer**
uint8_t MessageBuffer[128];
uint8_t MessageIndex = 0;

// Function to add MIDI data to the buffer
void addToBuffer(const uint8_t* Data, size_t size) {
    memcpy(MessageBuffer + MessageIndex, Data, size);
    MessageIndex += size;
}

// ===============================
// MIDI PARSER
// ===============================
MIDILAR::MidiFoundation::MessageParser myDecoder(256);

void setup() {
    Serial.begin(115200); // For debugging output in Serial Monitor
    Serial.println("MIDI Parser Initialized");

    // Fill Buffer
    MIDILAR::MidiFoundation::Message msg;
    
    // **Create and store a Note On MIDI message**
    msg.NoteOn(60, 120, 1);  // NoteOn: Note 60 (Middle C), Velocity 120, Channel 1
    addToBuffer(msg.Buffer(), msg.size());

    // **Create and store a Note Off MIDI message**
    msg.NoteOff(60, 120, 1);  // NoteOff: Same note, same velocity, same channel
    addToBuffer(msg.Buffer(), msg.size());

    // **Create and store a Control Change message (e.g., Mod Wheel)**
    msg.ControlChange(1, 64, 1);  // Control Change: CC 1 (Modulation), Value 64, Channel 1
    addToBuffer(msg.Buffer(), msg.size());

    // **Create and store a Real-Time Timing Clock message**
    msg.TimingTick();  // Used for MIDI clock synchronization
    addToBuffer(msg.Buffer(), msg.size());

    // **Create and store a System Common Song Select message**
    msg.SongSelect(3);  // Select Song number 3
    addToBuffer(msg.Buffer(), msg.size());

    // **Create and store a MIDI Time Code Quarter Frame message**
    msg.MTC_QuarterFrame(2, 0x3F);
    addToBuffer(msg.Buffer(), msg.size());

    // **Create and store a System Exclusive (SysEx) message**
    uint8_t sysExData[] = { 0x7D, 0x01, 0x02, 0x03 };  // Example SysEx payload
    msg.SystemExclusive(sysExData, sizeof(sysExData));
    addToBuffer(msg.Buffer(), msg.size());

    // ===============================
    // Using Default Callback Only
    // ===============================
    Serial.println("Default Callback Only");
    myDecoder.BindDefaultCallback(DefaultCallback);
    myDecoder.ProcessData(MessageBuffer, MessageIndex);
}

void loop() {

}