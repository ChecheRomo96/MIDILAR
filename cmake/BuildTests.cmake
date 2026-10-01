######################################################################################################
# Configure Native Unit Tests

    if(MIDILAR_TESTING)
        include(CTest)
        enable_testing()

        # Keep the MSVC runtime selected by the parent project and prevent the
        # test framework from becoming part of a MIDILAR installation.
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
        set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)

        find_package(GTest QUIET)

        if(NOT TARGET GTest::gtest_main)
            set(MIDILAR_GTEST_SOURCE_DIRECTORY
                "${CMAKE_BINARY_DIR}/_deps/googletest-src")
            set(MIDILAR_GTEST_BINARY_DIRECTORY
                "${CMAKE_BINARY_DIR}/_deps/googletest-build")

            if(EXISTS "${MIDILAR_GTEST_SOURCE_DIRECTORY}/CMakeLists.txt")
                add_subdirectory(
                    "${MIDILAR_GTEST_SOURCE_DIRECTORY}"
                    "${MIDILAR_GTEST_BINARY_DIRECTORY}"
                    EXCLUDE_FROM_ALL)
            else()
                include(FetchContent)
                set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

                FetchContent_Declare(
                    googletest
                    URL
                        "https://github.com/google/googletest/archive/063de7e9578f82b369302001269680b4b1553359.zip"
                    URL_HASH
                        "SHA256=f933817755e14daf4afa36230b994c5e9ab5a0476dbd659b1d70163e876b4b91"
                    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
                )
                FetchContent_MakeAvailable(googletest)
            endif()
        endif()

        include(GoogleTest)
    endif()
#
######################################################################################################
