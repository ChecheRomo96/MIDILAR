#####################################################################################################
# Setting up the project

    #################################################################################################
    # C Make Minimum Version
        
        cmake_minimum_required(VERSION 3.14)
    #
    #################################################################################################
    # Project Name Version and Languaje, print and add preprocessor flag
        
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

    set( ${PROJECT_NAME_UPPER}_ROOT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} )
#
#####################################################################################################
# Initialize the macro list for doxygen

    set( ${PROJECT_NAME_UPPER}_MACROS "")
#
#####################################################################################################
# Compile Configuration
    
    #set(MIDILAR_BUILD_TARGET "Submodule" CACHE STRING "MIDILAR build target")
    #set_property(CACHE MIDILAR_BUILD_TARGET PROPERTY STRINGS "Dev" "Submodule" "Export" "Install")

    option(${PROJECT_NAME_UPPER}_EXAMPLES "Enable building examples" OFF)
    option(${PROJECT_NAME_UPPER}_TESTING "Enable unit testing" OFF)
    option(MIDILAR_DOCS "Generate API documentation using Doxygen" OFF)

    #################################################################################################
    # SystemFoundation

        option(MIDILAR_SYSTEM_FOUNDATION "Enables the compilation of MIDILAR::SystemFoundation" ON)

        #option(MIDILAR_TOOLS_EXAMPLES "Enables Building Examples" FALSE)
        #option(MIDILAR_TOOLS_TESTING "Enabled Unit Testing" FALSE)

        ##############################################################################################
        # Clock

            option(MIDILAR_CALLBACK_HANDLER "Enables the compilation of MIDILAR::SystemFoundation::CallbackHandler" ON)
        #
        ##############################################################################################
        # Clock

            option(MIDILAR_SYSTEM_CLOCK "Enables the compilation of MIDILAR::SystemFoundation::Clock" ON)
        #
        ##############################################################################################
    #
    #################################################################################################
    # AudioFoundation

        option(MIDILAR_DSP_FOUNDATION "Enables the compilation of MIDILAR::AudioFoundation" ON)

        #option(MIDILAR_TOOLS_EXAMPLES "Enables Building Examples" FALSE)
        #option(MIDILAR_TOOLS_TESTING "Enabled Unit Testing" FALSE)

        #############################################################################################
        # LUT

            option(MIDILAR_LUT "Enables the compilation of MIDILAR::AudioFoundation::LUT::BaseLUT" ON)
        #
        ##############################################################################################
    #
    ##################################################################################################
    # MIDI Foundation

        option(MIDILAR_MIDI_FOUNDATION "Enables the compilation of MIDILAR::MidiFoundation" ON)

        #############################################################################################
        # ProtocolEnums

            option(MIDILAR_PROTOCOL_ENUMS "Enables the compilation of MIDILAR::MidiFoundation::Protocol Namespace" ON)
        #
        ##############################################################################################
        # Midi Message

            option(MIDILAR_MIDI_MESSAGE "Enables the compilation of MIDILAR::MidiFoundation::Message" ON)
        #
        ##############################################################################################
        # Message Parser

            option(MIDILAR_MIDI_MESSAGE_PARSER "Enables the compilation of MIDILAR::MidiFoundation::MessageParser" ON)
        #
        ##############################################################################################
        # MidiProcessor

            option(MIDILAR_MIDI_PROCESSOR "Enables the compilation of MIDILAR::MidiFoundation::Processor" ON)
        #
        ##############################################################################################
    #
    ##################################################################################################
    # MIDI Processors

        option(MIDILAR_MIDI_PROCESSORS "Enables the compilation of MIDILAR::MidiProcessors" ON)

        #option(MIDILAR_TOOLS_EXAMPLES "Enables Building Examples" FALSE)
        #option(MIDILAR_TOOLS_TESTING "Enabled Unit Testing" FALSE)

        #############################################################################################
        # 

        #
        ##############################################################################################
    #
    ##################################################################################################


#
######################################################################################################
# Append Source
    
    #Coverage Flags
    if(${MIDILAR_TESTING})
        enable_testing()
        set(COVERAGE_FLAGS "--coverage")
        
        # Apply the coverage flags to specific targets or directories
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_FLAGS}")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COVERAGE_FLAGS}")
    endif()

    # Add the src directory to the build
    add_subdirectory(src)

    # Disable coverage flags for other parts of the project
    if(${MIDILAR_TESTING})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE STRING "" FORCE)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "" FORCE)
    endif()
#
######################################################################################################
# Enable Doxygen to build the documentation

    option(${PROJECT_NAME_UPPER}_DOCS "Build documentation" OFF)

    if(${PROJECT_NAME_UPPER}_DOCS)
        # Check if Doxygen is installed
        find_package(Doxygen REQUIRED)

        if (DOXYGEN_FOUND)
            # Set input and output files
            set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile)
            set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

            # Join the macros list into a space-separated string
            string(REPLACE ";" " " DOXYGEN_PREDEFINED "${${PROJECT_NAME_UPPER}_MACROS}")

            # Use DOXYGEN_PREDEFINED for the Doxygen configuration
            set(DOXYGEN_PREDEFINED "${DOXYGEN_PREDEFINED}")

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
    target_compile_definitions(MIDILAR INTERFACE ${PROJECT_NAME_UPPER}_VERSION="${PROJECT_VERSION}" )
#
######################################################################################################
# Append Examples

    if( ${MIDILAR_EXAMPLES} )
        message(STATUS "Building ${CMAKE_PROJECT_NAME} Examples")
        add_subdirectory(examples)
    endif()
#
######################################################################################################
# Append Tests

if(MIDILAR_TESTING)
    message(STATUS "Building ${CMAKE_PROJECT_NAME} Tests")


    cmake_policy(SET CMP0135 NEW)

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

    # Force GoogleTest to use the same compiler settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Include necessary testing frameworks
    include(GoogleTest)
    include(CTest)

    # Load CTest custom exclusions if applicable
    if(EXISTS "${CMAKE_SOURCE_DIR}/CTestCustom.cmake")
        include("${CMAKE_SOURCE_DIR}/CTestCustom.cmake")
    endif()

    # Add the tests subdirectory
    add_subdirectory(tests)

    ctest_coverage( APPEND Coverage.xml)

    endif()
#
######################################################################################################
