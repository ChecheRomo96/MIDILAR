######################################################################################################
# Append Sources and Headers

    set(MIDILAR_SOURCES_LOCAL "")
    set(MIDILAR_HEADERS_LOCAL "")

    # Append Sources
    list(APPEND MIDILAR_SOURCES_LOCAL "${CMAKE_CURRENT_LIST_DIR}/MidiProcessor.cpp")

    # Append Headers (local to this subdirectory)
    list(APPEND MIDILAR_HEADERS_LOCAL "${CMAKE_CURRENT_LIST_DIR}/MidiProcessor.h")
#
######################################################################################################
# Add Subdirectories

    # Uncomment if there are nested subdirectories
    # add_subdirectory(SomeSubModule)

######################################################################################################
# Refresh Sources and Headers in Parent Scope

    # Pass sources and headers to parent scope
    set(MIDILAR_SOURCES "${MIDILAR_SOURCES};${MIDILAR_SOURCES_LOCAL}" PARENT_SCOPE)
    set(MIDILAR_HEADERS "${MIDILAR_HEADERS};${MIDILAR_HEADERS_LOCAL}" PARENT_SCOPE)

######################################################################################################
# MIDILAR Install Process

    # Install headers for this subdirectory
    install(
    FILES ${MIDILAR_HEADERS_LOCAL}
    DESTINATION "include/MIDILAR-${MIDILAR_VERSION}/MidiProcessor"
    )
#
######################################################################################################