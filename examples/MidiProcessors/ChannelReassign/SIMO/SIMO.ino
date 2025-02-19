#include <Arduino.h>
#include <MIDILAR_MidiProcessors.h>  // Include ChannelReassign
#include <MIDILAR_MidiFoundation.h>

using namespace MIDILAR::MidiProcessors;

// ===============================
// SERIAL & MIDI CONFIGURATION
// ===============================
#define MIDI_BAUD_RATE 31250  // Standard MIDI baud rate
#define INPUT_CHANNELS  0b1  // Incoming MIDI Channel
#define OUTPUT_CHANNELS 0b101010  // Reassigned MIDI Channel

// ===============================
// Channel Reassign Processor
// ===============================
ChannelReassign channelReassignProcessor;

// ===============================
// MIDI Output Callback
// ===============================
void MidiOutCallback(const uint8_t* Message, size_t size) {
    Serial1.write(Message,size);
}

// ===============================
// Arduino Setup & Loop
// ===============================
void setup() {
    Serial.begin(115200);
    Serial1.begin(MIDI_BAUD_RATE);  // MIDI IN/OUT

    // Bind MIDI output callback to the processor
    channelReassignProcessor.BindMidiOut(MidiOutCallback);
    channelReassignProcessor.SetOutputChannels(OUTPUT_CHANNELS);
    channelReassignProcessor.SetInputChannels(INPUT_CHANNELS);

    Serial.println("Channel Reassign Processor Initialized...");
}

void loop() {
    // Read MIDI Data from Serial1
    if (Serial1.available()) {
        uint8_t midiByte = Serial1.read();
        channelReassignProcessor.MidiInput(&midiByte, 1);  // Process input
    }
}
