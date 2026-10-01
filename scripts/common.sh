#!/bin/sh

. "$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)/romodular-adapter.sh"
. "$MIDILAR_ROMODULAR_SCRIPTS/common.sh"

midilar_die() {
    romodular_die "$@"
}

midilar_require_command() {
    romodular_require_command "$@"
}

midilar_require_value() {
    romodular_require_value "$@"
}

midilar_require_preset() {
    romodular_require_preset "$@"
}

midilar_build_dir() {
    romodular_build_dir "$@"
}

midilar_configuration() {
    romodular_configuration "$@"
}

midilar_require_configuration() {
    romodular_require_configuration "$@"
}

midilar_require_configured() {
    romodular_require_configured "$@"
}

midilar_absolute_path() {
    romodular_absolute_path "$@"
}

midilar_require_safe_dist_child() {
    romodular_require_safe_dist_child "$@"
}
