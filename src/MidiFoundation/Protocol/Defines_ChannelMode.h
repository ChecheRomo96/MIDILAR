#ifndef MIDILAR_PROTOCOL_CHANNEL_MODE_DEFINES_H
#define MIDILAR_PROTOCOL_CHANNEL_MODE_DEFINES_H

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Channel Mode Messages

        #define MIDI_CM_ALL_SOUND_OFF          0x78  // All Sound Off
        #define MIDI_CM_RESET_ALL_CONTROLLERS  0x79  // Reset All Controllers
        #define MIDI_CM_LOCAL_CONTROL          0x7A  // Local Control
        #define MIDI_CM_ALL_NOTES_OFF          0x7B  // All Notes Off
        #define MIDI_CM_OMNI_OFF          0x7C  // Omni Mode Off
        #define MIDI_CM_OMNI_ON           0x7D  // Omni Mode On
        #define MIDI_CM_MONO_ON           0x7E  // Mono Mode On
        #define MIDI_CM_POLY_ON           0x7F  // Poly Mode On

        // Additional Subcommands for Channel Mode Messages
        #define MIDI_CM_LOCAL_CONTROL_OFF      0x00  // Local Control Off
        #define MIDI_CM_LOCAL_CONTROL_ON       0x7F  // Local Control On

    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif//MIDILAR_PROTOCOL_CHANNEL_MODE_DEFINES_H