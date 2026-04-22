######################################################################################################
# Add Compiler Definitions

target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_NAME_UPPER}_VERSION="${PROJECT_VERSION}")

# Add optional features as compile definitions and update the macros list

message(STATUS "Enabled ${PROJECT_NAME} features:")

if(MIDILAR_SYSTEM_FOUNDATION)
    message(STATUS "MIDILAR::SystemFoundation")
    target_compile_definitions(MIDILAR PUBLIC MIDILAR_SYSTEM_FOUNDATION)
    list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_SYSTEM_FOUNDATION")

    include(${CMAKE_CURRENT_LIST_DIR}/SystemFoundation/CMakeMacros.cmake)
endif()

if(MIDILAR_MIDI_FOUNDATION)
    message(STATUS "MIDILAR::MidiFoundation")
    target_compile_definitions(MIDILAR PUBLIC MIDILAR_MIDI_FOUNDATION)
    list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_MIDI_FOUNDATION")

    include(${CMAKE_CURRENT_LIST_DIR}/MidiFoundation/CMakeMacros.cmake)
endif()

if(MIDILAR_DSP_FOUNDATION)
    message(STATUS "MIDILAR::dspFoundation")
    target_compile_definitions(MIDILAR PUBLIC MIDILAR_DSP_FOUNDATION)
    list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_DSP_FOUNDATION")

    include(${CMAKE_CURRENT_LIST_DIR}/dspFoundation/CMakeMacros.cmake)
endif()


if(MIDILAR_MIDI_DEVICES)
    message(STATUS "MIDILAR::MidiDevices")
    target_compile_definitions(MIDILAR PUBLIC MIDILAR_MIDI_DEVICES)
    list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_MIDI_DEVICES")

    include(${CMAKE_CURRENT_LIST_DIR}/MidiDevices/CMakeMacros.cmake)
endif()


# Other conditional flags can be added here as needed
######################################################################################################