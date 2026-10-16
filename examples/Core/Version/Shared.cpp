#include "Shared.h"

#include <MIDILAR/Core/Version.h>

namespace MIDILARExamples::Core::Version {

const char* MIDILARVersion() noexcept {
    return MIDILAR::Core::Version();
}

const char* FoundationVersion() noexcept {
    return MIDILAR::Core::FoundationVersion();
}

const char* MCCVersion() noexcept {
    return MIDILAR::Core::MCCVersion();
}

} // namespace MIDILARExamples::Core::Version
