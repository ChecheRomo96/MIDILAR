midilar_add_dox(
    "${CMAKE_CURRENT_LIST_DIR}/Examples.dox"
)


add_subdirectory(BuildTest)

if(MIDILAR_SYSTEM_CORE)
    add_subdirectory(SystemCore)
endif()

if(MIDILAR_DSP_CORE)
    add_subdirectory(DspCore)
endif()

if(MIDILAR_MIDI_CORE)
    add_subdirectory(MidiCore)
endif()

if(MIDILAR_MIDI_DEVICES)
    add_subdirectory(MidiDevices)
endif()
