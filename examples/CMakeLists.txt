midilar_add_dox(
    "${CMAKE_CURRENT_LIST_DIR}/Examples.dox"
)


add_subdirectory(BuildTest)

if(MIDILAR_SYSTEM_FOUNDATION)
    add_subdirectory(SystemFoundation)
endif()

if(MIDILAR_DSP_FOUNDATION)
    add_subdirectory(dspFoundation)
endif()

if(MIDILAR_MIDI_FOUNDATION)
    add_subdirectory(MidiFoundation)
endif()

if(MIDILAR_MIDI_DEVICES)
    add_subdirectory(MidiDevices)
endif()
