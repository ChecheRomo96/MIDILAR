add_custom_target(MIDILARExamples)

function(midilar_add_example TARGET_NAME)
    if(ARGC LESS 2)
        message(FATAL_ERROR
            "midilar_add_example(${TARGET_NAME}) requires at least one source file")
    endif()

    add_executable(${TARGET_NAME} ${ARGN})
    target_link_libraries(${TARGET_NAME} PRIVATE MIDILAR::MIDILAR)
    set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "_d")
    add_dependencies(MIDILARExamples ${TARGET_NAME})

    install(TARGETS ${TARGET_NAME}
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}/examples"
        CONFIGURATIONS Release
        COMPONENT Examples)
endfunction()

set(MIDILAR_DESKTOP_EXAMPLES_SUPPORTED OFF)
if(APPLE OR WIN32 OR CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(MIDILAR_DESKTOP_EXAMPLES_SUPPORTED ON)
endif()

add_subdirectory(MIDILAR)

set(MIDILAR_EXAMPLES_LAUNCHER_DIRECTORY
    "${CMAKE_CURRENT_BINARY_DIR}/launchers")

if(APPLE)
    set(MIDILAR_EXAMPLES_LAUNCHER
        "${MIDILAR_EXAMPLES_LAUNCHER_DIRECTORY}/MIDILARExamples.command")
    configure_file(
        "${MIDILAR_ROOT_DIRECTORY}/examples/Launchers/macOS.command.in"
        "${MIDILAR_EXAMPLES_LAUNCHER}"
        @ONLY NEWLINE_STYLE UNIX)
    install(PROGRAMS "${MIDILAR_EXAMPLES_LAUNCHER}"
        DESTINATION "${CMAKE_INSTALL_BINDIR}/examples"
        CONFIGURATIONS Release COMPONENT Examples)
elseif(WIN32)
    set(MIDILAR_EXAMPLES_LAUNCHER
        "${MIDILAR_EXAMPLES_LAUNCHER_DIRECTORY}/MIDILARExamples.cmd")
    configure_file(
        "${MIDILAR_ROOT_DIRECTORY}/examples/Launchers/Windows.cmd.in"
        "${MIDILAR_EXAMPLES_LAUNCHER}"
        @ONLY NEWLINE_STYLE WIN32)
    install(PROGRAMS "${MIDILAR_EXAMPLES_LAUNCHER}"
        DESTINATION "${CMAKE_INSTALL_BINDIR}/examples"
        CONFIGURATIONS Release COMPONENT Examples)
endif()
