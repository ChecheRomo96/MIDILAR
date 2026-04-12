#################################################################################################################################
# Output Targets Configuration

    option(${PROJECT_NAME_UPPER}_EXAMPLES "Enable building examples" OFF)
    option(${PROJECT_NAME_UPPER}_TESTING "Enable unit testing" OFF)
    option(MIDILAR_DOCS "Generate API documentation using Doxygen" OFF)
    option(MIDILAR_FULL_BUILD "Toggle on all modules" ON)
#
#################################################################################################################################
# SystemFoundation

    option(MIDILAR_SYSTEM_FOUNDATION "Enables the compilation of MIDILAR::SystemFoundation" ${MIDILAR_FULL_BUILD})

    #option(MIDILAR_TOOLS_EXAMPLES "Enables Building Examples" FALSE)
    #option(MIDILAR_TOOLS_TESTING "Enabled Unit Testing" FALSE)

    ##############################################################################################################################
    # Clock

        option(MIDILAR_CALLBACK_HANDLER "Enables the compilation of MIDILAR::SystemFoundation::CallbackHandler" ${MIDILAR_FULL_BUILD})
    #
    ##############################################################################################################################
    # Clock

        option(MIDILAR_SYSTEM_CLOCK "Enables the compilation of MIDILAR::SystemFoundation::Clock" ${MIDILAR_FULL_BUILD})
    #
    ##############################################################################################################################
#
#################################################################################################################################
# DSP Foundation

    option(MIDILAR_DSP_FOUNDATION "Enables the compilation of MIDILAR::AudioFoundation" ${MIDILAR_FULL_BUILD})

    #option(MIDILAR_TOOLS_EXAMPLES "Enables Building Examples" FALSE)
    #option(MIDILAR_TOOLS_TESTING "Enabled Unit Testing" FALSE)

    #############################################################################################################################
    # LUT

        option(MIDILAR_LUT "Enables the compilation of MIDILAR::dspFoundation::LUT" ${MIDILAR_FULL_BUILD})
        
        ##########################################################################################################################
        # LUT1D

            option(MIDILAR_LUT1D "Enables the compilation of MIDILAR::dspFoundation::LUT::LUT1D" ${MIDILAR_FULL_BUILD})
        #
        ##########################################################################################################################
        # LUT2D

            option(MIDILAR_LUT2D "Enables the compilation of MIDILAR::dspFoundation::LUT::LUT2D" ${MIDILAR_FULL_BUILD})
        #
        ##########################################################################################################################
        # LUT3D

            option(MIDILAR_LUT3D "Enables the compilation of MIDILAR::dspFoundation::LUT::LUT3D" ${MIDILAR_FULL_BUILD})
        #
        ##########################################################################################################################
        # Periodic

            option(MIDILAR_LUT_PERIODIC "Enables the compilation of MIDILAR::dspFoundation::LUT::Periodic" ${MIDILAR_FULL_BUILD})
            
            ######################################################################################################################
            # SineLUT

                option(MIDILAR_LUT_PERIODIC_SINE "Enables the compilation of MIDILAR::dspFoundation::LUT::Periodic::SineLUT" ${MIDILAR_FULL_BUILD})
            #
            ######################################################################################################################
            # SineLUT

                option(MIDILAR_LUT_PERIODIC_TRIANGLE "Enables the compilation of MIDILAR::dspFoundation::LUT::Periodic::TriangleLUT" ${MIDILAR_FULL_BUILD})
            #
            ######################################################################################################################
            # SineLUT

                option(MIDILAR_LUT_PERIODIC_SAW "Enables the compilation of MIDILAR::dspFoundation::LUT::Periodic::SawLUT" ${MIDILAR_FULL_BUILD})
            #
            ######################################################################################################################
            # SineLUT

                option(MIDILAR_LUT_PERIODIC_SQUARE "Enables the compilation of MIDILAR::dspFoundation::LUT::Periodic::SquareLUT" ${MIDILAR_FULL_BUILD})
            #
            ######################################################################################################################
        #
        ##########################################################################################################################
        # Shaping

            option(MIDILAR_LUT_SHAPING "Enables the compilation of MIDILAR::dspFoundation::LUT::Shaping" ${MIDILAR_FULL_BUILD})
            
            ######################################################################################################################
            # Gamma LUT

                option(MIDILAR_GAMMA_LUT "Enables the compilation of MIDILAR::dspFoundation::LUT::Shaping::GammaLUT" ${MIDILAR_FULL_BUILD})
            #
            ######################################################################################################################
        #
        ##########################################################################################################################
    #
    ##############################################################################################################################
    # Interpolators

        option(MIDILAR_INTERPOLATORS "Enables the compilation of MIDILAR::dspFoundation::Interpolators" ${MIDILAR_FULL_BUILD})
            
        ##########################################################################################################################
        # Linear

            option(MIDILAR_INTERPOLATORS_LINEAR "Enables the compilation of MIDILAR::dspFoundation::Interpolators::Linear" ${MIDILAR_FULL_BUILD})
        #
        ##########################################################################################################################
        # Bilinear

            option(MIDILAR_INTERPOLATORS_BILINEAR "Enables the compilation of MIDILAR::dspFoundation::Interpolators::Bilinear" ${MIDILAR_FULL_BUILD})
        #
        ##########################################################################################################################
        # Trilinear

            option(MIDILAR_INTERPOLATORS_TRILINEAR "Enables the compilation of MIDILAR::dspFoundation::Interpolators::Trilinear" ${MIDILAR_FULL_BUILD})
        #
        ##########################################################################################################################
    #
    ##############################################################################################################################
    # Generators

        option(MIDILAR_GENERATORS "Enables the compilation of MIDILAR::dspFoundation::Generators" ${MIDILAR_FULL_BUILD})
    
        ######################################################################################################################
        # Periodic

            option(MIDILAR_GENERATORS_PERIODIC "Enables the compilation of MIDILAR::dspFoundation::Generators::Periodic" ${MIDILAR_FULL_BUILD})
        
            ######################################################################################################################
            # Sine

                option(MIDILAR_GENERATORS_SINE "Enables the compilation of MIDILAR::dspFoundation::Generators::Periodic::Sine" ${MIDILAR_FULL_BUILD})
            #
            ######################################################################################################################
            # Square

                option(MIDILAR_GENERATORS_SQUARE "Enables the compilation of MIDILAR::dspFoundation::Generators::Periodic::Square" ${MIDILAR_FULL_BUILD})
            #
            ######################################################################################################################
            # Triangle

                option(MIDILAR_GENERATORS_TRIANGLE "Enables the compilation of MIDILAR::dspFoundation::Generators::Periodic::Triangle" ${MIDILAR_FULL_BUILD})
            #
            ######################################################################################################################
            # Sawtooth

                option(MIDILAR_GENERATORS_SAW "Enables the compilation of MIDILAR::dspFoundation::Generators::Periodic::Saw" ${MIDILAR_FULL_BUILD})
            #
            ######################################################################################################################
    
        #
        ######################################################################################################################
        # Shaping

            option(MIDILAR_GENERATORS_SHAPING "Enables the compilation of MIDILAR::dspFoundation::Generators::Shaping" ${MIDILAR_FULL_BUILD})

            ######################################################################################################################
            # Gamma

                option(MIDILAR_GENERATORS_GAMMA "Enables the compilation of MIDILAR::dspFoundation::Generators::Shaping::Gamma" ${MIDILAR_FULL_BUILD})
            #
            ######################################################################################################################
            # Enhanced Gamma

                option(MIDILAR_GENERATORS_ENHANCED_GAMMA "Enables the compilation of MIDILAR::dspFoundation::Generators::Shaping::EnhancedGamma" ${MIDILAR_FULL_BUILD})
            #
            ######################################################################################################################

        #
        ######################################################################################################################
        # Windowing

            option(MIDILAR_GENERATORS_WINDOWING "Enables the compilation of MIDILAR::dspFoundation::Generators::Windowing" ${MIDILAR_FULL_BUILD})
        #
        ######################################################################################################################
        # Envelopes

            option(MIDILAR_GENERATORS_ENVELOPES "Enables the compilation of MIDILAR::dspFoundation::Generators::Envelopes" ${MIDILAR_FULL_BUILD})
        #
        ######################################################################################################################
    #
    ##########################################################################################################################
    # Streaming

        option(MIDILAR_STREAMING "Enables the compilation of MIDILAR::dspFoundation::Generators" ${MIDILAR_FULL_BUILD})
            
        ##########################################################################################################################
        # Logic Streaming Devices

            option(MIDILAR_STREAMING_LOGIC "Enables the compilation of MIDILAR::dspFoundation::Generators::Periodic" ${MIDILAR_FULL_BUILD})
        
            ######################################################################################################################
            # Windowing

                option(MIDILAR_LEVEL_TRANSITION_TRACKER "Enables the compilation of MIDILAR::dspFoundation::Generators::Shaping" ${MIDILAR_FULL_BUILD})

                ##################################################################################################################
                # Edge Detector

                    option(MIDILAR_STREAMING_EDGE_DETECTOR "Enables the compilation of MIDILAR::dspFoundation::Generators::Windowing" ${MIDILAR_FULL_BUILD})
                #
                ##################################################################################################################
                # Schmitt Trigger

                    option(MIDILAR_STREAMING_SCHMITT_TRIGGER "Enables the compilation of MIDILAR::dspFoundation::Generators::Envelopes" ${MIDILAR_FULL_BUILD})
                #
                ##################################################################################################################    
            #
            ######################################################################################################################        
        #
        ##########################################################################################################################
    #
    ##############################################################################################################################
    # Transforms

        option(MIDILAR_TRANSFORMS "Enables the compilation of MIDILAR::dspFoundation::Transforms" ${MIDILAR_FULL_BUILD})
        
        ##########################################################################################################################
        # Logic Streaming Devices

            option(MIDILAR_DFT "Enables the compilation of MIDILAR::dspFoundation::Transforms::DFT" ${MIDILAR_FULL_BUILD})
        #
        ##########################################################################################################################
    #
    ##############################################################################################################################
#
##################################################################################################################################
# MIDI Foundation

    option(MIDILAR_MIDI_FOUNDATION "Enables the compilation of MIDILAR::MidiFoundation" ${MIDILAR_FULL_BUILD})

    #############################################################################################################################
    # ProtocolEnums

        option(MIDILAR_PROTOCOL_ENUMS "Enables the compilation of MIDILAR::MidiFoundation::Protocol Namespace" ${MIDILAR_FULL_BUILD})
    #
    ##############################################################################################################################
    # Midi Message

        option(MIDILAR_MIDI_MESSAGE "Enables the compilation of MIDILAR::MidiFoundation::Message" ${MIDILAR_FULL_BUILD})
    #
    ##############################################################################################################################
    # Message Parser

        option(MIDILAR_MIDI_MESSAGE_PARSER "Enables the compilation of MIDILAR::MidiFoundation::MessageParser" ${MIDILAR_FULL_BUILD})
    #
    ##############################################################################################################################
    # MidiProcessor

        option(MIDILAR_MIDI_PROCESSOR "Enables the compilation of MIDILAR::MidiFoundation::Processor" ${MIDILAR_FULL_BUILD})
    #
    ##############################################################################################################################
#
##################################################################################################################################
# MIDI Devices

    option(MIDILAR_MIDI_DEVICES "Enables the compilation of MIDILAR::MidiDevices" ${MIDILAR_FULL_BUILD})

    #option(MIDILAR_TOOLS_EXAMPLES "Enables Building Examples" FALSE)
    #option(MIDILAR_TOOLS_TESTING "Enabled Unit Testing" FALSE)

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
