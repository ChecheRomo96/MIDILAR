#################################################################################################################################
# CMake options for MIDILAR
    if(MIDILAR_FULL_BUILD)
        set(MIDILAR_SYSTEM_FOUNDATION ON)
        set(MIDILAR_DSP_FOUNDATION ON)
        set(MIDILAR_MIDI_FOUNDATION ON)
        set(MIDILAR_MIDI_DEVICES ON)
    endif()
#
#################################################################################################################################
# SystemFoundation

    option(MIDILAR_SYSTEM_FOUNDATION "Enables the compilation of MIDILAR::SystemFoundation" ON)
    include(${CMAKE_CURRENT_LIST_DIR}/SystemFoundation/CMakeOptions.cmake)
#
#################################################################################################################################
# DSP Foundation

    option(MIDILAR_DSP_FOUNDATION "Enables the compilation of MIDILAR::AudioFoundation" ${MIDILAR_FULL_BUILD})

    #############################################################################################################################
    # LUT
    
        option(MIDILAR_LUT "Enables the compilation of MIDILAR::dspFoundation::LUT" ${MIDILAR_FULL_BUILD})
        
        option(MIDILAR_LUT1D "Enables the compilation of MIDILAR::dspFoundation::LUT::LUT1D" ${MIDILAR_FULL_BUILD})
        option(MIDILAR_LUT2D "Enables the compilation of MIDILAR::dspFoundation::LUT::LUT2D" ${MIDILAR_FULL_BUILD})
        option(MIDILAR_LUT3D "Enables the compilation of MIDILAR::dspFoundation::LUT::LUT3D" ${MIDILAR_FULL_BUILD})
        option(MIDILAR_LUT_PERIODIC "Enables the compilation of MIDILAR::dspFoundation::LUT::Periodic" ${MIDILAR_FULL_BUILD})
        option(MIDILAR_LUT_SHAPING "Enables the compilation of MIDILAR::dspFoundation::LUT::Shaping" ${MIDILAR_FULL_BUILD})
    #
    ##############################################################################################################################
    # Interpolators

        option(MIDILAR_INTERPOLATORS "Enables the compilation of MIDILAR::dspFoundation::Interpolators" ${MIDILAR_FULL_BUILD})
    #
    ##############################################################################################################################
    # Generators

        option(MIDILAR_GENERATORS "Enables the compilation of MIDILAR::dspFoundation::Generators" ${MIDILAR_FULL_BUILD})
    
        option(MIDILAR_GENERATORS_PERIODIC "Enables the compilation of MIDILAR::dspFoundation::Generators::Periodic" ${MIDILAR_FULL_BUILD})
        option(MIDILAR_GENERATORS_SHAPING "Enables the compilation of MIDILAR::dspFoundation::Generators::Shaping" ${MIDILAR_FULL_BUILD})
        option(MIDILAR_GENERATORS_WINDOWING "Enables the compilation of MIDILAR::dspFoundation::Generators::Windowing" ${MIDILAR_FULL_BUILD})
        option(MIDILAR_GENERATORS_ENVELOPES "Enables the compilation of MIDILAR::dspFoundation::Generators::Envelopes" ${MIDILAR_FULL_BUILD})
    #
    ##########################################################################################################################
    # Streaming

        option(MIDILAR_STREAMING "Enables the compilation of MIDILAR::dspFoundation::Streaming" ${MIDILAR_FULL_BUILD})
            
        option(MIDILAR_STREAMING_LOGIC "Enables the compilation of MIDILAR::dspFoundation::Streaming::Logic" ${MIDILAR_FULL_BUILD}) 
        option(MIDILAR_STREAMING_FILTERS "Enables the compilation of MIDILAR::dspFoundation::Streaming::Filters" ${MIDILAR_FULL_BUILD}) 
    #
    ##############################################################################################################################
    # Transforms

        option(MIDILAR_TRANSFORMS "Enables the compilation of MIDILAR::dspFoundation::Transforms" ${MIDILAR_FULL_BUILD})
        
        option(MIDILAR_TRANSFORMS_DFT "Enables the compilation of MIDILAR::dspFoundation::Transforms::DFT" ${MIDILAR_FULL_BUILD})
        option(MIDILAR_TRANSFORMS_FFT "Enables the compilation of MIDILAR::dspFoundation::Transforms::FFT" ${MIDILAR_FULL_BUILD})
        #option(MIDILAR_TRANSFORMS_DCT "Enables the compilation of MIDILAR::dspFoundation::Transforms::DCT" ${MIDILAR_FULL_BUILD})
    #
    ##############################################################################################################################
#
##################################################################################################################################
# MIDI Foundation

    option(MIDILAR_MIDI_FOUNDATION "Enables the compilation of MIDILAR::MidiFoundation" ${MIDILAR_FULL_BUILD})
#
##################################################################################################################################
# MIDI Devices

    option(MIDILAR_MIDI_DEVICES "Enables the compilation of MIDILAR::MidiDevices" ${MIDILAR_FULL_BUILD})

    #############################################################################################################################
    # Channel Reassign

        option(MIDILAR_VELOCITY_SHAPER "Enables the compilation of MIDILAR::MidiDevices::ChannelReassign" ${MIDILAR_FULL_BUILD})
    #
    ##############################################################################################################################
    # Channel Reassign

        option(MIDILAR_CHANNEL_REASSIGN "Enables the compilation of MIDILAR::MidiDevices::ChannelReassign" ${MIDILAR_FULL_BUILD})
    #
    ##############################################################################################################################
#
##################################################################################################################################