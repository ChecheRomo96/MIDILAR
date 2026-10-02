# Resolves the MCC dependency and defines MCC::MCC.
#
# Resolution order:
#   0. A MCC::MCC target already defined by a parent project.
#   1. MIDILAR_MCC_PREFIX (cache or environment): an explicit package
#      prefix. Failing to use it is a fatal error.
#   2. ../MCC/dist/<MIDILAR_PLATFORM>: a sibling export. Skipped with a
#      warning when it is incompatible.
#   3. Normal find_package() search: MCC_DIR, CMAKE_PREFIX_PATH and
#      system locations.
#   4. MIDILAR_FETCH_MCC (default ON):
#      a. The GitHub Release package for tag v<MIDILAR_MCC_VERSION> and
#         this platform, verified against its published SHA-256.
#      b. The MCC sources at that tag, built as part of MIDILAR (used for
#         platforms without a Release package, such as AVR and Arm, and when
#         FETCHCONTENT_SOURCE_DIR_MCC points at a local working copy).
#
# Downloads live in ${CMAKE_BINARY_DIR}/_deps and are reused across
# reconfigurations.
#
# Outputs: MCC_VERSION, MCC_PLATFORM, MIDILAR_MCC_SOURCE
# and the internal cache entry MIDILAR_MCC_RESOLVED_PREFIX (empty when
# MCC is built from source and installed alongside MIDILAR).

set(MIDILAR_MCC_VERSION "0.5.2")

# MCC packages declare find_dependency(Foundation). Let that nested search see
# the Foundation package MIDILAR already resolved.
if(MIDILAR_FOUNDATION_RESOLVED_PREFIX)
    list(APPEND CMAKE_PREFIX_PATH "${MIDILAR_FOUNDATION_RESOLVED_PREFIX}")
endif()
set(MIDILAR_MCC_REPOSITORY "ChecheRomo96/MusicCompositionCore" CACHE STRING
    "GitHub repository (owner/name) used to fetch MCC")
option(MIDILAR_FETCH_MCC
    "Fetch MCC from GitHub when no local package is found" ON)

set(MIDILAR_MCC_SOURCE "")
# MCC_DIR as given by the user, before candidate searches reset it.
set(MIDILAR_USER_MCC_DIR "${MCC_DIR}")

# Restores MCC_DIR to the user's value so candidate searches never
# leave their own result in the cache.
macro(midilar_restore_mcc_dir)
    if(MIDILAR_USER_MCC_DIR AND
       NOT MIDILAR_USER_MCC_DIR MATCHES "-NOTFOUND$")
        set(MCC_DIR "${MIDILAR_USER_MCC_DIR}" CACHE PATH
            "Directory containing MCCConfig.cmake" FORCE)
    else()
        unset(MCC_DIR CACHE)
    endif()
endmacro()

function(midilar_find_mcc_package prefix)
    # A cached MCC_DIR takes precedence over PATHS, so search each
    # candidate prefix from scratch.
    unset(MCC_DIR CACHE)
    find_package(MCC ${MIDILAR_MCC_VERSION} CONFIG QUIET
        PATHS "${prefix}"
        NO_DEFAULT_PATH
        NO_CMAKE_FIND_ROOT_PATH)
    midilar_restore_mcc_dir()
    if(TARGET MCC::MCC)
        set(MCC_VERSION "${MCC_VERSION}" PARENT_SCOPE)
        set(MCC_PLATFORM "${MCC_PLATFORM}" PARENT_SCOPE)
        set(MCC_CXX_STANDARD "${MCC_CXX_STANDARD}" PARENT_SCOPE)
    endif()
endfunction()

# Downloads and extracts the Release package; sets MIDILAR_MCC_PACKAGE_PREFIX
# in the caller on success.
function(midilar_download_mcc_package)
    set(MIDILAR_MCC_PACKAGE_PREFIX "" PARENT_SCOPE)

    set(name "MCC-${MIDILAR_MCC_VERSION}-${MIDILAR_PLATFORM}")
    if(MIDILAR_PLATFORM MATCHES "^windows_")
        set(archive "${name}.zip")
    else()
        set(archive "${name}.tar.gz")
    endif()

    set(root "${CMAKE_BINARY_DIR}/_deps/mcc-package")
    set(prefix "${root}/${name}")
    if(EXISTS "${prefix}/lib/cmake/MCC/MCCConfig.cmake")
        set(MIDILAR_MCC_PACKAGE_PREFIX "${prefix}" PARENT_SCOPE)
        return()
    endif()

    set(url "https://github.com/${MIDILAR_MCC_REPOSITORY}/releases/download/v${MIDILAR_MCC_VERSION}")
    file(MAKE_DIRECTORY "${root}")

    file(DOWNLOAD "${url}/${archive}.sha256" "${root}/${archive}.sha256"
        STATUS status)
    list(GET status 0 code)
    if(NOT code EQUAL 0)
        file(REMOVE "${root}/${archive}.sha256")
        message(STATUS
            "MCC Release ${MIDILAR_MCC_VERSION} has no package for "
            "'${MIDILAR_PLATFORM}'")
        return()
    endif()

    file(STRINGS "${root}/${archive}.sha256" checksum_line LIMIT_COUNT 1)
    if(NOT checksum_line MATCHES "^([0-9a-fA-F]+)")
        message(FATAL_ERROR "Malformed checksum file for ${archive}")
    endif()
    set(checksum "${CMAKE_MATCH_1}")

    message(STATUS "Downloading ${archive}")
    # Do not pass EXPECTED_HASH to file(DOWNLOAD): CMake records a configure
    # error before STATUS can be inspected when the asset is unavailable.
    # Download first, fall back to sources on transport errors, and verify the
    # published digest explicitly before extracting anything.
    file(DOWNLOAD "${url}/${archive}" "${root}/${archive}"
        STATUS status)
    list(GET status 0 code)
    if(NOT code EQUAL 0)
        list(GET status 1 reason)
        file(REMOVE "${root}/${archive}" "${root}/${archive}.sha256")
        message(STATUS
            "MCC Release package ${archive} is unavailable (${reason}); "
            "falling back to sources")
        return()
    endif()

    file(SHA256 "${root}/${archive}" actual_checksum)
    string(TOLOWER "${checksum}" checksum)
    string(TOLOWER "${actual_checksum}" actual_checksum)
    if(NOT actual_checksum STREQUAL checksum)
        file(REMOVE "${root}/${archive}" "${root}/${archive}.sha256")
        message(FATAL_ERROR
            "SHA-256 mismatch for ${archive}: expected ${checksum}, got "
            "${actual_checksum}")
    endif()

    file(ARCHIVE_EXTRACT INPUT "${root}/${archive}" DESTINATION "${root}")
    file(REMOVE "${root}/${archive}" "${root}/${archive}.sha256")

    if(NOT EXISTS "${prefix}/lib/cmake/MCC/MCCConfig.cmake")
        message(FATAL_ERROR "${archive} does not contain a MCC package")
    endif()
    set(MIDILAR_MCC_PACKAGE_PREFIX "${prefix}" PARENT_SCOPE)
endfunction()

# 0. Target provided by a parent project.
if(TARGET MCC::MCC)
    set(MIDILAR_MCC_SOURCE "parent project")
    set(MIDILAR_MCC_RESOLVED_PREFIX "")
endif()

# 1. Explicit prefix.
if(NOT TARGET MCC::MCC AND NOT DEFINED MIDILAR_MCC_PREFIX AND
   NOT "$ENV{MIDILAR_MCC_PREFIX}" STREQUAL "")
    set(MIDILAR_MCC_PREFIX "$ENV{MIDILAR_MCC_PREFIX}")
endif()
set(MIDILAR_MCC_PREFIX "${MIDILAR_MCC_PREFIX}" CACHE PATH
    "MCC package prefix for the same platform and ABI")

if(NOT TARGET MCC::MCC AND NOT MIDILAR_MCC_PREFIX STREQUAL "")
    midilar_find_mcc_package("${MIDILAR_MCC_PREFIX}")
    if(NOT TARGET MCC::MCC)
        message(FATAL_ERROR
            "MIDILAR_MCC_PREFIX='${MIDILAR_MCC_PREFIX}' does not contain "
            "a MCC ${MIDILAR_MCC_VERSION}+ (0.x) package.")
    endif()
    set(MIDILAR_MCC_SOURCE "prefix")
    set(MIDILAR_MCC_RESOLVED_PREFIX "${MIDILAR_MCC_PREFIX}")
endif()

# 2. Sibling export.
if(NOT TARGET MCC::MCC)
    set(sibling_prefix "${MIDILAR_ROOT_DIRECTORY}/../MCC/dist/${MIDILAR_PLATFORM}")
    get_filename_component(sibling_prefix "${sibling_prefix}" ABSOLUTE)
    if(EXISTS "${sibling_prefix}/lib/cmake/MCC/MCCConfig.cmake")
        midilar_find_mcc_package("${sibling_prefix}")
        if(TARGET MCC::MCC)
            set(MIDILAR_MCC_SOURCE "sibling export")
            set(MIDILAR_MCC_RESOLVED_PREFIX "${sibling_prefix}")
        else()
            message(WARNING
                "Ignoring ${sibling_prefix}: it is not a compatible MCC "
                "${MIDILAR_MCC_VERSION}+ (0.x) package. Re-export it or "
                "let MIDILAR fetch MCC.")
        endif()
    endif()
endif()

# 3. Normal package search.
if(NOT TARGET MCC::MCC)
    midilar_restore_mcc_dir()
    find_package(MCC ${MIDILAR_MCC_VERSION} CONFIG QUIET)
    if(TARGET MCC::MCC)
        set(MIDILAR_MCC_SOURCE "${MCC_DIR}")
        get_filename_component(MIDILAR_MCC_RESOLVED_PREFIX
            "${MCC_DIR}/../../.." ABSOLUTE)
    else()
        midilar_restore_mcc_dir()
    endif()
endif()

# 4. Fetch from GitHub.
if(NOT TARGET MCC::MCC AND MIDILAR_FETCH_MCC)
    # A local source override means "build this working copy", so skip the
    # prebuilt Release package.
    if(FETCHCONTENT_SOURCE_DIR_MCC)
        set(MIDILAR_MCC_PACKAGE_PREFIX "")
    else()
        midilar_download_mcc_package()
    endif()
    if(NOT MIDILAR_MCC_PACKAGE_PREFIX STREQUAL "")
        midilar_find_mcc_package("${MIDILAR_MCC_PACKAGE_PREFIX}")
        if(NOT TARGET MCC::MCC)
            message(FATAL_ERROR
                "The downloaded MCC package at "
                "${MIDILAR_MCC_PACKAGE_PREFIX} could not be loaded.")
        endif()
        set(MIDILAR_MCC_SOURCE "GitHub Release v${MIDILAR_MCC_VERSION}")
        set(MIDILAR_MCC_RESOLVED_PREFIX "${MIDILAR_MCC_PACKAGE_PREFIX}")
    else()
        include(FetchContent)
        message(STATUS "Fetching MCC sources at v${MIDILAR_MCC_VERSION}")
        FetchContent_Declare(MCC
            GIT_REPOSITORY "https://github.com/${MIDILAR_MCC_REPOSITORY}.git"
            GIT_TAG "v${MIDILAR_MCC_VERSION}"
            GIT_SHALLOW TRUE)
        # The MCC subproject must describe the same platform as MIDILAR so it
        # accepts the Foundation target MIDILAR already resolved.
        set(MCC_PLATFORM "${MIDILAR_PLATFORM}" CACHE STRING
            "MCC target platform identifier" FORCE)
        FetchContent_MakeAvailable(MCC)
        if(NOT TARGET MCC::MCC)
            message(FATAL_ERROR
                "MCC v${MIDILAR_MCC_VERSION} sources do not define "
                "MCC::MCC.")
        endif()
        # A source build shares MIDILAR's toolchain, so only the version needs
        # checking; library.properties carries it for every MCC release.
        file(STRINGS "${mcc_SOURCE_DIR}/library.properties"
            mcc_version_line REGEX "^version=" LIMIT_COUNT 1)
        string(REGEX REPLACE "^version=" "" MCC_VERSION
            "${mcc_version_line}")
        string(REGEX MATCH "^[0-9]+" mcc_major "${MCC_VERSION}")
        string(REGEX MATCH "^[0-9]+" required_major "${MIDILAR_MCC_VERSION}")
        if(MCC_VERSION VERSION_LESS MIDILAR_MCC_VERSION OR
           NOT mcc_major STREQUAL required_major)
            message(FATAL_ERROR
                "MCC sources at ${mcc_SOURCE_DIR} are version "
                "'${MCC_VERSION}', but MIDILAR requires "
                "${MIDILAR_MCC_VERSION}+ (${required_major}.x).")
        endif()
        set(MCC_PLATFORM "${MIDILAR_PLATFORM}")
        if(FETCHCONTENT_SOURCE_DIR_MCC)
            set(MIDILAR_MCC_SOURCE "sources at ${FETCHCONTENT_SOURCE_DIR_MCC}")
        else()
            set(MIDILAR_MCC_SOURCE "sources at v${MIDILAR_MCC_VERSION}")
        endif()
        set(MIDILAR_MCC_RESOLVED_PREFIX "")
    endif()
endif()

if(NOT TARGET MCC::MCC)
    message(FATAL_ERROR
        "MIDILAR requires MCC ${MIDILAR_MCC_VERSION}+ (0.x) for the "
        "same platform and ABI. Set MIDILAR_MCC_PREFIX to a MCC "
        "package, export ../MCC/dist/${MIDILAR_PLATFORM}, or enable "
        "MIDILAR_FETCH_MCC.")
endif()

set(MIDILAR_MCC_RESOLVED_PREFIX "${MIDILAR_MCC_RESOLVED_PREFIX}" CACHE
    INTERNAL "MCC package prefix used by this build")

if(DEFINED MCC_CXX_STANDARD AND
   NOT MCC_CXX_STANDARD STREQUAL "" AND
   MCC_CXX_STANDARD GREATER MIDILAR_REQUIRED_CXX_STANDARD)
    message(FATAL_ERROR
        "MCC requires C++${MCC_CXX_STANDARD}, but MIDILAR is "
        "configured for C++${MIDILAR_REQUIRED_CXX_STANDARD}")
endif()

if(NOT MIDILAR_PLATFORM STREQUAL "documentation" AND
   DEFINED MCC_PLATFORM AND
   NOT MCC_PLATFORM STREQUAL "" AND
   NOT MCC_PLATFORM STREQUAL MIDILAR_PLATFORM)
    message(FATAL_ERROR
        "MIDILAR preset '${MIDILAR_PLATFORM}' cannot consume MCC package "
        "'${MCC_PLATFORM}'. Use a package for the same platform.")
endif()
