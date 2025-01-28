#ifndef MIDILAR_MIDI_DEFINES_H
#define MIDILAR_MIDI_DEFINES_H

    #include "Defines_ChannelVoice.h"
    #include "Defines_ChannelMode.h"
    #include "Defines_MTC.h"

    
    // System Common Commands
    #define MIDI_SONG_POSITION_POINTER    0xF2  // Song Position Pointer
    #define MIDI_SONG_SELECT              0xF3  // Song Select
    #define MIDI_TUNING_REQUEST           0xF6  // Tuning Request

    // Realtime
    #define MIDI_REALTIME_TIMING_TICK 0xF8
    #define MIDI_REALTIME_START 0xFA
    #define MIDI_REALTIME_CONTINUE 0xFB
    #define MIDI_REALTIME_STOP 0xFC
    #define MIDI_REALTIME_ACTIVE_SENSING 0xFE
    #define MIDI_REALTIME_SYSTEM_RESET 0xFF

    //System Exclusive
    #define MIDI_SYSEX_START 0xF0
    #define MIDI_SYSEX_END 0xF7

#endif//MIDILAR_MIDI_DEFINES_H