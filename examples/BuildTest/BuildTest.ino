#include <MIDILAR.h>

void setup() {
    Serial.begin(115200);
    while (!Serial);

    Serial.print("Using MIDILAR v");
    Serial.println(MIDILAR_VERSION);
    Serial.println();

    Serial.println("Enabled Modules:");
    Serial.println();


#ifdef MIDILAR_SYSTEM_CORE
    Serial.println("  MIDILAR::SystemCore.");

    #ifdef MIDILAR_SYSTEM_CALLBACK_HANDLER
        Serial.println("    - CallbackHandler.");
    #endif

    #ifdef MIDILAR_SYSTEM_CLOCK
        Serial.println("    - Clock.");
    #endif

    #ifdef MIDILAR_SYSTEM_RING_BUFFER
        Serial.println("    - RingBuffer.");
    #endif

    #ifdef MIDILAR_SYSTEM_TASK
        Serial.println("    - Task.");
    #endif

    #ifdef MIDILAR_SYSTEM_PERIODIC_TASK
        Serial.println("    - PeriodicTask.");
    #endif

    #ifdef MIDILAR_SYSTEM_TASK_SCHEDULER
        Serial.println("    - TaskScheduler.");
    #endif

    Serial.println();
#endif

#ifdef MIDILAR_DSP_CORE
    Serial.println("  MIDILAR::DspCore.");

        #ifdef MIDILAR_DSP_LUT
            Serial.println("    - LUT.");

        #ifdef MIDILAR_DSP_LUT1D
            Serial.println("      - LUT1D.");
        #endif

        #ifdef MIDILAR_DSP_LUT2D
            Serial.println("      - LUT2D.");
        #endif

        #ifdef MIDILAR_DSP_LUT3D
            Serial.println("      - LUT3D.");
        #endif
    #endif

    #ifdef MIDILAR_DSP_GENERATORS
        Serial.println("    - Generators.");
    #endif

    #ifdef MIDILAR_DSP_GENERATORS_PERIODIC
        Serial.println("    - Periodic Generators.");
    #endif

    #ifdef MIDILAR_DSP_GENERATORS_SHAPING
        Serial.println("    - Shaping Generators.");
    #endif

    #ifdef MIDILAR_DSP_INTERPOLATORS
        Serial.println("    - Interpolators.");
    #endif

    #ifdef MIDILAR_DSP_STREAMING
        Serial.println("    - Streaming.");
    #endif

    #ifdef MIDILAR_DSP_TRANSFORMS
        Serial.println("    - Transforms.");
    #endif

    Serial.println();
#endif

#ifdef MIDILAR_MIDI_CORE
    Serial.println("  MIDILAR::MidiCore.");

    #ifdef MIDILAR_MIDI_PROTOCOL
        Serial.println("    - Protocol.");

        #ifdef MIDILAR_MIDI_PROTOCOL_DEFINES
            Serial.println("      - Protocol Defines.");
        #endif

        #ifdef MIDILAR_MIDI_PROTOCOL_ENUMS
            Serial.println("      - Protocol Enums.");
        #endif
    #endif

    #ifdef MIDILAR_MIDI_MESSAGE
        Serial.println("    - Message.");
    #endif

    #ifdef MIDILAR_MIDI_MESSAGE_PARSER
        Serial.println("    - MessageParser.");
    #endif

    #ifdef MIDILAR_MIDI_NOTE
        Serial.println("    - Note.");
    #endif

    #ifdef MIDILAR_MIDI_DEVICE_BASE
        Serial.println("    - DeviceBase.");
    #endif

    Serial.println();
#endif

#ifdef MIDILAR_MIDI_DEVICES
    Serial.println("  MIDILAR::MidiDevices.");
    Serial.println();
#endif
}

void loop() {
}