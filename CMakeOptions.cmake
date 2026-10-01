option(MIDILAR_EXAMPLES "Enable building examples" OFF)
option(MIDILAR_TESTING "Enable unit testing" OFF)
option(MIDILAR_DOCS "Generate API documentation using Doxygen" OFF)
option(MIDILAR_FULL_BUILD "Enable every MIDILAR module" OFF)
option(MIDILAR_COVERAGE "Enable coverage instrumentation" OFF)

option(MIDILAR_CORE "Enable MIDILAR::Core" ON)

if(MIDILAR_FULL_BUILD)
    set(MIDILAR_CORE ON CACHE BOOL "Enable MIDILAR::Core" FORCE)
endif()
