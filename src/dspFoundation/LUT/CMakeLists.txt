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
        "${CMAKE_CURRENT_LIST_DIR}/LUT.h"
    )
    
    list(APPEND MIDILAR_DOX_LOCAL
        "${CMAKE_CURRENT_LIST_DIR}/LUT.dox"
    )
#
######################################################################################################
# add subdirectories

    if(MIDILAR_DSP_LUT1D)
        midilar_add_macro(PUBLIC MIDILAR_DSP_LUT1D)
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/LUT1D.h"
        )

        add_subdirectory(LUT1D)
    endif()

    if(MIDILAR_DSP_LUT2D)
        midilar_add_macro(PUBLIC MIDILAR_DSP_LUT2D)

        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/LUT2D.h"
        )

        add_subdirectory(LUT2D)
    endif()

    if(MIDILAR_DSP_LUT3D)
        midilar_add_macro(PUBLIC MIDILAR_DSP_LUT3D)

        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/LUT3D.h"
        )

        add_subdirectory(LUT3D)
    endif()

    if(MIDILAR_DSP_LUT_SHAPING)
        midilar_add_macro(PUBLIC MIDILAR_DSP_LUT_SHAPING)

        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Shaping.h"
        )

        add_subdirectory(Shaping)
    endif()

    if(MIDILAR_DSP_LUT_PERIODIC)
        midilar_add_macro(PUBLIC MIDILAR_DSP_LUT_PERIODIC)

        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Periodic.h"
        )

        add_subdirectory(Periodic)
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
        DESTINATION "include/MIDILAR-${MIDILAR_VERSION}/dspFoundation/LUT"
    )
#
######################################################################################################