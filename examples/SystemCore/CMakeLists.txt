if(MIDILAR_SYSTEM_CLOCK)
    add_subdirectory(Clock)
endif()

if(MIDILAR_SYSTEM_CALLBACK_HANDLER)
    add_subdirectory(CallbackHandler)
endif()

if(MIDILAR_SYSTEM_RING_BUFFER)
    add_subdirectory(RingBuffer)
endif()