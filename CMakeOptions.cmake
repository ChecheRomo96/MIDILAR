#set(MIDILAR_BUILD_TARGET "Submodule" CACHE STRING "MIDILAR build target")
#set_property(CACHE MIDILAR_BUILD_TARGET PROPERTY STRINGS "Dev" "Submodule" "Export" "Install")

option(${PROJECT_NAME_UPPER}_EXAMPLES "Enable building examples" OFF)
option(${PROJECT_NAME_UPPER}_TESTING "Enable unit testing" OFF)
option(MIDILAR_DOCS "Generate API documentation using Doxygen" OFF)

#################################################################################################
# SystemFoundation

    option(MIDILAR_SYSTEM_FOUNDATION "Enables the compilation of MIDILAR::SystemFoundation" ON)

    #option(MIDILAR_TOOLS_EXAMPLES "Enables Building Examples" FALSE)
    #option(MIDILAR_TOOLS_TESTING "Enabled Unit Testing" FALSE)

    ##############################################################################################
    # Clock

        option(MIDILAR_CALLBACK_HANDLER "Enables the compilation of MIDILAR::SystemFoundation::CallbackHandler" ON)
    #
    ##############################################################################################
    # Clock

        option(MIDILAR_SYSTEM_CLOCK "Enables the compilation of MIDILAR::SystemFoundation::Clock" ON)
    #
    ##############################################################################################
#
#################################################################################################
# AudioFoundation

    option(MIDILAR_DSP_FOUNDATION "Enables the compilation of MIDILAR::AudioFoundation" ON)

    #option(MIDILAR_TOOLS_EXAMPLES "Enables Building Examples" FALSE)
    #option(MIDILAR_TOOLS_TESTING "Enabled Unit Testing" FALSE)

    #############################################################################################
    # LUT

        option(MIDILAR_LUT "Enables the compilation of MIDILAR::AudioFoundation::LUT::BaseLUT" ON)
    #
    ##############################################################################################
#
##################################################################################################
# MIDI Foundation

    option(MIDILAR_MIDI_FOUNDATION "Enables the compilation of MIDILAR::MidiFoundation" ON)

    #############################################################################################
    # ProtocolEnums

        option(MIDILAR_PROTOCOL_ENUMS "Enables the compilation of MIDILAR::MidiFoundation::Protocol Namespace" ON)
    #
    ##############################################################################################
    # Midi Message

        option(MIDILAR_MIDI_MESSAGE "Enables the compilation of MIDILAR::MidiFoundation::Message" ON)
    #
    ##############################################################################################
    # Message Parser

        option(MIDILAR_MIDI_MESSAGE_PARSER "Enables the compilation of MIDILAR::MidiFoundation::MessageParser" ON)
    #
    ##############################################################################################
    # MidiProcessor

        option(MIDILAR_MIDI_PROCESSOR "Enables the compilation of MIDILAR::MidiFoundation::Processor" ON)
    #
    ##############################################################################################
#
##################################################################################################
# MIDI Processors

    option(MIDILAR_MIDI_PROCESSORS "Enables the compilation of MIDILAR::MidiProcessors" ON)

    #option(MIDILAR_TOOLS_EXAMPLES "Enables Building Examples" FALSE)
    #option(MIDILAR_TOOLS_TESTING "Enabled Unit Testing" FALSE)

    #############################################################################################
    # 

    #
    ##############################################################################################
#
##################################################################################################
