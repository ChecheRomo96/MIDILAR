######################################################################################################
# Add Subdirectories for Tests

    # SystemFoundation
    if(${MIDILAR_SYSTEM_FOUNDATION_STATUS})
        add_subdirectory(SystemFoundation)
    endif()

    # SystemFoundation
    if(${MIDILAR_AUDIO_FOUNDATION_STATUS})
        add_subdirectory(AudioFoundation)
    endif()

    # MidiProcessor
    if(${MIDILAR_MIDI_PROCESSOR_STATUS})
        add_subdirectory(MidiProcessor)
    endif()
#
######################################################################################################