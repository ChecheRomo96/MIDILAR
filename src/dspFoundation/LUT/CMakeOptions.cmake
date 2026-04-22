#################################################################################################################################
# CMake options for MIDILAR
    if(MIDILAR_FULL_BUILD)
        set(MIDILAR_LUT1D ON)
        set(MIDILAR_LUT2D ON)
        set(MIDILAR_LUT3D ON)
        set(MIDILAR_LUT_PERIODIC ON)
        set(MIDILAR_LUT_SHAPING ON)
    endif()
#
#################################################################################################################################
# LUT1D

    option(MIDILAR_LUT1D "Enables the compilation of MIDILAR::dspFoundation::LUT::LUT1D" ON)
#
#################################################################################################################################
# LUT2D

    option(MIDILAR_LUT2D "Enables the compilation of MIDILAR::dspFoundation::LUT::LUT2D" ON)
#
#################################################################################################################################
# LUT3D

    option(MIDILAR_LUT3D "Enables the compilation of MIDILAR::dspFoundation::LUT::LUT3D" ON)
#
##################################################################################################################################
# LUTPeriodic

    option(MIDILAR_LUT_PERIODIC "Enables the compilation of MIDILAR::dspFoundation::LUT::LUTPeriodic" ON)
#
##################################################################################################################################
# LUTShaping

    option(MIDILAR_LUT_SHAPING "Enables the compilation of MIDILAR::dspFoundation::LUT::LUTShaping" ON)
#
##################################################################################################################################
