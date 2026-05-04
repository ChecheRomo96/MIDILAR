#ifndef MIDILAR_MIDI_PROTOCOL_TOP_H
#define MIDILAR_MIDI_PROTOCOL_TOP_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<MidiCore/Protocol/Defines.h>)
        #ifndef MIDILAR_MIDI_PROTOCOL
            #define MIDILAR_MIDI_PROTOCOL
        #endif
        
        #ifndef MIDILAR_MIDI_PROTOCOL_DEFINES
            #define MIDILAR_MIDI_PROTOCOL_DEFINES
        #endif
        #include <MidiCore/Protocol/Defines.h>

        
        #if __has_include(<MidiCore/Protocol/Enums.h>)
            #ifndef MIDILAR_MIDI_PROTOCOL_ENUMS
                #define MIDILAR_MIDI_PROTOCOL_ENUMS
            #endif
            #include <MidiCore/Protocol/Enums.h>
        #endif
    #endif

#endif//MIDILAR_MIDI_PROTOCOL_TOP_H