add_subdirectory(BuildTest)

if(${MIDILAR_SYSTEM_FOUNDATION})
    add_subdirectory(SystemFoundation)
endif()

if(${MIDILAR_DSP_FOUNDATION})
    add_subdirectory(dspFoundation)
endif()

if(${MIDILAR_MIDI_FOUNDATION})
    add_subdirectory(MidiFoundation)
endif()

if(${MIDILAR_MIDI_PROCESSORS})
    add_subdirectory(MidiProcessors)
endif()
