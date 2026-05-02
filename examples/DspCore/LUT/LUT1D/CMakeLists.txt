add_subdirectory(BasicFill)
add_subdirectory(FunctionTable)
add_subdirectory(ResizeReserve)
add_subdirectory(RawData)
add_subdirectory(MoveSemantics)
add_subdirectory(ManualOscillator)

if(MIDILAR_DSP_GENERATORS)
    add_subdirectory(GeneratorsTest)
endif()

midilar_add_dox(
    "${CMAKE_CURRENT_LIST_DIR}/LUT1D.dox"
)