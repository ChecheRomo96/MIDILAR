######################################################################################################
# Append Sources and Headers
    
    # Initialize Lists
    set(MIDILAR_SOURCES_LOCAL "")
    set(MIDILAR_HEADERS_LOCAL "")

    # Append Sources
    #list( APPEND MIDILAR_SOURCES "${CMAKE_CURRENT_LIST_DIR}/MIDILAR_AudioFoundation.cpp")

    # Append Headeers
    list( APPEND MIDILAR_HEADERS_LOCAL "${CMAKE_CURRENT_LIST_DIR}/Periodic.h")


#
######################################################################################################
# Add Subdirectories

    if(${MIDILAR_GENERATORS_SINE})
        add_subdirectory(Sine)
    endif()

    if(${MIDILAR_GENERATORS_SQUARE})
        add_subdirectory(Square)
    endif()
    
    if(${MIDILAR_GENERATORS_TRIANGLE})
        add_subdirectory(Triangle)
    endif()
    
    if(${MIDILAR_GENERATORS_SAW})
        add_subdirectory(Saw)
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
        DESTINATION "include/MIDILAR-${MIDILAR_VERSION}/dspFoundation/Generators/Periodic"
    )
#
######################################################################################################