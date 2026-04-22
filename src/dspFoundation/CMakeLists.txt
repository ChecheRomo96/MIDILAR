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
        "${CMAKE_CURRENT_LIST_DIR}/dspFoundation.h"
    )
    list(APPEND MIDILAR_DOX_LOCAL
        "${CMAKE_CURRENT_LIST_DIR}/dspFoundation.dox"
    )
#
######################################################################################################
# Add Subdirectories

    if(MIDILAR_DSP_GENERATORS)
        midilar_add_macro(PUBLIC MIDILAR_DSP_GENERATORS)
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Generators.h"
        )

        add_subdirectory(Generators)
    endif()

    if(MIDILAR_DSP_INTERPOLATORS)
        midilar_add_macro(PUBLIC MIDILAR_DSP_INTERPOLATORS)
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Interpolators.h"
        )

        add_subdirectory(Interpolators)
    endif()

    if(MIDILAR_DSP_LUT)
        midilar_add_macro(PUBLIC MIDILAR_DSP_LUT)
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/LUT.h"
        )

        add_subdirectory(LUT)
    endif()

    if(MIDILAR_DSP_STREAMING)
        midilar_add_macro(PUBLIC MIDILAR_DSP_STREAMING)
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Streaming.h"
        )

        add_subdirectory(Streaming)
    endif()

    if(MIDILAR_DSP_TRANSFORMS)
        midilar_add_macro(PUBLIC MIDILAR_DSP_TRANSFORMS)
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Transforms.h"
        )

        add_subdirectory(Transforms)
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
        DESTINATION "include/MIDILAR-${MIDILAR_VERSION}/dspFoundation"
    )
#
######################################################################################################
