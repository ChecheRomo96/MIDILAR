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
        "${CMAKE_CURRENT_LIST_DIR}/SystemFoundation.h"
    )
    
    list(APPEND MIDILAR_DOX_LOCAL
        "${CMAKE_CURRENT_LIST_DIR}/SystemFoundation.dox"
    )
#
######################################################################################################
# add subdirectories

    if(MIDILAR_SYSTEM_CLOCK)
        midilar_add_macro(PUBLIC MIDILAR_SYSTEM_CLOCK)
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Clock.h"
        )

        add_subdirectory(Clock)
    endif()

    if(MIDILAR_SYSTEM_CALLBACK_HANDLER)
        midilar_add_macro(PUBLIC MIDILAR_SYSTEM_CALLBACK_HANDLER)   
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/CallbackHandler.h"
        )

        add_subdirectory(CallbackHandler)
    endif()
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
        DESTINATION "include/MIDILAR-${MIDILAR_VERSION}/SystemFoundation"
    )
#
######################################################################################################