#################################################################################################################################
# CMake options for MIDILAR
    if(MIDILAR_FULL_BUILD)
        set(MIDILAR_DSP_GENERATORS_SHAPING_GAMMA ON)
        set(MIDILAR_DSP_GENERATORS_SHAPING_GAMMA_MORPH ON)
        set(MIDILAR_DSP_GENERATORS_SHAPING_ATAN ON)
        set(MIDILAR_DSP_GENERATORS_SHAPING_TANH ON)
    endif()
#
#################################################################################################################################
# Gamma

    option(MIDILAR_DSP_GENERATORS_SHAPING_GAMMA "Enables the compilation of MIDILAR::dspFoundation::Generators::Shaping::Gamma" ON)
#
#################################################################################################################################
# Gamma Morph

    option(MIDILAR_DSP_GENERATORS_SHAPING_GAMMA_MORPH "Enables the compilation of MIDILAR::dspFoundation::Generators::Shaping::GammaMorph" ON)
#
#################################################################################################################################
# Atan

    option(MIDILAR_DSP_GENERATORS_SHAPING_ATAN "Enables the compilation of MIDILAR::dspFoundation::Generators::Shaping::Atan" ON)
#
#################################################################################################################################
# Tanh

    option(MIDILAR_DSP_GENERATORS_SHAPING_TANH "Enables the compilation of MIDILAR::dspFoundation::Generators::Shaping::Tanh" ON)
#
#################################################################################################################################
