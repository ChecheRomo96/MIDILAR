cmake_minimum_required(VERSION 3.25)

project(MIDILAR VERSION 0.1.0 LANGUAGES C CXX)

set(MIDILAR_REQUIRED_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(MIDILAR_ROOT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")
set(MIDILAR_PLATFORM "desktop" CACHE STRING "MIDILAR target platform identifier")
set(MIDILAR_EXPECTED_SYSTEM_PROCESSOR "" CACHE STRING
    "Regular expression for the expected target processor")

file(STRINGS "${MIDILAR_ROOT_DIRECTORY}/library.properties"
    MIDILAR_ARDUINO_VERSION_LINE REGEX "^version=" LIMIT_COUNT 1)
string(REGEX REPLACE "^version=" "" MIDILAR_ARDUINO_VERSION
    "${MIDILAR_ARDUINO_VERSION_LINE}")
if(NOT MIDILAR_ARDUINO_VERSION STREQUAL PROJECT_VERSION)
    message(FATAL_ERROR
        "Version mismatch: CMake declares ${PROJECT_VERSION}, but "
        "library.properties declares ${MIDILAR_ARDUINO_VERSION}")
endif()

file(STRINGS "${MIDILAR_ROOT_DIRECTORY}/src/MIDILAR_BuildSettings.h"
    MIDILAR_SOURCE_VERSION_LINE
    REGEX "^[ \t]*#define[ \t]+MIDILAR_VERSION[ \t]+\"[^\"]+\""
    LIMIT_COUNT 1)
if(NOT MIDILAR_SOURCE_VERSION_LINE MATCHES "\"([^\"]+)\"")
    message(FATAL_ERROR "MIDILAR_BuildSettings.h does not declare MIDILAR_VERSION")
endif()
if(NOT CMAKE_MATCH_1 STREQUAL PROJECT_VERSION)
    message(FATAL_ERROR
        "Version mismatch: CMake declares ${PROJECT_VERSION}, but "
        "MIDILAR_BuildSettings.h declares ${CMAKE_MATCH_1}")
endif()

if(MIDILAR_EXPECTED_SYSTEM_PROCESSOR AND
   NOT CMAKE_SYSTEM_PROCESSOR MATCHES "${MIDILAR_EXPECTED_SYSTEM_PROCESSOR}")
    message(FATAL_ERROR
        "Preset '${MIDILAR_PLATFORM}' expects processor "
        "'${MIDILAR_EXPECTED_SYSTEM_PROCESSOR}', but CMake detected "
        "'${CMAKE_SYSTEM_PROCESSOR}'")
endif()

include(cmake/MIDILARFoundation.cmake)
include(cmake/MIDILARMCC.cmake)

message(STATUS "Building MIDILAR Version = ${PROJECT_VERSION}")
message(STATUS "Required C++ standard = C++${MIDILAR_REQUIRED_CXX_STANDARD}")
message(STATUS "System      = ${CMAKE_SYSTEM_NAME}")
message(STATUS "Processor   = ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CXX ID      = ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Foundation  = ${Foundation_VERSION} (${Foundation_PLATFORM}) from ${MIDILAR_FOUNDATION_SOURCE}")
message(STATUS "MCC         = ${MCC_VERSION} (${MCC_PLATFORM}) from ${MIDILAR_MCC_SOURCE}")

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(cmake/MIDILARHelpers.cmake)
include(CMakeOptions.cmake)

if(MIDILAR_COVERAGE)
    message(STATUS "Enabling code coverage")
    set(MIDILAR_COVERAGE_FLAGS "--coverage -O0 -g")
    string(APPEND CMAKE_C_FLAGS_DEBUG " ${MIDILAR_COVERAGE_FLAGS}")
    string(APPEND CMAKE_CXX_FLAGS_DEBUG " ${MIDILAR_COVERAGE_FLAGS}")
    string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " ${MIDILAR_COVERAGE_FLAGS}")
endif()

add_subdirectory(src)
midilar_add_macro(PUBLIC MIDILAR_VERSION="${PROJECT_VERSION}")

if(MIDILAR_EXAMPLES)
    add_subdirectory(examples)
endif()

add_custom_target(MIDILARExportArtifacts)
add_dependencies(MIDILARExportArtifacts MIDILAR)
if(TARGET MIDILARExamples)
    add_dependencies(MIDILARExportArtifacts MIDILARExamples)
endif()

if(MIDILAR_DOCS)
    midilar_add_dox("${MIDILAR_ROOT_DIRECTORY}/README.md")
    include(cmake/BuildDocs.cmake)
endif()

if(MIDILAR_TESTING)
    include(cmake/BuildTests.cmake)
    add_subdirectory(tests)
    midilar_enable_warnings(MIDILAR)
endif()

configure_package_config_file(
    "${MIDILAR_ROOT_DIRECTORY}/cmake/MIDILARConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/MIDILARConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/MIDILAR")

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/MIDILARConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

configure_file(
    "${MIDILAR_ROOT_DIRECTORY}/cmake/MIDILARBuildInfo.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/MIDILARBuildInfo.cmake"
    @ONLY)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/MIDILARConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/MIDILARConfigVersion.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/MIDILARBuildInfo.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/MIDILAR"
    CONFIGURATIONS Release)
