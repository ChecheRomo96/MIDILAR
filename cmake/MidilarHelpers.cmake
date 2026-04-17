function(midilar_add_macro visibility)
    target_compile_definitions(MIDILAR ${visibility} ${ARGN})
    set_property(TARGET MIDILAR APPEND PROPERTY MIDILAR_DOXYGEN_PREDEFS ${ARGN})
endfunction()

function(midilar_add_dox)
    set_property(TARGET MIDILAR APPEND PROPERTY MIDILAR_DOXYGEN_INPUTS ${ARGN})
endfunction()

function(midilar_add_test test_target)
    add_executable(${test_target}
        ${ARGN}
    )

    target_link_libraries(${test_target}
        PRIVATE
            MIDILAR
            gtest
            gtest_main
    )

    target_include_directories(${test_target}
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}
    )

    gtest_discover_tests(${test_target})

    set_property(TARGET MIDILAR APPEND PROPERTY MIDILAR_TEST_TARGETS ${test_target})
endfunction()