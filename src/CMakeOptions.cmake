#################################################################################################################################
# CMake options for MIDILAR
    if(MIDILAR_FULL_BUILD)
        set(MIDILAR_SYSTEM_CORE ON)
        set(MIDILAR_DSP_CORE ON)
        set(MIDILAR_MIDI_CORE ON)
        set(MIDILAR_MIDI_DEVICES ON)
    endif()
#
#################################################################################################################################
# SystemCore

    option(MIDILAR_SYSTEM_CORE "Enables the compilation of MIDILAR::SystemCore" ON)
    include(${CMAKE_CURRENT_LIST_DIR}/SystemCore/CMakeOptions.cmake)
#
#################################################################################################################################
# SystemCore

    option(MIDILAR_MIDI_CORE "Enables the compilation of MIDILAR::MidiCore" ON)
    include(${CMAKE_CURRENT_LIST_DIR}/MidiCore/CMakeOptions.cmake)
#
#################################################################################################################################
# DspCore

    option(MIDILAR_DSP_CORE "Enables the compilation of MIDILAR::DspCore" ON)
    include(${CMAKE_CURRENT_LIST_DIR}/DspCore/CMakeOptions.cmake)
#
#################################################################################################################################
# Midi Devices

    option(MIDILAR_MIDI_DEVICES "Enables the compilation of MIDILAR::MidiDevices" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/MidiDevices/CMakeOptions.cmake)
#
#################################################################################################################################
