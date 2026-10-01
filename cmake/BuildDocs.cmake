find_package(Doxygen REQUIRED)

if(DOXYGEN_FOUND)

    set(DOXYGEN_IN  ${MIDILAR_ROOT_DIRECTORY}/docs/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/docs/Doxyfile)
    set(DOXYGEN_HTML_FOOTER
        ${MIDILAR_ROOT_DIRECTORY}/docs/assets/MIDILARFooter.html)
    set(DOXYGEN_HTML_EXTRA_FILES
        ${MIDILAR_ROOT_DIRECTORY}/docs/assets/MIDILARDocs.js)

    add_subdirectory(${MIDILAR_ROOT_DIRECTORY}/docs)

    get_target_property(MIDILAR_DOXYGEN_PREDEFS MIDILAR MIDILAR_DOXYGEN_PREDEFS)
    get_target_property(MIDILAR_DOXYGEN_INPUTS MIDILAR MIDILAR_DOXYGEN_INPUTS)

    if(NOT MIDILAR_DOXYGEN_PREDEFS)
        set(MIDILAR_DOXYGEN_PREDEFS "")
    endif()

    if(NOT MIDILAR_DOXYGEN_INPUTS)
        set(MIDILAR_DOXYGEN_INPUTS "")
    endif()

    # Doxygen does not run a C++ compiler, so it cannot infer the language
    # feature-test value selected by the MIDILAR target. Keep C++17 declarations
    # and documentation-only preprocessor paths visible.
    list(APPEND MIDILAR_DOXYGEN_PREDEFS
        DOXYGEN=1
        MIDILAR_CPLUSPLUS=201703L
    )

    string(REPLACE ";" " " DOXYGEN_PREDEFINED "${MIDILAR_DOXYGEN_PREDEFS}")
    string(REPLACE ";" " " DOXYGEN_INPUT "${MIDILAR_DOXYGEN_INPUTS}")

    message(STATUS "Doxygen Predefined:")
    foreach(item IN LISTS MIDILAR_DOXYGEN_PREDEFS)
        message(STATUS "  ${item}")
    endforeach()

    message(STATUS "Doxygen Inputs:")
    foreach(item IN LISTS MIDILAR_DOXYGEN_INPUTS)
        message(STATUS "  ${item}")
    endforeach()

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    message(STATUS "Doxygen configuration file created at ${DOXYGEN_OUT}")

    add_custom_target(MIDILARDocs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${MIDILAR_ROOT_DIRECTORY}
        COMMENT "Generating MIDILAR API documentation with Doxygen"
        VERBATIM
    )

    add_custom_target(docs DEPENDS MIDILARDocs)

else()
    message(WARNING "Doxygen is required to build the documentation.")
endif()
