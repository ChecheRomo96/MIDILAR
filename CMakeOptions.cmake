#################################################################################################################################
# Output Targets Configuration

    option(MIDILAR_EXAMPLES "Enable building examples" OFF)
    option(MIDILAR_TESTING "Enable unit testing" OFF)
    option(MIDILAR_DOCS "Generate API documentation using Doxygen" OFF)
    option(MIDILAR_FULL_BUILD "Toggle on all modules" OFF)
    option(MIDILAR_TESTING "Enable unit testing" OFF)
    option(MIDILAR_COVERAGE "Enable coverage testing" OFF)
    
    option(MIDILAR_DOCS "Build documentation" OFF)

#
#################################################################################################################################
# src CMake Options

    include(src/CMakeOptions.cmake)
#
#################################################################################################################################