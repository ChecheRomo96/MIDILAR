add_executable(MIDILAR_SineLUT SineLUT_Plot.cpp)
target_link_libraries(MIDILAR_SineLUT MIDILAR)

add_executable(MIDILAR_LogExpLUT LogExpLUT_Plot.cpp)
target_link_libraries(MIDILAR_LogExpLUT MIDILAR)