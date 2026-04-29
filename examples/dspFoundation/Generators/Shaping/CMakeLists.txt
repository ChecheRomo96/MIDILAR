midilar_add_dox(
    "${CMAKE_CURRENT_LIST_DIR}/Shaping.dox"
)

if(MIDILAR_DSP_GENERATORS_SHAPING_ATAN)
    add_subdirectory(ATan_Buffer)
    add_subdirectory(ATanHalf_Buffer)
    add_subdirectory(ATanNorm_Bias_Buffer)
endif()

if(MIDILAR_DSP_GENERATORS_SHAPING_GAMMA)
    add_subdirectory(Gamma_Buffer)
endif()

if(MIDILAR_DSP_GENERATORS_SHAPING_TANH)
    add_subdirectory(Tanh_Buffer)
    add_subdirectory(TanhHalf_Buffer)
    add_subdirectory(TanhHalfNorm_Buffer)
    add_subdirectory(TanhNorm_Buffer)
endif()
