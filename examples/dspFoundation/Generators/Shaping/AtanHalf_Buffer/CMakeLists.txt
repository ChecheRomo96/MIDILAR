# Desktop build for AtanHalf_Buffer

add_executable(AtanHalf_Buffer
    AtanHalf_Buffer.cpp
)

target_link_libraries(AtanHalf_Buffer
    PRIVATE
        MIDILAR
)
