######################################################################################################
# Append Sources and Headers

set(MIDILAR_SOURCES_LOCAL "")
set(MIDILAR_HEADERS_LOCAL "")

# Append Sources
list( APPEND MIDILAR_HEADERS_LOCAL "${CMAKE_CURRENT_LIST_DIR}/StreamingBase.h")

#
######################################################################################################
# Add Subdirectories

    if(${MIDILAR_STREAMING_FILTERS})
        add_subdirectory(Filters)
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
    DESTINATION "include/MIDILAR-${MIDILAR_VERSION}/DspCore/Streaming"
)
#
######################################################################################################