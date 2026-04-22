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
# Envelopes

    option(MIDILAR_LUT1D "Enables the compilation of MIDILAR::dspFoundation::LUT::LUT1D" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/LUT1D/CMakeOptions.cmake)
#
#################################################################################################################################
# Noise

    option(MIDILAR_LUT2D "Enables the compilation of MIDILAR::dspFoundation::LUT::LUT2D" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/LUT2D/CMakeOptions.cmake)
#
#################################################################################################################################
