######################################################################################################
# Add Subdirectories for Tests

    # SystemFoundation
    if(${MIDILAR_SYSTEM_FOUNDATION})
        add_subdirectory(SystemFoundation)
    endif()
    
    # SystemFoundation
    if(${MIDILAR_MIDI_FOUNDATION})
        add_subdirectory(MidiFoundation)
    endif()

    # SystemFoundation
    if(${MIDILAR_DSP_FOUNDATION})
        add_subdirectory(dspFoundation)
    endif()

    # MidiProcessor
    if(${MIDILAR_MIDI_PROCESSOR})
        #add_subdirectory(MidiProcessor)
    endif()
#
######################################################################################################