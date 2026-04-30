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
        string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER) # To be deleted
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
# Store this directory as a variable called MIDILAR_ROOT_DIRECTORY to reference in deeper directories

    set(MIDILAR_ROOT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
#
#####################################################################################################
# Include helper functions for Doxygen and macro management

    include(cmake/MidilarHelpers.cmake)
#
#####################################################################################################
# Compile Configuration
    
    if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/CMakeOptions.cmake")
        include(${CMAKE_CURRENT_LIST_DIR}/CMakeOptions.cmake)
    else()
        message(WARNING "CMakeOptions.cmake not found! Using default configuration.")
    endif()
#
######################################################################################################
# Append Source

    # Apply coverage flags only if MIDILAR_COVERAGE is ON
    if(MIDILAR_COVERAGE)
        message(STATUS "Enabling Code Coverage")

        set(COVERAGE_FLAGS "--coverage -O0 -g")

        string(APPEND CMAKE_C_FLAGS_DEBUG " ${COVERAGE_FLAGS}")
        string(APPEND CMAKE_CXX_FLAGS_DEBUG " ${COVERAGE_FLAGS}")
        string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " ${COVERAGE_FLAGS}")
    endif()
    
    # Add the src directory to the build
    add_subdirectory(src)
#
######################################################################################################
# Create the library

    midilar_add_macro(PUBLIC MIDILAR_VERSION="${PROJECT_VERSION}")

#
######################################################################################################
# Append Examples

    if(MIDILAR_EXAMPLES)
        message(STATUS "Building MIDILAR Examples")
        add_subdirectory(examples)
    endif()
#
######################################################################################################
# Enable Doxygen to build the documentation

    if(MIDILAR_DOCS)
        message(STATUS "Doxygen documentation generation enabled")
        include(cmake/BuildDocs.cmake)
    endif()
#
######################################################################################################
# Enable Doxygen to build the documentation

    if(MIDILAR_TESTING)
        message(STATUS "Building MIDILAR Tests")
        include(cmake/BuildTests.cmake)
        add_subdirectory(tests)
    endif()
#
######################################################################################################
