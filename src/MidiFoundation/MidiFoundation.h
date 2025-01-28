#ifndef MIDILAR_MIDI_FOUNDATION_H
#define MIDILAR_MIDI_FOUNDATION_H

    #include <MIDILAR_BuildSettings.h>

    #include "Protocol/Defines.h"
    
    /**
     * @namespace MIDILAR::MidiFoundation
     * @brief Contains classes and utilities for handling MIDI messages.
     */
    namespace MIDILAR::MidiFoundation{}

    #ifdef MIDILAR_PROTOCOL_ENUMS
        #include "Protocol/Enums.h"
    #endif

    #ifdef MIDILAR_MIDI_MESSAGE
        #include "Message.h"
    #endif

    #ifdef MIDILAR_MIDI_PROCESSOR
        #include "Processor.h"
    #endif

#endif//MIDILAR_MIDI_FOUNDATION_H