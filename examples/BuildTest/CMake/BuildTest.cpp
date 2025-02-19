#include <MIDILAR.h>
#include <iostream>

int main() {
    std::cout << "Using MIDILAR v" << MIDILAR_VERSION << std::endl << std::endl;

    std::cout << "Enabled Modules:" << std::endl << std::endl;

    bool modulesEnabled = false; // Track if any module is enabled

    #ifdef MIDILAR_SYSTEM_FOUNDATION
        modulesEnabled = true;
        std::cout << "  MIDILAR::SystemFoundation." << std::endl;

        #ifdef MIDILAR_CALLBACK_HANDLER
            std::cout << "    - CallbackHandler." << std::endl;
        #endif // MIDILAR_SYSTEM_CLOCK

        #ifdef MIDILAR_SYSTEM_CLOCK
            std::cout << "    - Clock." << std::endl;
        #endif // MIDILAR_SYSTEM_CLOCK

        std::cout << std::endl;
    #endif // MIDILAR_SYSTEM_FOUNDATION

    #ifdef MIDILAR_AUDIO_FOUNDATION
        modulesEnabled = true;
        std::cout << "  MIDILAR::AudioFoundation." << std::endl;
        
        #ifdef MIDILAR_LUT
            std::cout << "    - BaseLUT." << std::endl;
            std::cout << "    - SineLUT." << std::endl;
            std::cout << "    - LogExpLUT." << std::endl;
        #endif

        std::cout << std::endl;
    #endif // MIDILAR_AUDIO_FOUNDATION

    #ifdef MIDILAR_MIDI_FOUNDATION
        modulesEnabled = true;
        std::cout << "  MIDILAR::MidiFoundation." << std::endl;

        #ifdef MIDILAR_PROTOCOL_ENUMS
            std::cout << "    - Protocol Namespace." << std::endl;
        #endif // MIDILAR_MIDI_ENUMS
        
        #ifdef MIDILAR_MIDI_MESSAGE
            std::cout << "    - Message." << std::endl;
        #endif // MIDILAR_MIDI_MESSAGE
        
        #ifdef MIDILAR_MIDI_MESSAGE_PARSER
            std::cout << "    - MessageParser." << std::endl;
        #endif // MIDILAR_MIDI_MESSAGE

        #ifdef MIDILAR_MIDI_NOTE
            std::cout << "    - Note." << std::endl;
        #endif // MIDILAR_MIDI_MESSAGE

        #ifdef MIDILAR_MIDI_PROCESSOR
            std::cout << "    - Processor." << std::endl;
        #endif // MIDILAR_MIDI_PROCESSOR

        std::cout << std::endl;
    #endif // MIDILAR_MIDI_FOUNDATION

    if (!modulesEnabled) {
        std::cout << "  No modules enabled." << std::endl;
    }

    std::cout << std::endl << std::endl;

    return 0;
}