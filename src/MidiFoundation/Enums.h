#ifndef MIDILAR_MIDI_FOUNDATION_PROTOCOL_ENUMS_H
#define MIDILAR_MIDI_FOUNDATION_PROTOCOL_ENUMS_H

    #include <MIDILAR_BuildSettings.h>
    #include <stdint.h>
    #include "Defines.h"

    #if __has_include(<string>)
        #include <string>
    #endif

    namespace MIDILAR::MidiFoundation::Protocol {

        enum class ChannelVoice : uint8_t {
            NoteOff           = MIDI_NOTE_OFF,           // 0x80
            NoteOn            = MIDI_NOTE_ON,            // 0x90
            AfterTouch        = MIDI_AFTER_TOUCH,        // 0xA0
            ControlChange     = MIDI_CONTROL_CHANGE,     // 0xB0
            ProgramChange     = MIDI_PROGRAM_CHANGE,     // 0xC0
            ChannelPressure   = MIDI_CHANNEL_PRESSURE,   // 0xD0
            PitchBend         = MIDI_PITCH_BEND          // 0xE0
        };

        enum class ChannelMode : uint8_t {
            BankSelect             = MIDI_BANK_SELECT,            // 0x00
            Modulation             = MIDI_MODULATION,             // 0x01
            BreathControl          = MIDI_BREATH_CONTROL,         // 0x02
            FootPedal              = MIDI_FOOT_PEDAL,             // 0x04
            Portamento             = MIDI_PORTAMENTO,             // 0x05
            DataEntryMsb           = MIDI_DATA_ENTRY_MSB,         // 0x06
            Volume                 = MIDI_VOLUME,                 // 0x07
            Balance                = MIDI_BALANCE,                // 0x08
            Pan                    = MIDI_PAN,                    // 0x0A
            Expression             = MIDI_EXPRESSION,             // 0x0B
            DataEntryLsb           = MIDI_DATA_ENTRY_LSB,         // 0x26
            NrpnDataIncrement      = MIDI_NRPN_DATA_INCREMENT,    // 0x60
            NrpnDataDecrement      = MIDI_NRPN_DATA_DECREMENT,    // 0x61
            NrpnLsb                = MIDI_NRPN_LSB,               // 0x62
            NrpnMsb                = MIDI_NRPN_MSB,               // 0x63
            RpnLsb                 = MIDI_RPN_LSB,                // 0x64
            RpnMsb                 = MIDI_RPN_MSB,                // 0x65
            AllSoundOff            = MIDI_ALL_SOUND_OFF,          // 0x78
            ResetAllControllers    = MIDI_RESET_ALL_CONTROLLERS,  // 0x79
            LocalControl           = MIDI_LOCAL_CONTROL,          // 0x7A
            AllNotesOff            = MIDI_ALL_NOTES_OFF,          // 0x7B
            OmniModeOff            = MIDI_OMNI_MODE_OFF,          // 0x7C
            OmniModeOn             = MIDI_OMNI_MODE_ON,           // 0x7D
            MonoModeOn             = MIDI_MONO_MODE_ON,           // 0x7E
            PolyModeOn             = MIDI_POLY_MODE_ON,           // 0x7F
            LocalControlOff        = MIDI_LOCAL_CONTROL_OFF,      // 0x00
            LocalControlOn         = MIDI_LOCAL_CONTROL_ON        // 0x7F
        };

        namespace SystemCommon{
                
            static constexpr uint8_t SongPositionPointer = MIDI_SONG_POSITION_POINTER;   // 0xF2
            static constexpr uint8_t SongSelect          = MIDI_SONG_SELECT;             // 0xF3
            static constexpr uint8_t TunningRequest      = MIDI_TUNING_REQUEST;          // 0xF6
        
           namespace MTC {

                // Time Component Enum
                enum class TimeComponent : uint8_t {
                    FramesLSB       = MIDI_MTC_FRAMES_LSB,    // 0x00
                    FramesMSB       = MIDI_MTC_FRAMES_MSB,    // 0x10
                    SecondsLSB      = MIDI_MTC_SECONDS_LSB,   // 0x20
                    SecondsMSB      = MIDI_MTC_SECONDS_MSB,   // 0x30
                    MinutesLSB      = MIDI_MTC_MINUTES_LSB,   // 0x40
                    MinutesMSB      = MIDI_MTC_MINUTES_MSB,   // 0x50
                    HoursLSB        = MIDI_MTC_HOURS_LSB,     // 0x60
                    HoursMSB        = MIDI_MTC_HOURS_MSB,     // 0x70
                };

                // Frame Rate Enum
                enum class FrameRate : uint8_t {
                    FrameRate24FPS          = MIDI_MTC_FRAME_RATE_24FPS,            // 0b000
                    FrameRate25FPS          = MIDI_MTC_FRAME_RATE_25FPS,            // 0b010
                    FrameRate30FPSDropFrame = MIDI_MTC_FRAME_RATE_30FPS_DROP_FRAME, // 0b100
                    FrameRate30FPS          = MIDI_MTC_FRAME_RATE_30FPS             // 0b110
                };
            }

            enum class RealTime : uint8_t {
                TimingTick        = MIDI_REALTIME_TIMING_TICK,       // 0xF8
                Start             = MIDI_REALTIME_START,             // 0xFA
                Continue          = MIDI_REALTIME_CONTINUE,          // 0xFB
                Stop              = MIDI_REALTIME_STOP,              // 0xFC
                ActiveSensing     = MIDI_REALTIME_ACTIVE_SENSING,    // 0xFE
                SystemReset       = MIDI_REALTIME_SYSTEM_RESET       // 0xFF
            };

            enum class SysEx : uint8_t {
                Start = MIDI_EXCLUSIVE_START,   // 0xF0
                End = MIDI_EXCLUSIVE_END,       // 0xF7
            };
        }
    }

    namespace MidiProtocol = MIDILAR::MidiFoundation::Protocol;

#endif//MIDILAR_MIDI_FOUNDATION_PROTOCOL_ENUMS_H