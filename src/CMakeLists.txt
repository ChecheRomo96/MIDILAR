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
        "${CMAKE_CURRENT_LIST_DIR}/MIDILAR_BuildSettings.h"
    )

    list(APPEND ${PROJECT_NAME_UPPER}_HEADERS ${${PROJECT_NAME_UPPER}_HEADERS_LOCAL})

#
######################################################################################################
# Add Subdirectories

    if(${MIDILAR_SYSTEM_TOOLS_STATUS})
        message(STATUS "MIDILAR System Tools enabled")
        add_subdirectory(SystemTools)
    endif()

    add_subdirectory(MidiProcessor)
#
######################################################################################################
# Create the MIDILAR library

    message(STATUS "Final MIDILAR_HEADERS: ${MIDILAR_HEADERS}")

    add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME_UPPER}_SOURCES} ${${PROJECT_NAME_UPPER}_HEADERS})

    message(STATUS "Created ${PROJECT_NAME} library with sources:")
    foreach(source ${${PROJECT_NAME_UPPER}_SOURCES})
        message(STATUS "  ${source}")
    endforeach()

    message(STATUS "Headers to be installed:")
    foreach(header ${${PROJECT_NAME_UPPER}_HEADERS})
        message(STATUS "  ${header}")
    endforeach()
#
######################################################################################################
# Add Compiler Definitions

    target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_NAME_UPPER}_VERSION="${PROJECT_VERSION}")

    # Add optional features as compile definitions and update the macros list
    if(${MIDILAR_SYSTEM_TOOLS_STATUS})
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_SYSTEM_TOOLS_ENABLED)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_SYSTEM_TOOLS_ENABLED")
        if(${MIDILAR_CLOCK_STATUS})
            message(STATUS "MIDILAR::HardwareClock enabled")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_CLOCK_ENABLED)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_CLOCK_ENABLED")
        endif()
    endif()

# Other conditional flags can be added here as needed
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
        EXPORT MIDILARTargets
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