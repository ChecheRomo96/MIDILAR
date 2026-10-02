option(MIDILAR_EXAMPLES "Enable building examples" OFF)
option(MIDILAR_TESTING "Enable unit testing" OFF)
option(MIDILAR_DOCS "Generate API documentation using Doxygen" OFF)
option(MIDILAR_FULL_BUILD "Enable every MIDILAR module" OFF)
option(MIDILAR_COVERAGE "Enable coverage instrumentation" OFF)

option(MIDILAR_CORE "Enable MIDILAR::Core" ON)
option(MIDILAR_PROTOCOL "Enable MIDILAR::Protocol" ON)
option(MIDILAR_PARSER_DIAGNOSTICS "Count aborted messages and ignored bytes in Midi1Parser" OFF)

if(MIDILAR_FULL_BUILD)
    set(MIDILAR_CORE ON CACHE BOOL "Enable MIDILAR::Core" FORCE)
    set(MIDILAR_PROTOCOL ON CACHE BOOL "Enable MIDILAR::Protocol" FORCE)
endif()
