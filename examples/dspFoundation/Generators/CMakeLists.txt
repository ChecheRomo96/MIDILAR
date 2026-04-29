midilar_add_dox(
    "${CMAKE_CURRENT_LIST_DIR}/Generators.dox"
)

if(MIDILAR_DSP_GENERATORS_SHAPING)
    add_subdirectory(Shaping)
endif()