######################################################################################################
# Initialize MIDILAR_SOURCES_LOCAL and HEADERS_LIST_LOCAL as an empty string list

    set(MIDILAR_SOURCES_LOCAL "")
    set(MIDILAR_PRIVATE_HEADERS_LOCAL "")
    set(MIDILAR_PUBLIC_HEADERS_LOCAL "")
    set(MIDILAR_DOX_LOCAL "")
#
######################################################################################################
# Append Headers (local to this subdirectory)

    list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
        "${CMAKE_CURRENT_LIST_DIR}/Defines.h"
        "${CMAKE_CURRENT_LIST_DIR}/Defines_ChannelVoice.h"
        "${CMAKE_CURRENT_LIST_DIR}/Defines_ControlChange.h"
        "${CMAKE_CURRENT_LIST_DIR}/Defines_SystemCommon.h"
        "${CMAKE_CURRENT_LIST_DIR}/Defines_SystemExclusive.h"
        "${CMAKE_CURRENT_LIST_DIR}/Defines_RealTime.h"
        "${CMAKE_CURRENT_LIST_DIR}/Defines_MTC.h"
        "${CMAKE_CURRENT_LIST_DIR}/Defines_MSC.h"
        "${CMAKE_CURRENT_LIST_DIR}/Enums.h"
        "${CMAKE_CURRENT_LIST_DIR}/Enums_MTC.h"
    )
    
    list(APPEND MIDILAR_SOURCES_LOCAL
        "${CMAKE_CURRENT_LIST_DIR}/Enums_MTC.cpp"
    )
    
    #list(APPEND MIDILAR_DOX_LOCAL
    #    "${CMAKE_CURRENT_LIST_DIR}/Clock.dox"
    #)
#
######################################################################################################
# Add sources to the MIDILAR target

    target_sources(MIDILAR PRIVATE
        ${MIDILAR_PUBLIC_HEADERS_LOCAL}
        ${MIDILAR_PRIVATE_HEADERS_LOCAL}
        ${MIDILAR_SOURCES_LOCAL}
    )
#
######################################################################################################
# Add sources for doxygen

    if(MIDILAR_DOCS)
        midilar_add_dox(
            ${MIDILAR_PUBLIC_HEADERS_LOCAL}
            ${MIDILAR_DOX_LOCAL}
        )
    endif()
#
######################################################################################################
# Stage headers

    midilar_stage_headers(${MIDILAR_PUBLIC_HEADERS_LOCAL})
#
######################################################################################################
# MIDILAR Install Process

    # Install headers for this subdirectory
    install(
        FILES ${MIDILAR_PUBLIC_HEADERS_LOCAL}
        DESTINATION "include/MIDILAR-${MIDILAR_VERSION}/MidiCore/Protocol"
    )
#
######################################################################################################