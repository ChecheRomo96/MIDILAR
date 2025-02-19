#include <Arduino.h>
#include <MIDILAR_MidiFoundation.h>

// ===============================
// SERIAL & MIDI CONFIGURATION
// ===============================
#define MIDI_BAUD_RATE 31250  // Standard MIDI baud rate for Serial1

// ===============================
// MIDI Echo Processor Class (Base: Processor)
// ===============================
class MidiEchoProcessor : public MIDILAR::MidiFoundation::Processor {
public:
    MidiEchoProcessor() {
        SetCapabilities(static_cast<uint32_t>(Capabilities::MidiIn) |
                        static_cast<uint32_t>(Capabilities::MidiOut));
    }

    void MidiInput(const uint8_t* Message, size_t size) override {
        // Echo the message back out
        MidiOutput(Message, size);
    }
};

// ===============================
// MIDI Output Callback
// ===============================
void MidiOutCallback(const uint8_t* Message, size_t size) {
    // Send the echoed message via Serial1 (MIDI OUT)
    Serial1.write(Message, size);
}

// ===============================
// Arduino Setup & Loop
// ===============================
MidiEchoProcessor echoProcessor;  // Instantiate the MIDI echo processor

void setup() {
    Serial.begin(115200);  // Debug output to Serial Monitor
    Serial1.begin(MIDI_BAUD_RATE);  // MIDI IN/OUT on Serial1

    // Bind MIDI output callback
    echoProcessor.BindMidiOut(MidiOutCallback);

    Serial.println("MIDI Echo Processor Initialized...");
}

void loop() {
    // Check if MIDI data is available from Serial1 (MIDI IN)
    if (Serial1.available()) {
        uint8_t midiMessage[3];  // Buffer for incoming MIDI message
        size_t size = 0;

        while (Serial1.available() && size < sizeof(midiMessage)) {
            midiMessage[size++] = Serial1.read();
        }

        // Process the received MIDI message
        echoProcessor.MidiInput(midiMessage, size);
    }
}
