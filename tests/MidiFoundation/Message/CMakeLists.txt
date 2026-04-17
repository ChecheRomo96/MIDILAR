######################################################################################################
# Build and Link Tests for CallbackHandler Module

    # Add the test executable for CallbackHandler
    add_executable(MIDILAR_Midi_Message_Tests
        MidiFoundation_Message.cc
    )

    # Link the test executable with gtest, gtest_main, and the MIDILAR library
    target_link_libraries(MIDILAR_Midi_Message_Tests
        PRIVATE
            gtest
            gtest_main
            MIDILAR
    )

    # Register the test with CTest
    gtest_discover_tests(MIDILAR_Midi_Message_Tests)
#
######################################################################################################