######################################################################################################
# Add Compiler Definitions

target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_NAME_UPPER}_VERSION="${PROJECT_VERSION}")

# Add optional features as compile definitions and update the macros list

message(STATUS "Enabled ${PROJECT_NAME} features:")

if(MIDILAR_SYSTEM_CORE)
    message(STATUS "MIDILAR::SystemCore")
    target_compile_definitions(MIDILAR PUBLIC MIDILAR_SYSTEM_CORE)
    list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_SYSTEM_CORE")

    include(${CMAKE_CURRENT_LIST_DIR}/SystemCore/CMakeMacros.cmake)
endif()

if(MIDILAR_MIDI_CORE)
    message(STATUS "MIDILAR::MidiCore")
    target_compile_definitions(MIDILAR PUBLIC MIDILAR_MIDI_CORE)
    list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_MIDI_CORE")

    include(${CMAKE_CURRENT_LIST_DIR}/MidiCore/CMakeMacros.cmake)
endif()

if(MIDILAR_DSP_CORE)
    message(STATUS "MIDILAR::DspCore")
    target_compile_definitions(MIDILAR PUBLIC MIDILAR_DSP_CORE)
    list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_DSP_CORE")

    include(${CMAKE_CURRENT_LIST_DIR}/DspCore/CMakeMacros.cmake)
endif()


if(MIDILAR_MIDI_DEVICES)
    message(STATUS "MIDILAR::MidiDevices")
    target_compile_definitions(MIDILAR PUBLIC MIDILAR_MIDI_DEVICES)
    list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_MIDI_DEVICES")

    #include(${CMAKE_CURRENT_LIST_DIR}/MidiDevices/CMakeMacros.cmake)
endif()


# Other conditional flags can be added here as needed
######################################################################################################