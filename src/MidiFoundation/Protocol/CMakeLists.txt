######################################################################################################
# Initialize MIDILAR_SOURCES_LOCAL and HEADERS_LIST_LOCAL as an empty string list

    set(MIDILAR_SOURCES_LOCAL "")
    set(MIDILAR_HEADERS_LOCAL "")
#
######################################################################################################
# File Inclusion
    ##################################################################################################
    # Append Sources
        #list(APPEND MIDILAR_SOURCES_LOCAL "${CMAKE_CURRENT_LIST_DIR}/MIDILAR_SystemFoundation.cpp")
    #
    ##################################################################################################
    # Append Headers (local to this subdirectory)
        list(APPEND MIDILAR_HEADERS_LOCAL 
            "${CMAKE_CURRENT_LIST_DIR}/Defines.h"
            "${CMAKE_CURRENT_LIST_DIR}/Defines_ChannelVoice.h"
            "${CMAKE_CURRENT_LIST_DIR}/Defines_ControlChange.h"
            "${CMAKE_CURRENT_LIST_DIR}/Defines_SystemCommon.h"
            "${CMAKE_CURRENT_LIST_DIR}/Defines_SystemExclusive.h"
            "${CMAKE_CURRENT_LIST_DIR}/Defines_RealTime.h"
            "${CMAKE_CURRENT_LIST_DIR}/Defines_MTC.h"
            "${CMAKE_CURRENT_LIST_DIR}/Defines_MSC.h"
        )
    #
    ##################################################################################################
#
######################################################################################################
# Conditional File Inclusion

    if(${MIDILAR_PROTOCOL_ENUMS})
        list(APPEND MIDILAR_HEADERS_LOCAL 
            "${CMAKE_CURRENT_LIST_DIR}/Enums.h"
            "${CMAKE_CURRENT_LIST_DIR}/Enums_MTC.h"
        )

        list(APPEND MIDILAR_SOURCES_LOCAL 
            "${CMAKE_CURRENT_LIST_DIR}/Enums_MTC.cpp"
        )
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
    DESTINATION "include/MIDILAR-${MIDILAR_VERSION}/MidiFoundation/Protocol"
    )
#
######################################################################################################