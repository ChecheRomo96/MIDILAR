######################################################################################################
# Append Tests

    if(MIDILAR_TESTING)
        enable_testing()
        cmake_policy(SET CMP0135 NEW)

        # Force GoogleTest to use the same MSVC runtime as the rest of the project
        if(MSVC)
            set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        endif()

        # Disable GoogleTest installation
        set(INSTALL_GTEST OFF CACHE BOOL "Disable installation of GoogleTest" FORCE)
        set(INSTALL_GMOCK OFF CACHE BOOL "Disable installation of GoogleMock" FORCE)

        # Include GoogleTest via FetchContent
        include(FetchContent)

        FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip
        )

        FetchContent_MakeAvailable(googletest)

        # Prevent GoogleTest targets from appearing in ALL_BUILD
        set_target_properties(gtest gtest_main gmock gmock_main PROPERTIES
            EXCLUDE_FROM_ALL TRUE
        )
        
        # Include testing helpers
        include(CTest)
        include(GoogleTest)

        # Add the tests subdirectory
        add_subdirectory(${MIDILAR_ROOT_DIRECTORY}/tests)
    endif()
#
######################################################################################################