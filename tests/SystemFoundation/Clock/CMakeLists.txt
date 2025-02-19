######################################################################################################
# Build and Link Tests for Clock Module

    # Add the test executable for Clock
    add_executable(SystemFoundation_Clock_Test
        SystemFoundation_Clock.cc
    )

    # Link the test executable with gtest, gtest_main, and the MIDILAR library
    target_link_libraries(SystemFoundation_Clock_Test
        PRIVATE
            gtest
            gtest_main
            MIDILAR
    )

    # Include directories for MIDILAR headers
    target_include_directories(SystemFoundation_Clock_Test
        PRIVATE
            "$<BUILD_INTERFACE:${MIDILAR_SRC_DIRECTORY}>"
            "$<INSTALL_INTERFACE:include/MIDILAR-${MIDILAR_VERSION}>"
    )

    
    target_compile_options(SystemFoundation_Clock_Test PRIVATE -coverage)
    target_link_options(SystemFoundation_Clock_Test PRIVATE -coverage)

    # Register the test with CTest
    gtest_discover_tests(SystemFoundation_Clock_Test)
#
######################################################################################################