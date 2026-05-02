set(MIDILAR_SYSTEM_CALLBACK_HANDLER_TEST_SOURCES
    CallbackHandler.cc
    CallbackBindingTests.cc
    FrequencyTests.cc
    TimeRefreshTests.cc
    EdgeCaseTests.cc
)

midilar_add_test(test_MIDILAR_CallbackHandler
    ${MIDILAR_SYSTEM_CALLBACK_HANDLER_TEST_SOURCES}
)