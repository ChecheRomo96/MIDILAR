#################################################################################################################################
# CMake options for MIDILAR
    if(MIDILAR_FULL_BUILD)
        set(MIDILAR_DSP_GENERATORS_ENVELOPES ON)
        set(MIDILAR_DSP_GENERATORS_PERIODIC ON)
        set(MIDILAR_DSP_GENERATORS_SHAPING ON)
        set(MIDILAR_DSP_GENERATORS_WINDOWING ON)
    endif()
#
#################################################################################################################################
# Envelopes

    option(MIDILAR_DSP_GENERATORS_ENVELOPES "Enables the compilation of MIDILAR::dspFoundation::Generators::Envelopes" ON)
    include(${CMAKE_CURRENT_LIST_DIR}/Envelopes/CMakeOptions.cmake)
#
#################################################################################################################################
# Noise

    option(MIDILAR_DSP_GENERATORS_NOISE "Enables the compilation of MIDILAR::dspFoundation::Generators::Noise" ON)
    include(${CMAKE_CURRENT_LIST_DIR}/Noise/CMakeOptions.cmake)
#
##################################################################################################################################
# Periodic

    option(MIDILAR_DSP_GENERATORS_PERIODIC "Enables the compilation of MIDILAR::dspFoundation::Generators::Periodic" ON)
    include(${CMAKE_CURRENT_LIST_DIR}/Periodic/CMakeOptions.cmake)
#
##################################################################################################################################
# Shaping

    option(MIDILAR_DSP_GENERATORS_SHAPING "Enables the compilation of MIDILAR::dspFoundation::Generators::Shaping" ON)
    include(${CMAKE_CURRENT_LIST_DIR}/Shaping/CMakeOptions.cmake)
#
##################################################################################################################################
# Windowing

    option(MIDILAR_DSP_GENERATORS_WINDOWING "Enables the compilation of MIDILAR::dspFoundation::Generators::Windowing" ON)
    include(${CMAKE_CURRENT_LIST_DIR}/Windowing/CMakeOptions.cmake)
#
##################################################################################################################################
