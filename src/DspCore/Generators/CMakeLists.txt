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
        "${CMAKE_CURRENT_LIST_DIR}/Generators.h"
    )
    list(APPEND MIDILAR_DOX_LOCAL
        "${CMAKE_CURRENT_LIST_DIR}/Generators.dox"
    )
#
######################################################################################################
# Add Subdirectories

    if(MIDILAR_DSP_GENERATORS_ENVELOPES)
        midilar_add_macro(PUBLIC MIDILAR_DSP_GENERATORS_ENVELOPES)
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Envelopes.h"
        )

        add_subdirectory(Envelopes)
    endif()
    
    if(MIDILAR_DSP_GENERATORS_NOISE)
        midilar_add_macro(PUBLIC MIDILAR_DSP_GENERATORS_NOISE)
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Noise.h"
        )

        add_subdirectory(Noise)
    endif() 
    
    if(MIDILAR_DSP_GENERATORS_PERIODIC)
        midilar_add_macro(PUBLIC MIDILAR_DSP_GENERATORS_PERIODIC)
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Periodic.h"
        )
        
        add_subdirectory(Periodic)
    endif()
    
    if(MIDILAR_DSP_GENERATORS_SHAPING)
        midilar_add_macro(PUBLIC MIDILAR_DSP_GENERATORS_SHAPING)
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Shaping.h"
        )
        
        add_subdirectory(Shaping)
    endif()
    
    if(MIDILAR_DSP_GENERATORS_WINDOWING)
        midilar_add_macro(PUBLIC MIDILAR_DSP_GENERATORS_WINDOWING)
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Windowing.h"
        )
        
        add_subdirectory(Windowing)
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
        DESTINATION "include/MIDILAR-${MIDILAR_VERSION}/DspCore/Generators"
    )
#
######################################################################################################