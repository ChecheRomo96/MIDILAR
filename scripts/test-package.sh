#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
. "$SCRIPT_DIR/common.sh"

usage() {
    printf '%s\n' "Usage: $0 <preset> [--foundation-prefix <path>] [--parallel <jobs>] [--fresh]"
}

PRESET=""
FOUNDATION_PREFIX=""
PARALLEL=""
FRESH=0

while [ "$#" -gt 0 ]; do
    case "$1" in
        --foundation-prefix)
            midilar_require_value "$1" "${2:-}"
            FOUNDATION_PREFIX=$2
            shift 2
            ;;
        --parallel)
            midilar_require_value "$1" "${2:-}"
            PARALLEL=$2
            shift 2
            ;;
        --fresh)
            FRESH=1
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        -*)
            midilar_die "unknown option: $1"
            ;;
        *)
            [ -z "$PRESET" ] || midilar_die "only one preset may be specified"
            PRESET=$1
            shift
            ;;
    esac
done

midilar_require_preset "$PRESET"
case "$PRESET" in
    macos_*|linux_*|windows_*)
        ;;
    *)
        midilar_die "package consumer tests require a runnable desktop preset"
        ;;
esac

# Without an explicit prefix, MIDILAR resolves Foundation itself: sibling export,
# GitHub Release package or sources (see cmake/MIDILARFoundation.cmake).
if [ -z "$FOUNDATION_PREFIX" ]; then
    FOUNDATION_PREFIX=${MIDILAR_FOUNDATION_PREFIX:-}
fi
if [ -n "$FOUNDATION_PREFIX" ]; then
    FOUNDATION_PREFIX=$(midilar_absolute_path "$FOUNDATION_PREFIX")
    FOUNDATION_CONFIG="$FOUNDATION_PREFIX/lib/cmake/Foundation/FoundationConfig.cmake"
    [ -f "$FOUNDATION_CONFIG" ] || \
        midilar_die "Foundation package not found at $FOUNDATION_PREFIX"
fi

set -- "$SCRIPT_DIR/export.sh" "$PRESET"
[ "$FRESH" -eq 0 ] || set -- "$@" --fresh
[ -z "$PARALLEL" ] || set -- "$@" --parallel "$PARALLEL"
if [ -n "$FOUNDATION_PREFIX" ]; then
    set -- "$@" -- -DMIDILAR_FOUNDATION_PREFIX="$FOUNDATION_PREFIX"
fi
"$@"

# The Foundation package MIDILAR used; empty when Foundation was built from
# sources and installed next to MIDILAR.
RESOLVED_FOUNDATION_PREFIX=$(sed -n \
    's/^MIDILAR_FOUNDATION_RESOLVED_PREFIX:INTERNAL=//p' \
    "$(midilar_build_dir "$PRESET")/CMakeCache.txt")
# Likewise for MCC.
RESOLVED_MCC_PREFIX=$(sed -n \
    's/^MIDILAR_MCC_RESOLVED_PREFIX:INTERNAL=//p' \
    "$(midilar_build_dir "$PRESET")/CMakeCache.txt")

MIDILAR_PREFIX="$MIDILAR_DIST_ROOT/$PRESET"
CONSUMER_SOURCE="$MIDILAR_ROOT/tests/PackageConsumer"
CONSUMER_BUILD="$MIDILAR_BUILD_ROOT/package-consumer/$PRESET"

MIDILAR_CACHE="$(midilar_build_dir "$PRESET")/CMakeCache.txt"

midilar_cache_value() {
    sed -n "s/^$1:[^=]*=//p" "$MIDILAR_CACHE" | sed -n '1p'
}

GENERATOR=$(midilar_cache_value CMAKE_GENERATOR)
GENERATOR_PLATFORM=$(midilar_cache_value CMAKE_GENERATOR_PLATFORM)
CXX_COMPILER=$(midilar_cache_value CMAKE_CXX_COMPILER)
TOOLCHAIN_FILE=$(midilar_cache_value CMAKE_TOOLCHAIN_FILE)
OSX_ARCHITECTURES=$(midilar_cache_value CMAKE_OSX_ARCHITECTURES)
CROSSCOMPILING=$(midilar_cache_value CMAKE_CROSSCOMPILING)

[ "$CROSSCOMPILING" != "TRUE" ] || \
    midilar_die "package execution requires a native preset"
[ -n "$GENERATOR" ] || midilar_die "configured preset has no CMake generator"

# The package consumer must use the same ABI and compiler family as the
# package. Recreate it so a previous run cannot retain another generator.
cmake -E remove_directory "$CONSUMER_BUILD"

set -- cmake \
    -S "$CONSUMER_SOURCE" \
    -B "$CONSUMER_BUILD" \
    -G "$GENERATOR" \
    -DMIDILAR_DIR="$MIDILAR_PREFIX/lib/cmake/MIDILAR" \
    -DCMAKE_PREFIX_PATH="$MIDILAR_PREFIX;$RESOLVED_MCC_PREFIX;$RESOLVED_FOUNDATION_PREFIX"

[ -z "$GENERATOR_PLATFORM" ] || set -- "$@" -A "$GENERATOR_PLATFORM"
[ -z "$OSX_ARCHITECTURES" ] || \
    set -- "$@" "-DCMAKE_OSX_ARCHITECTURES=$OSX_ARCHITECTURES"

if [ -n "$TOOLCHAIN_FILE" ]; then
    set -- "$@" "-DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE"
else
    case "$GENERATOR" in
        "Visual Studio"*|Xcode)
            ;;
        *)
            [ -z "$CXX_COMPILER" ] || \
                set -- "$@" "-DCMAKE_CXX_COMPILER=$CXX_COMPILER"
            ;;
    esac
fi

"$@"

set -- cmake --build "$CONSUMER_BUILD" --config Release
[ -z "$PARALLEL" ] || set -- "$@" --parallel "$PARALLEL"
"$@"

set -- ctest --test-dir "$CONSUMER_BUILD" --output-on-failure \
    --no-tests=error --build-config Release
[ -z "$PARALLEL" ] || set -- "$@" --parallel "$PARALLEL"
"$@"

printf '%s\n' "Verified installed MIDILAR package and its transitive MCC and Foundation dependencies"
