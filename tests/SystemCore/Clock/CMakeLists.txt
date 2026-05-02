set(MIDILAR_SYSTEM_CLOCK_TEST_SOURCES
    Clock_ConstructionTests.cc
    Clock_CallbackBindingTests.cc
    Clock_FrequencyTests.cc
    Clock_TimeRefreshTests.cc
    Clock_EdgeCaseTests.cc
)

midilar_add_test(MIDILAR_System_Clock_Tests
    ${MIDILAR_SYSTEM_CLOCK_TEST_SOURCES}
)