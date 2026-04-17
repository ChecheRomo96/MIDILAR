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
        "${CMAKE_CURRENT_LIST_DIR}/MidiFoundation.h"
    )
    
    list(APPEND MIDILAR_DOX_LOCAL
        "${CMAKE_CURRENT_LIST_DIR}/MidiFoundation.dox"
    )
#
######################################################################################################
# add subdirectories

    if(MIDILAR_MIDI_PROTOCOL)
        midilar_add_macro(PUBLIC MIDILAR_MIDI_PROTOCOL)
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Protocol.h"
        )

        add_subdirectory(Protocol)
    endif()
    
    if(MIDILAR_MIDI_MESSAGE)
        midilar_add_macro(PUBLIC MIDILAR_MIDI_MESSAGE)
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/Message.h"
        )

        add_subdirectory(Message)
    endif()
    
    if(MIDILAR_MIDI_MESSAGE_PARSER)
        midilar_add_macro(PUBLIC MIDILAR_MIDI_MESSAGE_PARSER)
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/MessageParser.h"
        )

        add_subdirectory(MessageParser)
    endif()
    
    if(MIDILAR_MIDI_DEVICE_BASE)
        midilar_add_macro(PUBLIC MIDILAR_MIDI_DEVICE_BASE)
        
        list(APPEND MIDILAR_PUBLIC_HEADERS_LOCAL
            "${CMAKE_CURRENT_LIST_DIR}/DeviceBase.h"
        )

        add_subdirectory(DeviceBase)
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
# MIDILAR Install Process

    # Install headers for this subdirectory
    install(
        FILES ${MIDILAR_PUBLIC_HEADERS_LOCAL}
        DESTINATION "include/MIDILAR-${MIDILAR_VERSION}/SystemFoundation"
    )
#
######################################################################################################