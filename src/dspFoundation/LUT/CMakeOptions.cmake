#################################################################################################################################
# CMake options for MIDILAR
    if(MIDILAR_FULL_BUILD)
        set(MIDILAR_DSP_LUT1D ON)
        set(MIDILAR_DSP_LUT2D ON)
        set(MIDILAR_DSP_LUT3D ON)
    endif()
#
#################################################################################################################################
# LUT1D

    option(MIDILAR_DSP_LUT1D "Enables the compilation of MIDILAR::dspFoundation::LUT::LUT1D" ON)
#
#################################################################################################################################
# LUT2D

    option(MIDILAR_DSP_LUT2D "Enables the compilation of MIDILAR::dspFoundation::LUT::LUT2D" ON)
#
#################################################################################################################################
# LUT3D

    option(MIDILAR_DSP_LUT3D "Enables the compilation of MIDILAR::dspFoundation::LUT::LUT3D" ON)
#
##################################################################################################################################
