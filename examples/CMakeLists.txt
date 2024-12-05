add_subdirectory(BuildTest)

if(${MIDILAR_SYSTEM_FOUNDATION_STATUS})
    add_subdirectory(SystemFoundation)
endif()

if(${MIDILAR_AUDIO_FOUNDATION_STATUS})
    add_subdirectory(AudioFoundation)
endif()

add_subdirectory(MidiProcessor)