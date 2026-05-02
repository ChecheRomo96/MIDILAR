#################################################################################################################################
# CMake options for MIDILAR
    if(MIDILAR_FULL_BUILD)
        set(MIDILAR_MIDI_PROTOCOL_DEFINES ON)
        set(MIDILAR_MIDI_PROTOCOL_ENUMS ON)
    endif()
#    
#################################################################################################################################
# 

    option(MIDILAR_MIDI_PROTOCOL_DEFINES "Enables the compilation of MIDI protocol defines in MIDILAR::MidiCore::Protocol" ON)
    option(MIDILAR_MIDI_PROTOCOL_ENUMS "Enables the compilation of MIDI protocol enums in MIDILAR::MidiCore::Protocol" ON)
#
#################################################################################################################################