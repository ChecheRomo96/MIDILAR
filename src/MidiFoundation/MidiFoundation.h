#ifndef MIDILAR_MIDI_FOUNDATION_H
#define MIDILAR_MIDI_FOUNDATION_H

    #include <System/BuildSettings.h>

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
        #include "Message/Message.h"
    #endif

    #ifdef MIDILAR_MIDI_PROCESSOR
        #include "Processor/Processor.h"
    #endif

    #ifdef MIDILAR_MIDI_MESSAGE_PARSER
        #include "MessageParser/MessageParser.h"
    #endif

#endif//MIDILAR_MIDI_FOUNDATION_H