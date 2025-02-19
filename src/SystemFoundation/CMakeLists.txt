######################################################################################################
# Initialize MIDILAR_SOURCES_LOCAL and HEADERS_LIST_LOCAL as an empty string list

    set(MIDILAR_SOURCES_LOCAL "")
    set(MIDILAR_HEADERS_LOCAL "")
#
######################################################################################################
# add subdirectories

    if(${MIDILAR_SYSTEM_CLOCK})
        add_subdirectory(Clock)
    endif()

    if(${MIDILAR_CALLBACK_HANDLER})
        add_subdirectory(CallbackHandler)
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
        DESTINATION "include/MIDILAR-${MIDILAR_VERSION}/SystemFoundation"
    )
#
######################################################################################################