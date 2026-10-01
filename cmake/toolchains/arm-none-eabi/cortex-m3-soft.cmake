set(MIDILAR_ARM_CPU "cortex-m3" CACHE STRING "Target Arm CPU" FORCE)
set(MIDILAR_FLOAT_ABI "soft" CACHE STRING "Target floating-point ABI" FORCE)
set(MIDILAR_FPU "" CACHE STRING "Target FPU name" FORCE)

include("${CMAKE_CURRENT_LIST_DIR}/../arm-none-eabi.cmake")
