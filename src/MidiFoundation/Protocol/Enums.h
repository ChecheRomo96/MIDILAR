#ifndef MIDILAR_MIDI_FOUNDATION_PROTOCOL_ENUMS_H
#define MIDILAR_MIDI_FOUNDATION_PROTOCOL_ENUMS_H

    #include <System/BuildSettings.h>
    #include <stdint.h>
    #include "Defines.h"

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

        enum class ControlChange : uint8_t {
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
            RpnMsb                 = MIDI_RPN_MSB                 // 0x65
        };

        enum class ChannelMode : uint8_t {
            AllSoundOff            = MIDI_ALL_SOUND_OFF,          // 0x78
            ResetAllControllers    = MIDI_RESET_ALL_CONTROLLERS,  // 0x79
            LocalControl           = MIDI_LOCAL_CONTROL,          // 0x7A
            AllNotesOff            = MIDI_ALL_NOTES_OFF,          // 0x7B
            OmniOff                = MIDI_OMNI_OFF,               // 0x7C
            OmniOn                 = MIDI_OMNI_ON,                // 0x7D
            Mono                   = MIDI_MONO_ON,                // 0x7E
            Poly                   = MIDI_POLY_ON,                // 0x7F
        };

        enum class LocalControlStatus : uint8_t{
            Off        = MIDI_LOCAL_CONTROL_OFF,      // 0x00
            On         = MIDI_LOCAL_CONTROL_ON        // 0x7F
        };

        namespace SystemCommon{
                
            static constexpr uint8_t SongPositionPointer = MIDI_SONG_POSITION_POINTER;   // 0xF2
            static constexpr uint8_t SongSelect          = MIDI_SONG_SELECT;             // 0xF3
            static constexpr uint8_t TunningRequest      = MIDI_TUNING_REQUEST;          // 0xF6
        
            namespace RealTime{
                static constexpr uint8_t TimingTick    = MIDI_REALTIME_TIMING_TICK;       // 0xF8
                static constexpr uint8_t Start         = MIDI_REALTIME_START;             // 0xFA
                static constexpr uint8_t Continue      = MIDI_REALTIME_CONTINUE;          // 0xFB
                static constexpr uint8_t Stop          = MIDI_REALTIME_STOP;              // 0xFC
                static constexpr uint8_t ActiveSensing = MIDI_REALTIME_ACTIVE_SENSING;    // 0xFE
                static constexpr uint8_t SystemReset   = MIDI_REALTIME_SYSTEM_RESET;      // 0xFF
            }

            namespace SystemExclusive{
                static constexpr uint8_t Start  = MIDI_SYSEX_START;     // 0xF0
                static constexpr uint8_t End    = MIDI_SYSEX_END;       // 0xF7
            };
        }
    }

    namespace MidiProtocol = MIDILAR::MidiFoundation::Protocol;
    namespace MidiProtocol_SystemCommon = MIDILAR::MidiFoundation::Protocol::SystemCommon;
    namespace MidiProtocol_RealTime = MIDILAR::MidiFoundation::Protocol::SystemCommon::RealTime;
    namespace MidiProtocol_SysEx = MIDILAR::MidiFoundation::Protocol::SystemCommon::SystemExclusive;


    #include "Enums_MTC.h"

#endif//MIDILAR_MIDI_FOUNDATION_PROTOCOL_ENUMS_H