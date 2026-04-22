#################################################################################################################################
# CMake options for MIDILAR
    if(MIDILAR_FULL_BUILD)
        set(MIDILAR_GENERATORS_PERIODIC_SINE ON)
        set(MIDILAR_GENERATORS_PERIODIC_SQUARE ON)
        set(MIDILAR_GENERATORS_PERIODIC_TRIANGLE ON)
        set(MIDILAR_GENERATORS_PERIODIC_SAW ON)
    endif()
#
#################################################################################################################################
# Sine Wave

    option(MIDILAR_GENERATORS_PERIODIC_SINE "Enables the compilation of MIDILAR::dspFoundation::Generators::Sine" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/Sine/CMakeOptions.cmake)
#
#################################################################################################################################
# Square Wave

    option(MIDILAR_GENERATORS_PERIODIC_SQUARE "Enables the compilation of MIDILAR::dspFoundation::Generators::Square" ON)
    #include(${CMAKE_CURRENT_LIST_DIR}/Square/CMakeOptions.cmake)
#
#################################################################################################################################
