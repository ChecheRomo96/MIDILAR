#################################################################################################################################
# CMake options for MIDILAR
    if(MIDILAR_FULL_BUILD)
        set(MIDILAR_SYSTEM_FOUNDATION ON)
        set(MIDILAR_DSP_FOUNDATION ON)
        set(MIDILAR_MIDI_FOUNDATION ON)
        set(MIDILAR_MIDI_DEVICES ON)
    endif()
#
#################################################################################################################################
# SystemFoundation

    option(MIDILAR_SYSTEM_FOUNDATION "Enables the compilation of MIDILAR::SystemFoundation" ON)
    include(${CMAKE_CURRENT_LIST_DIR}/SystemFoundation/CMakeOptions.cmake)
#
#################################################################################################################################
# SystemFoundation

    option(MIDILAR_MIDI_FOUNDATION "Enables the compilation of MIDILAR::MidiFoundation" ON)
    include(${CMAKE_CURRENT_LIST_DIR}/MidiFoundation/CMakeOptions.cmake)
#
#################################################################################################################################
# dspFoundation

    option(MIDILAR_DSP_FOUNDATION "Enables the compilation of MIDILAR::dspFoundation" ON)
    include(${CMAKE_CURRENT_LIST_DIR}/dspFoundation/CMakeOptions.cmake)
#
#################################################################################################################################
# Midi Devices

    option(MIDILAR_MIDI_DEVICES "Enables the compilation of MIDILAR::MidiDevices" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/MidiDevices/CMakeOptions.cmake)
#
#################################################################################################################################
