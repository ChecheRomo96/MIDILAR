#ifndef MIDILAR_MIDI_CORE_TOP_H
#define MIDILAR_MIDI_CORE_TOP_H

    #include <MIDILAR_BuildSettings.h>

    #if __has_include(<MidiCore/MidiCore.h>)
        #ifndef MIDILAR_MIDI_CORE
            #define MIDILAR_MIDI_CORE   
        #endif
        #include <MidiCore/MidiCore.h>
    #endif

#endif//MIDILAR_MIDI_CORE_TOP_H