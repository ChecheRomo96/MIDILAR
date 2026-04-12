######################################################################################################
# Store this directory as a variable called MIDILAR_SRC_DIRECTORY to reference in deeper directories

    set(${PROJECT_NAME_UPPER}_SRC_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
#
######################################################################################################
# Initialize MIDILAR_SOURCES and HEADERS_LIST as an empty string list

    set(${PROJECT_NAME_UPPER}_SOURCES "")
    set(${PROJECT_NAME_UPPER}_HEADERS "")

    set(${PROJECT_NAME_UPPER}_SOURCES_LOCAL "")
    set(${PROJECT_NAME_UPPER}_HEADERS_LOCAL "")
#
######################################################################################################
# Append Sources and Headers

    list(
        APPEND ${PROJECT_NAME_UPPER}_HEADERS_LOCAL 
        "${CMAKE_CURRENT_LIST_DIR}/MIDILAR.h" 
    )

    list(APPEND ${PROJECT_NAME_UPPER}_HEADERS ${${PROJECT_NAME_UPPER}_HEADERS_LOCAL})

#
######################################################################################################
# Add Subdirectories

    add_subdirectory(System)
    
    if(${MIDILAR_SYSTEM_FOUNDATION})
        message(STATUS "MIDILAR::SystemFoundation enabled")
        list(
            APPEND ${PROJECT_NAME_UPPER}_HEADERS_LOCAL 
            "${CMAKE_CURRENT_LIST_DIR}/MIDILAR_SystemFoundation.h"
        )
        add_subdirectory(SystemFoundation)
    endif()

    if(${MIDILAR_DSP_FOUNDATION})
        message(STATUS "MIDILAR::dspFoundation enabled")
        list(
            APPEND ${PROJECT_NAME_UPPER}_HEADERS_LOCAL 
            "${CMAKE_CURRENT_LIST_DIR}/MIDILAR_dspFoundation.h"
        )
        add_subdirectory(dspFoundation)
    endif()

    if(${MIDILAR_MIDI_FOUNDATION})
        message(STATUS "MIDILAR::MidiFoundation enabled")
        list(
            APPEND ${PROJECT_NAME_UPPER}_HEADERS_LOCAL 
            "${CMAKE_CURRENT_LIST_DIR}/MIDILAR_MidiFoundation.h"
        )
        add_subdirectory(MidiFoundation)
    endif()


    if(${MIDILAR_MIDI_DEVICES})
        message(STATUS "MIDILAR::MidiDevices enabled")
        list(
            APPEND ${PROJECT_NAME_UPPER}_HEADERS_LOCAL 
            "${CMAKE_CURRENT_LIST_DIR}/MIDILAR_MidiDevices.h"
        )
        add_subdirectory(MidiDevices)
    endif()
#
######################################################################################################
# Create the MIDILAR library
    
    add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME_UPPER}_SOURCES} ${${PROJECT_NAME_UPPER}_HEADERS})

    # Set explicit linker language in case the target ends up header-only
    set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

    message(STATUS "Created ${PROJECT_NAME} library with sources:")
    foreach(source ${${PROJECT_NAME_UPPER}_SOURCES})
        message(STATUS "  ${source}")
    endforeach()

    message(STATUS "Headers to be installed:")
    foreach(header ${${PROJECT_NAME_UPPER}_HEADERS})
        message(STATUS "  ${header}")
    endforeach()

    # Optional warning if the source list is empty (but still valid!)
    if(NOT ${PROJECT_NAME_UPPER}_SOURCES)
        message(WARNING "${PROJECT_NAME} is being built as a header-only library (no .cpp sources found).")
    endif()
#
######################################################################################################
# Add Compiler Definitions

    if(EXISTS "${CMAKE_SOURCE_DIR}/CMakeOptions.cmake")
        include(CMakeOptions.cmake)
    else()
        message(WARNING "src/CMakeOptions.cmake not found! Impossible to add compiler macro definitions to targets.")
    endif()
# 
######################################################################################################
# Update Macros List at PARENT_SCOPE

    set(${PROJECT_NAME_UPPER}_MACROS "${${PROJECT_NAME_UPPER}_MACROS}" PARENT_SCOPE)
#
######################################################################################################
# Include directories for build and install

    target_include_directories(${PROJECT_NAME}
        PUBLIC
            "$<BUILD_INTERFACE:${${PROJECT_NAME_UPPER}_SRC_DIRECTORY}>"
            "$<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>"
    )
#
######################################################################################################
# MIDILAR install process

    install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include/${PROJECT_NAME}-${${PROJECT_NAME_UPPER}_VERSION}
    )

    install(
        FILES ${${PROJECT_NAME_UPPER}_HEADERS_LOCAL}
        DESTINATION "include/${PROJECT_NAME}-${${PROJECT_NAME_UPPER}_VERSION}"
    )

    # Export targets for find_package
    install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION lib/cmake/${PROJECT_NAME}
    )
#
######################################################################################################