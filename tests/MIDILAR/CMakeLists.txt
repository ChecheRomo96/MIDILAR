if(MIDILAR_CORE)
    add_subdirectory(Core)
endif()


if(MIDILAR_PROTOCOL)
    add_subdirectory(Protocol)
endif()

if(MIDILAR_DEVICES)
    add_subdirectory(Devices)
endif()
