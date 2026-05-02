if(${MIDILAR_MIDI_MESSAGE})
    add_subdirectory(Message)
endif()

if(${MIDILAR_MIDI_PROCESSOR})
    add_subdirectory(Processor)
endif()

if(${MIDILAR_MIDI_MESSAGE_PARSER})
    add_subdirectory(MessageParser)
endif()
