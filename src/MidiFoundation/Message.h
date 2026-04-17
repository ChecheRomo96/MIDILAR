#ifndef MIDILAR_MIDI_MESSAGE_H
#define MIDILAR_MIDI_MESSAGE_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<MidiFoundation/Message/Message.h>)
        #define MIDILAR_MIDI_MESSAGE
        #include <MidiFoundation/Message/Message.h>
    #endif

#endif//MIDILAR_MIDI_MESSAGE_H