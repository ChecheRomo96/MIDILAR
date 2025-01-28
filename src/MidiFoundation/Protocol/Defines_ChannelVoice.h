#ifndef MIDILAR_PROTOCOL_CHANNEL_VOICE_DEFINES_H
#define MIDILAR_PROTOCOL_CHANNEL_VOICE_DEFINES_H

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Channel Voice Messages

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
        /**
         * @ingroup NoteMessages
         * @brief Note Off message (0x80)
         * 
         * Sent when a key is released on a MIDI instrument. This command includes the key number 
         * and the release velocity, which can optionally be used by the receiving device.
         */
            #define MIDI_NOTE_OFF 0x80
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @ingroup NoteMessages
         * @brief Note On message (0x90)
         * 
         * Sent when a key is pressed on a MIDI instrument. This command includes the key number 
         * and the velocity, which determines how hard the key was pressed.
         */
            #define MIDI_NOTE_ON 0x90
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @ingroup AfterTouch
         * @brief After Touch (Polyphonic Key Pressure) message (0xA0)
         * 
         * Sent to indicate the pressure applied to a specific key after it has been pressed. 
         * Useful for creating expressive performances.
         */
            #define MIDI_AFTER_TOUCH 0xA0
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @ingroup ControlChange
         * @brief Control Change message (0xB0)
         * 
         * Used to modify parameters of a MIDI instrument or effect, such as volume, pan, or modulation. 
         * Includes a controller number and a value.
         */
            #define MIDI_CONTROL_CHANGE 0xB0
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @ingroup ProgramChange
         * @brief Program Change message (0xC0)
         * 
         * Used to change the instrument or sound being played by a MIDI device. 
         * Typically associated with selecting patches or presets.
         */
            #define MIDI_PROGRAM_CHANGE 0xC0
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @ingroup ChannelPressure
         * @brief Channel Pressure (Aftertouch) message (0xD0)
         * 
         * Similar to polyphonic aftertouch, but applies to the entire MIDI channel rather than individual notes. 
         * Indicates how much pressure is being applied to the keys.
         */
            #define MIDI_CHANNEL_PRESSURE 0xD0
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @ingroup PitchBend
         * @brief Pitch Bend Change message (0xE0)
         * 
         * Used to bend the pitch of a note up or down. Includes a 14-bit value for high precision. 
         * Commonly used for vibrato or portamento effects.
         */
            #define MIDI_PITCH_BEND 0xE0    
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    #include "Defines_ControlChange.h"

#endif//MIDILAR_PROTOCOL_CHANNEL_VOICE_DEFINES_H