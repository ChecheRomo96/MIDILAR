add_subdirectory(BuildTest)

if(${MIDILAR_SYSTEM_TOOLS_STATUS})
    add_subdirectory(SystemTools)
endif()

add_subdirectory(MidiProcessor)