#################################################################################################################################
# CMake options for MIDILAR
    if(MIDILAR_FULL_BUILD)
        set(MIDILAR_MIDI_PROTOCOL ON)
        set(MIDILAR_MIDI_MESSAGE ON)
        set(MIDILAR_MIDI_MESSAGE_PARSER ON)
        set(MIDILAR_MIDI_DEVICE_BASE ON)
    endif()
#
#################################################################################################################################
# MIDI Protocol

    option(MIDILAR_MIDI_PROTOCOL "Enables the compilation of MIDILAR::MidiCore::Protocol" ON)
    include(${CMAKE_CURRENT_LIST_DIR}/Protocol/CMakeOptions.cmake)
#
##################################################################################################################################
# Message

    option(MIDILAR_MIDI_MESSAGE "Enables the compilation of MIDILAR::MidiCore::Message" ON)
#
##################################################################################################################################
# Message Parser

    option(MIDILAR_MIDI_MESSAGE_PARSER "Enables the compilation of MIDILAR::MidiCore::MessageParser" ON)
#
##################################################################################################################################
# Device Base

    option(MIDILAR_MIDI_DEVICE_BASE "Enables the compilation of MIDILAR::MidiCore::DeviceBase" ON)
#
#################################################################################################################################
