######################################################################################################
# Initialize MIDILAR_SOURCES_LOCAL and HEADERS_LIST_LOCAL as an empty string list

set(MIDILAR_SOURCES_LOCAL "")
set(MIDILAR_HEADERS_LOCAL "")

# Append Sources
#list(APPEND MIDILAR_SOURCES_LOCAL "${CMAKE_CURRENT_LIST_DIR}/MIDILAR_SystemFoundation.cpp")

#
######################################################################################################
# Conditional File Inclusion

    #if(${MIDILAR_MIDI_MESSAGE})
    #    message(STATUS "MIDILAR::SystemFoundation::Message enabled")
    #    add_subdirectory(Message)
    #endif()
#
######################################################################################################
# Add subdirectory

    add_subdirectory(ChannelReassign)
    #add_subdirectory(VelocityShaper)
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
    DESTINATION "include/MIDILAR-${MIDILAR_VERSION}/MidiProcessors"
    )
#
######################################################################################################