#include <MIDILAR_MidiFoundation.h>
#include <stdint.h>
#include <Arduino.h>

void setup() {
    Serial.begin(31250); // Standard MIDI baud rate
    Serial.println("MIDI Transmission Started");

    using namespace MIDILAR::MidiFoundation;

    // Create and send a Note On message (Middle C, Velocity 120, Channel 1)
    Message noteOnMessage;
    noteOnMessage.NoteOn(60, 120, 1);
    sendMIDI(noteOnMessage, "Note On");

    delay(500); // Wait for half a second

    // Create and send a Note Off message
    Message noteOffMessage;
    noteOffMessage.NoteOff(60, 0, 1);
    sendMIDI(noteOffMessage, "Note Off");

    delay(500); // Wait for half a second

    // Create and send a Control Change message (Modulation Wheel on Channel 1)
    Message controlChangeMessage;
    controlChangeMessage.ControlChange(1, 64, 1);
    sendMIDI(controlChangeMessage, "Control Change (Modulation)");

    delay(500);

    // Create and send a Program Change message (Switch to instrument #10 on Channel 1)
    Message programChangeMessage;
    programChangeMessage.ProgramChange(10, 1);
    sendMIDI(programChangeMessage, "Program Change");

    delay(500);

    // Create and send a Pitch Bend message (Centered Pitch Bend on Channel 1)
    Message pitchBendMessage;
    pitchBendMessage.PitchBend(8192, 1); // 8192 = Center
    sendMIDI(pitchBendMessage, "Pitch Bend");

    delay(500);

    // Create and send a System Exclusive (SysEx) message
    uint8_t sysExData[] = { 0x7E, 0x7F, 0x09, 0x01 }; // Example SysEx data
    Message sysExMessage;
    sysExMessage.SystemExclusive(sysExData, sizeof(sysExData));
    sendMIDI(sysExMessage, "SysEx");

    Serial.println("MIDI Messages Sent");
}

void loop() {
    // Nothing in the loop, since this is a one-time message sender
}

// Function to send a MIDI message via Serial
void sendMIDI(const MIDILAR::MidiFoundation::Message& msg, const char* description) {
    Serial.print(description);
    Serial.print(" Message: ");

    for (size_t i = 0; i < msg.size(); i++) {
        Serial.print(msg.Buffer()[i], HEX);
        if (i < msg.size() - 1) {
            Serial.print(", ");
        }
        Serial1.write(msg.Buffer()[i]); // Send MIDI data via Serial1
    }

    Serial.println();
}
