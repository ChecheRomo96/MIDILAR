midilar_add_dox(
    "${CMAKE_CURRENT_LIST_DIR}/LUT.dox"
)

if(MIDILAR_DSP_LUT1D)
    add_subdirectory(LUT1D)
endif()