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

    # Include directories for MIDILAR headers
    target_include_directories(SystemFoundation_CallbackHandler_Test
        PRIVATE
            "$<BUILD_INTERFACE:${MIDILAR_SRC_DIRECTORY}>"
            "$<INSTALL_INTERFACE:include/MIDILAR-${MIDILAR_VERSION}>"
    )

    
    target_compile_options(SystemFoundation_CallbackHandler_Test PRIVATE -coverage)
    target_link_options(SystemFoundation_CallbackHandler_Test PRIVATE -coverage)

    # Register the test with CTest
    gtest_discover_tests(SystemFoundation_CallbackHandler_Test)
#
######################################################################################################