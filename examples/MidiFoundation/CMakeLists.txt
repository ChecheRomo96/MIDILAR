add_subdirectory(Message)

if(${MIDILAR_MIDI_PROCESSOR})
    add_subdirectory(Processor)
endif()

add_subdirectory(MessageParser)
