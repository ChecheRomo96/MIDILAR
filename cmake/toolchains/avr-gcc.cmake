# MIDILAR compatibility wrapper for RoModularBuild.
# Target profiles continue to set the established MIDILAR_* variables.

include("${CMAKE_CURRENT_LIST_DIR}/MIDILARRoModularCompatibility.cmake")

midilar_forward_toolchain_cache(MIDILAR_AVR_TOOLCHAIN_ROOT ROMODULAR_AVR_TOOLCHAIN_ROOT
    PATH "" "Optional AVR-GCC installation root")
midilar_forward_toolchain_cache(MIDILAR_AVR_TOOLCHAIN_PREFIX ROMODULAR_AVR_TOOLCHAIN_PREFIX
    STRING "avr" "AVR-GCC compiler prefix")
midilar_forward_toolchain_cache(MIDILAR_AVR_MCU ROMODULAR_AVR_MCU
    STRING "" "AVR MCU name accepted by -mmcu")
midilar_forward_toolchain_cache(MIDILAR_AVR_ARCHITECTURE ROMODULAR_AVR_ARCHITECTURE
    STRING "avr" "AVR architecture used as CMAKE_SYSTEM_PROCESSOR metadata")
midilar_forward_toolchain_cache(MIDILAR_AVR_ADDITIONAL_FLAGS ROMODULAR_AVR_ADDITIONAL_FLAGS
    STRING "" "Additional flags shared by C and C++")

set(MIDILAR_ROMODULAR_ROOT
    "${CMAKE_CURRENT_LIST_DIR}/../../tools/RoModularBuild")
set(MIDILAR_ROMODULAR_AVR_TOOLCHAIN
    "${MIDILAR_ROMODULAR_ROOT}/cmake/toolchains/avr-gcc.cmake")
if(NOT EXISTS "${MIDILAR_ROMODULAR_AVR_TOOLCHAIN}")
    message(FATAL_ERROR
        "RoModularBuild is not initialized; run "
        "'git submodule update --init --recursive'")
endif()

include("${MIDILAR_ROMODULAR_AVR_TOOLCHAIN}")
