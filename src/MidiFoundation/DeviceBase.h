    #ifndef MIDILAR_MIDI_DEVICE_BASE_H
    #define MIDILAR_MIDI_DEVICE_BASE_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<MidiFoundation/DeviceBase/DeviceBase.h>)
        #define MIDILAR_MIDI_DEVICE_BASE
        #include <MidiFoundation/DeviceBase/DeviceBase.h>
    #endif

#endif//MIDILAR_MIDI_DEVICE_BASE_H