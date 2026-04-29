# Desktop build for TanhNorm_Buffer

add_executable(TanhNorm_Buffer
    TanhNorm_Buffer.cpp
)

target_link_libraries(TanhNorm_Buffer
    PRIVATE
        MIDILAR
)
