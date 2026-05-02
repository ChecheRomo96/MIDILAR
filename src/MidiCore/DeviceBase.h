    #ifndef MIDILAR_MIDI_DEVICE_BASE_H
    #define MIDILAR_MIDI_DEVICE_BASE_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<MidiCore/DeviceBase/DeviceBase.h>)
        #define MIDILAR_MIDI_DEVICE_BASE
        #include <MidiCore/DeviceBase/DeviceBase.h>
    #endif

#endif//MIDILAR_MIDI_DEVICE_BASE_H