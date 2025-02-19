#include <MIDILAR.h>

void setup() {
    Serial.begin(115200);
    while (!Serial); // Wait for Serial to be ready (useful for some boards like ESP32, Leonardo, etc.)

    Serial.print("Using MIDILAR v");
    Serial.println(MIDILAR_VERSION);
    Serial.println();

    Serial.println("Enabled Modules:");
    Serial.println();

    bool modulesEnabled = false; // Track if any module is enabled

    #ifdef MIDILAR_SYSTEM_FOUNDATION
        modulesEnabled = true;
        Serial.println("  MIDILAR::SystemFoundation.");

        #ifdef MIDILAR_CALLBACK_HANDLER
            Serial.println("    - CallbackHandler.");
        #endif // MIDILAR_CALLBACK_HANDLER

        #ifdef MIDILAR_SYSTEM_CLOCK
            Serial.println("    - Clock.");
        #endif // MIDILAR_SYSTEM_CLOCK

        Serial.println();
    #endif // MIDILAR_SYSTEM_FOUNDATION

    #ifdef MIDILAR_AUDIO_FOUNDATION
        modulesEnabled = true;
        Serial.println("  MIDILAR::AudioFoundation.");

        #ifdef MIDILAR_LUT
            Serial.println("    - BaseLUT.");
            Serial.println("    - SineLUT.");
            Serial.println("    - LogExpLUT.");
        #endif

        Serial.println();
    #endif // MIDILAR_AUDIO_FOUNDATION

    #ifdef MIDILAR_MIDI_FOUNDATION
        modulesEnabled = true;
        Serial.println("  MIDILAR::MidiFoundation.");

        #ifdef MIDILAR_PROTOCOL_ENUMS
            Serial.println("    - Protocol Namespace.");
        #endif // MIDILAR_PROTOCOL_ENUMS
        
        #ifdef MIDILAR_MIDI_MESSAGE
            Serial.println("    - Message.");
        #endif // MIDILAR_MIDI_MESSAGE
        
        #ifdef MIDILAR_MIDI_MESSAGE_PARSER
            Serial.println("    - MessageParser.");
        #endif // MIDILAR_MIDI_MESSAGE_PARSER

        #ifdef MIDILAR_MIDI_NOTE
            Serial.println("    - Note.");
        #endif // MIDILAR_MIDI_NOTE

        #ifdef MIDILAR_MIDI_PROCESSOR
            Serial.println("    - Processor.");
        #endif // MIDILAR_MIDI_PROCESSOR

        Serial.println();
    #endif // MIDILAR_MIDI_FOUNDATION

    if (!modulesEnabled) {
        Serial.println("  No modules enabled.");
    }

    Serial.println();
}

void loop() {
    // Nothing to do in loop
}
