######################################################################################################
# Initialize MIDILAR_SOURCES and HEADERS_LIST as an empty string list

    set(${PROJECT_NAME_UPPER}_SOURCES "")
    set(${PROJECT_NAME_UPPER}_HEADERS "")

    set(${PROJECT_NAME_UPPER}_SOURCES_LOCAL "")
    set(${PROJECT_NAME_UPPER}_HEADERS_LOCAL "")
#
######################################################################################################
# Append Sources and Headers

    list(
        APPEND ${PROJECT_NAME_UPPER}_HEADERS_LOCAL 
        "${CMAKE_CURRENT_LIST_DIR}/BuildSettings.h"
    )

    list(APPEND ${PROJECT_NAME_UPPER}_HEADERS ${${PROJECT_NAME_UPPER}_HEADERS_LOCAL})
#
    
    include(ModuleDependencies.cmake)
    include(DependencyResolution.cmake)
    
#
######################################################################################################
# Refresh Sources and Headers in Parent Scope

    set(MIDILAR_SOURCES "${MIDILAR_SOURCES};${MIDILAR_SOURCES_LOCAL}" PARENT_SCOPE)
    set(MIDILAR_HEADERS "${MIDILAR_HEADERS};${MIDILAR_HEADERS_LOCAL}" PARENT_SCOPE)
#
######################################################################################################
# MIDILAR Install Process

    # Install headers for this subdirectory
    install(
    FILES ${MIDILAR_HEADERS_LOCAL}
    DESTINATION "include/MIDILAR-${MIDILAR_VERSION}/System"
    )
#
######################################################################################################


######################################################################################################
# Initialize MIDILAR_SOURCES_LOCAL and HEADERS_LIST_LOCAL as an empty string list

    set(MIDILAR_SOURCES_LOCAL "")
    set(MIDILAR_PRIVATE_HEADERS_LOCAL "")
    set(MIDILAR_PUBLIC_HEADERS_LOCAL "")
#
######################################################################################################
# Append Headers (local to this subdirectory)

    list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
        "${CMAKE_CURRENT_LIST_DIR}/BuildSettings.h"
        "${CMAKE_CURRENT_LIST_DIR}/BuildSettings.h"
    )
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
            ${MIDILAR_PRIVATE_HEADERS_LOCAL}
        )
    endif()
#
######################################################################################################
# add subdirectories

    if(MIDILAR_SYSTEM_CLOCK)
        midilar_add_macro(PUBLIC MIDILAR_SYSTEM_CLOCK)
        add_subdirectory(Clock)
    endif()

    if(MIDILAR_SYSTEM_CALLBACK_HANDLER)
        midilar_add_macro(PUBLIC MIDILAR_SYSTEM_CALLBACK_HANDLER)   
        add_subdirectory(CallbackHandler)
    endif()
#
######################################################################################################
# MIDILAR Install Process

    # Install headers for this subdirectory
    install(
        FILES ${MIDILAR_PUBLIC_HEADERS_LOCAL}
        DESTINATION "include/MIDILAR-${MIDILAR_VERSION}/System"
    )
#
######################################################################################################