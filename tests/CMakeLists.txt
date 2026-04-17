######################################################################################################
# Enable testing

    enable_testing()
    include(GoogleTest)
#
######################################################################################################
# Add Subdirectories for Tests

    if(MIDILAR_SYSTEM_FOUNDATION)
        add_subdirectory(SystemFoundation)
    endif()

    if(MIDILAR_MIDI_FOUNDATION)
        add_subdirectory(MidiFoundation)
    endif()

    if(MIDILAR_DSP_FOUNDATION)
        add_subdirectory(dspFoundation)
    endif()

    if(MIDILAR_MIDI_PROCESSOR)
        # add_subdirectory(MidiProcessor)
    endif()
#
######################################################################################################
# Optional coverage target

    if(MIDILAR_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        find_program(LCOV_EXECUTABLE lcov)
        find_program(GENHTML_EXECUTABLE genhtml)

        if(LCOV_EXECUTABLE AND GENHTML_EXECUTABLE)
            add_custom_target(MIDILAR_run_tests_coverage
                COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG> --output-on-failure
                COMMAND ${LCOV_EXECUTABLE} --capture --directory ${CMAKE_BINARY_DIR} --output-file coverage.info
                COMMAND ${LCOV_EXECUTABLE} --remove coverage.info
                        '/usr/*'
                        '*/_deps/*'
                        '*/tests/*'
                        --output-file coverage_filtered.info
                COMMAND ${GENHTML_EXECUTABLE} coverage_filtered.info --output-directory coverage
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Running MIDILAR tests and generating coverage report"
            )
        else()
            message(WARNING "MIDILAR_COVERAGE is ON, but lcov/genhtml were not found")
        endif()
    endif()
#
######################################################################################################