######################################################################################################
# Add Subdirectories for Tests
    #CallbackHandler
    if(MIDILAR_MIDI_MESSAGE)
        add_subdirectory(Message)
    endif()

    # Clock
    if(MIDILAR_MIDI_PROTOCOL)
        add_subdirectory(Protocol)
    endif()

#
######################################################################################################
