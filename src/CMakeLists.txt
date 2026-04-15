######################################################################################################
# Store this directory as a variable called MIDILAR_SRC_DIRECTORY to reference in deeper directories

    set(MIDILAR_SRC_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
    add_library(MIDILAR STATIC)
    set_target_properties(MIDILAR PROPERTIES LINKER_LANGUAGE CXX)    
    set_target_properties(MIDILAR PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
#
######################################################################################################
# Append Sources and Headers

    target_sources(MIDILAR PRIVATE "${CMAKE_CURRENT_LIST_DIR}/MIDILAR.h")
    midilar_add_dox("${CMAKE_CURRENT_LIST_DIR}/MIDILAR.dox")
#
######################################################################################################
# Add Subdirectories

    add_subdirectory(System)
    
    if(MIDILAR_SYSTEM_FOUNDATION)
        message(STATUS "MIDILAR::SystemFoundation enabled")
        target_sources(MIDILAR PRIVATE "${CMAKE_CURRENT_LIST_DIR}/MIDILAR_SystemFoundation.h")
        add_subdirectory(SystemFoundation)
    endif()

    if(MIDILAR_DSP_FOUNDATION)
        message(STATUS "MIDILAR::dspFoundation enabled")
        target_sources(MIDILAR PRIVATE "${CMAKE_CURRENT_LIST_DIR}/MIDILAR_dspFoundation.h")
        add_subdirectory(dspFoundation)
    endif()

    if(MIDILAR_MIDI_FOUNDATION)
        message(STATUS "MIDILAR::MidiFoundation enabled")
        target_sources(MIDILAR PRIVATE "${CMAKE_CURRENT_LIST_DIR}/MIDILAR_MidiFoundation.h")
        add_subdirectory(MidiFoundation)
    endif()


    if(MIDILAR_MIDI_DEVICES)
        message(STATUS "MIDILAR::MidiDevices enabled")
        target_sources(MIDILAR PRIVATE "${CMAKE_CURRENT_LIST_DIR}/MIDILAR_MidiDevices.h")
        add_subdirectory(MidiDevices)
    endif()
#
######################################################################################################
# Add Compiler Definitions

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CMakeMacros.cmake")
        include("${CMAKE_CURRENT_SOURCE_DIR}/CMakeMacros.cmake")
    else()
        message(WARNING "src/CMakeMacros.cmake not found! Impossible to add compiler macro definitions to targets.")
    endif()
# 
######################################################################################################
# Include directories for build and install

    target_include_directories(MIDILAR
        PUBLIC
            "$<BUILD_INTERFACE:${MIDILAR_SRC_DIRECTORY}>"
            "$<INSTALL_INTERFACE:include/MIDILAR-${PROJECT_VERSION}>"
    )
#
######################################################################################################
# MIDILAR install process

    install(TARGETS MIDILAR
        EXPORT MIDILARTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include/MIDILAR-${MIDILAR_VERSION}
    )

    install(
        FILES ${MIDILAR_HEADERS_LOCAL}
        DESTINATION "include/MIDILAR-${MIDILAR_VERSION}"
    )

    # Export targets for find_package
    install(EXPORT MIDILARTargets
        FILE MIDILARTargets.cmake
        NAMESPACE MIDILAR::
        DESTINATION lib/cmake/MIDILAR
    )
#
######################################################################################################