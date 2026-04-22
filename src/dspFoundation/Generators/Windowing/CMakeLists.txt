######################################################################################################
# Append Sources and Headers

    set(MIDILAR_SOURCES_LOCAL "")
    set(MIDILAR_HEADERS_LOCAL "")

    list(APPEND MIDILAR_HEADERS_LOCAL
        #"${CMAKE_CURRENT_LIST_DIR}/Windowing.h"
        #"${CMAKE_CURRENT_LIST_DIR}/Bartlett.h"
        #"${CMAKE_CURRENT_LIST_DIR}/Barthann.h"
        #"${CMAKE_CURRENT_LIST_DIR}/Blackman.h"
        #"${CMAKE_CURRENT_LIST_DIR}/BlackmanHarris.h"
        #"${CMAKE_CURRENT_LIST_DIR}/Bohman.h"
        #"${CMAKE_CURRENT_LIST_DIR}/Chebyshev.h"
        #"${CMAKE_CURRENT_LIST_DIR}/Flattop.h"
        #"${CMAKE_CURRENT_LIST_DIR}/Gaussian.h"
        "${CMAKE_CURRENT_LIST_DIR}/Hamming.h"
        "${CMAKE_CURRENT_LIST_DIR}/Hann.h"
        #"${CMAKE_CURRENT_LIST_DIR}/Kaiser.h"
        #"${CMAKE_CURRENT_LIST_DIR}/Nuttall.h"
        #"${CMAKE_CURRENT_LIST_DIR}/Parzen.h"
        #"${CMAKE_CURRENT_LIST_DIR}/Rectangular.h"
        #"${CMAKE_CURRENT_LIST_DIR}/Taylor.h"
        #"${CMAKE_CURRENT_LIST_DIR}/Triangular.h"
        #"${CMAKE_CURRENT_LIST_DIR}/Tukey.h"
    )

######################################################################################################
# Refresh Sources List and Header List at PARENT_SCOPE

    set(MIDILAR_SOURCES "${MIDILAR_SOURCES};${MIDILAR_SOURCES_LOCAL}" PARENT_SCOPE)
    set(MIDILAR_HEADERS "${MIDILAR_HEADERS};${MIDILAR_HEADERS_LOCAL}" PARENT_SCOPE)
#
######################################################################################################
# Stage headers

    midilar_stage_headers(${MIDILAR_PUBLIC_HEADERS_LOCAL})
#
######################################################################################################
# MIDILAR install process

    install(
        FILES ${MIDILAR_HEADERS_LOCAL}
        DESTINATION "include/MIDILAR-${MIDILAR_VERSION}/dspFoundation/Generators/Windowing"
    )
#
######################################################################################################