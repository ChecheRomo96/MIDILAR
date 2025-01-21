######################################################################################################
# Add Subdirectories for Tests
    #CallbackHandler
    add_subdirectory(CallbackHandler)
    
    # Clock
    if(${MIDILAR_SYSTEM_CLOCK})
        add_subdirectory(Clock)
    endif()

#
######################################################################################################
