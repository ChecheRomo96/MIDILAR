######################################################################################################
# Initialize MIDILAR_SOURCES_LOCAL and HEADERS_LIST_LOCAL as an empty string list

    set(MIDILAR_SOURCES_LOCAL "")
    set(MIDILAR_HEADERS_LOCAL "")

    # Append Sources
    #list(APPEND MIDILAR_SOURCES_LOCAL "${CMAKE_CURRENT_LIST_DIR}/MIDILAR_SystemFoundation.cpp")

    # Append Headers (local to this subdirectory)
    list(APPEND MIDILAR_HEADERS_LOCAL "${CMAKE_CURRENT_LIST_DIR}/Defines.h")
#
######################################################################################################
# Conditional File Inclusion

    if(${MIDILAR_PROTOCOL_ENUMS})
        list(APPEND MIDILAR_HEADERS_LOCAL "${CMAKE_CURRENT_LIST_DIR}/Enums.h")
    endif()

    if(${MIDILAR_MIDI_MESSAGE})
        list(APPEND MIDILAR_HEADERS_LOCAL "${CMAKE_CURRENT_LIST_DIR}/Message.h")
        list(APPEND MIDILAR_SOURCES_LOCAL "${CMAKE_CURRENT_LIST_DIR}/Message.cpp")
    endif()

    if(${MIDILAR_MIDI_PROCESSOR})
        list(APPEND MIDILAR_HEADERS_LOCAL "${CMAKE_CURRENT_LIST_DIR}/Processor.h")
        list(APPEND MIDILAR_SOURCES_LOCAL "${CMAKE_CURRENT_LIST_DIR}/Processor.cpp")
    endif()
#
######################################################################################################
# Refresh Sources and Headers in Parent Scope

    set(MIDILAR_SOURCES "${MIDILAR_SOURCES};${MIDILAR_SOURCES_LOCAL}" PARENT_SCOPE)
    set(MIDILAR_HEADERS "${MIDILAR_HEADERS};${MIDILAR_HEADERS_LOCAL}" PARENT_SCOPE)
#
######################################################################################################
# MIDILAR Install Process

    # Install headers for this subdirectory
    install(
    FILES ${MIDILAR_HEADERS_LOCAL}
    DESTINATION "include/MIDILAR-${MIDILAR_VERSION}/MidiFoundation"
    )
#
######################################################################################################