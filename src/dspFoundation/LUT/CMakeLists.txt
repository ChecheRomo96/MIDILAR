######################################################################################################
# Append Sources and Headers

    set(MIDILAR_SOURCES_LOCAL "")
    set(MIDILAR_HEADERS_LOCAL "")

    # Append Sources
    #list( APPEND MIDILAR_SOURCES "${CMAKE_CURRENT_LIST_DIR}/MIDILAR_AudioFoundation.cpp")

#
######################################################################################################
# Dependancy Tree

    set(
        MIDILAR_LUT_DEPS 
        "${MIDILAR_DSP_FOUNDATION_DEPS};MIDILAR_LUT"
        CACHE INTERNAL "Dependencies for dspFoudnation"
    )
#
######################################################################################################
# Add Subdirectories


    if(${MIDILAR_LUT1D})
        add_subdirectory(LUT1D)
    endif()

    if(${MIDILAR_LUT2D})
        add_subdirectory(LUT2D)
    endif()

    if(${MIDILAR_LUT3D})
        add_subdirectory(LUT3D)
    endif()

    if(${MIDILAR_LUT_PERIODIC})
        add_subdirectory(Periodic)
    endif()
    
    if(${MIDILAR_LUT_SHAPING})
        add_subdirectory(Shaping)
    endif()

#
######################################################################################################
# Refresh Sources List and Header List at PARENT_SCOPE

    set(MIDILAR_SOURCES "${MIDILAR_SOURCES};${MIDILAR_SOURCES_LOCAL}" PARENT_SCOPE)
    set(MIDILAR_HEADERS "${MIDILAR_HEADERS};${MIDILAR_HEADERS_LOCAL}" PARENT_SCOPE)
    set(MIDILAR_LUT_DEPS ${MIDILAR_LUT_DEPS} PARENT_SCOPE)
#
######################################################################################################
# MIDILAR install process

    install(
        FILES ${MIDILAR_HEADERS_LOCAL}
        DESTINATION "include/MIDILAR-${MIDILAR_VERSION}/dspFoundation/LUT"
    )
#
######################################################################################################