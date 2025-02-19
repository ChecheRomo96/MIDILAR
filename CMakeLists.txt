#####################################################################################################
# Setting up the project

    #################################################################################################
    # CMake Minimum Version
        
    cmake_minimum_required(VERSION 3.14)
    #
    #################################################################################################
    # Project Name, Version, and Language

        project(MIDILAR VERSION 1.0.0 LANGUAGES CXX)
        message(STATUS "Building ${PROJECT_NAME} Version = ${PROJECT_VERSION}")
        string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
    #
    #################################################################################################
    # Set up C++ Version

        set(CMAKE_CXX_STANDARD 17) 
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
    #
    #################################################################################################
    # Set output directory to the build folder's bin directory

        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    #
    #################################################################################################
#
#####################################################################################################
# Store this directory as a variable called MCC_SRC_DIRECTORY to reference in deeper directories

    set(${PROJECT_NAME_UPPER}_ROOT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
#
#####################################################################################################
# Initialize the macro list for doxygen

    set(${PROJECT_NAME_UPPER}_MACROS "")
#
#####################################################################################################
# Compile Configuration
    
    if(EXISTS "${CMAKE_SOURCE_DIR}/CMakeOptions.cmake")
        include(CMakeOptions.cmake)
    else()
        message(WARNING "CMakeOptions.cmake not found! Using default configuration.")
    endif()
#
######################################################################################################
# Append Source

    # Add the src directory to the build
    add_subdirectory(src)
#
######################################################################################################
# Enable Doxygen to build the documentation

    option(${PROJECT_NAME_UPPER}_DOCS "Build documentation" OFF)

    if(${PROJECT_NAME_UPPER}_DOCS)
        # Check if Doxygen is installed
        find_package(Doxygen REQUIRED)

        if(DOXYGEN_FOUND)
            # Set input and output files
            set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile)
            set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

            # Join the macros list into a space-separated string
            string(REPLACE ";" " " DOXYGEN_PREDEFINED "${${PROJECT_NAME_UPPER}_MACROS}")

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
    endif()
#
######################################################################################################
# Create the library
    target_compile_definitions(MIDILAR INTERFACE ${PROJECT_NAME_UPPER}_VERSION="${PROJECT_VERSION}")
#
######################################################################################################
# Append Examples

    option(MIDILAR_EXAMPLES "Enable building examples" OFF)

    if(MIDILAR_EXAMPLES)
        message(STATUS "Building ${PROJECT_NAME} Examples")
        add_subdirectory(examples EXCLUDE_FROM_ALL)
    endif()
#
######################################################################################################
# Append Tests

    option(MIDILAR_TESTING "Enable unit testing" OFF)
    option(MIDILAR_COVERAGE "Enable coverage testing" OFF)

    if(MIDILAR_TESTING)
        message(STATUS "Building ${PROJECT_NAME} Tests")

        enable_testing()
        cmake_policy(SET CMP0135 NEW)
        
        # Apply coverage flags only if MIDILAR_COVERAGE is ON
        if(MIDILAR_COVERAGE)
            message(STATUS "Enabling Code Coverage")

            set(COVERAGE_FLAGS "-fprofile-arcs -ftest-coverage -fprofile-abs-path --coverage")

            # Ensure flags only apply in Debug mode
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${COVERAGE_FLAGS}")
            set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${COVERAGE_FLAGS}")
            set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${COVERAGE_FLAGS}")
        endif()

        # Disable GoogleTest installation
        set(INSTALL_GTEST OFF CACHE BOOL "Disable installation of GoogleTest")
        set(INSTALL_GMOCK OFF CACHE BOOL "Disable installation of GoogleMock")

        # Include GoogleTest via FetchContent
        include(FetchContent)
        FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip
        )
        FetchContent_MakeAvailable(googletest)

        # Disable GoogleTest from being listed in CTest
        set_target_properties(gtest gtest_main PROPERTIES EXCLUDE_FROM_ALL TRUE)

        # Force GoogleTest to use the same compiler settings
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

        # Include necessary testing frameworks
        include(GoogleTest)
        include(CTest)
        
        # Add the tests subdirectory
        add_subdirectory(tests)

    endif()
#
######################################################################################################