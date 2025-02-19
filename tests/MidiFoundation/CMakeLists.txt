######################################################################################################
# Build and Link Tests for Midi Protocol Module

    # Add the test executable for Clock
    add_executable(MidiFoundation_Protocol_Test
    MidiFoundation_Protocol.cc
    )

    # Link the test executable with gtest, gtest_main, and the MIDILAR library
    target_link_libraries(MidiFoundation_Protocol_Test
        PRIVATE
            gtest
            gtest_main
            MIDILAR
    )

    # Include directories for MIDILAR headers
    target_include_directories(MidiFoundation_Protocol_Test
        PRIVATE
            "$<BUILD_INTERFACE:${MIDILAR_SRC_DIRECTORY}>"
            "$<INSTALL_INTERFACE:include/MIDILAR-${MIDILAR_VERSION}>"
    )

    
    target_compile_options(MidiFoundation_Protocol_Test PRIVATE -coverage)
    target_link_options(MidiFoundation_Protocol_Test PRIVATE -coverage)

    # Register the test with CTest
    gtest_discover_tests(MidiFoundation_Protocol_Test)
#
######################################################################################################
# Build and Link Tests for Midi Message Module

    # Add the test executable for Message
    add_executable(MidiFoundation_Message_Test
    MidiFoundation_Message.cc
    )

    # Link the test executable with gtest, gtest_main, and the MIDILAR library
    target_link_libraries(MidiFoundation_Message_Test
        PRIVATE
            gtest
            gtest_main
            MIDILAR
    )

    # Include directories for MIDILAR headers
    target_include_directories(MidiFoundation_Message_Test
        PRIVATE
            "$<BUILD_INTERFACE:${MIDILAR_SRC_DIRECTORY}>"
            "$<INSTALL_INTERFACE:include/MIDILAR-${MIDILAR_VERSION}>"
    )

    
    target_compile_options(MidiFoundation_Message_Test PRIVATE -coverage)
    target_link_options(MidiFoundation_Message_Test PRIVATE -coverage)

    # Register the test with CTest
    gtest_discover_tests(MidiFoundation_Message_Test)
#
######################################################################################################