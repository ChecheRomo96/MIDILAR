# MIDILAR compatibility wrapper for RoModularBuild.
# Target profiles continue to set the established MIDILAR_* variables.

include("${CMAKE_CURRENT_LIST_DIR}/MIDILARRoModularCompatibility.cmake")

midilar_forward_toolchain_cache(MIDILAR_ARM_TOOLCHAIN_ROOT ROMODULAR_ARM_TOOLCHAIN_ROOT
    PATH "" "Optional GNU Arm Embedded installation root")
midilar_forward_toolchain_cache(MIDILAR_ARM_TOOLCHAIN_PREFIX ROMODULAR_ARM_TOOLCHAIN_PREFIX
    STRING "arm-none-eabi" "GNU Arm Embedded compiler prefix")
midilar_forward_toolchain_cache(MIDILAR_ARM_CPU ROMODULAR_ARM_CPU
    STRING "cortex-m3" "Target Arm CPU")
midilar_forward_toolchain_cache(MIDILAR_FLOAT_ABI ROMODULAR_FLOAT_ABI
    STRING "soft" "Target floating-point ABI")
midilar_forward_toolchain_cache(MIDILAR_FPU ROMODULAR_FPU
    STRING "" "Target FPU name")
midilar_forward_toolchain_cache(MIDILAR_ARM_ADDITIONAL_FLAGS ROMODULAR_ARM_ADDITIONAL_FLAGS
    STRING "" "Additional flags shared by C and C++")
midilar_forward_toolchain_cache(MIDILAR_ARM_SYSROOT ROMODULAR_ARM_SYSROOT
    PATH "" "Optional target sysroot containing the C runtime headers and libraries")

set(MIDILAR_ROMODULAR_ROOT
    "${CMAKE_CURRENT_LIST_DIR}/../../tools/RoModularBuild")
set(MIDILAR_ROMODULAR_ARM_TOOLCHAIN
    "${MIDILAR_ROMODULAR_ROOT}/cmake/toolchains/arm-none-eabi.cmake")
if(NOT EXISTS "${MIDILAR_ROMODULAR_ARM_TOOLCHAIN}")
    message(FATAL_ERROR
        "RoModularBuild is not initialized; run "
        "'git submodule update --init --recursive'")
endif()

include("${MIDILAR_ROMODULAR_ARM_TOOLCHAIN}")
