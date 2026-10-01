#include <MIDILAR.h>

// SPEC-EMB-1..2: size budgets and trivial copyability of MIDILAR value types
// are checked here, when the library is built for every target, AVR and Arm
// included. Value types are added by later reconstruction phases.
#define MIDILAR_CHECK_VALUE_TYPE(Type, MaximumBytes)                                  \
    static_assert(sizeof(Type) <= (MaximumBytes), #Type " exceeds its size budget"); \
    static_assert(__is_trivially_copyable(Type), #Type " must be trivially copyable")

#undef MIDILAR_CHECK_VALUE_TYPE
