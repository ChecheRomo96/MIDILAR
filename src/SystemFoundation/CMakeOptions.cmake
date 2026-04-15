#################################################################################################################################
# CMake options for MIDILAR
    if(MIDILAR_FULL_BUILD)
        set(MIDILAR_SYSTEM_CALLBACK_HANDLER ON)
        set(MIDILAR_SYSTEM_CLOCK ON)
    endif()
#
#################################################################################################################################
# CallbackHandler

    option(MIDILAR_SYSTEM_CALLBACK_HANDLER "Enables the compilation of MIDILAR::SystemFoundation::CallbackHandler" ON)
    include(${CMAKE_CURRENT_LIST_DIR}/CallbackHandler/CMakeOptions.cmake)
#
##################################################################################################################################
# Clock

    option(MIDILAR_SYSTEM_CLOCK "Enables the compilation of MIDILAR::SystemFoundation::Clock" ON)
    include(${CMAKE_CURRENT_LIST_DIR}/Clock/CMakeOptions.cmake)
#
#################################################################################################################################
