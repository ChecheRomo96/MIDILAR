# Desktop build for Tanh_Buffer

add_executable(Tanh_Buffer
    Tanh_Buffer.cpp
)

target_link_libraries(Tanh_Buffer
    PRIVATE
        MIDILAR
)
