# Check if Doxygen is installed
find_package(Doxygen REQUIRED)

if(DOXYGEN_FOUND)
    # Set input and output files
    set(DOXYGEN_IN ${MIDILAR_ROOT_DIRECTORY}/docs/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/docs/Doxyfile)
    
    add_subdirectory(${MIDILAR_ROOT_DIRECTORY}/docs)
    
    get_target_property(MIDILAR_DOXYGEN_PREDEFS MIDILAR MIDILAR_DOXYGEN_PREDEFS)
    get_target_property(MIDILAR_DOXYGEN_INPUTS MIDILAR MIDILAR_DOXYGEN_INPUTS)

    string(JOIN " " DOXYGEN_PREDEFINED ${MIDILAR_DOXYGEN_PREDEFS})
    string(JOIN " " DOXYGEN_INPUT ${MIDILAR_DOXYGEN_INPUTS})

    message(STATUS "Doxygen Predefined:")
    foreach(item IN LISTS MIDILAR_DOXYGEN_PREDEFS)
        message(STATUS "  ${item}")
    endforeach()
    
    message(STATUS "Doxygen Inputs:")
    foreach(item IN LISTS MIDILAR_DOXYGEN_INPUTS)
        message(STATUS "  ${item}")
    endforeach()

    # Configure the Doxyfile
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message(STATUS "Doxygen configuration file created at ${DOXYGEN_OUT}")

    # Add a target for documentation generation (not part of ALL_BUILD)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
else()
    message(WARNING "Doxygen is required to build the documentation.")
endif()