######################################################################################################
# Initialize MIDILAR_SOURCES_LOCAL and HEADERS_LIST_LOCAL as an empty string list

    set(MIDILAR_SOURCES_LOCAL "")
    set(MIDILAR_HEADERS_LOCAL "")

    # Append Sources and headers
    list(APPEND MIDILAR_HEADERS_LOCAL "${CMAKE_CURRENT_LIST_DIR}/MessageParser.h")
    list(APPEND MIDILAR_SOURCES_LOCAL "${CMAKE_CURRENT_LIST_DIR}/MessageParser.cpp")

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
    DESTINATION "include/MIDILAR-${MIDILAR_VERSION}/MidiFoundation/MessageParser"
    )
#
######################################################################################################