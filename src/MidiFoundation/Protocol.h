#ifndef MIDILAR_MIDI_PROTOCOL_H
#define MIDILAR_MIDI_PROTOCOL_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<MidiFoundation/Protocol/Defines.h>)
        #define MIDILAR_MIDI_PROTOCOL_DEFINES
        #include <MidiFoundation/Protocol/Defines.h>
    #endif
    
    #if __has_include(<MidiFoundation/Protocol/Enums.h>)
        #define MIDILAR_MIDI_PROTOCOL_ENUMS
        #include <MidiFoundation/Protocol/Enums.h>
    #endif

    if defined(MIDILAR_MIDI_PROTOCOL_ENUMS) || defined(MIDILAR_MIDI_PROTOCOL_DEFINES)
        #define MIDILAR_MIDI_PROTOCOL
    #endif

#endif//MIDILAR_MIDI_PROTOCOL_H