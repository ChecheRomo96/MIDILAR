# Resolves the DspCore dependency and defines DspCore::DspCore.
# A parent target, an explicit/sibling package, the regular package search, or
# the pinned sources at v0.1.0 may satisfy the dependency.
set(MIDILAR_DSPCORE_VERSION "0.1.0")
set(MIDILAR_DSPCORE_REPOSITORY "ChecheRomo96/DspCore" CACHE STRING
    "GitHub repository (owner/name) used to fetch DspCore")
option(MIDILAR_FETCH_DSPCORE "Fetch DspCore when no package is found" ON)

if(NOT TARGET DspCore::DspCore AND NOT DEFINED MIDILAR_DSPCORE_PREFIX AND
   NOT "$ENV{MIDILAR_DSPCORE_PREFIX}" STREQUAL "")
    set(MIDILAR_DSPCORE_PREFIX "$ENV{MIDILAR_DSPCORE_PREFIX}")
endif()
set(MIDILAR_DSPCORE_PREFIX "${MIDILAR_DSPCORE_PREFIX}" CACHE PATH
    "DspCore package prefix for the same platform and ABI")

if(NOT TARGET DspCore::DspCore AND NOT MIDILAR_DSPCORE_PREFIX STREQUAL "")
    find_package(DspCore ${MIDILAR_DSPCORE_VERSION} CONFIG REQUIRED
        PATHS "${MIDILAR_DSPCORE_PREFIX}" NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
    set(MIDILAR_DSPCORE_SOURCE "prefix ${MIDILAR_DSPCORE_PREFIX}")
elseif(NOT TARGET DspCore::DspCore)
    find_package(DspCore ${MIDILAR_DSPCORE_VERSION} CONFIG QUIET)
    if(TARGET DspCore::DspCore)
        set(MIDILAR_DSPCORE_SOURCE "${DspCore_DIR}")
    endif()
endif()

if(NOT TARGET DspCore::DspCore AND MIDILAR_FETCH_DSPCORE)
    include(FetchContent)
    set(DSPCORE_PLATFORM "${MIDILAR_PLATFORM}" CACHE STRING
        "DspCore target platform identifier" FORCE)
    FetchContent_Declare(DspCore
        GIT_REPOSITORY "https://github.com/${MIDILAR_DSPCORE_REPOSITORY}.git"
        GIT_TAG "v${MIDILAR_DSPCORE_VERSION}"
        GIT_SHALLOW TRUE)
    FetchContent_MakeAvailable(DspCore)
    set(MIDILAR_DSPCORE_SOURCE "sources at v${MIDILAR_DSPCORE_VERSION}")
endif()

if(NOT TARGET DspCore::DspCore)
    message(FATAL_ERROR
        "MIDILAR requires DspCore ${MIDILAR_DSPCORE_VERSION}+ (0.x). Set "
        "MIDILAR_DSPCORE_PREFIX or enable MIDILAR_FETCH_DSPCORE.")
endif()
