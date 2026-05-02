######################################################################################################
# Add Subdirectories for Tests
    #CallbackHandler
    if(MIDILAR_SYSTEM_CALLBACK_HANDLER)
        add_subdirectory(CallbackHandler)
    endif()

    # Clock
    if(MIDILAR_SYSTEM_CLOCK)
        add_subdirectory(Clock)
    endif()

#
######################################################################################################
