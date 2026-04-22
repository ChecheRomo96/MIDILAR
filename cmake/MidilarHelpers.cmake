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

function(midilar_stage_headers)
    foreach(HEADER ${ARGV})
        file(RELATIVE_PATH REL_HEADER "${MIDILAR_SRC_DIRECTORY}" "${HEADER}")
        get_filename_component(REL_DIR "${REL_HEADER}" DIRECTORY)
        file(MAKE_DIRECTORY "${MIDILAR_BUILD_INCLUDE_DIR}/${REL_DIR}")

        configure_file(
            "${HEADER}"
            "${MIDILAR_BUILD_INCLUDE_DIR}/${REL_HEADER}"
            COPYONLY
        )
    endforeach()
endfunction()