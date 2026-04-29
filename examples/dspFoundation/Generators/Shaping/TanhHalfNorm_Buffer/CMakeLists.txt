# Desktop build for TanhHalfNorm_Buffer

add_executable(TanhHalfNorm_Buffer
    TanhHalfNorm_Buffer.cpp
)

target_link_libraries(TanhHalfNorm_Buffer
    PRIVATE
        MIDILAR
)
