#################################################################################################################################
# CMake options for MIDILAR
    if(MIDILAR_FULL_BUILD)
        set(MIDILAR_GENERATORS ON)
        set(MIDILAR_INTERPOLATORS ON)
        set(MIDILAR_LUT ON)
        set(MIDILAR_STREAMING ON)
        set(MIDILAR_TRANSFORMS ON)
    endif()
#
#################################################################################################################################
# Generators

    option(MIDILAR_GENERATORS "Enables the compilation of MIDILAR::dspFoundation::Generators" ON)
    include(${CMAKE_CURRENT_LIST_DIR}/Generators/CMakeOptions.cmake)
#
##################################################################################################################################
# Interpolators

    option(MIDILAR_INTERPOLATORS "Enables the compilation of MIDILAR::dspFoundation::Interpolators" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/Interpolators/CMakeOptions.cmake)
#
##################################################################################################################################
# LUT

    option(MIDILAR_LUT "Enables the compilation of MIDILAR::dspFoundation::LUT" ON)
    include(${CMAKE_CURRENT_LIST_DIR}/LUT/CMakeOptions.cmake)
#
##################################################################################################################################
# Streaming

    option(MIDILAR_STREAMING "Enables the compilation of MIDILAR::dspFoundation::Streaming" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/Streaming/CMakeOptions.cmake)
#
##################################################################################################################################
# Transforms

    option(MIDILAR_TRANSFORMS "Enables the compilation of MIDILAR::dspFoundation::Transforms" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/Transforms/CMakeOptions.cmake)
#
##################################################################################################################################
