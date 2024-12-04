######################################################################################################
# Build and Link Tests for Clock Module

    # Add the test executable for Clock
    add_executable(MIDILAR_MidiProcessor
        MidiProcessor.cc
    )

    # Link the test executable with gtest, gtest_main, and the MIDILAR library
    target_link_libraries(MIDILAR_MidiProcessor
        PRIVATE
            gtest
            gtest_main
            MIDILAR
    )

    # Include directories for MIDILAR headers
    target_include_directories(MIDILAR_MidiProcessor
        PRIVATE
            "$<BUILD_INTERFACE:${MIDILAR_SRC_DIRECTORY}>"
            "$<INSTALL_INTERFACE:include/MIDILAR-${MIDILAR_VERSION}>"
    )

    # Register the test with CTest
    gtest_discover_tests(MIDILAR_MidiProcessor)
#
######################################################################################################