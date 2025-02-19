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
        "${CMAKE_CURRENT_LIST_DIR}/BuildSettings.h"
    )

    list(APPEND ${PROJECT_NAME_UPPER}_HEADERS ${${PROJECT_NAME_UPPER}_HEADERS_LOCAL})

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
    DESTINATION "include/MIDILAR-${MIDILAR_VERSION}/System"
    )
#
######################################################################################################