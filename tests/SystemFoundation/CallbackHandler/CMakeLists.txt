######################################################################################################
# Build and Link Tests for CallbackHandler Module

    # Add the test executable for CallbackHandler
    add_executable(SystemFoundation_CallbackHandler_Test
        SystemFoundation_CallbackHandler.cc
    )

    # Link the test executable with gtest, gtest_main, and the MIDILAR library
    target_link_libraries(SystemFoundation_CallbackHandler_Test
        PRIVATE
            gtest
            gtest_main
            MIDILAR
    )

    # Register the test with CTest
    gtest_discover_tests(SystemFoundation_CallbackHandler_Test)
#
######################################################################################################