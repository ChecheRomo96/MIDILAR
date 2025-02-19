#ifndef MIDILAR_MIDI_MSC_DEFINES_H
#define MIDILAR_MIDI_MSC_DEFINES_H

    #define MIDI_SYSEX_MSC_GO                        0x01  // MSC Command: Go
    #define MIDI_SYSEX_MSC_STOP                      0x02  // MSC Command: Stop
    #define MIDI_SYSEX_MSC_RESUME                    0x03  // MSC Command: Resume
    #define MIDI_SYSEX_MSC_TIMED_GO                  0x04  // MSC Command: Timed Go
    #define MIDI_SYSEX_MSC_SET                       0x05  // MSC Command: Set
    #define MIDI_SYSEX_MSC_FIRE                      0x06  // MSC Command: Fire
    #define MIDI_SYSEX_MSC_STANDBY                   0x07  // MSC Command: Standby
    #define MIDI_SYSEX_MSC_RESET                     0x08  // MSC Command: Reset
    #define MIDI_SYSEX_MSC_GO_OFF                    0x09  // MSC Command: Go Off
    #define MIDI_SYSEX_MSC_GO_STOP                   0x0A  // MSC Command: Go Stop
    #define MIDI_SYSEX_MSC_SEQUENCE                  0x10  // MSC Command: Sequence
    #define MIDI_SYSEX_MSC_ENABLE                    0x11  // MSC Command: Enable
    #define MIDI_SYSEX_MSC_DISABLE                   0x12  // MSC Command: Disable
    #define MIDI_SYSEX_MSC_SWAP                      0x13  // MSC Command: Swap
    #define MIDI_SYSEX_MSC_QUEUE                     0x14  // MSC Command: Queue
    #define MIDI_SYSEX_MSC_OPEN_CUE_LIST             0x20  // MSC Command: Open Cue List
    #define MIDI_SYSEX_MSC_CLOSE_CUE_LIST            0x21  // MSC Command: Close Cue List
    #define MIDI_SYSEX_MSC_START_CLOCK               0x30  // MSC Command: Start Clock
    #define MIDI_SYSEX_MSC_STOP_CLOCK                0x31  // MSC Command: Stop Clock
    #define MIDI_SYSEX_MSC_STEP_CLOCK                0x32  // MSC Command: Step Clock
    #define MIDI_SYSEX_MSC_SET_CLOCK                 0x33  // MSC Command: Set Clock
    #define MIDI_SYSEX_MSC_INCREMENT_CLOCK           0x34  // MSC Command: Increment Clock
    #define MIDI_SYSEX_MSC_DECREMENT_CLOCK           0x35  // MSC Command: Decrement Clock

#endif//MIDILAR_MIDI_MTC_DEFINES_H