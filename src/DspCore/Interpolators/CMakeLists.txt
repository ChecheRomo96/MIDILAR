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
        "${CMAKE_CURRENT_LIST_DIR}/Interpolators.h"
    )
    list(APPEND MIDILAR_DOX_LOCAL
        "${CMAKE_CURRENT_LIST_DIR}/Interpolators.dox"
    )
#
######################################################################################################
# Add Subdirectories

    if(MIDILAR_DSP_INTERPOLATORS_LINEAR)
        midilar_add_macro(PUBLIC MIDILAR_DSP_INTERPOLATORS_LINEAR)
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Linear.h"
        )

        add_subdirectory(Linear)
    endif()

    if(MIDILAR_DSP_INTERPOLATORS_BILINEAR)
        midilar_add_macro(PUBLIC MIDILAR_DSP_INTERPOLATORS_BILINEAR)
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Bilinear.h"
        )

        add_subdirectory(Bilinear)
    endif()

    if(MIDILAR_DSP_INTERPOLATORS_TRILINEAR)
        midilar_add_macro(PUBLIC MIDILAR_DSP_INTERPOLATORS_TRILINEAR)
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Trilinear.h"
        )

        add_subdirectory(Trilinear)
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
        DESTINATION "include/MIDILAR-${MIDILAR_VERSION}/DspCore/Interpolators"
    )
#
######################################################################################################
