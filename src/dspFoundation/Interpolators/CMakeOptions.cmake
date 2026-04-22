#################################################################################################################################
# CMake options for MIDILAR
    if(MIDILAR_FULL_BUILD)
        set(MIDILAR_DSP_INTERPOLATORS_LINEAR ON)
        set(MIDILAR_DSP_INTERPOLATORS_BILINEAR ON)
        set(MIDILAR_DSP_INTERPOLATORS_TRILINEAR ON)
    endif()
#
#################################################################################################################################
# Linear Interpolation

    option(MIDILAR_DSP_INTERPOLATORS_LINEAR "Enables the compilation of MIDILAR::dspFoundation::Interpolators::Linear" ON)
#
##################################################################################################################################
# Bilinear Interpolation

    option(MIDILAR_DSP_INTERPOLATORS_BILINEAR "Enables the compilation of MIDILAR::dspFoundation::Interpolators::Bilinear" ON)
#
##################################################################################################################################
# Trilinear Interpolation

    option(MIDILAR_DSP_INTERPOLATORS_TRILINEAR "Enables the compilation of MIDILAR::dspFoundation::Interpolators::Trilinear" ON)
#
##################################################################################################################################
