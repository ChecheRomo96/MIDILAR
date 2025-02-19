#include <Arduino.h>
#include <MIDILAR_MidiFoundation.h>

// ===============================
// SERIAL & MIDI CONFIGURATION
// ===============================
#define MIDI_BAUD_RATE 31250  // Standard MIDI baud rate
#define MIDI_NOTE_ON 0x90  // MIDI Note On status byte
#define VELOCITY_THRESHOLD 64  // Velocity filter threshold

// ===============================
// MIDI Filter Processor Class
// ===============================
class MidiFilterProcessor : public MIDILAR::MidiFoundation::Processor {
public:
    MidiFilterProcessor() {
        SetCapabilities(static_cast<uint32_t>(Capabilities::MidiIn) |
                        static_cast<uint32_t>(Capabilities::MidiOut));
    }

    void MidiInput(const uint8_t* Message, size_t size) override {
        // Check if message is Note On and velocity is below the threshold
        if (Message[0] == MIDI_NOTE_ON && Message[2] < VELOCITY_THRESHOLD) {
            Serial.println("Filtered out MIDI message.");
            return;
        }

        // Pass through unfiltered messages
        MidiOutput(Message, size);
    }
};

// ===============================
// MIDI Output Callback
// ===============================
void MidiOutCallback(const uint8_t* Message, size_t size) {
    Serial1.write(Message, size);  // Send filtered MIDI data
    Serial.print("Filtered MIDI message passed through: ");
    for (size_t i = 0; i < size; ++i) {
        Serial.print(Message[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

// ===============================
// Arduino Setup & Loop
// ===============================
MidiFilterProcessor filterProcessor;  // Instantiate MIDI filter processor

void setup() {
    Serial.begin(115200);  // Debug output
    Serial1.begin(MIDI_BAUD_RATE);  // MIDI IN/OUT on Serial1

    // Bind MIDI output callback
    filterProcessor.BindMidiOut(MidiOutCallback);

    Serial.println("MIDI Filter Processor Initialized...");
}

void loop() {
    // Read incoming MIDI data and process it
    if (Serial1.available() >= 3) { // MIDI messages are typically 3 bytes
        uint8_t midiMessage[3];
        Serial1.readBytes(midiMessage, 3);
        filterProcessor.MidiInput(midiMessage, 3);
    }
}
