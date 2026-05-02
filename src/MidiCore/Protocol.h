#ifndef MIDILAR_MIDI_PROTOCOL_H
#define MIDILAR_MIDI_PROTOCOL_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<MidiCore/Protocol/Defines.h>)
        #define MIDILAR_MIDI_PROTOCOL_DEFINES
        #include <MidiCore/Protocol/Defines.h>
    #endif
    
    #if __has_include(<MidiCore/Protocol/Enums.h>)
        #define MIDILAR_MIDI_PROTOCOL_ENUMS
        #include <MidiCore/Protocol/Enums.h>
    #endif

    if defined(MIDILAR_MIDI_PROTOCOL_ENUMS) || defined(MIDILAR_MIDI_PROTOCOL_DEFINES)
        #define MIDILAR_MIDI_PROTOCOL
    #endif

#endif//MIDILAR_MIDI_PROTOCOL_H