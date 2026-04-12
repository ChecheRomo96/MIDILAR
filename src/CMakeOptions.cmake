
######################################################################################################
# Add Compiler Definitions

target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_NAME_UPPER}_VERSION="${PROJECT_VERSION}")

# Add optional features as compile definitions and update the macros list

message(STATUS "Enabled ${PROJECT_NAME} features:")

if(${MIDILAR_SYSTEM_FOUNDATION})
    message(STATUS "MIDILAR::SystemFoundation")
    target_compile_definitions(MIDILAR PUBLIC MIDILAR_SYSTEM_FOUNDATION)
    list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_SYSTEM_FOUNDATION")

    if(${MIDILAR_CALLBACK_HANDLER})
        message(STATUS "MIDILAR::SystemFoundation::CallbackHandler")
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_CALLBACK_HANDLER)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_CALLBACK_HANDLER")
    endif()
    
    if(${MIDILAR_SYSTEM_CLOCK})
        message(STATUS "MIDILAR::SystemFoundation::Clock")
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_SYSTEM_CLOCK)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_SYSTEM_CLOCK")
    endif()

endif()

if(${MIDILAR_MIDI_FOUNDATION})
message(STATUS "MIDILAR::MidiFoundation")
    target_compile_definitions(MIDILAR PUBLIC MIDILAR_MIDI_FOUNDATION)
    list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_MIDI_FOUNDATION")

    if(${MIDILAR_PROTOCOL_ENUMS})
        message(STATUS "MIDILAR::MidiFoundation::Protocol")
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_PROTOCOL_ENUMS)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_PROTOCOL_ENUMS")
    endif()
    
    if(${MIDILAR_MIDI_MESSAGE})
        message(STATUS "MIDILAR::MidiFoundation::Message")
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_MIDI_MESSAGE)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_MIDI_MESSAGE")
    endif()
    
    if(${MIDILAR_MIDI_MESSAGE_PARSER})
        message(STATUS "MIDILAR::MidiFoundation::MessageParser")
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_MIDI_MESSAGE_PARSER)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_MIDI_PROCESSOR")
    endif()
    
    if(${MIDILAR_MIDI_PROCESSOR})
        message(STATUS "MIDILAR::MidiFoundation::Processor")
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_MIDI_PROCESSOR)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_MIDI_PROCESSOR")
    endif()

endif()

if(${MIDILAR_DSP_FOUNDATION})
    message(STATUS "MIDILAR::dspFoundation")
    target_compile_definitions(MIDILAR PUBLIC MIDILAR_DSP_FOUNDATION)
    list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_DSP_FOUNDATION")
    
    if(${MIDILAR_LUT})
        message(STATUS "MIDILAR::dspFoundation::LUT")
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_LUT)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_LUT")
        
        if(${MIDILAR_LUT1D})
            message(STATUS "MIDILAR::dspFoundation::LUT::LUT1D")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_LUT1D)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_LUT1D")
        endif()
        
        if(${MIDILAR_LUT2D})
            message(STATUS "MIDILAR::dspFoundation::LUT::LUT2D")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_LUT2D)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_LUT2D")
        endif()
        
        if(${MIDILAR_LUT3D})
            message(STATUS "MIDILAR::dspFoundation::LUT::LUT3D")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_LUT3D)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_LUT3D")
        endif()
        
        if(${MIDILAR_LUT_PERIODIC})
            message(STATUS "MIDILAR::dspFoundation::LUT::Periodic")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_LUT_PERIODIC)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_LUT_PERIODIC")

    
            if(${MIDILAR_LUT_PERIODIC_SINE})
                message(STATUS "MIDILAR::dspFoundation::LUT::Periodic::SineLUT")
                target_compile_definitions(MIDILAR PUBLIC MIDILAR_LUT_PERIODIC_SINE)
                list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_LUT_PERIODIC_SINE")
            endif()

            if(${MIDILAR_LUT_PERIODIC_TRIANGLE})
                message(STATUS "MIDILAR::dspFoundation::LUT::Periodic::TriangleLUT")
                target_compile_definitions(MIDILAR PUBLIC MIDILAR_LUT_PERIODIC_TRIANGLE)
                list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_LUT_PERIODIC_TRIANGLE")
            endif()
            
            if(${MIDILAR_LUT_PERIODIC_SQUARE})
                message(STATUS "MIDILAR::dspFoundation::LUT::Periodic::SquareLUT")
                target_compile_definitions(MIDILAR PUBLIC MIDILAR_LUT_PERIODIC_SQUARE)
                list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_LUT_PERIODIC_SQUARE")
            endif()
            
            if(${MIDILAR_LUT_PERIODIC_SAW})
                message(STATUS "MIDILAR::dspFoundation::LUT::Periodic::SawLUT")
                target_compile_definitions(MIDILAR PUBLIC MIDILAR_LUT_PERIODIC_SAW)
                list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_LUT_PERIODIC_SAW")
            endif()
            
        endif()
        
        if(${MIDILAR_LUT_SHAPING})
            message(STATUS "MIDILAR::dspFoundation::LUT::Shaping")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_LUT_SHAPING)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_LUT_SHAPING")

    
            if(${MIDILAR_GAMMA_LUT})
                message(STATUS "MIDILAR::dspFoundation::LUT::Shaping::GammaLUT")
                target_compile_definitions(MIDILAR PUBLIC MIDILAR_GAMMA_LUT)
                list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_GAMMA_LUT")
            endif()
            
        endif()

    endif()

    if(${MIDILAR_INTERPOLATORS})
        message(STATUS "MIDILAR::dspFoundation::Interpolators")
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_INTERPOLATORS)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_INTERPOLATORS")
    
    
        if(${MIDILAR_INTERPOLATORS_LINEAR})
            message(STATUS "MIDILAR::dspFoundation::Interpolators::Linear")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_INTERPOLATORS_LINEAR)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_INTERPOLATORS_LINEAR")
        endif()
        
        if(${MIDILAR_INTERPOLATORS_BILINEAR})
            message(STATUS "MIDILAR::dspFoundation::Interpolators::Bilinear")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_INTERPOLATORS_BILINEAR)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_INTERPOLATORS_BILINEAR")
        endif()
        
        if(${MIDILAR_INTERPOLATORS_TRILINEAR})
            message(STATUS "MIDILAR::dspFoundation::Interpolators::Trilinear")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_INTERPOLATORS_TRILINEAR)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_INTERPOLATORS_TRILINEAR")
        endif()
    endif()
    
    if(${MIDILAR_GENERATORS})
        message(STATUS "MIDILAR::dspFoundation::Generators")
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_GENERATORS)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_GENERATORS")
        
        if(${MIDILAR_GENERATORS_PERIODIC})
            message(STATUS "MIDILAR::dspFoundation::Generators::Periodic")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_GENERATORS_PERIODIC)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_GENERATORS_PERIODIC")
        
            if(${MIDILAR_GENERATORS_SINE})
                message(STATUS "MIDILAR::dspFoundation::Generators::Periodic::Sine")
                target_compile_definitions(MIDILAR PUBLIC MIDILAR_GENERATORS_SINE)
                list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_GENERATORS_SINE")
            endif()
            
            if(${MIDILAR_GENERATORS_TRIANGLE})
                message(STATUS "MIDILAR::dspFoundation::Generators::Periodic::Triangle")
                target_compile_definitions(MIDILAR PUBLIC MIDILAR_GENERATORS_TRIANGLE)
                list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_GENERATORS_TRIANGLE")
            endif()
            
            if(${MIDILAR_GENERATORS_SQUARE})
                message(STATUS "MIDILAR::dspFoundation::Generators::Periodic::Square")
                target_compile_definitions(MIDILAR PUBLIC MIDILAR_GENERATORS_SQUARE)
                list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_GENERATORS_SQUARE")
            endif()
            
            if(${MIDILAR_GENERATORS_SAW})
                message(STATUS "MIDILAR::dspFoundation::Generators::Periodic::Saw")
                target_compile_definitions(MIDILAR PUBLIC MIDILAR_GENERATORS_SAW)
                list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_GENERATORS_SAW")
            endif()
        endif()
        
        if(${MIDILAR_GENERATORS_SHAPING})
            message(STATUS "MIDILAR::dspFoundation::Generators::Shaping")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_GENERATORS_SHAPING)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_GENERATORS_SHAPING")
        
            if(${MIDILAR_GENERATORS_GAMMA})
                message(STATUS "MIDILAR::dspFoundation::Generators::Shaping::Gamma")
                target_compile_definitions(MIDILAR PUBLIC MIDILAR_GENERATORS_GAMMA)
                list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_GENERATORS_GAMMA")
            endif()
            
            if(${MIDILAR_GENERATORS_ENHANCED_GAMMA})
                message(STATUS "MIDILAR::dspFoundation::Generators::Shaping::EnhancedGamma")
                target_compile_definitions(MIDILAR PUBLIC MIDILAR_GENERATORS_ENHANCED_GAMMA)
                list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_GENERATORS_ENHANCED_GAMMA")
            endif()
        
        endif()
        
        if(${MIDILAR_GENERATORS_WINDOWING})
            message(STATUS "MIDILAR::dspFoundation::Generators::Windowing")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_GENERATORS_WINDOWING)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_GENERATORS_WINDOWING")
        endif()
        
        if(${MIDILAR_GENERATORS_ENVELOPES})
            message(STATUS "MIDILAR::dspFoundation::Generators::Envelopes")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_GENERATORS_ENVELOPES)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_GENERATORS_ENVELOPES")
        endif()

    endif()

    if(${MIDILAR_STREAMING})
        message(STATUS "MIDILAR::dspFoundation::Streaming")
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_STREAMING)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_STREAMING")
        
        if(${MIDILAR_STREAMING_LOGIC})
        message(STATUS "MIDILAR::dspFoundation::Streaming::Logic")
            target_compile_definitions(MIDILAR PUBLIC MIDILAR_STREAMING_LOGIC)
            list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_STREAMING_LOGIC")

            if(${MIDILAR_LEVEL_TRANSITION_TRACKER})
            message(STATUS "MIDILAR::dspFoundation::Streaming::Logic::LevelTransitionTracker")
                target_compile_definitions(MIDILAR PUBLIC MIDILAR_LEVEL_TRANSITION_TRACKER)
                list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_LEVEL_TRANSITION_TRACKER")
            
                
                if(${MIDILAR_STREAMING_EDGE_DETECTOR})
                    message(STATUS "MIDILAR::dspFoundation::Streaming::Logic::EdgeDetector")
                    target_compile_definitions(MIDILAR PUBLIC MIDILAR_STREAMING_EDGE_DETECTOR)
                    list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_STREAMING_EDGE_DETECTOR")
                endif()
                
                if(${MIDILAR_STREAMING_SCHMITT_TRIGGER})
                message(STATUS "MIDILAR::dspFoundation::Streaming::Logic::SchmittTrigger")
                    target_compile_definitions(MIDILAR PUBLIC MIDILAR_STREAMING_SCHMITT_TRIGGER)
                    list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_STREAMING_SCHMITT_TRIGGER")
                endif()
            endif()
        endif()
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


if(${MIDILAR_MIDI_DEVICES})
    target_compile_definitions(MIDILAR PUBLIC MIDILAR_MIDI_DEVICES)
    list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_MIDI_DEVICES")
    
    if(${MIDILAR_CHANNEL_REASSIGN})
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_CHANNEL_REASSIGN)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_CHANNEL_REASSIGN")
    endif()

    if(${MIDILAR_VELOCITY_SHAPER})
        target_compile_definitions(MIDILAR PUBLIC MIDILAR_VELOCITY_SHAPER)
        list(APPEND ${PROJECT_NAME_UPPER}_MACROS "MIDILAR_VELOCITY_SHAPER")
    endif()
endif()


# Other conditional flags can be added here as needed
######################################################################################################