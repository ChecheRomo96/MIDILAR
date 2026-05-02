# Desktop build for TanhHalf_Buffer

add_executable(TanhHalf_Buffer
    TanhHalf_Buffer.cpp
)

target_link_libraries(TanhHalf_Buffer
    PRIVATE
        MIDILAR
)
