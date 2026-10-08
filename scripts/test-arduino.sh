#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
. "$SCRIPT_DIR/common.sh"

usage() {
    printf '%s\n' "Usage: $0 [--fqbn <board>] [--foundation <source-directory>] [--dspcore <source-directory>] [--mcc <source-directory>] [--cpstl <source-directory>]"
    printf '%s\n' "Foundation defaults to MIDILAR_FOUNDATION_SOURCE or the sibling ../Foundation;"
    printf '%s\n' "MCC defaults to MIDILAR_MCC_SOURCE or the sibling ../MCC;"
    printf '%s\n' "CPSTL defaults to MIDILAR_CPSTL_SOURCE or the sibling ../CPSTL."
}

# The validated Arduino source-mode board. Other cores are not validated here.
FQBN=arduino:avr:uno
FOUNDATION=${MIDILAR_FOUNDATION_SOURCE:-$MIDILAR_ROOT/../Foundation}
CPSTL=${MIDILAR_CPSTL_SOURCE:-$MIDILAR_ROOT/../CPSTL}
MCC=${MIDILAR_MCC_SOURCE:-$MIDILAR_ROOT/../MCC}
DSPCORE=${MIDILAR_DSPCORE_SOURCE:-$MIDILAR_ROOT/../DspCore}

while [ "$#" -gt 0 ]; do
    case "$1" in
        --fqbn)
            midilar_require_value "$1" "${2:-}"
            FQBN=$2
            shift 2
            ;;
        --mcc)
            midilar_require_value "$1" "${2:-}"
            MCC=$2
            shift 2
            ;;
        --dspcore)
            midilar_require_value "$1" "${2:-}"
            DSPCORE=$2
            shift 2
            ;;
        --foundation)
            midilar_require_value "$1" "${2:-}"
            FOUNDATION=$2
            shift 2
            ;;
        --cpstl)
            midilar_require_value "$1" "${2:-}"
            CPSTL=$2
            shift 2
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            midilar_die "unknown argument: $1"
            ;;
    esac
done

command -v arduino-cli >/dev/null 2>&1 || midilar_die "arduino-cli not found"
[ -f "$FOUNDATION/library.properties" ] || \
    midilar_die "Foundation Arduino library not found at $FOUNDATION"
FOUNDATION=$(midilar_absolute_path "$FOUNDATION")
[ -f "$MCC/library.properties" ] || \
    midilar_die "MCC Arduino library not found at $MCC"
MCC=$(midilar_absolute_path "$MCC")
[ -f "$DSPCORE/library.properties" ] || \
    midilar_die "DspCore Arduino library not found at $DSPCORE"
DSPCORE=$(midilar_absolute_path "$DSPCORE")
[ -f "$CPSTL/library.properties" ] || \
    midilar_die "CPSTL Arduino library not found at $CPSTL"
CPSTL=$(midilar_absolute_path "$CPSTL")

BUILD_ROOT="$MIDILAR_ROOT/build/arduino/$(printf '%s' "$FQBN" | tr ':' '_')"
rm -rf "$BUILD_ROOT"

# Compile each sketch against the repository, MCC and Foundation as libraries,
# exactly as an Arduino user who installed both would.
COUNT=0
for SKETCH in "$MIDILAR_ROOT"/examples/MIDILAR/*/*/*.ino; do
    SKETCH_DIR=$(dirname -- "$SKETCH")
    NAME=${SKETCH_DIR#"$MIDILAR_ROOT/examples/MIDILAR/"}
    LOG="$BUILD_ROOT/$NAME.log"
    mkdir -p -- "$(dirname -- "$LOG")"
    printf '%s\n' "== $NAME ($FQBN)"

    STATUS=0
    arduino-cli compile \
        --fqbn "$FQBN" \
        --library "$MIDILAR_ROOT" \
        --library "$MCC" \
        --library "$FOUNDATION" \
        --library "$DSPCORE" \
        --library "$CPSTL" \
        --build-path "$BUILD_ROOT/$NAME" \
        --warnings default \
        "$SKETCH_DIR" >"$LOG" 2>&1 || STATUS=$?
    cat -- "$LOG"
    [ "$STATUS" -eq 0 ] || midilar_die "$NAME failed to compile"

    # The stock AVR core passes -fpermissive, which demotes real type errors
    # to warnings; any warning in MIDILAR or its examples fails the gate.
    if grep -F "$MIDILAR_ROOT/" "$LOG" | grep -q "warning:"; then
        midilar_die "$NAME compiled with MIDILAR warnings"
    fi
    COUNT=$((COUNT + 1))
done

[ "$COUNT" -gt 0 ] || midilar_die "no Arduino sketches found"
printf '%s\n' "All $COUNT Arduino sketches compiled for $FQBN."
