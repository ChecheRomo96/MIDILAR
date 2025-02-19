#include <Arduino.h>
#include <MIDILAR_MidiProcessors.h>  // Include VelocityShaper
#include <MIDILAR_MidiFoundation.h>

using namespace MIDILAR::MidiProcessors;

// ===============================
// SERIAL & MIDI CONFIGURATION
// ===============================
#define MIDI_BAUD_RATE 31250  // Standard MIDI baud rate
#define DEFAULT_MORPH 0.5f    // Default shaping curve parameter
#define DEFAULT_EXP_GAIN 2.0f // Default exponentiation gain

// ===============================
// Velocity Shaper Processor
// ===============================
VelocityShaper velocityShaper;

// ===============================
// MIDI Output Callback
// ===============================
// This function prints and forwards MIDI messages.
void MidiOutCallback(const uint8_t* Message, size_t size) {
    Serial.print("Modified MIDI Message: ");
    for (size_t i = 0; i < size; ++i) {
        Serial.print(Message[i], HEX);
        Serial.print(" ");
    }
    Serial.println();

    // Forward to MIDI OUT (Serial1)
    Serial1.write(Message, size);
}

// ===============================
// Arduino Setup & Loop
// ===============================
void setup() {
    Serial.begin(115200);  // Debug output to Serial Monitor
    Serial1.begin(MIDI_BAUD_RATE);  // MIDI IN/OUT

    // Bind MIDI output callback
    velocityShaper.BindMidiOut(MidiOutCallback);

    // Configure initial shaping parameters
    velocityShaper.SetInputChannels(0b1);
    velocityShaper.SetMorph(DEFAULT_MORPH);
    velocityShaper.SetExpGain(DEFAULT_EXP_GAIN);

    Serial.println("Velocity Shaper Initialized...");
    Serial.println("Use 'm0.3' to set morph to 0.3, 'e1.8' to set exp gain to 1.8.");
}

void loop() {
    // Read MIDI Data from Serial1
    if (Serial1.available()) {
        uint8_t midiByte = Serial1.read();
        velocityShaper.MidiInput(&midiByte, 1);  // Process input
    }

    // Read Serial Commands for Parameter Tuning
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');

        if (command.startsWith("m")) {
            float morph = command.substring(1).toFloat();
            velocityShaper.SetMorph(morph);
            Serial.print("Morph set to: "); Serial.println(morph);
        }
        else if (command.startsWith("e")) {
            float expGain = command.substring(1).toFloat();
            velocityShaper.SetExpGain(expGain);
            Serial.print("Exp Gain set to: "); Serial.println(expGain);
        }
    }
}
