#include <MIDILAR.h>
#include <iostream>

int main() {
    std::cout << "Using MIDILAR v" << MIDILAR_VERSION << std::endl << std::endl;

    std::cout << "Enabled Modules:" << std::endl << std::endl;

    bool modulesEnabled = false; // Track if any module is enabled

    #ifdef MIDILAR_SYSTEM_FOUNDATION
        modulesEnabled = true;
        std::cout << "  MIDILAR::SystemFoundation." << std::endl;

        #ifdef MIDILAR_SYSTEM_CLOCK
            std::cout << "    - Clock." << std::endl;
        #endif // MIDILAR_SYSTEM_CLOCK

        std::cout << std::endl;
    #endif // MIDILAR_SYSTEM_FOUNDATION

    #ifdef MIDILAR_AUDIO_FOUNDATION
        modulesEnabled = true;
        std::cout << "  MIDILAR::AudioFoundation." << std::endl;
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

    MIDILAR::MidiFoundation::Message msg;

        uint8_t Program = 0;  // Initialize the program number (0–255, wraps around)
        uint8_t Channel = 0;  // Initialize the MIDI channel (0–255, wraps around)

        // Outer loop: Iterate through all possible `uint8_t` values for the channel
        do {
            // Inner loop: Iterate through all possible `uint8_t` values for the program number
            do {
                // Construct the Program Change message for the current Program and Channel
                msg.ProgramChange(Program, Channel);

                std::cout << (uint32_t) msg.Data(0) << " - " << (uint32_t) MIDI_PROGRAM_CHANGE + ((Channel < 16) ? Channel : 15) << " --- ";
                std::cout << (uint32_t) msg.Data(1) << " - " << (uint32_t) ((Program < 128) ? Program : 127) << std::endl;

                ++Program;
            }while( Program != 130 );
            ++Channel;
        }while( Channel != 17 );
    

    return 0;
}