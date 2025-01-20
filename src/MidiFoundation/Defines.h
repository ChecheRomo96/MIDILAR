#ifndef MIDILAR_MIDI_FOUNDATION_PROTOCOL_BASE_H
#define MIDILAR_MIDI_FOUNDATION_PROTOCOL_BASE_H

    // Channel Voice Messages
    #define MIDI_NOTE_OFF 0x80
    #define MIDI_NOTE_ON 0x90
    #define MIDI_AFTER_TOUCH 0xA0
    #define MIDI_CONTROL_CHANGE 0xB0
    #define MIDI_PROGRAM_CHANGE 0xC0
    #define MIDI_CHANNEL_PRESSURE 0xD0
    #define MIDI_PITCH_BEND 0xE0
    
    // Channel Mode Messages
    #define MIDI_BANK_SELECT            0x00  // Bank Select
    #define MIDI_MODULATION             0x01  // Modulation
    #define MIDI_BREATH_CONTROL         0x02  // Breath Control
    #define MIDI_FOOT_PEDAL             0x04  // Foot Pedal
    #define MIDI_PORTAMENTO             0x05  // Portamento
    #define MIDI_DATA_ENTRY_MSB         0x06  // Data Entry MSB
    #define MIDI_VOLUME                 0x07  // Volume
    #define MIDI_BALANCE                0x08  // Balance
    #define MIDI_PAN                    0x0A  // Pan
    #define MIDI_EXPRESSION             0x0B  // Expression

    #define MIDI_DATA_ENTRY_LSB         0x26  // Data Entry LSB

    #define MIDI_NRPN_DATA_INCREMENT    0x60  // NRPN Data Increment
    #define MIDI_NRPN_DATA_DECREMENT    0x61  // NRPN Data Decrement
    #define MIDI_NRPN_LSB               0x62  // NRPN LSB
    #define MIDI_NRPN_MSB               0x63  // NRPN MSB
    #define MIDI_RPN_LSB                0x64  // RPN LSB
    #define MIDI_RPN_MSB                0x65  // RPN MSB

    #define MIDI_ALL_SOUND_OFF          0x78  // All Sound Off
    #define MIDI_RESET_ALL_CONTROLLERS  0x79  // Reset All Controllers
    #define MIDI_LOCAL_CONTROL          0x7A  // Local Control
    #define MIDI_ALL_NOTES_OFF          0x7B  // All Notes Off
    #define MIDI_OMNI_MODE_OFF          0x7C  // Omni Mode Off
    #define MIDI_OMNI_MODE_ON           0x7D  // Omni Mode On
    #define MIDI_MONO_MODE_ON           0x7E  // Mono Mode On
    #define MIDI_POLY_MODE_ON           0x7F  // Poly Mode On

    // Additional Subcommands for Channel Mode Messages
    #define MIDI_LOCAL_CONTROL_OFF      0x00  // Local Control Off
    #define MIDI_LOCAL_CONTROL_ON       0x7F  // Local Control On


    // System Common Commands
    #define MIDI_SONG_POSITION_POINTER    0xF2  // Song Position Pointer
    #define MIDI_SONG_SELECT              0xF3  // Song Select
    #define MIDI_TUNING_REQUEST           0xF6  // Tuning Request

    // MTC (MIDI Time Code) Commands
    #define MIDI_MTC_QUARTER_FRAME        0xF1  // Quarter Frame
    #define MIDI_MTC_FRAMES_LSB           0x00  // Frames LSB
    #define MIDI_MTC_FRAMES_MSB           0x10  // Frames MSB
    #define MIDI_MTC_SECONDS_LSB          0x20  // Seconds LSB
    #define MIDI_MTC_SECONDS_MSB          0x30  // Seconds MSB
    #define MIDI_MTC_MINUTES_LSB          0x40  // Minutes LSB
    #define MIDI_MTC_MINUTES_MSB          0x50  // Minutes MSB
    #define MIDI_MTC_HOURS_LSB            0x60  // Hours LSB
    #define MIDI_MTC_HOURS_MSB            0x70  // Hours MSB

    // MTC Frame Rates
    #define MIDI_MTC_FRAME_RATE_24FPS            0b000  // 24 FPS
    #define MIDI_MTC_FRAME_RATE_25FPS            0b010  // 25 FPS
    #define MIDI_MTC_FRAME_RATE_30FPS_DROP_FRAME 0b100  // 30 FPS (Drop Frame)
    #define MIDI_MTC_FRAME_RATE_30FPS            0b110  // 30 FPS

    // Real-Time MIDI Messages
    #define MIDI_REALTIME_TIMING_TICK        0xF8  // Timing Tick
    #define MIDI_REALTIME_START              0xFA  // Start
    #define MIDI_REALTIME_CONTINUE           0xFB  // Continue
    #define MIDI_REALTIME_STOP               0xFC  // Stop
    #define MIDI_REALTIME_ACTIVE_SENSING     0xFE  // Active Sensing
    #define MIDI_REALTIME_SYSTEM_RESET       0xFF  // System Reset

    // MIDI Exclusive Messages
    #define MIDI_EXCLUSIVE_START             0xF0  // Exclusive Start
    #define MIDI_EXCLUSIVE_END               0xF7  // Exclusive End

#endif//MIDILAR_MIDI_FOUNDATION_PROTOCOL_BASE_H