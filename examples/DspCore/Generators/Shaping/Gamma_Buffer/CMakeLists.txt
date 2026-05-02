# Desktop build for Gamma_Buffer

add_executable(Gamma_Buffer
    Gamma_Buffer.cpp
)

target_link_libraries(Gamma_Buffer
    PRIVATE
        MIDILAR
)
