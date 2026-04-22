######################################################################################################
# Store this directory as a variable called MIDILAR_SRC_DIRECTORY to reference in deeper directories

    set(MIDILAR_SRC_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
    add_library(MIDILAR STATIC)
    set_target_properties(MIDILAR PROPERTIES LINKER_LANGUAGE CXX)    
    set_target_properties(MIDILAR PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

    set(MIDILAR_BUILD_INCLUDE_DIR
        "${CMAKE_BINARY_DIR}/include/MIDILAR-${MIDILAR_VERSION}"
    )
#
######################################################################################################
# Initialize MIDILAR_SOURCES_LOCAL and HEADERS_LIST_LOCAL as an empty string list

    set(MIDILAR_SOURCES_LOCAL "")
    set(MIDILAR_PRIVATE_HEADERS_LOCAL "")
    set(MIDILAR_PUBLIC_HEADERS_LOCAL "")
    set(MIDILAR_DOX_LOCAL "")
#
######################################################################################################
# Append Headers (local to this subdirectory)

    list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
        "${CMAKE_CURRENT_LIST_DIR}/MIDILAR.h"
        "${CMAKE_CURRENT_LIST_DIR}/MIDILAR_BuildSettings.h"
    )

    list(APPEND MIDILAR_SOURCES_LOCAL
        "${CMAKE_CURRENT_LIST_DIR}/MIDILAR.cpp"
    )
    
    list(APPEND MIDILAR_DOX_LOCAL
        "${CMAKE_CURRENT_LIST_DIR}/MIDILAR.dox"
    )

#
######################################################################################################
# Add sources to the MIDILAR target

    target_sources(MIDILAR PRIVATE
        ${MIDILAR_PUBLIC_HEADERS_LOCAL}
        ${MIDILAR_PRIVATE_HEADERS_LOCAL}
        ${MIDILAR_SOURCES_LOCAL}
    )
#
######################################################################################################
# add subdirectories

    if(MIDILAR_SYSTEM_FOUNDATION)
        midilar_add_macro(PUBLIC MIDILAR_SYSTEM_FOUNDATION)
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/MIDILAR_SystemFoundation.h"
        )

        add_subdirectory(SystemFoundation)
    endif()

    if(MIDILAR_DSP_FOUNDATION)
        midilar_add_macro(PUBLIC MIDILAR_DSP_FOUNDATION)   
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/MIDILAR_dspFoundation.h"
        )

        add_subdirectory(dspFoundation)
    endif()
    

    if(MIDILAR_MIDI_FOUNDATION)
        midilar_add_macro(PUBLIC MIDILAR_MIDI_FOUNDATION)   
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/MIDILAR_MidiFoundation.h"
            "${CMAKE_CURRENT_LIST_DIR}/MIDILAR_MidiProtocol.h"
        )

        add_subdirectory(MidiFoundation)
    endif()
    

    if(MIDILAR_MIDI_DEVICES)
        midilar_add_macro(PUBLIC MIDILAR_MIDI_DEVICES)   
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/MIDILAR_MidiDevices.h"
        )

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
            "$<BUILD_INTERFACE:${MIDILAR_BUILD_INCLUDE_DIR}>"
            "$<INSTALL_INTERFACE:include/MIDILAR-${PROJECT_VERSION}>"
    )
#
######################################################################################################
# Add sources for doxygen

    if(MIDILAR_DOCS)
        midilar_add_dox(
            ${MIDILAR_PUBLIC_HEADERS_LOCAL}
            ${MIDILAR_DOX_LOCAL}
        )
    endif()
#
######################################################################################################
# Stage headers

    midilar_stage_headers(${MIDILAR_PUBLIC_HEADERS_LOCAL})
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