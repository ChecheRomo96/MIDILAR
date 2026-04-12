######################################################################################################
# Append Sources and Headers
    
    set(MIDILAR_SOURCES_LOCAL "")
    set(MIDILAR_HEADERS_LOCAL "")

    # Append Sources
    #list( APPEND MIDILAR_SOURCES "${CMAKE_CURRENT_LIST_DIR}/MIDILAR_AudioFoundation.cpp")

    if(${MIDILAR_GENERATORS_ENVELOPES})
        add_subdirectory(Envelopes)
    endif()
    
    if(${MIDILAR_GENERATORS_PERIODIC})
        add_subdirectory(Periodic)
    endif()
    
    if(${MIDILAR_GENERATORS_SHAPING})
        add_subdirectory(Shaping)
    endif()
    
    if(${MIDILAR_GENERATORS_WINDOWING})
        list( APPEND MIDILAR_HEADERS_LOCAL "${CMAKE_CURRENT_LIST_DIR}/Windowing.h")
    endif()
#
######################################################################################################
# Add Subdirectories

    #add_subdirectory(Periodic)
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
        DESTINATION "include/MIDILAR-${MIDILAR_VERSION}/dspFoundation/Generators"
    )
#
######################################################################################################