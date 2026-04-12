######################################################################################################
# Append Sources and Headers

    set(MIDILAR_SOURCES_LOCAL "")
    set(MIDILAR_HEADERS_LOCAL "")

    # Append Sources
    #list( APPEND MIDILAR_SOURCES "${CMAKE_CURRENT_LIST_DIR}/MIDILAR_AudioFoundation.cpp")

    # Append Headers
    #list( APPEND MIDILAR_HEADERS_LOCAL "${CMAKE_CURRENT_LIST_DIR}/TransformBase.h")
#
######################################################################################################
# Add Subdirectories

    if(${MIDILAR_GENERATORS})
        add_subdirectory(Generators)
    endif()

    if(${MIDILAR_LUT_INTERPOLATORS})
        add_subdirectory(Interpolators)
    endif()
    
    if(${MIDILAR_LUT})
        add_subdirectory(LUT)
    endif()
    
    if(${MIDILAR_TRANSFORMS})
        add_subdirectory(Transforms)
    endif()
    
    if(${MIDILAR_STREAMING})
        add_subdirectory(Streaming)
    endif()
#
######################################################################################################
# Refresh Sources List and Header List at PARENT_SCOPE

    set(MIDILAR_SOURCES "${MIDILAR_SOURCES};${MIDILAR_SOURCES_LOCAL}" PARENT_SCOPE)
    set(MIDILAR_HEADERS "${MIDILAR_HEADERS};${MIDILAR_HEADERS_LOCAL}" PARENT_SCOPE)
#
######################################################################################################
# MIDILAR install process

    install(
        FILES ${MIDILAR_HEADERS_LOCAL}
        DESTINATION "include/MIDILAR-${MIDILAR_VERSION}/AudioFoundation"
    )
#
######################################################################################################