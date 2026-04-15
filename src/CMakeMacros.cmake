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

    message(STATUS "MIDILAR::MidiFoundation::Protocol")
    message(STATUS "MIDILAR::MidiFoundation::Message")
    message(STATUS "MIDILAR::MidiFoundation::MessageParser")
    message(STATUS "MIDILAR::MidiFoundation::DeviceBase")
endif()

if(MIDILAR_DSP_FOUNDATION)
    message(STATUS "MIDILAR::dspFoundation")
    target_compile_definitions(MIDILAR PUBLIC MIDILAR_DSP_FOUNDATION)
    list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_DSP_FOUNDATION")
    
    if(MIDILAR_LUT)
        message(STATUS "MIDILAR::dspFoundation::LUT")
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_LUT)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_LUT")
        
        if(MIDILAR_LUT1D)
            message(STATUS "MIDILAR::dspFoundation::LUT::LUT1D")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_LUT1D)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_LUT1D")
        endif()
        
        if(MIDILAR_LUT2D)
            message(STATUS "MIDILAR::dspFoundation::LUT::LUT2D")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_LUT2D)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_LUT2D")
        endif()
        
        if(MIDILAR_LUT3D)
            message(STATUS "MIDILAR::dspFoundation::LUT::LUT3D")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_LUT3D)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_LUT3D")
        endif()
        
        if(MIDILAR_LUT_PERIODIC)
            message(STATUS "MIDILAR::dspFoundation::LUT::Periodic")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_LUT_PERIODIC)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_LUT_PERIODIC")

            message(STATUS "MIDILAR::dspFoundation::LUT::Periodic::SineLUT")
            message(STATUS "MIDILAR::dspFoundation::LUT::Periodic::TriangleLUT")
            message(STATUS "MIDILAR::dspFoundation::LUT::Periodic::SquareLUT")
            message(STATUS "MIDILAR::dspFoundation::LUT::Periodic::SawLUT")
            
        endif()
        
        if(MIDILAR_LUT_SHAPING)
            message(STATUS "MIDILAR::dspFoundation::LUT::Shaping")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_LUT_SHAPING)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_LUT_SHAPING")

            message(STATUS "MIDILAR::dspFoundation::LUT::Shaping::GammaLUT")
            message(STATUS "MIDILAR::dspFoundation::LUT::Shaping::EnhancedGammaLUT")
        endif()

    endif()

    if(MIDILAR_INTERPOLATORS)
        message(STATUS "MIDILAR::dspFoundation::Interpolators")
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_INTERPOLATORS)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_INTERPOLATORS")
    
        message(STATUS "MIDILAR::dspFoundation::Interpolators::Linear")
        message(STATUS "MIDILAR::dspFoundation::Interpolators::Bilinear")
        message(STATUS "MIDILAR::dspFoundation::Interpolators::Trilinear")
    endif()
    
    if(MIDILAR_GENERATORS)
        message(STATUS "MIDILAR::dspFoundation::Generators")
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_GENERATORS)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_GENERATORS")
        
        if(MIDILAR_GENERATORS_PERIODIC)
            message(STATUS "MIDILAR::dspFoundation::Generators::Periodic")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_GENERATORS_PERIODIC)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_GENERATORS_PERIODIC")
        
            message(STATUS "MIDILAR::dspFoundation::Generators::Periodic::Sine")
            message(STATUS "MIDILAR::dspFoundation::Generators::Periodic::Triangle")
            message(STATUS "MIDILAR::dspFoundation::Generators::Periodic::Square")
            message(STATUS "MIDILAR::dspFoundation::Generators::Periodic::Saw")
        endif()
        
        if(MIDILAR_GENERATORS_SHAPING)
            message(STATUS "MIDILAR::dspFoundation::Generators::Shaping")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_GENERATORS_SHAPING)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_GENERATORS_SHAPING")
        
            message(STATUS "MIDILAR::dspFoundation::Generators::Shaping::Gamma")
            message(STATUS "MIDILAR::dspFoundation::Generators::Shaping::EnhancedGamma")
        
        endif()
        
        if(MIDILAR_GENERATORS_WINDOWING)
            message(STATUS "MIDILAR::dspFoundation::Generators::Windowing")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_GENERATORS_WINDOWING)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_GENERATORS_WINDOWING")
        endif()
        
        if(MIDILAR_GENERATORS_ENVELOPES)
            message(STATUS "MIDILAR::dspFoundation::Generators::Envelopes")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_GENERATORS_ENVELOPES)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_GENERATORS_ENVELOPES")
        endif()

    endif()

    if(MIDILAR_STREAMING)
        message(STATUS "MIDILAR::dspFoundation::Streaming")
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_STREAMING)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_STREAMING")
    endif()

    if(MIDILAR_TRANSFORMS)
        message(STATUS "MIDILAR::dspFoundation::Transforms")
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_TRANSFORMS)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_TRANSFORMS")

        if(MIDILAR_DFT)
            message(STATUS "MIDILAR::dspFoundation::Transforms::DFT")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_DFT)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_DFT")
        endif()
    endif()

endif()


if(MIDILAR_MIDI_DEVICES)
    target_compile_definitions(MIDILAR PUBLIC MIDILAR_MIDI_DEVICES)
    list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_MIDI_DEVICES")
    
    if(MIDILAR_CHANNEL_REASSIGN)
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_CHANNEL_REASSIGN)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_CHANNEL_REASSIGN")
    endif()

    if(MIDILAR_VELOCITY_SHAPER)
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_VELOCITY_SHAPER)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_VELOCITY_SHAPER")
    endif()
endif()


# Other conditional flags can be added here as needed
######################################################################################################