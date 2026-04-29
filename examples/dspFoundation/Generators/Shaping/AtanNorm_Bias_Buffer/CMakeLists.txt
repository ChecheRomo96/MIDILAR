# Desktop build for AtanNorm_Bias_Buffer

add_executable(AtanNorm_Bias_Buffer
    AtanNorm_Bias_Buffer.cpp
)

target_link_libraries(AtanNorm_Bias_Buffer
    PRIVATE
        MIDILAR
)
