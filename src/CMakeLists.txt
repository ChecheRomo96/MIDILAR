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


    if(${MIDILAR_MIDI_PROCESSORS})
        message(STATUS "MIDILAR::MidiProcessors enabled")
        list(
            APPEND ${PROJECT_NAME_UPPER}_HEADERS_LOCAL 
            "${CMAKE_CURRENT_LIST_DIR}/MIDILAR_MidiProcessors.h"
        )
        add_subdirectory(MidiProcessors)
    endif()
#
######################################################################################################
# Create the MIDILAR library
    
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
    if(${MIDILAR_SYSTEM_FOUNDATION})
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_SYSTEM_FOUNDATION)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_SYSTEM_FOUNDATION")

        if(${MIDILAR_CALLBACK_HANDLER})
            message(STATUS "MIDILAR::SystemFoundation::CallbackHandler enabled")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_CALLBACK_HANDLER)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_CALLBACK_HANDLER")
        endif()
        
        if(${MIDILAR_SYSTEM_CLOCK})
            message(STATUS "MIDILAR::SystemFoundation::Clock enabled")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_SYSTEM_CLOCK)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_SYSTEM_CLOCK")
        endif()

    endif()

    if(${MIDILAR_MIDI_FOUNDATION})
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_MIDI_FOUNDATION)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_MIDI_FOUNDATION")

        if(${MIDILAR_PROTOCOL_ENUMS})
            message(STATUS "MIDILAR::MidiFoundation::Protocol enabled")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_PROTOCOL_ENUMS)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_PROTOCOL_ENUMS")
        endif()
        
        if(${MIDILAR_MIDI_MESSAGE})
            message(STATUS "MIDILAR::MidiFoundation::Message enabled")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_MIDI_MESSAGE)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_MIDI_MESSAGE")
        endif()
        
        if(${MIDILAR_MIDI_MESSAGE_PARSER})
            message(STATUS "MIDILAR::MidiFoundation::MessageParser enabled")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_MIDI_MESSAGE_PARSER)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_MIDI_PROCESSOR")
        endif()
        
        if(${MIDILAR_MIDI_PROCESSOR})
            message(STATUS "MIDILAR::MidiFoundation::Processor enabled")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_MIDI_PROCESSOR)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_MIDI_PROCESSOR")
        endif()

    endif()
    
    if(${MIDILAR_DSP_FOUNDATION})
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_DSP_FOUNDATION)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_DSP_FOUNDATION")
        
        if(${MIDILAR_LUT})
            message(STATUS "MIDILAR::dspFoundation::LUT enabled")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_LUT)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_LUT")
        endif()
    endif()

    
    if(${MIDILAR_MIDI_PROCESORS})
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_MIDI_PROCESORS)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_MIDI_PROCESORS")
        
        if(${MIDILAR_LUT})

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