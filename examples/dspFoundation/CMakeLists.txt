midilar_add_dox(
    "${CMAKE_CURRENT_LIST_DIR}/dspFoundation.dox"
)

if(MIDILAR_DSP_LUT)
    add_subdirectory(LUT)
endif()


if(MIDILAR_DSP_GENERATORS)
    add_subdirectory(Generators)
endif()