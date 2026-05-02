#################################################################################################################################
# CMake options for MIDILAR
    if(MIDILAR_FULL_BUILD)
        set(MIDILAR_DSP_GENERATORS_PERIODIC_SINE ON)
        set(MIDILAR_DSP_GENERATORS_PERIODIC_SQUARE ON)
        set(MIDILAR_DSP_GENERATORS_PERIODIC_TRIANGLE ON)
        set(MIDILAR_DSP_GENERATORS_PERIODIC_SAW ON)
    endif()
#
#################################################################################################################################
# Sine Wave

    option(MIDILAR_DSP_GENERATORS_PERIODIC_SINE "Enables the compilation of MIDILAR::DspCore::Generators::Periodic::Sine" ON)
#
#################################################################################################################################
# Square Wave

    option(MIDILAR_DSP_GENERATORS_PERIODIC_SQUARE "Enables the compilation of MIDILAR::DspCore::Generators::Periodic::Square" ON)
#
#################################################################################################################################
# Triangle Wave

    option(MIDILAR_DSP_GENERATORS_PERIODIC_TRIANGLE "Enables the compilation of MIDILAR::DspCore::Generators::Periodic::Triangle" ON)
#
#################################################################################################################################
# Sawtooth Wave

    option(MIDILAR_DSP_GENERATORS_PERIODIC_SAW "Enables the compilation of MIDILAR::DspCore::Generators::Periodic::Saw" ON)
#
#################################################################################################################################
