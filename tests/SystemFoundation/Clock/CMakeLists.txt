set(MIDILAR_SYSTEM_CLOCK_TEST_SOURCES
    Clock_ConstructionTests.cc
    Clock_CallbackBindingTests.cc
    Clock_FrequencyTests.cc
    Clock_TimeRefreshTests.cc
    Clock_EdgeCaseTests.cc
)

add_executable(MIDILAR_System_Clock_Tests
    ${MIDILAR_SYSTEM_CLOCK_TEST_SOURCES}
)

target_link_libraries(MIDILAR_System_Clock_Tests
    PRIVATE
        MIDILAR
        gtest_main
)

target_include_directories(MIDILAR_System_Clock_Tests
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

gtest_discover_tests(MIDILAR_System_Clock_Tests)
