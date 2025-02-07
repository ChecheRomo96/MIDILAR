add_subdirectory(BuildTest)

if(${MIDILAR_SYSTEM_FOUNDATION})
    add_subdirectory(SystemFoundation)
endif()

if(${MIDILAR_AUDIO_FOUNDATION})
    add_subdirectory(AudioFoundation)
endif()

if(${MIDILAR_MIDI_FOUNDATION})
    add_subdirectory(MidiFoundation)
endif()
