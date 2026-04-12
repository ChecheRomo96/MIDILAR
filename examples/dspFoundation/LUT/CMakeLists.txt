if(${MIDILAR_LUT1D})
    add_subdirectory(LUT1D)
endif()

if(${MIDILAR_LUT2D})
    add_subdirectory(LUT2D)
endif()

if(${MIDILAR_LUT3D})
    add_subdirectory(LUT3D)
endif()

if(${MIDILAR_LUT_PERIODIC})
    add_subdirectory(Periodic)
endif()

add_subdirectory(Shaping)