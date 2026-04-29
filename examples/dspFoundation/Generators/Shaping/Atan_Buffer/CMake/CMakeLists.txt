# Desktop build for Atan_Buffer

add_executable(Atan_Buffer
    Atan_Buffer.cpp
)

target_link_libraries(Atan_Buffer
    PRIVATE
        MIDILAR
)
