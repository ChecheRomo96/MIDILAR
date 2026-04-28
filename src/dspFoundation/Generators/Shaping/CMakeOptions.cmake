#################################################################################################################################
# CMake options for MIDILAR
    if(MIDILAR_FULL_BUILD)
        set(MIDILAR_DSP_GENERATORS_SHAPING_GAMMA ON)
        set(MIDILAR_DSP_GENERATORS_SHAPING_GAMMA_MORPH ON)
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
