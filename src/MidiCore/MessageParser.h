#ifndef MIDILAR_MIDI_MESSAGE_PARSER_TOP_H
#define MIDILAR_MIDI_MESSAGE_PARSER_TOP_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<MidiCore/MessageParser/MessageParser.h>)
        #define MIDILAR_MIDI_MESSAGE_PARSER
        #include <MidiCore/MessageParser/MessageParser.h>
    #endif

#endif//MIDILAR_MIDI_MESSAGE_PARSER_TOP_H