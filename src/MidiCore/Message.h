#ifndef MIDILAR_MIDI_MESSAGE_H
#define MIDILAR_MIDI_MESSAGE_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<MidiCore/Message/Message.h>)
        #define MIDILAR_MIDI_MESSAGE
        #include <MidiCore/Message/Message.h>
    #endif

#endif//MIDILAR_MIDI_MESSAGE_H