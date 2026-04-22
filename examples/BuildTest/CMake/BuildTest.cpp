#include <MIDILAR.h>
#include <iostream>

int main() {
    std::cout << "Using MIDILAR v" << MIDILAR_VERSION << std::endl << std::endl;

    std::cout << "Enabled Modules:" << std::endl << std::endl;

    bool modulesEnabled = false; // Track if any module is enabled

    #ifdef MIDILAR_SYSTEM_FOUNDATION
        modulesEnabled = true;
        std::cout << "  - SystemFoundation." << std::endl;
        
        #ifdef MIDILAR_SYSTEM_CALLBACK_HANDLER
            std::cout << "    - CallbackHandler." << std::endl;
        #endif // MIDILAR_CALLBACK_HANDLER

        #ifdef MIDILAR_SYSTEM_CLOCK
            std::cout << "    - System Clock." << std::endl;
        #endif // MIDILAR_SYSTEM_CLOCK

        std::cout << std::endl;
    #endif // MIDILAR_SYSTEM_FOUNDATION

    #ifdef MIDILAR_DSP_FOUNDATION
        modulesEnabled = true;
        std::cout << "  - dspFoundation." << std::endl;
        
        #ifdef MIDILAR_DSP_GENERATORS
            std::cout << "    - Generators." << std::endl;


            #ifdef MIDILAR_DSP_GENERATORS_ENVELOPES
			    std::cout << "      - Envelopes." << std::endl;
                std::cout << "        - LinearSegment." << std::endl;
                std::cout << "        - ExpSegment." << std::endl;
                std::cout << "        - LogSegment." << std::endl;
                std::cout << "        - LUTSegment." << std::endl;
            #endif // MIDILAR_DSP_GENERATORS_ENVELOPES

            #ifdef MIDILAR_DSP_GENERATORS_NOISE
                std::cout << "      - Noise." << std::endl;
                std::cout << "        - White." << std::endl;
            #endif // MIDILAR_DSP_GENERATORS_NOISE

            #ifdef MIDILAR_DSP_GENERATORS_PERIODIC
                std::cout << "      - Periodic." << std::endl;
                std::cout << "        - Sine." << std::endl;
                std::cout << "        - Saw." << std::endl;
                std::cout << "        - Triangle." << std::endl;
                std::cout << "        - Square." << std::endl;
            #endif // MIDILAR_DSP_GENERATORS_PERIODIC

            #ifdef MIDILAR_DSP_GENERATORS_SHAPING
                std::cout << "      - Shaping." << std::endl;
            #endif // MIDILAR_DSP_GENERATORS_SHAPING
                
            #ifdef MIDILAR_DSP_GENERATORS_WINDOWING
                std::cout << "      - Windowing." << std::endl;
            #endif // MIDILAR_DSP_GENERATORS_SHAPING

        #endif // MIDILAR_GENERATORS

        #ifdef MIDILAR_DSP_LUT
            std::cout << "    - LUT." << std::endl;

            #ifdef MIDILAR_DSP_LUT1D
                std::cout << "        - LUT1D." << std::endl;
            #endif // MIDILAR_DSP_LUT1D
            
            #ifdef MIDILAR_DSP_LUT2D
                std::cout << "        - LUT2D." << std::endl;
            #endif // MIDILAR_DSP_LUT2D
            
            #ifdef MIDILAR_DSP_LUT3D
                std::cout << "        - LUT3D." << std::endl;
            #endif // MIDILAR_DSP_LUT3D

            #ifdef MIDILAR_DSP_LUT_PERIODIC
                std::cout << "        - Periodic." << std::endl;
                
                std::cout << "            - Sine." << std::endl;
                std::cout << "            - Triangle." << std::endl;
                std::cout << "            - Square." << std::endl;
                std::cout << "            - Saw." << std::endl;
            #endif // MIDILAR_DSP_LUT_PERIODIC

            #ifdef MIDILAR_DSP_LUT_SHAPING
                std::cout << "        - Shaping." << std::endl;

                std::cout << "            - LogExp1D." << std::endl;
                std::cout << "            - LogExp2D." << std::endl;
                std::cout << "            - LogExp3D." << std::endl;
            #endif // MIDILAR_DSP_LUT_SHAPING
        #endif // MIDILAR_DSP_LUT

        #ifdef MIDILAR_DSP_INTERPOLATORS
		    std::cout << "    - Interpolators." << std::endl;
        #endif // MIDILAR_DSP_INTERPOLATORS


        std::cout << std::endl;

    #endif // MIDILAR_DSP_FOUNDATION

    #ifdef MIDILAR_MIDI_FOUNDATION
        modulesEnabled = true;
        std::cout << "  - MidiFoundation." << std::endl;

        std::cout << "    - Protocol Enums." << std::endl;
        std::cout << "    - Message." << std::endl;
        std::cout << "    - MessageParser." << std::endl;
        std::cout << "    - Note." << std::endl;
        std::cout << "    - DeviceBase." << std::endl;
        std::cout << std::endl;
    #endif // MIDILAR_MIDI_FOUNDATION

    
    #ifdef MIDILAR_MIDI_DEVICES
        modulesEnabled = true;
        std::cout << "  - MidiDevices." << std::endl;

        #ifdef MIDILAR_MIDI_DEVICES_CHANNEL_REASSIGN
            std::cout << "    - Channel Reassign." << std::endl;
        #endif // MIDILAR_MIDI_DEVICES_CHANNEL_REASSIGN
        
        #ifdef MIDILAR_MIDI_DEVICES_VELOCITY_SHAPER
            std::cout << "    - Velocity Shaper." << std::endl;
        #endif // MIDILAR_MIDI_DEVICES_VELOCITY_SHAPER
        

    #endif // MIDILAR_MIDI_DEVICES

    if (!modulesEnabled) {
        std::cout << "  No modules enabled." << std::endl;
    }

    std::cout << std::endl << std::endl;

    return 0;
}