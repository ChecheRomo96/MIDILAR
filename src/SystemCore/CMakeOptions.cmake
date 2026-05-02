#################################################################################################################################
# CMake options for MIDILAR
    if(MIDILAR_FULL_BUILD)
        set(MIDILAR_SYSTEM_CALLBACK_HANDLER ON)
        set(MIDILAR_SYSTEM_CLOCK ON)
    endif()
#
#################################################################################################################################
# CallbackHandler

    option(MIDILAR_SYSTEM_CALLBACK_HANDLER "Enables the compilation of MIDILAR::SystemCore::CallbackHandler" ON)
#
##################################################################################################################################
# Clock

    option(MIDILAR_SYSTEM_CLOCK "Enables the compilation of MIDILAR::SystemCore::Clock" ON)
#
##################################################################################################################################
# CallbackHandler

    option(MIDILAR_SYSTEM_RING_BUFFER "Enables the compilation of MIDILAR::SystemCore::RingBuffer" ON)
#
#################################################################################################################################
